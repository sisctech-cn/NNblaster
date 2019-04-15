/**
 ******************************************************************************
 * file    ai_base_float32.c
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

#if (AI_BASE_FLOAT_FP32 == 1)
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
/* conversions between float and integers */
AI_BASE_FLOAT32_ICVT_INT16 AiBaseFloat32InternalCvtSf32I16(AI_BASE_FLOAT32_F_UINT a, AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags, BOOL_T is_unsigned)
{
    UINT32_T a_sign, addend, rnd_bits;
    INT32_T a_exp;
    AI_BASE_FLOAT32_F_UINT a_mant, a_mant_temp;
    AI_BASE_FLOAT32_ICVT_UINT16 r, r_max;

    a_sign = a >> (AI_BASE_FLOAT32_F_SIZE - 1);
    a_exp = (a >> AI_BASE_FLOAT32_MANT_SIZE) & AI_BASE_FLOAT32_EXP_MASK;
    a_mant = a & AI_BASE_FLOAT32_MANT_MASK;
    if (a_exp == AI_BASE_FLOAT32_EXP_MASK && a_mant != 0)
        a_sign = 0; /* NaN is like +infinity */
    if (a_exp == 0) {
        a_exp = 1;
    } else {
        a_mant |= ((AI_BASE_FLOAT32_F_UINT)1 << AI_BASE_FLOAT32_MANT_SIZE);
    }
    a_mant <<= AI_BASE_FLOAT32_RND_SIZE;
    a_exp = a_exp - (AI_BASE_FLOAT32_EXP_MASK / 2) - AI_BASE_FLOAT32_MANT_SIZE;

    if (is_unsigned)
        r_max = (AI_BASE_FLOAT32_ICVT_UINT16)(a_sign - 1);
    else
        r_max = (AI_BASE_FLOAT32_ICVT_UINT16)((AI_BASE_FLOAT32_ICVT_UINT16)1 << (AI_BASE_FLOAT32_ICVT_SIZE16 - 1)) - (AI_BASE_FLOAT32_ICVT_UINT16)(a_sign ^ 1);
    if (a_exp >= 0) {
        if (a_exp <= (AI_BASE_FLOAT32_ICVT_SIZE16 - 1 - AI_BASE_FLOAT32_MANT_SIZE)) {
            a_mant_temp = (a_mant >> AI_BASE_FLOAT32_RND_SIZE);
			r = (AI_BASE_FLOAT32_ICVT_UINT16)(a_mant_temp << a_exp);
            if (r > r_max) {
                /* goto overflow; */
				*pfflags |= AI_BASE_FLOAT32_FFLAG_INVALID_OP;
				return r_max;
			}
        } else {
			/* overflow: */
            *pfflags |= AI_BASE_FLOAT32_FFLAG_INVALID_OP;
            return r_max;
        }
    } else {
        a_mant = AiBaseFloat32RshiftRnd(a_mant, -a_exp);

        switch(rm) {
        case AI_BASE_FLOAT32_RM_RNE:
        case AI_BASE_FLOAT32_RM_RMM:
            addend = (1 << (AI_BASE_FLOAT32_RND_SIZE - 1));
            break;
        case AI_BASE_FLOAT32_RM_RTZ:
            addend = 0;
            break;
        default:
        case AI_BASE_FLOAT32_RM_RDN:
        case AI_BASE_FLOAT32_RM_RUP:
            if (a_sign ^ (rm & 1))
                addend = (1 << AI_BASE_FLOAT32_RND_SIZE) - 1;
            else
                addend = 0;
            break;
        }
        
        rnd_bits = a_mant & ((1 << AI_BASE_FLOAT32_RND_SIZE ) - 1);
        a_mant = (a_mant + addend) >> AI_BASE_FLOAT32_RND_SIZE;
        /* half way: select even result */
        if (rm == AI_BASE_FLOAT32_RM_RNE && rnd_bits == (1 << (AI_BASE_FLOAT32_RND_SIZE - 1)))
            a_mant &= ~1;
        if (a_mant > r_max) {
            /* goto overflow; */
			*pfflags |= AI_BASE_FLOAT32_FFLAG_INVALID_OP;
            return r_max;
		}
        r = a_mant;
        if (rnd_bits != 0)
            *pfflags |= AI_BASE_FLOAT32_FFLAG_INEXACT;
    }
    if (a_sign)
        r = (AI_BASE_FLOAT32_ICVT_UINT16)((AI_BASE_FLOAT32_ICVT_UINT16)0-r);
    return r;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT32_ICVT_INT16 AiBaseFloat32CvtSf32I16(AI_BASE_FLOAT32_F_UINT a, AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags)
{
    return AiBaseFloat32InternalCvtSf32I16(a, rm, pfflags, AI_BASE_FLOAT32_FALSE);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT32_ICVT_UINT16 AiBaseFloat32CvtSf32U16(AI_BASE_FLOAT32_F_UINT a, AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags)
{
    return AiBaseFloat32InternalCvtSf32I16 (a, rm, pfflags, AI_BASE_FLOAT32_TRUE);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* conversions between float and integers */
AI_BASE_FLOAT32_F_UINT AiBaseFloat32InternalCvtI16Sf32(AI_BASE_FLOAT32_ICVT_INT16 a, AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags, BOOL_T is_unsigned)
{
    UINT32_T a_sign;
    INT32_T a_exp;
    AI_BASE_FLOAT32_F_UINT a_mant;
    AI_BASE_FLOAT32_ICVT_UINT16 r, mask;
    int l;

    if (!is_unsigned && a < 0) {
        a_sign = 1;
        r = -a;
    } else {
        a_sign = 0;
        r = a;
    }
    a_exp = (AI_BASE_FLOAT32_EXP_MASK / 2) + AI_BASE_FLOAT32_F_SIZE - 2;
    /* need to reduce range before generic float normalization */
    l = AI_BASE_FLOAT32_ICVT_SIZE16 - AiBaseFloat32ClzU16(r) - (AI_BASE_FLOAT32_F_SIZE - 1);
    if (l > 0) {
        mask = r & (((AI_BASE_FLOAT32_ICVT_UINT16)1 << l) - 1);
        r = (r >> l) | ((r & mask) != 0);
        a_exp += l;
    }
    a_mant = (AI_BASE_FLOAT32_F_UINT)r;
    return AiBaseFloat32NormalizeSf(a_sign, a_exp, a_mant, rm, pfflags);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT32_F_UINT AiBaseFloat32CvtI16Sf32(AI_BASE_FLOAT32_ICVT_INT16 a, AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags)
{
    return AiBaseFloat32InternalCvtI16Sf32(a, rm, pfflags, AI_BASE_FLOAT32_FALSE);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT32_F_UINT AiBaseFloat32CvtU16Sf32(AI_BASE_FLOAT32_ICVT_UINT16 a, AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags)
{
    return AiBaseFloat32InternalCvtI16Sf32(a, rm, pfflags, AI_BASE_FLOAT32_TRUE);
}

/*------------------------- End ---------------------------------------------*/
#endif

