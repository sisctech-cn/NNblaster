/**
 ******************************************************************************
 * file    ai_base_float64.c
 * author  sunlingge
 * version V100
 * brief   the ai base float 64 driver.
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
AI_BASE_FLOAT64_F_UINT AiBaseFloat64PackSf(UINT32_T a_sign, UINT32_T a_exp, AI_BASE_FLOAT64_F_UINT a_mant)
{
    return ((AI_BASE_FLOAT64_F_UINT)a_sign << (AI_BASE_FLOAT64_F_SIZE - 1)) |
        ((AI_BASE_FLOAT64_F_UINT)a_exp << AI_BASE_FLOAT64_MANT_SIZE) | 
        (a_mant & AI_BASE_FLOAT64_MANT_MASK);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_F_UINT AiBaseFloat64UnpackSf(UINT32_T *pa_sign, INT32_T *pa_exp, AI_BASE_FLOAT64_F_UINT a)
{
    *pa_sign = a >> (AI_BASE_FLOAT64_F_SIZE - 1);
    *pa_exp = (a >> AI_BASE_FLOAT64_MANT_SIZE) & AI_BASE_FLOAT64_EXP_MASK;
    return a & AI_BASE_FLOAT64_MANT_MASK;
} 

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_F_UINT AiBaseFloat64RshiftRnd(AI_BASE_FLOAT64_F_UINT a, INT32_T d)
{
    AI_BASE_FLOAT64_F_UINT mask;
    if (d != 0) {
        if (d >= AI_BASE_FLOAT64_F_SIZE) {
            a = (a != 0);
        } else {
            mask = ((AI_BASE_FLOAT64_F_UINT)1 << d) - 1;
            a = (a >> d) | ((a & mask) != 0);
        }
    }
    return a;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* a_mant is considered to have its MSB at AI_BASE_FLOAT64_F_SIZE - 2 bits */
