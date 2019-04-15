/**
 ******************************************************************************
 * file    ai_base_float64_cvtfloat64.c
 * author  sunlingge
 * version V100
 * brief   the ai base float 32 driver.
 * description
 *	1. double: sign(63 bit), exponent(52-62 bit), fraction(00-51 bit)
 *	2. float:  sign(31 bit), exponent(23-30 bit), fraction(00-22 bit)
 *  3. fp16:   sign(15 bit), exponent(10-14 bit), fraction(00-09 bit)
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"

#if (AI_BASE_FLOAT_FP64 == 1)
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
ERROR_FLOAT64_T AiBaseFloat64BfGetFloat64(const AI_BASE_FLOAT64_BF_T *a, FLOAT64_T *pres, AI_BASE_FLOAT64_BF_RND_T rnd_mode)
{
    AI_BASE_FLOAT64_UNION64 u;
    AI_BASE_FLOAT64_SLIMB_T e;
    AI_BASE_FLOAT64_LIMB_T m;
	AI_BASE_FLOAT64_BF_T b_s, *b = &b_s;
	ERROR_FLOAT64_T ret;
    
    ret = AI_BASE_FLOAT64_ERROR_SUCCESS;
    if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
        u.u = (UINT64_T)AI_BASE_FLOAT64_FP64_QUIET_NAN; /* quiet nan */
		*pres = (FLOAT64_T)(u.d);
		return ret;
    } 

        
    AiBaseFloat64BfInit(b);
    AiBaseFloat64BfSet(b, a);
    if (AiBaseFloat64BfIsFinite(b)) {
        ret = AiBaseFloat64BfRound(b, AI_BASE_FLOAT64_FP64_ROUND_PREC, rnd_mode | AI_BASE_FLOAT64_BF_FLAG_SUBNORMAL | AiBaseFloat64BfSetExpBits(AI_BASE_FLOAT64_FP64_EXP_LENGTH));
    }
    if (b->expn == AI_BASE_FLOAT64_BF_EXP_INF) {
        e = (1 << AI_BASE_FLOAT64_FP64_EXP_LENGTH) - 1;
        m = 0;
    } else if (b->expn == AI_BASE_FLOAT64_BF_EXP_ZERO) {
        e = 0;
        m = 0;
    } else {
        e = (b->expn + AI_BASE_FLOAT64_FP64_BIAS - 1);
        m = ((UINT64_T)(b->tab[0]));
	   if (e <= 0) {
            /* subnormal */
            m = m >> ((AI_BASE_FLOAT64_FP64_EXP_LENGTH + AI_BASE_FLOAT64_FP64_SIGN_LENGTH) - e);
            e = 0;
        } else {
			/* normal */
            m = (m << AI_BASE_FLOAT64_FP64_SIGN_LENGTH);
			m = (m >> (AI_BASE_FLOAT64_FP64_EXP_LENGTH + AI_BASE_FLOAT64_FP64_SIGN_LENGTH));
        }
    }
    u.u = (UINT64_T)(m | ((UINT64_T)e << AI_BASE_FLOAT64_FP64_MANT_LENGTH) | ((UINT64_T)b->sign << AI_BASE_FLOAT64_FP64_SIGN_SHIFT));
    AiBaseFloat64BfDelete(b);
    
    *pres = (FLOAT64_T)u.d;
    return ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_FLOAT16_T AiBaseFloat64BfSetFloat64(AI_BASE_FLOAT64_BF_T *a, FLOAT64_T d)
{
    AI_BASE_FLOAT64_UNION64 u;
    AI_BASE_FLOAT64_LIMB_T m;
	AI_BASE_FLOAT64_SLIMB_T e;
	AI_BASE_FLOAT64_LIMB_T sgn;
    INT32_T shift;
    
    u.d = d;
    sgn = u.u >> AI_BASE_FLOAT64_FP64_SIGN_SHIFT;
    e = (u.u >> AI_BASE_FLOAT64_FP64_EXP_SHIFT) & ((1 << AI_BASE_FLOAT64_FP64_EXP_LENGTH) - 1);
    m = u.u & (((UINT64_T)1 << AI_BASE_FLOAT64_FP64_MANT_LENGTH) - 1);
    if (e == ((1 << AI_BASE_FLOAT64_FP64_EXP_LENGTH) - 1)) {
        if (m != 0) {
            AiBaseFloat64BfSetNan(a);
        } else {
            AiBaseFloat64BfSetInf(a, sgn);
        }
		return AI_BASE_FLOAT64_ERROR_SUCCESS;
	} 

	if (e == 0) {
		if (m == 0) {
			AiBaseFloat64BfSetZero(a, sgn);
		} else {
			/* subnormal number */
			m <<= (AI_BASE_FLOAT64_FP64_EXP_LENGTH + AI_BASE_FLOAT64_FP64_SIGN_LENGTH);
			shift = AiBaseFloat64Clz((AI_BASE_FLOAT64_LIMB_T)m);
			m <<= shift;
			e = -shift;
			a->expn = e - AI_BASE_FLOAT64_FP64_BIAS + 1;
			AiBaseFloat64BfResize(a, 1);
			a->tab[0] = m;
			a->sign = sgn;
		}
		return AI_BASE_FLOAT64_ERROR_SUCCESS;
	} 

	/* normal number */
	m = (m << AI_BASE_FLOAT64_FP64_EXP_LENGTH) | ((UINT64_T)1 << AI_BASE_FLOAT64_FP64_SIGN_SHIFT);
	a->expn = e - AI_BASE_FLOAT64_FP64_BIAS + 1;
	AiBaseFloat64BfResize(a, 1);
	a->tab[0] = m;
	a->sign = sgn;
	return AI_BASE_FLOAT64_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
#endif

