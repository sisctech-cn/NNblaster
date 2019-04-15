/**
 ******************************************************************************
 * file    ai_base_float_oepr.c
 * author  sunlingge
 * version V100
 * brief   the ai base float oper driver.
 * description
 *	1. double: sign(63 bit), exponent(52-62 bit), fraction(00-51 bit)
 *	2. float:  sign(31 bit), exponent(23-30 bit), fraction(00-22 bit)
 *  3. fp16:   sign(15 bit), exponent(10-14 bit), fraction(00-09 bit)
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"
#pragma warning(disable:4996)

#if (AI_BASE_FLOAT_FP32 == 1)
/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/
ERROR_T AiServerLogFp16(UINT8_T *str);

/*------------------------- Variable Definition------------------------------*/
const UINT8_T *g_ai_base_float32_op_str[AI_BASE_FLOAT32_OP_COUNT] = {
    "mul",
    "add",
    "sub",
    "rint",
    "round",
    "cmp_eq",
    "cmp_lt",
    "cmp_le",
    "recip",
    "div",
    "fmod",
    "rem",
    "rsqrt",
    "sqrt",
    "or",
    "xor",
    "and",
    "can_round",
    "mul_l2radix",
    "tna_cmd_l2radix",
    "atof",
    "ftoa",
    "exp",
    "log",
    "cos",
    "sin",
    "tan",
    "atan",
    "atan2",
    "asin",
    "acos",
    "pow",
};

const UINT8_T *g_ai_base_float32_rnd_str[AI_BASE_FLOAT32_RND_COUNT] = {
    "N",
    "Z",
    "D",
    "U",
    "NA",
    "F",
};