AI_BASE_FLOAT64_F_UINT AiBaseFloat64RoundPackSf(UINT32_T a_sign, INT32_T a_exp, AI_BASE_FLOAT64_F_UINT a_mant, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags)
{
    INT32_T diff;
    UINT32_T addend, rnd_bits;

    switch(rm) {
    case AI_BASE_FLOAT64_RM_RNE:
    case AI_BASE_FLOAT64_RM_RMM:
        addend = (1 << (AI_BASE_FLOAT64_RND_SIZE - 1));
        break;
    case AI_BASE_FLOAT64_RM_RTZ:
        addend = 0;
        break;
    default:
    case AI_BASE_FLOAT64_RM_RDN:
    case AI_BASE_FLOAT64_RM_RUP:
        //  printf("s=%d rm=%d m=%x\n", a_sign, rm, a_mant);
        if (a_sign ^ (rm & 1))
            addend = (1 << AI_BASE_FLOAT64_RND_SIZE) - 1;
        else
            addend = 0;
        break;
    }

    /* potentially subnormal */
    if (a_exp <= 0) {
        BOOL_T is_subnormal;
        /* Note: we set the underflow flag if the rounded result
           is subnormal and inexact */
        is_subnormal = (a_exp < 0 || 
                        (a_mant + addend) < ((AI_BASE_FLOAT64_F_UINT)1 << (AI_BASE_FLOAT64_F_SIZE - 1)));
        diff = 1 - a_exp;
        a_mant = AiBaseFloat64RshiftRnd(a_mant, diff);
        rnd_bits = a_mant & ((1 << AI_BASE_FLOAT64_RND_SIZE ) - 1);
        if (is_subnormal && rnd_bits != 0) {
            *pfflags |= AI_BASE_FLOAT64_FFLAG_UNDERFLOW;
        }
        a_exp = 1;
    } else {
        rnd_bits = a_mant & ((1 << AI_BASE_FLOAT64_RND_SIZE ) - 1);
    }
    if (rnd_bits != 0)
        *pfflags |= AI_BASE_FLOAT64_FFLAG_INEXACT;
    a_mant = (a_mant + addend) >> AI_BASE_FLOAT64_RND_SIZE;
    /* half way: select even result */
    if (rm == AI_BASE_FLOAT64_RM_RNE && rnd_bits == (1 << (AI_BASE_FLOAT64_RND_SIZE - 1)))
        a_mant &= ~1;
    /* Note the rounding adds at least 1, so this is the maximum
       value */
    a_exp += a_mant >> (AI_BASE_FLOAT64_MANT_SIZE + 1);
    if (a_mant <= AI_BASE_FLOAT64_MANT_MASK) {
        /* denormalized or zero */
        a_exp = 0;
    } else if (a_exp >= AI_BASE_FLOAT64_EXP_MASK) {
        /* overflow */
        if (addend == 0) {
            a_exp = AI_BASE_FLOAT64_EXP_MASK - 1;
            a_mant = AI_BASE_FLOAT64_MANT_MASK;
        } else {
            /* infinity */
            a_exp = AI_BASE_FLOAT64_EXP_MASK;
            a_mant = 0;
        }
        *pfflags |= AI_BASE_FLOAT64_FFLAG_OVERFLOW | AI_BASE_FLOAT64_FFLAG_INEXACT;
    }
    return AiBaseFloat64PackSf(a_sign, a_exp, a_mant);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* a_mant is considered to have at most AI_BASE_FLOAT64_F_SIZE - 1 bits */
AI_BASE_FLOAT64_F_UINT AiBaseFloat64NormalizeSf(UINT32_T a_sign, INT32_T a_exp, AI_BASE_FLOAT64_F_UINT a_mant, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags)
{
    INT32_T shift;
    shift = AiBaseFloat64Clz(a_mant) - (AI_BASE_FLOAT64_F_SIZE - 1 - AI_BASE_FLOAT64_IMANT_SIZE);
    if (!(shift >= 0))
		return 0;
    a_exp -= shift;
    a_mant <<= shift;
    return AiBaseFloat64RoundPackSf(a_sign, a_exp, a_mant, rm, pfflags);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* same as AiBaseFloat64NormalizeSf() but with a double word mantissa. a_mant1 is
   considered to have at most AI_BASE_FLOAT64_F_SIZE - 1 bits */
AI_BASE_FLOAT64_F_UINT AiBaseFloat64Normalize2Sf(UINT32_T a_sign, INT32_T a_exp, AI_BASE_FLOAT64_F_UINT a_mant1, AI_BASE_FLOAT64_F_UINT a_mant0,
                            AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags)
{
    INT32_T l, shift;
    if (a_mant1 == 0) {
        l = AI_BASE_FLOAT64_F_SIZE + AiBaseFloat64Clz(a_mant0);
    } else {
        l = AiBaseFloat64Clz(a_mant1);
    }
    shift = l - (AI_BASE_FLOAT64_F_SIZE - 1 - AI_BASE_FLOAT64_IMANT_SIZE);
    if (!(shift >= 0))
		return 0;
    a_exp -= shift;
    if (shift == 0) {
        a_mant1 |= (a_mant0 != 0);
    } else if (shift < AI_BASE_FLOAT64_F_SIZE) {
        a_mant1 = (a_mant1 << shift) | (a_mant0 >> (AI_BASE_FLOAT64_F_SIZE - shift));
        a_mant0 <<= shift;
        a_mant1 |= (a_mant0 != 0);
    } else {
        a_mant1 = a_mant0 << (shift - AI_BASE_FLOAT64_F_SIZE);
    }
    return AiBaseFloat64RoundPackSf(a_sign, a_exp, a_mant1, rm, pfflags);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
BOOL_T AiBaseFloat64IssignanSf(AI_BASE_FLOAT64_F_UINT a)
{
    UINT32_T a_exp1;
    AI_BASE_FLOAT64_F_UINT a_mant;
    a_exp1 = (a >> (AI_BASE_FLOAT64_MANT_SIZE - 1)) & ((1 << (AI_BASE_FLOAT64_EXP_SIZE + 1)) - 1);
    a_mant = a & AI_BASE_FLOAT64_MANT_MASK;
    return (a_exp1 == (2 * AI_BASE_FLOAT64_EXP_MASK) && a_mant != 0);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
BOOL_T AiBaseFloat64IsnanSf(AI_BASE_FLOAT64_F_UINT a)
{
    UINT32_T a_exp;
    AI_BASE_FLOAT64_F_UINT a_mant;
    a_exp = (a >> AI_BASE_FLOAT64_MANT_SIZE) & AI_BASE_FLOAT64_EXP_MASK;
    a_mant = a & AI_BASE_FLOAT64_MANT_MASK;
    return (a_exp == AI_BASE_FLOAT64_EXP_MASK && a_mant != 0);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_F_UINT AiBaseFloat64AddSf(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_F_UINT b, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags)
{
    UINT32_T a_sign, b_sign, a_exp, b_exp;
    AI_BASE_FLOAT64_F_UINT tmp, a_mant, b_mant;

    /* swap so that  abs(a) >= abs(b) */
    if ((a & ~AI_BASE_FLOAT64_SIGN_MASK) < (b & ~AI_BASE_FLOAT64_SIGN_MASK)) {
        tmp = a;
        a = b;
        b = tmp;
    }
    a_sign = a >> (AI_BASE_FLOAT64_F_SIZE - 1);
    b_sign = b >> (AI_BASE_FLOAT64_F_SIZE - 1);
    a_exp = (a >> AI_BASE_FLOAT64_MANT_SIZE) & AI_BASE_FLOAT64_EXP_MASK;
    b_exp = (b >> AI_BASE_FLOAT64_MANT_SIZE) & AI_BASE_FLOAT64_EXP_MASK;
    a_mant = (a & AI_BASE_FLOAT64_MANT_MASK) << 3;
    b_mant = (b & AI_BASE_FLOAT64_MANT_MASK) << 3;
    if ((a_exp == AI_BASE_FLOAT64_EXP_MASK)) {
        if (a_mant != 0) {
            /* NaN result */
            if (!(a_mant & (AI_BASE_FLOAT64_QNAN_MASK << 3)) || AiBaseFloat64IssignanSf(b))
                *pfflags |= AI_BASE_FLOAT64_FFLAG_INVALID_OP;
            return AI_BASE_FLOAT64_F_QNAN;
        } else if (b_exp == AI_BASE_FLOAT64_EXP_MASK && a_sign != b_sign) {
            *pfflags |= AI_BASE_FLOAT64_FFLAG_INVALID_OP;
            return AI_BASE_FLOAT64_F_QNAN;
        } else {
            /* infinity */
            return a;
        }
    }
    if (a_exp == 0) {
        a_exp = 1;
    } else {
        a_mant |= (AI_BASE_FLOAT64_F_UINT)1 << (AI_BASE_FLOAT64_MANT_SIZE + 3);
    }
    if (b_exp == 0) {
        b_exp = 1;
    } else {
        b_mant |= (AI_BASE_FLOAT64_F_UINT)1 << (AI_BASE_FLOAT64_MANT_SIZE + 3);
    }
    b_mant = AiBaseFloat64RshiftRnd(b_mant, a_exp - b_exp);
    if (a_sign == b_sign) {
        /* same signs : add the absolute values  */
        a_mant += b_mant;
    } else {
        /* different signs : subtract the absolute values  */
        a_mant -= b_mant;
        if (a_mant == 0) {
            /* zero result : the sign needs a specific handling */
            a_sign = (rm == AI_BASE_FLOAT64_RM_RDN);
        }
    }
    a_exp += (AI_BASE_FLOAT64_RND_SIZE - 3);
    return AiBaseFloat64NormalizeSf(a_sign, a_exp, a_mant, rm, pfflags);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_F_UINT AiBaseFloat64SubSf32(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_F_UINT b, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags)
{
    return AiBaseFloat64AddSf(a, b ^ AI_BASE_FLOAT64_SIGN_MASK, rm, pfflags);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_F_UINT AiBaseFloat64NormalizeSubnormalSf(INT32_T *pa_exp, AI_BASE_FLOAT64_F_UINT a_mant)
{
    INT32_T shift;
    shift = AI_BASE_FLOAT64_MANT_SIZE - ((AI_BASE_FLOAT64_F_SIZE - 1 - AiBaseFloat64Clz(a_mant)));
    *pa_exp = 1 - shift;
    return a_mant << shift;
}

#ifdef AI_BASE_FLOAT64_F_ULONG
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_F_UINT AiBaseFloat64MulU(AI_BASE_FLOAT64_F_UINT *plow, AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_F_UINT b)
{
    AI_BASE_FLOAT64_F_ULONG r;
    r = (AI_BASE_FLOAT64_F_ULONG)a * (AI_BASE_FLOAT64_F_ULONG)b;
    *plow = (UINT32_T)r;
    return r >> AI_BASE_FLOAT64_F_SIZE;
}
#else
#if 0
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_F_UINT AiBaseFloat64MulU(AI_BASE_FLOAT64_F_UINT *plow, AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_F_UINT b)
{
    AI_BASE_FLOAT64_F_UHALF a0, a1, b0, b1, r0, r1, r2, r3;
    AI_BASE_FLOAT64_F_UINT r00, r01, r10, r11, c;
    a0 = a;
    a1 = a >> AI_BASE_FLOAT64_FH_SIZE;
    b0 = b;
    b1 = b >> AI_BASE_FLOAT64_FH_SIZE;

    r00 = (AI_BASE_FLOAT64_F_UINT)a0 * (AI_BASE_FLOAT64_F_UINT)b0;
    r01 = (AI_BASE_FLOAT64_F_UINT)a0 * (AI_BASE_FLOAT64_F_UINT)b1;
    r10 = (AI_BASE_FLOAT64_F_UINT)a1 * (AI_BASE_FLOAT64_F_UINT)b0;
    r11 = (AI_BASE_FLOAT64_F_UINT)a1 * (AI_BASE_FLOAT64_F_UINT)b1;
    
    r0 = r00;
    c = (r00 >> AI_BASE_FLOAT64_FH_SIZE) + (AI_BASE_FLOAT64_F_UHALF)r01 + (AI_BASE_FLOAT64_F_UHALF)r10;
    r1 = c;
    c = (c >> AI_BASE_FLOAT64_FH_SIZE) + (r01 >> AI_BASE_FLOAT64_FH_SIZE) + (r10 >> AI_BASE_FLOAT64_FH_SIZE) + (AI_BASE_FLOAT64_F_UHALF)r11;
    r2 = c;
    r3 = (c >> AI_BASE_FLOAT64_FH_SIZE) + (r11 >> AI_BASE_FLOAT64_FH_SIZE);

    *plow = ((AI_BASE_FLOAT64_F_UINT)r1 << AI_BASE_FLOAT64_FH_SIZE) | r0;
    return ((AI_BASE_FLOAT64_F_UINT)r3 << AI_BASE_FLOAT64_FH_SIZE) | r2;
}
#endif
#endif

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_F_UINT AiBaseFloat64MulSf(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_F_UINT b, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm,
              UINT32_T *pfflags)
{
    UINT32_T a_sign, b_sign, r_sign;
    INT32_T a_exp, b_exp, r_exp;
    AI_BASE_FLOAT64_F_UINT a_mant, b_mant, r_mant, r_mant_low;

    a_sign = a >> (AI_BASE_FLOAT64_F_SIZE - 1);
    b_sign = b >> (AI_BASE_FLOAT64_F_SIZE - 1);
    r_sign = a_sign ^ b_sign;
    a_exp = (a >> AI_BASE_FLOAT64_MANT_SIZE) & AI_BASE_FLOAT64_EXP_MASK;
    b_exp = (b >> AI_BASE_FLOAT64_MANT_SIZE) & AI_BASE_FLOAT64_EXP_MASK;
    a_mant = a & AI_BASE_FLOAT64_MANT_MASK;
    b_mant = b & AI_BASE_FLOAT64_MANT_MASK;
    if (a_exp == AI_BASE_FLOAT64_EXP_MASK || b_exp == AI_BASE_FLOAT64_EXP_MASK) {
        if (AiBaseFloat64IsnanSf(a) || AiBaseFloat64IsnanSf(b)) {
            if (AiBaseFloat64IssignanSf(a) || AiBaseFloat64IssignanSf(b)) {
                *pfflags |= AI_BASE_FLOAT64_FFLAG_INVALID_OP;
            }
            return AI_BASE_FLOAT64_F_QNAN;
        } else {
            /* infinity */
            if ((a_exp == AI_BASE_FLOAT64_EXP_MASK && (b_exp == 0 && b_mant == 0)) ||
                (b_exp == AI_BASE_FLOAT64_EXP_MASK && (a_exp == 0 && a_mant == 0))) {
                *pfflags |= AI_BASE_FLOAT64_FFLAG_INVALID_OP;
                return AI_BASE_FLOAT64_F_QNAN;
            } else {
                return AiBaseFloat64PackSf(r_sign, AI_BASE_FLOAT64_EXP_MASK, 0);
            }
        }
    }
    if (a_exp == 0) {
        if (a_mant == 0)
            return AiBaseFloat64PackSf(r_sign, 0, 0); /* zero */
        a_mant = AiBaseFloat64NormalizeSubnormalSf(&a_exp, a_mant);
    } else {
        a_mant |= (AI_BASE_FLOAT64_F_UINT)1 << AI_BASE_FLOAT64_MANT_SIZE;
    }
    if (b_exp == 0) {
        if (b_mant == 0)
            return AiBaseFloat64PackSf(r_sign, 0, 0); /* zero */
        b_mant = AiBaseFloat64NormalizeSubnormalSf(&b_exp, b_mant);
    } else {
        b_mant |= (AI_BASE_FLOAT64_F_UINT)1 << AI_BASE_FLOAT64_MANT_SIZE;
    }
    r_exp = a_exp + b_exp - (1 << (AI_BASE_FLOAT64_EXP_SIZE - 1)) + 2;
    
    r_mant = AiBaseFloat64MulU(&r_mant_low,a_mant << AI_BASE_FLOAT64_RND_SIZE, b_mant << (AI_BASE_FLOAT64_RND_SIZE + 1));
    r_mant |= (r_mant_low != 0);
    return AiBaseFloat64NormalizeSf(r_sign, r_exp, r_mant, rm, pfflags);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* fused multiply and add */
AI_BASE_FLOAT64_F_UINT AiBaseFloat64FmaSf(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_F_UINT b, AI_BASE_FLOAT64_F_UINT c, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm,
              UINT32_T *pfflags)
{
    UINT32_T a_sign, b_sign, c_sign, r_sign;
    INT32_T a_exp, b_exp, c_exp, r_exp, shift;
    AI_BASE_FLOAT64_F_UINT a_mant, b_mant, c_mant, r_mant1, r_mant0, c_mant1, c_mant0, mask;

    a_sign = a >> (AI_BASE_FLOAT64_F_SIZE - 1);
    b_sign = b >> (AI_BASE_FLOAT64_F_SIZE - 1);
    c_sign = c >> (AI_BASE_FLOAT64_F_SIZE - 1);
    r_sign = a_sign ^ b_sign;
    a_exp = (a >> AI_BASE_FLOAT64_MANT_SIZE) & AI_BASE_FLOAT64_EXP_MASK;
    b_exp = (b >> AI_BASE_FLOAT64_MANT_SIZE) & AI_BASE_FLOAT64_EXP_MASK;
    c_exp = (c >> AI_BASE_FLOAT64_MANT_SIZE) & AI_BASE_FLOAT64_EXP_MASK;
    a_mant = a & AI_BASE_FLOAT64_MANT_MASK;
    b_mant = b & AI_BASE_FLOAT64_MANT_MASK;
    c_mant = c & AI_BASE_FLOAT64_MANT_MASK;
    if (a_exp == AI_BASE_FLOAT64_EXP_MASK || b_exp == AI_BASE_FLOAT64_EXP_MASK || c_exp == AI_BASE_FLOAT64_EXP_MASK) {
        if (AiBaseFloat64IsnanSf(a) || AiBaseFloat64IsnanSf(b) || AiBaseFloat64IsnanSf(c)) {
            if (AiBaseFloat64IssignanSf(a) || AiBaseFloat64IssignanSf(b) || AiBaseFloat64IssignanSf(c)) {
                *pfflags |= AI_BASE_FLOAT64_FFLAG_INVALID_OP;
            }
            return AI_BASE_FLOAT64_F_QNAN;
        } else {
            /* infinities */
            if ((a_exp == AI_BASE_FLOAT64_EXP_MASK && (b_exp == 0 && b_mant == 0)) ||
                (b_exp == AI_BASE_FLOAT64_EXP_MASK && (a_exp == 0 && a_mant == 0)) ||
                ((a_exp == AI_BASE_FLOAT64_EXP_MASK || b_exp == AI_BASE_FLOAT64_EXP_MASK) &&
                 (c_exp == AI_BASE_FLOAT64_EXP_MASK && r_sign != c_sign))) {
                *pfflags |= AI_BASE_FLOAT64_FFLAG_INVALID_OP;
                return AI_BASE_FLOAT64_F_QNAN;
            } else if (c_exp == AI_BASE_FLOAT64_EXP_MASK) {
                return AiBaseFloat64PackSf(c_sign, AI_BASE_FLOAT64_EXP_MASK, 0);
            } else {
                return AiBaseFloat64PackSf(r_sign, AI_BASE_FLOAT64_EXP_MASK, 0);
            }
        }
    }
    if (a_exp == 0) {
        if (a_mant == 0)
            goto mul_zero;
        a_mant = AiBaseFloat64NormalizeSubnormalSf(&a_exp, a_mant);
    } else {
        a_mant |= (AI_BASE_FLOAT64_F_UINT)1 << AI_BASE_FLOAT64_MANT_SIZE;
    }
    if (b_exp == 0) {
        if (b_mant == 0) {
        mul_zero:
            if (c_exp == 0 && c_mant == 0) {
                if (c_sign != r_sign)
                    r_sign = (rm == AI_BASE_FLOAT64_RM_RDN);
                return AiBaseFloat64PackSf(r_sign, 0, 0);
            } else {
                return c;
            }
        }
        b_mant = AiBaseFloat64NormalizeSubnormalSf(&b_exp, b_mant);
    } else {
        b_mant |= (AI_BASE_FLOAT64_F_UINT)1 << AI_BASE_FLOAT64_MANT_SIZE;
    }
    /* multiply */
    r_exp = a_exp + b_exp - (1 << (AI_BASE_FLOAT64_EXP_SIZE - 1)) + 3;
    
    r_mant1 = AiBaseFloat64MulU(&r_mant0, a_mant << AI_BASE_FLOAT64_RND_SIZE, b_mant << AI_BASE_FLOAT64_RND_SIZE);
    /* normalize to AI_BASE_FLOAT64_F_SIZE - 3 */
    if (r_mant1 < ((AI_BASE_FLOAT64_F_UINT)1 << (AI_BASE_FLOAT64_F_SIZE - 3))) {
        r_mant1 = (r_mant1 << 1) | (r_mant0 >> (AI_BASE_FLOAT64_F_SIZE - 1));
        r_mant0 <<= 1;
        r_exp--;
    }

    /* add */
    if (c_exp == 0) {
        if (c_mant == 0) {
            /* add zero */
            r_mant1 |= (r_mant0 != 0);
            return AiBaseFloat64NormalizeSf(r_sign, r_exp, r_mant1, rm, pfflags);
        }
        c_mant = AiBaseFloat64NormalizeSubnormalSf(&c_exp, c_mant);
    } else {
        c_mant |= (AI_BASE_FLOAT64_F_UINT)1 << AI_BASE_FLOAT64_MANT_SIZE;
    }
    c_exp++;
    c_mant1 = c_mant << (AI_BASE_FLOAT64_RND_SIZE - 1);
    c_mant0 = 0;

    //    printf("r_s=%d r_exp=%d r_mant=%08x %08x\n", r_sign, r_exp, (UINT32_T)r_mant1, (UINT32_T)r_mant0);
    //    printf("c_s=%d c_exp=%d c_mant=%08x %08x\n", c_sign, c_exp, (UINT32_T)c_mant1, (UINT32_T)c_mant0);

    /* ensure that abs(r) >= abs(c) */
    if (!(r_exp > c_exp || (r_exp == c_exp && r_mant1 >= c_mant1))) {
        AI_BASE_FLOAT64_F_UINT tmp;
        INT32_T c_tmp;
        /* swap */
        tmp = r_mant1; r_mant1 = c_mant1; c_mant1 = tmp;
        tmp = r_mant0; r_mant0 = c_mant0; c_mant0 = tmp;
        c_tmp = r_exp; r_exp = c_exp; c_exp = c_tmp;
        c_tmp = r_sign; r_sign = c_sign; c_sign = c_tmp;
    }
    /* right shift c_mant */
    shift = r_exp - c_exp;
    if (shift >= 2 * AI_BASE_FLOAT64_F_SIZE) {
        c_mant0 = (c_mant0 | c_mant1) != 0;
        c_mant1 = 0;
    } else if (shift >= AI_BASE_FLOAT64_F_SIZE + 1) {
        c_mant0 = AiBaseFloat64RshiftRnd(c_mant1, shift - AI_BASE_FLOAT64_F_SIZE);
        c_mant1 = 0;
    } else if (shift == AI_BASE_FLOAT64_F_SIZE) {
        c_mant0 = c_mant1 | (c_mant0 != 0);
        c_mant1 = 0;
    } else if (shift != 0) {
        mask = ((AI_BASE_FLOAT64_F_UINT)1 << shift) - 1;
        c_mant0 = (c_mant1 << (AI_BASE_FLOAT64_F_SIZE - shift)) | (c_mant0 >> shift) | ((c_mant0 & mask) != 0);
        c_mant1 = c_mant1 >> shift;
    }
    //    printf("  r_mant=%08x %08x\n", (UINT32_T)r_mant1, (UINT32_T)r_mant0);
    //    printf("  c_mant=%08x %08x\n", (UINT32_T)c_mant1, (UINT32_T)c_mant0);
    /* add or subtract */
    if (r_sign == c_sign) {
        r_mant0 += c_mant0;
        r_mant1 += c_mant1 + (r_mant0 < c_mant0);
    } else {
        AI_BASE_FLOAT64_F_UINT tmp;
        tmp = r_mant0;
        r_mant0 -= c_mant0;
        r_mant1 = r_mant1 - c_mant1 - (r_mant0 > tmp);
        if ((r_mant0 | r_mant1) == 0) {
            /* zero result : the sign needs a specific handling */
            r_sign = (rm == AI_BASE_FLOAT64_RM_RDN);
        }
    }
#if 0
    //    printf("  r1_mant=%08x %08x\n", (UINT32_T)r_mant1, (UINT32_T)r_mant0);
    /* normalize */
    if (r_mant1 == 0) {
        r_mant1 = r_mant0;
        r_exp -= AI_BASE_FLOAT64_F_SIZE;
    } else {
        shift = AiBaseFloat64Clz(r_mant1) - (AI_BASE_FLOAT64_F_SIZE - 1 - AI_BASE_FLOAT64_IMANT_SIZE);
        if (shift != 0) {
            r_mant1 = (r_mant1 << shift) | (r_mant0 >> (AI_BASE_FLOAT64_F_SIZE - shift));
            r_mant0 <<= shift;
            r_exp -= shift;
        }
        r_mant1 |= (r_mant0 != 0);
    }
    return AiBaseFloat64NormalizeSf(r_sign, r_exp, r_mant1, rm, pfflags);
#else
    return AiBaseFloat64Normalize2Sf(r_sign, r_exp, r_mant1, r_mant0, rm, pfflags);
#endif
}

#ifdef AI_BASE_FLOAT64_F_ULONG
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_F_UINT AiBaseFloat64DivremU(AI_BASE_FLOAT64_F_UINT *pr, AI_BASE_FLOAT64_F_UINT ah, AI_BASE_FLOAT64_F_UINT al, AI_BASE_FLOAT64_F_UINT b)
{
    AI_BASE_FLOAT64_F_ULONG a;
    a = ((AI_BASE_FLOAT64_F_ULONG)ah << AI_BASE_FLOAT64_F_SIZE) | al;
    *pr = (UINT32_T)(a % b);
    return (UINT32_T)(a / b);
}
#else
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* XXX: optimize */
AI_BASE_FLOAT64_F_UINT AiBaseFloat64DivremU(AI_BASE_FLOAT64_F_UINT *pr, AI_BASE_FLOAT64_F_UINT a1, AI_BASE_FLOAT64_F_UINT a0, AI_BASE_FLOAT64_F_UINT b)
{
    INT32_T i, qb, ab;

    assert(a1 < b);
    for(i = 0; i < AI_BASE_FLOAT64_F_SIZE; i++) {
        ab = a1 >> (AI_BASE_FLOAT64_F_SIZE - 1);
        a1 = (a1 << 1) | (a0 >> (AI_BASE_FLOAT64_F_SIZE - 1));
        if (ab || a1 >= b) {
            a1 -= b;
            qb = 1;
        } else {
            qb = 0;
        }
        a0 = (a0 << 1) | qb;
    }
    *pr = a1;
    return a0;
}
#endif

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_F_UINT AiBaseFloat64DivSf(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_F_UINT b, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm,
              UINT32_T *pfflags)
{
    UINT32_T a_sign, b_sign, r_sign;
    INT32_T a_exp, b_exp, r_exp;
    AI_BASE_FLOAT64_F_UINT a_mant, b_mant, r_mant, r;

    a_sign = a >> (AI_BASE_FLOAT64_F_SIZE - 1);
    b_sign = b >> (AI_BASE_FLOAT64_F_SIZE - 1);
    r_sign = a_sign ^ b_sign;
    a_exp = (a >> AI_BASE_FLOAT64_MANT_SIZE) & AI_BASE_FLOAT64_EXP_MASK;
    b_exp = (b >> AI_BASE_FLOAT64_MANT_SIZE) & AI_BASE_FLOAT64_EXP_MASK;
    a_mant = a & AI_BASE_FLOAT64_MANT_MASK;
    b_mant = b & AI_BASE_FLOAT64_MANT_MASK;
    if (a_exp == AI_BASE_FLOAT64_EXP_MASK) {
        if (a_mant != 0 || AiBaseFloat64IsnanSf(b)) {
            if (AiBaseFloat64IssignanSf(a) || AiBaseFloat64IssignanSf(b)) {
                *pfflags |= AI_BASE_FLOAT64_FFLAG_INVALID_OP;
            }
            return AI_BASE_FLOAT64_F_QNAN;
        } else if (b_exp == AI_BASE_FLOAT64_EXP_MASK) {
            *pfflags |= AI_BASE_FLOAT64_FFLAG_INVALID_OP;
            return AI_BASE_FLOAT64_F_QNAN;
        } else {
            return AiBaseFloat64PackSf(r_sign, AI_BASE_FLOAT64_EXP_MASK, 0);
        }
    } else if (b_exp == AI_BASE_FLOAT64_EXP_MASK) {
        if (b_mant != 0) {
            if (AiBaseFloat64IssignanSf(a) || AiBaseFloat64IssignanSf(b)) {
                *pfflags |= AI_BASE_FLOAT64_FFLAG_INVALID_OP;
            }
            return AI_BASE_FLOAT64_F_QNAN;
        } else {
            return AiBaseFloat64PackSf(r_sign, 0, 0);
        }
    }

    if (b_exp == 0) {
        if (b_mant == 0) { 
            /* zero */
            if (a_exp == 0 && a_mant == 0) {
                *pfflags |= AI_BASE_FLOAT64_FFLAG_INVALID_OP;
                return AI_BASE_FLOAT64_F_QNAN;
            } else {
                *pfflags |= AI_BASE_FLOAT64_FFLAG_DIVIDE_ZERO;
                return AiBaseFloat64PackSf(r_sign, AI_BASE_FLOAT64_EXP_MASK, 0);
            }
        }
        b_mant = AiBaseFloat64NormalizeSubnormalSf(&b_exp, b_mant);
    } else {
        b_mant |= (AI_BASE_FLOAT64_F_UINT)1 << AI_BASE_FLOAT64_MANT_SIZE;
    }
    if (a_exp == 0) {
        if (a_mant == 0)
            return AiBaseFloat64PackSf(r_sign, 0, 0); /* zero */
        a_mant = AiBaseFloat64NormalizeSubnormalSf(&a_exp, a_mant);
    } else {
        a_mant |= (AI_BASE_FLOAT64_F_UINT)1 << AI_BASE_FLOAT64_MANT_SIZE;
    }
    r_exp = a_exp - b_exp + (1 << (AI_BASE_FLOAT64_EXP_SIZE - 1)) - 1;
    r_mant = AiBaseFloat64DivremU(&r, a_mant, 0, b_mant << 2);
    if (r != 0)
        r_mant |= 1;
    return AiBaseFloat64NormalizeSf(r_sign, r_exp, r_mant, rm, pfflags);
}

#ifdef AI_BASE_FLOAT64_F_ULONG
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* compute sqrt(a) with a = ah*2^AI_BASE_FLOAT64_F_SIZE+al and a < 2^(AI_BASE_FLOAT64_F_SIZE - 2)
   return true if not exact square. */
INT32_T AiBaseFloat64SqrtremU(AI_BASE_FLOAT64_F_UINT *pr, AI_BASE_FLOAT64_F_UINT ah, AI_BASE_FLOAT64_F_UINT al)
{
    AI_BASE_FLOAT64_F_ULONG a, u, s;
    INT32_T l, inexact;

    /* 2^l >= a */
    if (ah != 0) {
        l = 2 * AI_BASE_FLOAT64_F_SIZE - AiBaseFloat64Clz(ah - 1);
    } else {
        if (al == 0) {
            *pr = 0;
            return 0;
        }
        l = AI_BASE_FLOAT64_F_SIZE - AiBaseFloat64Clz(al - 1);
    }
    a = ((AI_BASE_FLOAT64_F_ULONG)ah << AI_BASE_FLOAT64_F_SIZE) | al;
    u = (AI_BASE_FLOAT64_F_ULONG)1 << ((l + 1) / 2);
    for(;;) {
        s = u;
        u = ((a / s) + s) / 2;
        if (u >= s)
            break;
    }
    inexact = (a - s * s) != 0;
    *pr = (UINT32_T)s;
    return inexact;
}

#else
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64SqrtremU(AI_BASE_FLOAT64_F_UINT *pr, AI_BASE_FLOAT64_F_UINT a1, AI_BASE_FLOAT64_F_UINT a0)
{
    INT32_T l, inexact;
    AI_BASE_FLOAT64_F_UINT u, s, r, q, sq0, sq1;

    /* 2^l >= a */
    if (a1 != 0) {
        l = 2 * AI_BASE_FLOAT64_F_SIZE - AiBaseFloat64Clz(a1 - 1);
    } else {
        if (a0 == 0) {
            *pr = 0;
            return 0;
        }
        l = AI_BASE_FLOAT64_F_SIZE - AiBaseFloat64Clz(a0 - 1);
    }
    u = (AI_BASE_FLOAT64_F_UINT)1 << ((l + 1) / 2);
    for(;;) {
        s = u;
        q = AiBaseFloat64DivremU(&r, a1, a0, s);
        u = (q + s) / 2;
        if (u >= s)
            break;
    }
    sq1 = AiBaseFloat64MulU(&sq0, s, s);
    inexact = (sq0 != a0 || sq1 != a1);
    *pr = s;
    return inexact;
}
#endif

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_F_UINT AiBaseFloat64SqrtSf(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm,
               UINT32_T *pfflags)
{
    UINT32_T a_sign;
    INT32_T a_exp;
    AI_BASE_FLOAT64_F_UINT a_mant;

    a_sign = a >> (AI_BASE_FLOAT64_F_SIZE - 1);
    a_exp = (a >> AI_BASE_FLOAT64_MANT_SIZE) & AI_BASE_FLOAT64_EXP_MASK;
    a_mant = a & AI_BASE_FLOAT64_MANT_MASK;
    if (a_exp == AI_BASE_FLOAT64_EXP_MASK) {
        if (a_mant != 0) {
            if (AiBaseFloat64IssignanSf(a)) {
                *pfflags |= AI_BASE_FLOAT64_FFLAG_INVALID_OP;
            }
            return AI_BASE_FLOAT64_F_QNAN;
        } else if (a_sign) {
            goto neg_error;
        } else {
            return a; /* +infinity */
        }
    }
    if (a_sign) {
        if (a_exp == 0 && a_mant == 0)
            return a; /* -zero */
    neg_error:
        *pfflags |= AI_BASE_FLOAT64_FFLAG_INVALID_OP;
        return AI_BASE_FLOAT64_F_QNAN;
    }
    if (a_exp == 0) {
        if (a_mant == 0)
            return AiBaseFloat64PackSf(0, 0, 0); /* zero */
        a_mant = AiBaseFloat64NormalizeSubnormalSf(&a_exp, a_mant);
    } else {
        a_mant |= (AI_BASE_FLOAT64_F_UINT)1 << AI_BASE_FLOAT64_MANT_SIZE;
    }
    a_exp -= AI_BASE_FLOAT64_EXP_MASK / 2;
    /* simpler to handle an even exponent */
    if (a_exp & 1) {
        a_exp--;
        a_mant <<= 1;
    }
    a_exp = (a_exp >> 1) + AI_BASE_FLOAT64_EXP_MASK / 2;
    a_mant <<= (AI_BASE_FLOAT64_F_SIZE - 4 - AI_BASE_FLOAT64_MANT_SIZE);
    if (AiBaseFloat64SqrtremU(&a_mant, a_mant, 0))
        a_mant |= 1;
    return AiBaseFloat64NormalizeSf(a_sign, a_exp, a_mant, rm, pfflags);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* comparisons */
AI_BASE_FLOAT64_F_UINT AiBaseFloat64MinSf32(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_F_UINT b, UINT32_T *pfflags)
{
    UINT32_T a_sign, b_sign;

    if (AiBaseFloat64IsnanSf(a) || AiBaseFloat64IsnanSf(b)) {
        if (AiBaseFloat64IssignanSf(a) || AiBaseFloat64IssignanSf(b)) {
            *pfflags |= AI_BASE_FLOAT64_FFLAG_INVALID_OP;
            return AI_BASE_FLOAT64_F_QNAN;
        } else if (AiBaseFloat64IsnanSf(a)) {
            if (AiBaseFloat64IsnanSf(b)) 
                return AI_BASE_FLOAT64_F_QNAN;
            else
                return b;
        } else {
            return a;
        }
    }
    a_sign = a >> (AI_BASE_FLOAT64_F_SIZE - 1);
    b_sign = b >> (AI_BASE_FLOAT64_F_SIZE - 1);

    if (a_sign != b_sign) {
        if (a_sign)
            return a;
        else
            return b;
    } else {
        if ((a < b) ^ a_sign)
            return a;
        else
            return b;
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_F_UINT AiBaseFloat64MaxSf32(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_F_UINT b, UINT32_T *pfflags)
{
    UINT32_T a_sign, b_sign;

    if (AiBaseFloat64IsnanSf(a) || AiBaseFloat64IsnanSf(b)) {
        if (AiBaseFloat64IssignanSf(a) || AiBaseFloat64IssignanSf(b)) {
            *pfflags |= AI_BASE_FLOAT64_FFLAG_INVALID_OP;
            return AI_BASE_FLOAT64_F_QNAN;
        } else if (AiBaseFloat64IsnanSf(a)) {
            if (AiBaseFloat64IsnanSf(b)) 
                return AI_BASE_FLOAT64_F_QNAN;
            else
                return b;
        } else {
            return a;
        }
    }
    a_sign = a >> (AI_BASE_FLOAT64_F_SIZE - 1);
    b_sign = b >> (AI_BASE_FLOAT64_F_SIZE - 1);

    if (a_sign != b_sign) {
        if (a_sign)
            return b;
        else
            return a;
    } else {
        if ((a < b) ^ a_sign)
            return b;
        else
            return a;
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64EqQuietSf32(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_F_UINT b, UINT32_T *pfflags)
{
    if (AiBaseFloat64IsnanSf(a) || AiBaseFloat64IsnanSf(b)) {
        if (AiBaseFloat64IssignanSf(a) || AiBaseFloat64IssignanSf(b)) {
            *pfflags |= AI_BASE_FLOAT64_FFLAG_INVALID_OP;
        }
        return 0;
    }

    if ((AI_BASE_FLOAT64_F_UINT)((a | b) << 1) == 0)
        return 1; /* zero case */
    return (a == b);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64LeSf32(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_F_UINT b, UINT32_T *pfflags)
{
    UINT32_T a_sign, b_sign;

    if (AiBaseFloat64IsnanSf(a) || AiBaseFloat64IsnanSf(b)) {
        *pfflags |= AI_BASE_FLOAT64_FFLAG_INVALID_OP;
        return 0;
    }

    a_sign = a >> (AI_BASE_FLOAT64_F_SIZE - 1);
    b_sign = b >> (AI_BASE_FLOAT64_F_SIZE - 1);
    if (a_sign != b_sign) {
        return (a_sign || ((AI_BASE_FLOAT64_F_UINT)((a | b) << 1) == 0));
    } else {
        if (a_sign) {
            return (a >= b);
        } else {
            return (a <= b);
        }
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64LtSf32(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_F_UINT b, UINT32_T *pfflags)
{
    UINT32_T a_sign, b_sign;

    if (AiBaseFloat64IsnanSf(a) || AiBaseFloat64IsnanSf(b)) {
        *pfflags |= AI_BASE_FLOAT64_FFLAG_INVALID_OP;
        return 0;
    }

    a_sign = a >> (AI_BASE_FLOAT64_F_SIZE - 1);
    b_sign = b >> (AI_BASE_FLOAT64_F_SIZE - 1);
    if (a_sign != b_sign) {
        return (a_sign && ((AI_BASE_FLOAT64_F_UINT)((a | b) << 1) != 0));
    } else {
        if (a_sign) {
            return (a > b);
        } else {
            return (a < b);
        }
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
UINT32_T AiBaseFloat64FclassSf32(AI_BASE_FLOAT64_F_UINT a)
{
    UINT32_T a_sign;
    INT32_T a_exp;
    AI_BASE_FLOAT64_F_UINT a_mant;
    UINT32_T ret;

    a_sign = a >> (AI_BASE_FLOAT64_F_SIZE - 1);
    a_exp = (a >> AI_BASE_FLOAT64_MANT_SIZE) & AI_BASE_FLOAT64_EXP_MASK;
    a_mant = a & AI_BASE_FLOAT64_MANT_MASK;
    if (a_exp == AI_BASE_FLOAT64_EXP_MASK) {
        if (a_mant != 0) {
            if (a_mant & AI_BASE_FLOAT64_QNAN_MASK)
                ret = AI_BASE_FLOAT64_FCLASS_QNAN;
            else
                ret = AI_BASE_FLOAT64_FCLASS_SNAN;
        } else {
            if (a_sign)
                ret = AI_BASE_FLOAT64_FCLASS_NINF;
            else
                ret = AI_BASE_FLOAT64_FCLASS_PINF;
        }
    } else if (a_exp == 0) {
        if (a_mant == 0) {
            if (a_sign)
                ret = AI_BASE_FLOAT64_FCLASS_NZERO;
            else
                ret = AI_BASE_FLOAT64_FCLASS_PZERO;
        } else {
            if (a_sign)
                ret = AI_BASE_FLOAT64_FCLASS_NSUBNORMAL;
            else
                ret = AI_BASE_FLOAT64_FCLASS_PSUBNORMAL;
        }
    } else {
        if (a_sign)
            ret = AI_BASE_FLOAT64_FCLASS_NNORMAL;
        else
            ret = AI_BASE_FLOAT64_FCLASS_PNORMAL;
    }
    return ret;
}

/*------------------------- End ---------------------------------------------*/
#endif
