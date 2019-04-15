/**
 ******************************************************************************
 * file    ai_base_float16_cvtfloat16.c
 * author  sunlingge
 * version V100
 * brief   the ai base float 16 driver.
 * description
 *	1. double: sign(63 bit), exponent(52-62 bit), fraction(00-51 bit)
 *	2. float:  sign(31 bit), exponent(23-30 bit), fraction(00-22 bit)
 *  3. fp16:   sign(15 bit), exponent(10-14 bit), fraction(00-09 bit)
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"

#if (AI_BASE_FLOAT_FP16 == 1)
/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/

/*------------------------- Function Definition -----------------------------*/
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_FLOAT16_T AiBaseFloat16BfGetFloat16(const AI_BASE_FLOAT16_BF_T *a, FLOAT16_T *pres, AI_BASE_FLOAT16_BF_RND_T rnd_mode)
{
    AI_BASE_FLOAT16_UNION16 u;
    AI_BASE_FLOAT16_SLIMB_T e;
    AI_BASE_FLOAT16_LIMB_T m;
	AI_BASE_FLOAT16_BF_T b_s, *b = &b_s;
	ERROR_FLOAT16_T ret;
    
    ret = AI_BASE_FLOAT16_ERROR_SUCCESS;
    if (a->expn == AI_BASE_FLOAT16_BF_EXP_NAN) {
        u.u = (UINT16_T)AI_BASE_FLOAT16_FP16_QUIET_NAN; /* quiet nan */
		*pres = (FLOAT16_T)(u.d);
		return ret;
    } 

    AiBaseFloat16BfInit(b);
    AiBaseFloat16BfSet(b, a);
    if (AiBaseFloat16BfIsFinite(b)) {
        ret = AiBaseFloat16BfRound(b, AI_BASE_FLOAT16_FP16_ROUND_PREC, rnd_mode | AI_BASE_FLOAT16_BF_FLAG_SUBNORMAL | AiBaseFloat16BfSetExpBits(AI_BASE_FLOAT16_FP16_EXP_LENGTH));
    }
    if (b->expn == AI_BASE_FLOAT16_BF_EXP_INF) {
        e = (1 << AI_BASE_FLOAT16_FP16_EXP_LENGTH) - 1;
        m = 0;
    } else if (b->expn == AI_BASE_FLOAT16_BF_EXP_ZERO) {
        e = 0;
        m = 0;
    } else {
        e = b->expn + AI_BASE_FLOAT16_FP16_BIAS - 1;
        m = ((UINT16_T)(b->tab[0]));
        if (e <= 0) {
            /* subnormal */
            m = m >> ((AI_BASE_FLOAT16_FP16_EXP_LENGTH + AI_BASE_FLOAT16_FP16_SIGN_LENGTH) - e);
            e = 0;
        } else {
			/* normal */
			m = (m << AI_BASE_FLOAT16_FP16_SIGN_LENGTH);
			m = (m  >> (AI_BASE_FLOAT16_FP16_EXP_LENGTH + AI_BASE_FLOAT16_FP16_SIGN_LENGTH));
        }
    }
    u.u = (UINT16_T)(m | ((UINT64_T)e << AI_BASE_FLOAT16_FP16_MANT_LENGTH) | ((UINT64_T)b->sign << AI_BASE_FLOAT16_FP16_SIGN_SHIFT));
    *pres = (FLOAT16_T)(u.d);

    return ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_FLOAT16_T AiBaseFloat16BfSetFloat16(AI_BASE_FLOAT16_BF_T *a, FLOAT16_T d)
{
    AI_BASE_FLOAT16_UNION16 u;
    AI_BASE_FLOAT16_LIMB_T m;
	AI_BASE_FLOAT16_SLIMB_T e;
	AI_BASE_FLOAT16_LIMB_T sgn;
    INT32_T shift;
    
    u.d = d;
    sgn = u.u >> AI_BASE_FLOAT16_FP16_SIGN_SHIFT;
    e = (u.u >> AI_BASE_FLOAT16_FP16_EXP_SHIFT) & ((1 << AI_BASE_FLOAT16_FP16_EXP_LENGTH) - 1);
    m = u.u & (((UINT16_T)1 << AI_BASE_FLOAT16_FP16_MANT_LENGTH) - 1);
	if (e == ((1 << AI_BASE_FLOAT16_FP16_EXP_LENGTH) - 1)) {
		if (m != 0) {
			AiBaseFloat16BfSetNan(a);
		} else {
			AiBaseFloat16BfSetInf(a, sgn);
		}
		return AI_BASE_FLOAT16_ERROR_SUCCESS;
	} 

	if (e == 0) {
		if (m == 0) {
			AiBaseFloat16BfSetZero(a, sgn);
		} else {
			/* subnormal number */
			m <<= (AI_BASE_FLOAT16_FP16_EXP_LENGTH + AI_BASE_FLOAT16_FP16_SIGN_LENGTH);
			shift = AiBaseFloat16Clz((AI_BASE_FLOAT16_LIMB_T)m);
			m <<= shift;
			e = -shift;
			a->expn = e - AI_BASE_FLOAT16_FP16_BIAS + 1;
			AiBaseFloat16BfResize(a, 1);
			a->tab[0] = m;
			a->sign = sgn;
		}
		return AI_BASE_FLOAT16_ERROR_SUCCESS;
	} 

	/* normal number */
	m = (m << AI_BASE_FLOAT16_FP16_EXP_LENGTH) | ((UINT16_T)1 << AI_BASE_FLOAT16_FP16_SIGN_SHIFT);
	a->expn = e - AI_BASE_FLOAT16_FP16_BIAS + 1;
	AiBaseFloat16BfResize(a, 1);
	a->tab[0] = m;
	a->sign = sgn;
	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
#endif