/*------------------------- Function Definition -----------------------------*/
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat32SetSpecial(AI_BASE_FLOAT32_BF_T *a, INT32_T idx)
{
    switch(idx) {
    case 0:
        AiBaseFloat32BfSetUi(a, 0);
        break;
    case 1:
        AiBaseFloat32BfSetUi(a, 0);
        a->sign = 1; /* -0 */
        break;
    case 2:
        AiBaseFloat32BfSetSi(a, 0);
        a->expn = AI_BASE_FLOAT32_BF_EXP_INF;
        break;
    case 3:
        AiBaseFloat32BfSetSi(a, 0);
        a->expn = AI_BASE_FLOAT32_BF_EXP_INF;
        a->sign = 1;
        break;
    case 4:
        AiBaseFloat32BfSetSi(a, 1);
        break;
    case 5:
        AiBaseFloat32BfSetSi(a, -1);
        break;
    case 6:
        AiBaseFloat32BfSetNan(a);
        break;
    default:
        abort();
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat32MpRandinit(AI_BASE_FLOAT32_MP_RANDSTATE_T *state, UINT64_T seed)
{
    state->val = seed;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
UINT64_T AiBaseFloat32MpRandom64(AI_BASE_FLOAT32_MP_RANDSTATE_T *s)
{
    s->val = s->val * 6364136223846793005 + 1;
    /* avoid bad modulo properties 
       XXX: use mersenne twistter generator */
    return (s->val << 32) | (s->val >> 32);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* random number between 0 and 1 with large sequences of identical bits */
void AiBaseFloat32BfRrandom(AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_MP_RANDSTATE_T *state)
{
    AI_BASE_FLOAT32_SLIMB_T n, max_run_len, cur_len, j, len, bit_index, nb_bits;
    INT32_T cur_state, m;
    
    n = (prec + AI_BASE_FLOAT32_LIMB_BITS - 1) / AI_BASE_FLOAT32_LIMB_BITS;

    /* same idea as GMP. It would be probably better to use a non uniform law */
    m = AiBaseFloat32MpRandom64(state) % 4 + 1;
    max_run_len = AiBaseFloat32BfMax(prec / m, 1);
    cur_state = AiBaseFloat32MpRandom64(state) & 1;
    cur_len = AiBaseFloat32MpRandom64(state) % max_run_len + 1;
    nb_bits = n * AI_BASE_FLOAT32_LIMB_BITS;
    
    AiBaseFloat32BfResize(a, n);
    memset(a->tab, 0, sizeof(AI_BASE_FLOAT32_LIMB_T) * n);
    bit_index = nb_bits - prec;
    while (bit_index < nb_bits) {
        len = AiBaseFloat32BfMin(cur_len, nb_bits - bit_index);
        if (cur_state) {
            /* XXX: inefficient */
            for(j = 0; j < len; j++) {
                a->tab[bit_index >> AI_BASE_FLOAT32_LIMB_LOG2_BITS] |= (AI_BASE_FLOAT32_LIMB_T)((AI_BASE_FLOAT32_LIMB_T)1 << (bit_index & (AI_BASE_FLOAT32_LIMB_BITS - 1)));
                bit_index++;
            }
        }
        bit_index += len;
        cur_len -= len;
        if (cur_len == 0) {
            cur_len = AiBaseFloat32MpRandom64(state) % max_run_len + 1;
            cur_state ^= 1;
        }
    }
    a->sign = 0;
    a->expn = 0;
    AiBaseFloat32BfNormalizeAndRound(a, prec, AI_BASE_FLOAT32_RND_Z);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat32BfRrandomLarge(AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_MP_RANDSTATE_T *s)
{
    AI_BASE_FLOAT32_LIMB_T prec1;
    prec1 = AiBaseFloat32MpRandom64(s) % (2 * prec) + 1;
    AiBaseFloat32BfRrandom(a, prec1, s);
    a->sign = AiBaseFloat32MpRandom64(s) & 1;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* random integer with 0 to prec bits */
void AiBaseFloat32BfRrandomInit(AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_MP_RANDSTATE_T *rnd_state)
{
    AI_BASE_FLOAT32_LIMB_T prec1;
    prec1 = AiBaseFloat32MpRandom64(rnd_state) % prec + 1;
    AiBaseFloat32BfRrandom(a, prec1, rnd_state);
    if (a->expn != AI_BASE_FLOAT32_BF_EXP_ZERO)
        a->expn += prec1;
    a->sign = AiBaseFloat32MpRandom64(rnd_state) & 1;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* random integer with long sequences of '0' and '1' */
UINT64_T AiBaseFloat32RrandomU(INT32_T len, AI_BASE_FLOAT32_MP_RANDSTATE_T *s)
{
    INT32_T bit, pos, n, end;
    UINT64_T a;
    
    bit = AiBaseFloat32MpRandom64(s) & 1;
    pos = 0;
    a = 0;
    for(;;) {
        n = (AiBaseFloat32MpRandom64(s) % len) + 1;
        end = pos + n;
        if (end > len)
            end = len;
        if (bit) {
            n = end - pos;
            a |= ((UINT64_T)(1 << n) - 1) << pos;
        }
        if (end >= len)
            break;
        pos = end;
        bit ^= 1;
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
UINT64_T AiBaseFloat32RrandomSf64(AI_BASE_FLOAT32_MP_RANDSTATE_T *s)
{
    UINT32_T a_exp, a_sign;
    UINT64_T a_mant;
    a_sign = AiBaseFloat32MpRandom64(s) & 1;

    /* generate exponent close to the min/max more often than random */
    switch(AiBaseFloat32MpRandom64(s) & 15) {
    case 0:
        a_exp = (AiBaseFloat32MpRandom64(s) % (2 * AI_BASE_FLOAT32_F64_MANT_SIZE)) & AI_BASE_FLOAT32_F64_EXP_MASK;
        break;
    case 1:
        a_exp = (AI_BASE_FLOAT32_F64_EXP_MASK - (AiBaseFloat32MpRandom64(s) % (2 * AI_BASE_FLOAT32_F64_MANT_SIZE))) & AI_BASE_FLOAT32_F64_EXP_MASK;
        break;
    default:
        a_exp = AiBaseFloat32MpRandom64(s) & AI_BASE_FLOAT32_F64_EXP_MASK;
        break;
    }
    a_mant = AiBaseFloat32RrandomU(AI_BASE_FLOAT32_F64_MANT_SIZE, s);
    return ((UINT64_T)a_sign << 63) | ((UINT64_T)a_exp << AI_BASE_FLOAT32_F64_MANT_SIZE) | a_mant;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT64_T AiBaseFloat32GetClockMsec(void)
{
#if 0
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000LL + (tv.tv_usec / 1000);
#endif
	return 0;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
UINT64_T AiBaseFloat32GetCycles(void)
{
	return 0;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat32SoftfpGetRndMode(AI_BASE_FLOAT32_RND_ENUM rnd_mode)
{
    switch(rnd_mode) {
    case AI_BASE_FLOAT32_RND_N:
        return AI_BASE_FLOAT32_RM_RNE;
    case AI_BASE_FLOAT32_RND_Z:
        return AI_BASE_FLOAT32_RM_RTZ;
    case AI_BASE_FLOAT32_RND_U:
        return AI_BASE_FLOAT32_RM_RUP;
    case AI_BASE_FLOAT32_RND_D:
        return AI_BASE_FLOAT32_RM_RDN;
    case AI_BASE_FLOAT32_RND_NA:
        return AI_BASE_FLOAT32_RM_RMM;
    default:
        break;
    }
	return AI_BASE_FLOAT32_RM_RNE;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat32SoftfpSetStatus(UINT32_T fflags)
{
    INT32_T ret = 0;
    if (fflags & AI_BASE_FLOAT32_FFLAG_INVALID_OP)
        ret |= AI_BASE_FLOAT32_BF_ST_INVALID_OP;
    if (fflags & AI_BASE_FLOAT32_FFLAG_DIVIDE_ZERO)
        ret |= AI_BASE_FLOAT32_BF_ST_DIVIDE_ZERO;
    if (fflags & AI_BASE_FLOAT32_FFLAG_OVERFLOW)
        ret |= AI_BASE_FLOAT32_BF_ST_OVERFLOW;
    if (fflags & AI_BASE_FLOAT32_FFLAG_UNDERFLOW)
        ret |= AI_BASE_FLOAT32_BF_ST_UNDERFLOW;
    if (fflags & AI_BASE_FLOAT32_FFLAG_INEXACT)
        ret |= AI_BASE_FLOAT32_BF_ST_INEXACT;
    return ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseUpdateDlgStatus(UINT8_T *format, ...);
UINT64_T g_ai_base_float32_flops = 0;
INT32_T AiBaseFloat32BfExecOp(AI_BASE_FLOAT32_OP_ENUM op, AI_BASE_FLOAT32_BF_T *r, AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_BF_T *b,
               AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags)
{
    INT32_T ret = 0;

	g_ai_base_float32_flops++;
	if ((g_ai_base_float32_flops % 5000000) == 0) {
		AiBaseUpdateDlgStatus("g_ai_base_float32_flops=%lld", g_ai_base_float32_flops);
	}

	AiBaseFloat32BfInit(r);
    switch(op) {
    case AI_BASE_FLOAT32_OP_MUL:
        ret = AiBaseFloat32BfMul(r, a, b, prec, flags);
        break;
    case AI_BASE_FLOAT32_OP_ADD:
        ret = AiBaseFloat32BfAdd(r, a, b, prec, flags);
        break;
    case AI_BASE_FLOAT32_OP_SUB:
        ret = AiBaseFloat32BfSub(r, a, b, prec, flags);
        break;
    case AI_BASE_FLOAT32_OP_RINT:
        AiBaseFloat32BfSet(r, a);
        ret = AiBaseFloat32BfRint(r, prec, flags);
        break;
    case AI_BASE_FLOAT32_OP_ROUND:
        AiBaseFloat32BfSet(r, a);
        ret = AiBaseFloat32BfRound(r, prec, flags);
        break;
    case AI_BASE_FLOAT32_OP_CMP_EQ:
        ret = AiBaseFloat32BfCmpEq(a, b);
        break;
    case AI_BASE_FLOAT32_OP_CMP_LT:
        ret = AiBaseFloat32BfCmpLt(a, b);
        break;
    case AI_BASE_FLOAT32_OP_CMP_LE:
        ret = AiBaseFloat32BfCmpLe(a, b);
        break;
    case AI_BASE_FLOAT32_OP_RECIP:
        AiBaseFloat32BfRecip(r, a, prec);
        break;
    case AI_BASE_FLOAT32_OP_DIV:
        ret = AiBaseFloat32BfDiv(r, a, b, prec, flags);
        break;
    case AI_BASE_FLOAT32_OP_FMOD:
        ret = AiBaseFloat32BfFmod(r, a, b, prec, flags);
        break;
    case AI_BASE_FLOAT32_OP_REM:
        ret = AiBaseFloat32BfRemainder(r, a, b, prec, flags);
        break;
    case AI_BASE_FLOAT32_OP_RSQRT:
        AiBaseFloat32BfRsqrt(r, a, prec);
        break;
    case AI_BASE_FLOAT32_OP_SQRT:
        ret = AiBaseFloat32BfSqrt(r, a, prec, flags);
        break;
    case AI_BASE_FLOAT32_OP_OR:
        AiBaseFloat32BfLogicOr(r, a, b);
        break;
    case AI_BASE_FLOAT32_OP_XOR:
        AiBaseFloat32BfLogicXor(r, a, b);
        break;
    case AI_BASE_FLOAT32_OP_AND:
        AiBaseFloat32BfLogicAnd(r, a, b);
        break;
    case AI_BASE_FLOAT32_OP_EXP:
        ret = AiBaseFloat32BfExp(r, a, prec, flags);
        break;
    case AI_BASE_FLOAT32_OP_LOG:
        ret = AiBaseFloat32BfLog(r, a, prec, flags);
        break;
    case AI_BASE_FLOAT32_OP_COS:
        ret = AiBaseFloat32BfCos(r, a, prec, flags);
        break;
    case AI_BASE_FLOAT32_OP_SIN:
        ret = AiBaseFloat32BfSin(r, a, prec, flags);
        break;
    case AI_BASE_FLOAT32_OP_TAN:
        ret = AiBaseFloat32BfTan(r, a, prec, flags);
        break;
    case AI_BASE_FLOAT32_OP_ATAN:
        ret = AiBaseFloat32BfAtan(r, a, prec, flags);
        break;
    case AI_BASE_FLOAT32_OP_ATAN2:
        ret = AiBaseFloat32BfAtan2(r, a, b, prec, flags);
        break;
    case AI_BASE_FLOAT32_OP_ASIN:
        ret = AiBaseFloat32BfAsin(r, a, prec, flags);
        break;
    case AI_BASE_FLOAT32_OP_ACOS:
        ret = AiBaseFloat32BfAcos(r, a, prec, flags);
        break;
    case AI_BASE_FLOAT32_OP_POW:
        ret = AiBaseFloat32BfPow(r, a, b, prec, flags);
        break;
    default:
        break;
    }

    return ret;
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
INT32_T AiBaseFloat32CmpEq(FLOAT32_T a, FLOAT32_T b)
{
	AI_BASE_FLOAT32_OP_ENUM op;
	AI_BASE_FLOAT32_BF_T r_bf;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_BF_T b_bf;
	AI_BASE_FLOAT32_LIMB_T prec;
	AI_BASE_FLOAT32_BF_FLAGS_T flags;
	FLOAT32_T r;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;
	INT32_T ret;

	op = AI_BASE_FLOAT32_OP_CMP_EQ;
	AiBaseFloat32BfSetFloat32(&a_bf, a);
	AiBaseFloat32BfSetFloat32(&b_bf, b);
	prec = AI_BASE_FLOAT32_FP32_ROUND_PREC;
	flags = AI_BASE_FLOAT32_BF_RNDNA | AI_BASE_FLOAT32_BF_FLAG_SUBNORMAL | AiBaseFloat32BfSetExpBits(AI_BASE_FLOAT32_FP32_EXP_LENGTH);
	ret = AiBaseFloat32BfExecOp(op, &r_bf, &a_bf, &b_bf, prec, flags);
	AiBaseFloat32BfGetFloat32(&r_bf, &r, rnd_mode);

#if 1
	{
		FLOAT32_T a_32;
		FLOAT32_T b_32;
		INT32_T r_32;

		a_32 = (a);
		b_32 = (b);
		r_32 = (INT32_T)(a_32 == b_32);
		if (r_32 != ret) {
			AiBaseLogErrorCritial();
			return ret;
		}
	}
#endif

	return ret;
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
INT32_T AiBaseFloat32CmpLt(FLOAT32_T a, FLOAT32_T b)
{
	AI_BASE_FLOAT32_OP_ENUM op;
	AI_BASE_FLOAT32_BF_T r_bf;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_BF_T b_bf;
	AI_BASE_FLOAT32_LIMB_T prec;
	AI_BASE_FLOAT32_BF_FLAGS_T flags;
	FLOAT32_T r;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;
	INT32_T ret;

	op = AI_BASE_FLOAT32_OP_CMP_LT;
	AiBaseFloat32BfSetFloat32(&a_bf, a);
	AiBaseFloat32BfSetFloat32(&b_bf, b);
	prec = AI_BASE_FLOAT32_FP32_ROUND_PREC;
	flags = AI_BASE_FLOAT32_BF_RNDNA | AI_BASE_FLOAT32_BF_FLAG_SUBNORMAL | AiBaseFloat32BfSetExpBits(AI_BASE_FLOAT32_FP32_EXP_LENGTH);
	ret = AiBaseFloat32BfExecOp(op, &r_bf, &a_bf, &b_bf, prec, flags);
	AiBaseFloat32BfGetFloat32(&r_bf, &r, rnd_mode);

#if 1
	{
		FLOAT32_T a_32;
		FLOAT32_T b_32;
		INT32_T r_32;

		a_32 = (a);
		b_32 = (b);
		r_32 = (INT32_T)(a_32 < b_32);
		if (r_32 != ret) {
			AiBaseLogErrorCritial();
			return ret;
		}
	}
#endif

	return ret;
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
INT32_T AiBaseFloat32CmpLe(FLOAT32_T a, FLOAT32_T b)
{
	AI_BASE_FLOAT32_OP_ENUM op;
	AI_BASE_FLOAT32_BF_T r_bf;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_BF_T b_bf;
	AI_BASE_FLOAT32_LIMB_T prec;
	AI_BASE_FLOAT32_BF_FLAGS_T flags;
	FLOAT32_T r;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;
	INT32_T ret;

	op = AI_BASE_FLOAT32_OP_CMP_LE;
	AiBaseFloat32BfSetFloat32(&a_bf, a);
	AiBaseFloat32BfSetFloat32(&b_bf, b);
	prec = AI_BASE_FLOAT32_FP32_ROUND_PREC;
	flags = AI_BASE_FLOAT32_BF_RNDNA | AI_BASE_FLOAT32_BF_FLAG_SUBNORMAL | AiBaseFloat32BfSetExpBits(AI_BASE_FLOAT32_FP32_EXP_LENGTH);
	ret = AiBaseFloat32BfExecOp(op, &r_bf, &a_bf, &b_bf, prec, flags);
	AiBaseFloat32BfGetFloat32(&r_bf, &r, rnd_mode);

#if 1
	{
		FLOAT32_T a_32;
		FLOAT32_T b_32;
		INT32_T r_32;

		a_32 = (a);
		b_32 = (b);
		r_32 = (INT32_T)(a_32 <= b_32);
		if (r_32 != ret) {
			AiBaseLogErrorCritial();
			return ret;
		}
	}
#endif

	return ret;
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
INT32_T AiBaseFloat32CmpGt(FLOAT32_T a, FLOAT32_T b)
{
	AI_BASE_FLOAT32_OP_ENUM op;
	AI_BASE_FLOAT32_BF_T r_bf;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_BF_T b_bf;
	AI_BASE_FLOAT32_LIMB_T prec;
	AI_BASE_FLOAT32_BF_FLAGS_T flags;
	FLOAT32_T r;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;
	INT32_T ret;

	op = AI_BASE_FLOAT32_OP_CMP_LT;
	AiBaseFloat32BfSetFloat32(&a_bf, b);
	AiBaseFloat32BfSetFloat32(&b_bf, a);
	prec = AI_BASE_FLOAT32_FP32_ROUND_PREC;
	flags = AI_BASE_FLOAT32_BF_RNDNA | AI_BASE_FLOAT32_BF_FLAG_SUBNORMAL | AiBaseFloat32BfSetExpBits(AI_BASE_FLOAT32_FP32_EXP_LENGTH);
	ret = AiBaseFloat32BfExecOp(op, &r_bf, &a_bf, &b_bf, prec, flags);
	AiBaseFloat32BfGetFloat32(&r_bf, &r, rnd_mode);

#if 1
	{
		FLOAT32_T a_32;
		FLOAT32_T b_32;
		INT32_T r_32;

		a_32 = (a);
		b_32 = (b);
		r_32 = (INT32_T)(a_32 > b_32);
		if (r_32 != ret) {
			AiBaseLogErrorCritial();
			return ret;
		}
	}
#endif

	return ret;
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
INT32_T AiBaseFloat32CmpGe(FLOAT32_T a, FLOAT32_T b)
{
	AI_BASE_FLOAT32_OP_ENUM op;
	AI_BASE_FLOAT32_BF_T r_bf;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_BF_T b_bf;
	AI_BASE_FLOAT32_LIMB_T prec;
	AI_BASE_FLOAT32_BF_FLAGS_T flags;
	FLOAT32_T r;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;
	INT32_T ret;

	op = AI_BASE_FLOAT32_OP_CMP_LE;
	AiBaseFloat32BfSetFloat32(&a_bf, b);
	AiBaseFloat32BfSetFloat32(&b_bf, a);
	prec = AI_BASE_FLOAT32_FP32_ROUND_PREC;
	flags = AI_BASE_FLOAT32_BF_RNDNA | AI_BASE_FLOAT32_BF_FLAG_SUBNORMAL | AiBaseFloat32BfSetExpBits(AI_BASE_FLOAT32_FP32_EXP_LENGTH);
	ret = AiBaseFloat32BfExecOp(op, &r_bf, &a_bf, &b_bf, prec, flags);
	AiBaseFloat32BfGetFloat32(&r_bf, &r, rnd_mode);

#if 1
	{
		FLOAT32_T a_32;
		FLOAT32_T b_32;
		INT32_T r_32;

		a_32 = (a);
		b_32 = (b);
		r_32 = (INT32_T)(a_32 >= b_32);
		if (r_32 != ret) {
			AiBaseLogErrorCritial();
			return ret;
		}
	}
#endif

	return ret;
}


/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
FLOAT32_T AiBaseFloat32Add(FLOAT32_T a, FLOAT32_T b)
{
	AI_BASE_FLOAT32_OP_ENUM op;
	AI_BASE_FLOAT32_BF_T r_bf;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_BF_T b_bf;
	AI_BASE_FLOAT32_LIMB_T prec;
	AI_BASE_FLOAT32_BF_FLAGS_T flags;
	FLOAT32_T r;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;

	op = AI_BASE_FLOAT32_OP_ADD;
	AiBaseFloat32BfSetFloat32(&a_bf, a);
	AiBaseFloat32BfSetFloat32(&b_bf, b);
	prec = AI_BASE_FLOAT32_FP32_ROUND_PREC;
	flags = AI_BASE_FLOAT32_BF_RNDNA | AI_BASE_FLOAT32_BF_FLAG_SUBNORMAL | AiBaseFloat32BfSetExpBits(AI_BASE_FLOAT32_FP32_EXP_LENGTH);
	AiBaseFloat32BfExecOp(op, &r_bf, &a_bf, &b_bf, prec, flags);
	AiBaseFloat32BfGetFloat32(&r_bf, &r, rnd_mode);

#if 1
	{
		FLOAT32_T a_32;
		FLOAT32_T b_32;
		FLOAT32_T r_32;
		FLOAT32_T r_cmp;

		a_32 = (a);
		b_32 = (b);
		r_32 = a_32+b_32;
		r_cmp = AiBaseFloat32CvtF32F16(r_32);
		if ((r_cmp != r) && 
			((r_cmp + 1) != r) &&
			(r_cmp != (r + 1))){
			AiBaseLogErrorCritial();
			return r;
		}
	}
#endif

	return r;
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
FLOAT32_T AiBaseFloat32Sub(FLOAT32_T a, FLOAT32_T b)
{
	AI_BASE_FLOAT32_OP_ENUM op;
	AI_BASE_FLOAT32_BF_T r_bf;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_BF_T b_bf;
	AI_BASE_FLOAT32_LIMB_T prec;
	AI_BASE_FLOAT32_BF_FLAGS_T flags;
	FLOAT32_T r;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;

	op = AI_BASE_FLOAT32_OP_SUB;
	AiBaseFloat32BfSetFloat32(&a_bf, a);
	AiBaseFloat32BfSetFloat32(&b_bf, b);
	prec = AI_BASE_FLOAT32_FP32_ROUND_PREC;
	flags = AI_BASE_FLOAT32_BF_RNDNA | AI_BASE_FLOAT32_BF_FLAG_SUBNORMAL | AiBaseFloat32BfSetExpBits(AI_BASE_FLOAT32_FP32_EXP_LENGTH);
	AiBaseFloat32BfExecOp(op, &r_bf, &a_bf, &b_bf, prec, flags);
	AiBaseFloat32BfGetFloat32(&r_bf, &r, rnd_mode);

#if 1
	{
		FLOAT32_T a_32;
		FLOAT32_T b_32;
		FLOAT32_T r_32;
		FLOAT32_T r_cmp;

		a_32 = (a);
		b_32 = (b);
		r_32 = a_32 - b_32;
		r_cmp = AiBaseFloat32CvtF32F16(r_32);
		if ((r_cmp != r) &&
			((r_cmp + 1) != r) &&
			(r_cmp != (r + 1))) {
			AiBaseLogErrorCritial();
			return r;
		}
	}
#endif

	return r;
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
FLOAT32_T AiBaseFloat32Mul(FLOAT32_T a, FLOAT32_T b)
{
	AI_BASE_FLOAT32_OP_ENUM op;
	AI_BASE_FLOAT32_BF_T r_bf;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_BF_T b_bf;
	AI_BASE_FLOAT32_LIMB_T prec;
	AI_BASE_FLOAT32_BF_FLAGS_T flags;
	FLOAT32_T r;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;

	op = AI_BASE_FLOAT32_OP_MUL;
	AiBaseFloat32BfSetFloat32(&a_bf, a);
	AiBaseFloat32BfSetFloat32(&b_bf, b);
	prec = AI_BASE_FLOAT32_FP32_ROUND_PREC;
	flags = AI_BASE_FLOAT32_BF_RNDNA | AI_BASE_FLOAT32_BF_FLAG_SUBNORMAL | AiBaseFloat32BfSetExpBits(AI_BASE_FLOAT32_FP32_EXP_LENGTH);
	AiBaseFloat32BfExecOp(op, &r_bf, &a_bf, &b_bf, prec, flags);
	AiBaseFloat32BfGetFloat32(&r_bf, &r, rnd_mode);

#if 1
	{
		FLOAT32_T a_32;
		FLOAT32_T b_32;
		FLOAT32_T r_32;
		FLOAT32_T r_cmp;

		a_32 = (a);
		b_32 = (b);
		r_32 = a_32*b_32;
		r_cmp = AiBaseFloat32CvtF32F16(r_32);
		if ((r_cmp != r) &&
			((r_cmp + 1) != r) &&
			(r_cmp != (r + 1))) {
			AiBaseLogErrorCritial();
			return r;
		}
	}
#endif

	return r;
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
FLOAT32_T AiBaseFloat32Div(FLOAT32_T a, FLOAT32_T b)
{
	AI_BASE_FLOAT32_OP_ENUM op;
	AI_BASE_FLOAT32_BF_T r_bf;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_BF_T b_bf;
	AI_BASE_FLOAT32_LIMB_T prec;
	AI_BASE_FLOAT32_BF_FLAGS_T flags;
	FLOAT32_T r;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;

	op = AI_BASE_FLOAT32_OP_DIV;
	AiBaseFloat32BfSetFloat32(&a_bf, a);
	AiBaseFloat32BfSetFloat32(&b_bf, b);
	prec = AI_BASE_FLOAT32_FP32_ROUND_PREC;
	flags = AI_BASE_FLOAT32_BF_RNDNA | AI_BASE_FLOAT32_BF_FLAG_SUBNORMAL | AiBaseFloat32BfSetExpBits(AI_BASE_FLOAT32_FP32_EXP_LENGTH);
	AiBaseFloat32BfExecOp(op, &r_bf, &a_bf, &b_bf, prec, flags);
	AiBaseFloat32BfGetFloat32(&r_bf, &r, rnd_mode);

#if 1
	{
		FLOAT32_T a_32;
		FLOAT32_T b_32;
		FLOAT32_T r_32;
		FLOAT32_T r_cmp;
		FLOAT32_T r_cmp_32;
		FLOAT32_T r_cmp_16;

		a_32 = (a);
		b_32 = (b);
		r_32 = a_32/b_32;
		r_cmp_32 = (r);
		r_cmp_16 = (r_32);
		r_cmp = (r_32);
		if ((r_cmp != r) &&
			((r_cmp + 1) != r) &&
			(r_cmp != (r + 1))) {
			AiBaseLogErrorCritial();
			return r;
		}
	}
#endif

	return r;
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
FLOAT32_T AiBaseFloat32Exp(FLOAT32_T a)
{
#if 1
	FLOAT32_T a_32;
	FLOAT32_T r_32;
	FLOAT32_T r_cmp;

	a_32 = (a);
	r_32 = (FLOAT32_T)exp(a_32);
	r_cmp = (r_32);
	return r_cmp;
#endif
#if 0
	AI_BASE_FLOAT32_OP_ENUM op;
	AI_BASE_FLOAT32_BF_T r_bf;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_BF_T b_bf;
	AI_BASE_FLOAT32_LIMB_T prec;
	AI_BASE_FLOAT32_BF_FLAGS_T flags;
	FLOAT32_T r;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;

	op = AI_BASE_FLOAT32_OP_EXP;
	AiBaseFloat32BfSetFloat32(&a_bf, a);
	prec = AI_BASE_FLOAT32_FP32_ROUND_PREC;
	flags = AI_BASE_FLOAT32_BF_RNDNA | AI_BASE_FLOAT32_BF_FLAG_SUBNORMAL | AiBaseFloat32BfSetExpBits(AI_BASE_FLOAT32_FP32_EXP_LENGTH);
	AiBaseFloat32BfExecOp(op, &r_bf, &a_bf, &b_bf, prec, flags);
	AiBaseFloat32BfGetFloat32(&r_bf, &r, rnd_mode);

#if 1
	{
		FLOAT32_T a_32;
		FLOAT32_T r_32;
		FLOAT32_T r_cmp;

		a_32 = AiBaseFloat32CvtF16F32(a);
		r_32 = (FLOAT32_T)exp(a_32);
		r_cmp = AiBaseFloat32CvtF32F16(r_32);
		if ((r_cmp != r) &&
			((r_cmp + 1) != r) &&
			(r_cmp != (r + 1))) {
//			AiBaseLogErrorCritial();
//			return r;
			return r_cmp;
		}
	}
#endif

	return r;
#endif
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
FLOAT32_T AiBaseFloat32Sqrt(FLOAT32_T a)
{
	AI_BASE_FLOAT32_OP_ENUM op;
	AI_BASE_FLOAT32_BF_T r_bf;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_BF_T b_bf;
	AI_BASE_FLOAT32_LIMB_T prec;
	AI_BASE_FLOAT32_BF_FLAGS_T flags;
	FLOAT32_T r;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;

	op = AI_BASE_FLOAT32_OP_SQRT;
	AiBaseFloat32BfSetFloat32(&a_bf, a);
	prec = AI_BASE_FLOAT32_FP32_ROUND_PREC;
	flags = AI_BASE_FLOAT32_BF_RNDNA | AI_BASE_FLOAT32_BF_FLAG_SUBNORMAL | AiBaseFloat32BfSetExpBits(AI_BASE_FLOAT32_FP32_EXP_LENGTH);
	AiBaseFloat32BfExecOp(op, &r_bf, &a_bf, &b_bf, prec, flags);
	AiBaseFloat32BfGetFloat32(&r_bf, &r, rnd_mode);

#if 1
	{
		FLOAT32_T a_32;
		FLOAT32_T r_32;
		FLOAT32_T r_cmp;
		FLOAT32_T r_cmp_32;
		FLOAT32_T r_32_soft;

		a_32 = (a);
		r_32 = (FLOAT32_T)sqrt(a_32);
		r_cmp = (r_32);
		r_cmp_32 = (r_cmp);
		r_32_soft = (r);
		if ((r_cmp != r) &&
			((r_cmp + 1) != r) &&
			(r_cmp != (r + 1))) {
			AiBaseLogErrorCritial();
			return r;
		}
	}
#endif

	return r;
}

/*------------------------- End ---------------------------------------------*/
#endif
