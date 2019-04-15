/**
 ******************************************************************************
 * file    ai_base_float_test.c
 * author  sunlingge
 * version V100
 * brief   the ai base float driver.
 * description
 *	1. double: sign(63 bit), exponent(52-62 bit), fraction(00-51 bit)
 *	2. float:  sign(31 bit), exponent(23-30 bit), fraction(00-22 bit)
 *  3. fp16:   sign(15 bit), exponent(10-14 bit), fraction(00-09 bit)
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"
#pragma warning(disable:4996)

#if (AI_BASE_FLOAT_FP64 == 1)
/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/
ERROR_T AiServerLogFp64(UINT8_T *str);

/*------------------------- Variable Definition------------------------------*/
const UINT8_T *g_ai_base_float64_op_str[AI_BASE_FLOAT64_OP_COUNT] = {
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

const UINT8_T *g_ai_base_float64_rnd_str[AI_BASE_FLOAT64_RND_COUNT] = {
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
void AiBaseFloat64SetSpecial(AI_BASE_FLOAT64_BF_T *a, INT32_T idx)
{
    switch(idx) {
    case 0:
        AiBaseFloat64BfSetUi(a, 0);
        break;
    case 1:
        AiBaseFloat64BfSetUi(a, 0);
        a->sign = 1; /* -0 */
        break;
    case 2:
        AiBaseFloat64BfSetSi(a, 0);
        a->expn = AI_BASE_FLOAT64_BF_EXP_INF;
        break;
    case 3:
        AiBaseFloat64BfSetSi(a, 0);
        a->expn = AI_BASE_FLOAT64_BF_EXP_INF;
        a->sign = 1;
        break;
    case 4:
        AiBaseFloat64BfSetSi(a, 1);
        break;
    case 5:
        AiBaseFloat64BfSetSi(a, -1);
        break;
    case 6:
        AiBaseFloat64BfSetNan(a);
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
void AiBaseFloat64MpRandinit(AI_BASE_FLOAT64_MP_RANDSTATE_T *state, UINT64_T seed)
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
UINT64_T AiBaseFloat64MpRandom64(AI_BASE_FLOAT64_MP_RANDSTATE_T *s)
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
void AiBaseFloat64BfRrandom(AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_MP_RANDSTATE_T *state)
{
    AI_BASE_FLOAT64_SLIMB_T n, max_run_len, cur_len, j, len, bit_index, nb_bits;
    INT32_T cur_state, m;
    
    n = (prec + AI_BASE_FLOAT64_LIMB_BITS - 1) / AI_BASE_FLOAT64_LIMB_BITS;

    /* same idea as GMP. It would be probably better to use a non
       uniform law */
    m = AiBaseFloat64MpRandom64(state) % 4 + 1;
    max_run_len = AiBaseFloat64BfMax(prec / m, 1);
    cur_state = AiBaseFloat64MpRandom64(state) & 1;
    cur_len = AiBaseFloat64MpRandom64(state) % max_run_len + 1;
    nb_bits = n * AI_BASE_FLOAT64_LIMB_BITS;
    
    AiBaseFloat64BfResize(a, n);
    memset(a->tab, 0, (size_t)(sizeof(AI_BASE_FLOAT64_DLIMB_T) * n));
    bit_index = nb_bits - prec;
    while (bit_index < nb_bits) {
        len = AiBaseFloat64BfMin(cur_len, nb_bits - bit_index);
        if (cur_state) {
            /* XXX: inefficient */
            for(j = 0; j < len; j++) {
                a->tab[bit_index >> AI_BASE_FLOAT64_LIMB_LOG2_BITS] |= (AI_BASE_FLOAT64_DLIMB_T)((AI_BASE_FLOAT64_LIMB_T)1 << (bit_index & (AI_BASE_FLOAT64_LIMB_BITS - 1)));
                bit_index++;
            }
        }
        bit_index += len;
        cur_len -= len;
        if (cur_len == 0) {
            cur_len = AiBaseFloat64MpRandom64(state) % max_run_len + 1;
            cur_state ^= 1;
        }
    }
    a->sign = 0;
    a->expn = 0;
    AiBaseFloat64BfNormalizeAndRound(a, prec, AI_BASE_FLOAT64_RND_Z);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64BfRrandomLarge(AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_MP_RANDSTATE_T *s)
{
    AI_BASE_FLOAT64_LIMB_T prec1;
    prec1 = AiBaseFloat64MpRandom64(s) % (2 * prec) + 1;
    AiBaseFloat64BfRrandom(a, prec1, s);
    a->sign = AiBaseFloat64MpRandom64(s) & 1;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* random integer with 0 to prec bits */
void AiBaseFloat64BfRrandomInit(AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_MP_RANDSTATE_T *rnd_state)
{
    AI_BASE_FLOAT64_LIMB_T prec1;
    prec1 = AiBaseFloat64MpRandom64(rnd_state) % prec + 1;
    AiBaseFloat64BfRrandom(a, prec1, rnd_state);
    if (a->expn != AI_BASE_FLOAT64_BF_EXP_ZERO)
        a->expn += prec1;
    a->sign = AiBaseFloat64MpRandom64(rnd_state) & 1;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* random integer with long sequences of '0' and '1' */
UINT64_T AiBaseFloat64RrandomU(INT32_T len, AI_BASE_FLOAT64_MP_RANDSTATE_T *s)
{
    INT32_T bit, pos, n, end;
    UINT64_T a;
    
    bit = AiBaseFloat64MpRandom64(s) & 1;
    pos = 0;
    a = 0;
    for(;;) {
        n = (AiBaseFloat64MpRandom64(s) % len) + 1;
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
UINT64_T AiBaseFloat64RrandomSf64(AI_BASE_FLOAT64_MP_RANDSTATE_T *s)
{
    UINT32_T a_exp, a_sign;
    UINT64_T a_mant;
    a_sign = AiBaseFloat64MpRandom64(s) & 1;

    /* generate exponent close to the min/max more often than random */
    switch(AiBaseFloat64MpRandom64(s) & 15) {
    case 0:
        a_exp = (AiBaseFloat64MpRandom64(s) % (2 * AI_BASE_FLOAT64_F64_MANT_SIZE)) & AI_BASE_FLOAT64_F64_EXP_MASK;
        break;
    case 1:
        a_exp = (AI_BASE_FLOAT64_F64_EXP_MASK - (AiBaseFloat64MpRandom64(s) % (2 * AI_BASE_FLOAT64_F64_MANT_SIZE))) & AI_BASE_FLOAT64_F64_EXP_MASK;
        break;
    default:
        a_exp = AiBaseFloat64MpRandom64(s) & AI_BASE_FLOAT64_F64_EXP_MASK;
        break;
    }
    a_mant = AiBaseFloat64RrandomU(AI_BASE_FLOAT64_F64_MANT_SIZE, s);
    return ((UINT64_T)a_sign << 63) | ((UINT64_T)a_exp << AI_BASE_FLOAT64_F64_MANT_SIZE) | a_mant;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT64_T AiBaseFloat64GetClockMsec(void)
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
UINT64_T AiBaseFloat64GetCycles(void)
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
INT32_T AiBaseFloat64SoftfpGetRndMode(AI_BASE_FLOAT64_RND_ENUM rnd_mode)
{
    switch(rnd_mode) {
    case AI_BASE_FLOAT64_RND_N:
        return AI_BASE_FLOAT64_RM_RNE;
    case AI_BASE_FLOAT64_RND_Z:
        return AI_BASE_FLOAT64_RM_RTZ;
    case AI_BASE_FLOAT64_RND_U:
        return AI_BASE_FLOAT64_RM_RUP;
    case AI_BASE_FLOAT64_RND_D:
        return AI_BASE_FLOAT64_RM_RDN;
    case AI_BASE_FLOAT64_RND_NA:
        return AI_BASE_FLOAT64_RM_RMM;
    default:
        break;
    }
	return AI_BASE_FLOAT64_RM_RNE;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64SoftfpSetStatus(UINT32_T fflags)
{
    INT32_T ret = 0;
    if (fflags & AI_BASE_FLOAT64_FFLAG_INVALID_OP)
        ret |= AI_BASE_FLOAT64_BF_ST_INVALID_OP;
    if (fflags & AI_BASE_FLOAT64_FFLAG_DIVIDE_ZERO)
        ret |= AI_BASE_FLOAT64_BF_ST_DIVIDE_ZERO;
    if (fflags & AI_BASE_FLOAT64_FFLAG_OVERFLOW)
        ret |= AI_BASE_FLOAT64_BF_ST_OVERFLOW;
    if (fflags & AI_BASE_FLOAT64_FFLAG_UNDERFLOW)
        ret |= AI_BASE_FLOAT64_BF_ST_UNDERFLOW;
    if (fflags & AI_BASE_FLOAT64_FFLAG_INEXACT)
        ret |= AI_BASE_FLOAT64_BF_ST_INEXACT;
    return ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfExecOp(AI_BASE_FLOAT64_OP_ENUM op, AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_BF_T *b,
               AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    INT32_T ret = 0;

    switch(op) {
    case AI_BASE_FLOAT64_OP_MUL:
        ret = AiBaseFloat64BfMul(r, a, b, prec, flags);
        break;
    case AI_BASE_FLOAT64_OP_ADD:
        ret = AiBaseFloat64BfAdd(r, a, b, prec, flags);
        break;
    case AI_BASE_FLOAT64_OP_SUB:
        ret = AiBaseFloat64BfSub(r, a, b, prec, flags);
        break;
    case AI_BASE_FLOAT64_OP_RINT:
        AiBaseFloat64BfSet(r, a);
        ret = AiBaseFloat64BfRint(r, prec, flags);
        break;
    case AI_BASE_FLOAT64_OP_ROUND:
        AiBaseFloat64BfSet(r, a);
        ret = AiBaseFloat64BfRound(r, prec, flags);
        break;
    case AI_BASE_FLOAT64_OP_CMP_EQ:
        ret = AiBaseFloat64BfCmpEq(a, b);
        break;
    case AI_BASE_FLOAT64_OP_CMP_LT:
        ret = AiBaseFloat64BfCmpLt(a, b);
        break;
    case AI_BASE_FLOAT64_OP_CMP_LE:
        ret = AiBaseFloat64BfCmpLe(a, b);
        break;
    case AI_BASE_FLOAT64_OP_RECIP:
        AiBaseFloat64BfRecip(r, a, prec);
        break;
    case AI_BASE_FLOAT64_OP_DIV:
        ret = AiBaseFloat64BfDiv(r, a, b, prec, flags);
        break;
    case AI_BASE_FLOAT64_OP_FMOD:
        ret = AiBaseFloat64BfFmod(r, a, b, prec, flags);
        break;
    case AI_BASE_FLOAT64_OP_REM:
        ret = AiBaseFloat64BfRemainder(r, a, b, prec, flags);
        break;
    case AI_BASE_FLOAT64_OP_RSQRT:
        AiBaseFloat64BfRsqrt(r, a, prec);
        break;
    case AI_BASE_FLOAT64_OP_SQRT:
        ret = AiBaseFloat64BfSqrt(r, a, prec, flags);
        break;
    case AI_BASE_FLOAT64_OP_OR:
        AiBaseFloat64BfLogicOr(r, a, b);
        break;
    case AI_BASE_FLOAT64_OP_XOR:
        AiBaseFloat64BfLogicXor(r, a, b);
        break;
    case AI_BASE_FLOAT64_OP_AND:
        AiBaseFloat64BfLogicAnd(r, a, b);
        break;
    case AI_BASE_FLOAT64_OP_EXP:
        ret = AiBaseFloat64BfExp(r, a, prec, flags);
        break;
    case AI_BASE_FLOAT64_OP_LOG:
        ret = AiBaseFloat64BfLog(r, a, prec, flags);
        break;
    case AI_BASE_FLOAT64_OP_COS:
        ret = AiBaseFloat64BfCos(r, a, prec, flags);
        break;
    case AI_BASE_FLOAT64_OP_SIN:
        ret = AiBaseFloat64BfSin(r, a, prec, flags);
        break;
    case AI_BASE_FLOAT64_OP_TAN:
        ret = AiBaseFloat64BfTan(r, a, prec, flags);
        break;
    case AI_BASE_FLOAT64_OP_ATAN:
        ret = AiBaseFloat64BfAtan(r, a, prec, flags);
        break;
    case AI_BASE_FLOAT64_OP_ATAN2:
        ret = AiBaseFloat64BfAtan2(r, a, b, prec, flags);
        break;
    case AI_BASE_FLOAT64_OP_ASIN:
        ret = AiBaseFloat64BfAsin(r, a, prec, flags);
        break;
    case AI_BASE_FLOAT64_OP_ACOS:
        ret = AiBaseFloat64BfAcos(r, a, prec, flags);
        break;
    case AI_BASE_FLOAT64_OP_POW:
        ret = AiBaseFloat64BfPow(r, a, b, prec, flags);
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
void AiBaseFloat64PrintStatus(INT32_T status)
{
#if 0
    printf("%c%c%c%c%c",
           (status & AI_BASE_FLOAT64_BF_ST_INVALID_OP) ? 'I' : '-',
           (status & AI_BASE_FLOAT64_BF_ST_DIVIDE_ZERO) ? 'Z' : '-',
           (status & AI_BASE_FLOAT64_BF_ST_OVERFLOW) ? 'O' : '-',
           (status & AI_BASE_FLOAT64_BF_ST_UNDERFLOW) ? 'U' : '-',
           (status & AI_BASE_FLOAT64_BF_ST_INEXACT) ? 'X' : '-');
#endif
	return;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
BOOL_T AiBaseFloat64BfIsSame(const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b)
{
    return a->sign == b->sign && AiBaseFloat64BfCmpu(a, b) == 0;
}

#if 0
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64TestAtof(AI_BASE_FLOAT64_LIMB_T prec, INT32_T duration_ms,
               INT32_T exp_bits, AI_BASE_FLOAT64_RND_ENUM rnd_mode, INT32_T seed)
{
    AI_BASE_FLOAT64_DYN_BUF dbuf;
    INT32_T radix, it, c, e, status, ref_status, err, rnd_mode1, test_loop;
    AI_BASE_FLOAT64_MP_RANDSTATE_T rnd_state;
    AI_BASE_FLOAT64_SLIMB_T n_digits, prec1, i;
    UINT8_T *str;
    AI_BASE_FLOAT64_BF_T r, r_ref;
    INT64_T ti, ti_ref, nb_limbs, start_time;
    
    AiBaseFloat64MpRandinit(&rnd_state, seed);

    AiBaseFloat64BfInit(&r);
    AiBaseFloat64BfInit(&r_ref);
    ti = 0;
    ti_ref = 0;
    start_time = AiBaseFloat64GetClockMsec();
    test_loop = 1;
    it = 0;
    for(;;) {
        /* build a random string representing a number */
        if (AiBaseFloat64MpRandom64(&rnd_state) & 1)
            radix = (AiBaseFloat64MpRandom64(&rnd_state) % 35) + 2;
        else
            radix = 10;
        prec1 = (AI_BASE_FLOAT64_LIMB_T)AiBaseFloat64Ceil(prec / AiBaseFloat64Log2(radix));
        n_digits = AiBaseFloat64MpRandom64(&rnd_state) % (prec1 * 3) + 1;
        AiBaseFloat64DbufInit(&dbuf);
        if (AiBaseFloat64MpRandom64(&rnd_state) & 1)
            AiBaseFloat64DbufPutc(&dbuf, '-');

        for(i = 0; i < n_digits; i++) {
            c = AiBaseFloat64MpRandom64(&rnd_state) % radix;
            if (c < 10)
                c += '0';
            else
                c += 'a' - 10;
            AiBaseFloat64DbufPutc(&dbuf, c);
        }
        if (radix == 10)
            AiBaseFloat64DbufPutc(&dbuf, 'e');
        else
            AiBaseFloat64DbufPutc(&dbuf, '@');
        e = prec1 * 20;
        e = (AiBaseFloat64MpRandom64(&rnd_state) % (2 * e + 1)) - e;
        AiBaseFloat64DbufPrintf(&dbuf, "%d", e);
        AiBaseFloat64DbufPutc(&dbuf, '\0');
        str = (UINT8_T *)dbuf.buf;

        ti -= AiBaseFloat64GetCycles();
        status = AiBaseFloat64BfAtof(&r, str, NULL, radix, prec, rnd_mode) &
            AI_BASE_FLOAT64_BF_ST_INEXACT;
        ti += AiBaseFloat64GetCycles();
        rnd_mode1 = rnd_mode;
        if (rnd_mode == AI_BASE_FLOAT64_RND_F)
            rnd_mode1 = AI_BASE_FLOAT64_RND_D;

        ti_ref -= AiBaseFloat64GetCycles();
        ref_status = AiBaseFloat64MpfrExecSetstr(&r_ref, str, radix, prec, rnd_mode1);
        ti_ref += AiBaseFloat64GetCycles();
        
        if (rnd_mode == AI_BASE_FLOAT64_RND_F) {
            err = !AiBaseFloat64BfIsSame(&r, &r_ref);
            if (err && rnd_mode == AI_BASE_FLOAT64_RND_F) {
                ref_status = AiBaseFloat64MpfrExecSetstr(&r_ref, str, radix, prec, AI_BASE_FLOAT64_RND_U);
                err = !AiBaseFloat64BfIsSame(&r, &r_ref);
            }
        } else {
            err = !AiBaseFloat64BfIsSame(&r, &r_ref) || status != ref_status;
        }
        
        if (err) {
            printf("\nERROR (%d):\n", it);
            printf("radix=%d\n", radix);
            printf("str=%s\n", str);
            AiBaseFloat64BfPrintStr("r  ", &r);
            AiBaseFloat64BfPrintStr("ref", &r_ref);
            printf("st    ="); AiBaseFloat64PrintStatus(status); printf("\n");
            printf("ref_st="); AiBaseFloat64PrintStatus(ref_status); printf("\n");
            exit(1);
        }
        free(str);
        it++;
        if ((it & (test_loop - 1)) == 0) {
            if ((AiBaseFloat64GetClockMsec() - start_time) >= duration_ms)
                break;
            test_loop *= 2;
        }
    }
    AiBaseFloat64BfDelete(&r);
    AiBaseFloat64BfDelete(&r_ref);

    nb_limbs = (prec + 63) / 64;
    printf(" %8u %8.1f %8.1f\n",
           it,
           (double)ti / it / nb_limbs,
           (double)ti_ref / it / nb_limbs);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64TestFtoa(AI_BASE_FLOAT64_LIMB_T prec, INT32_T duration_ms,
               INT32_T exp_bits, AI_BASE_FLOAT64_RND_ENUM rnd_mode, INT32_T seed)
{
    INT32_T radix, it, e, test_loop;
    AI_BASE_FLOAT64_MP_RANDSTATE_T rnd_state;
    AI_BASE_FLOAT64_SLIMB_T n_digits, prec1, nb_limbs;
    UINT8_T *r_str, *r_ref_str;
    AI_BASE_FLOAT64_BF_T a;
    INT64_T ti, ti_ref, start_time;
    
    AiBaseFloat64MpRandinit(&rnd_state, seed);
    AiBaseFloat64BfInit(&a);
    ti_ref = 0;
    ti = 0;
    start_time = AiBaseFloat64GetClockMsec();
    test_loop = 1;
    it = 0;
    for(;;) {
        /* build a random string representing a number */
        if ((AiBaseFloat64MpRandom64(&rnd_state) & 1) && 0)
            radix = (AiBaseFloat64MpRandom64(&rnd_state) % 35) + 2;
        else
            radix = 10;
        n_digits = (AI_BASE_FLOAT64_LIMB_T)AiBaseFloat64Ceil(prec / AiBaseFloat64Log2(radix));
        prec1 = AiBaseFloat64MpRandom64(&rnd_state) % (3 * prec) + 2;
        AiBaseFloat64BfRrandom(&a, prec1, &rnd_state);
        e = prec * 20;
        if (a.expn != AI_BASE_FLOAT64_BF_EXP_ZERO)
            a.expn += (AiBaseFloat64MpRandom64(&rnd_state) % (2 * e + 1)) - e;
        ti -= AiBaseFloat64GetCycles();
        AiBaseFloat64BfFtoa(&r_str, &a, radix, n_digits, rnd_mode |
                BF_FTOA_FORMAT_FIXED | BF_FTOA_FORCE_EXP);
        ti += AiBaseFloat64GetCycles();
        {
            AI_BASE_FLOAT64_MPFR_T a1;
            AI_BASE_FLOAT64_MPFR_EXP_T expn;
            AI_BASE_FLOAT64_DYN_BUF s_s, *s = &s_s;
            UINT8_T *str, *p;
            AI_BASE_FLOAT64_SLIMB_T i;
            
            AiBaseFloat64MpfrInit2(a1, AiBaseFloat64BfMax(a.len, 1) * AI_BASE_FLOAT64_LIMB_BITS);
            AiBaseFloat64BfToMpfr(a1, &a);
            ti_ref -= AiBaseFloat64GetCycles();
            str = AiBaseFloat64MpfrGetStr(NULL, &expn, radix, n_digits, a1,
                               AiBaseFloat64MpfrGetRndMode(rnd_mode));
            ti_ref += AiBaseFloat64GetCycles();
            /* add the decimal point and exponent */
            AiBaseFloat64DbufInit(s);
            p = str;
            if (*p == '-')
                AiBaseFloat64DbufPutc(s, *p++);
            AiBaseFloat64DbufPutc(s, *p++);
            if (n_digits > 1) {
                AiBaseFloat64DbufPutc(s, '.');
                for(i = 1; i < n_digits; i++) {
                    AiBaseFloat64DbufPutc(s, *p++);
                }
            }
            expn--;
            if ((radix & (radix - 1)) == 0 && radix <= 16) {
                INT32_T radix_bits = 1;
                while ((1 << radix_bits) != radix)
                    radix_bits++;
                AiBaseFloat64DbufPrintf(s, "p%" AI_BASE_FLOAT64_PRID64 , (INT64_T)(expn * radix_bits));
            } else {
                AiBaseFloat64DbufPrintf(s, "%c%" AI_BASE_FLOAT64_PRID64 , radix <= 10 ? 'e' : '@', (INT64_T)expn);
            }
            AiBaseFloat64DbufPutc(s, '\0');
            
            r_ref_str = (UINT8_T *)s->buf;
            AiBaseFloat64MpfrClear(a1);
            AiBaseFloat64MpfrFreeStr(str);
        }
        
        if (strcmp(r_ref_str, r_str) != 0) {
            printf("\nERROR (%d):\n", it);
            printf("radix=%d\n", radix);
            AiBaseFloat64BfPrintStr("a  ", &a);
            printf("r  =%s\n", r_str);
            printf("ref=%s\n", r_ref_str);
            exit(1);
        }
        free(r_str);
        free(r_ref_str);
        it++;
        if ((it & (test_loop - 1)) == 0) {
            if ((AiBaseFloat64GetClockMsec() - start_time) >= duration_ms)
                break;
            test_loop *= 2;
        }
    }
    AiBaseFloat64BfDelete(&a);

    nb_limbs = (prec + 63) / 64;
    printf(" %8u %8.1f %8.1f\n",
           it,
           (double)ti / it / nb_limbs,
           (double)ti_ref / it / nb_limbs);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64TestCanRound(AI_BASE_FLOAT64_LIMB_T prec, INT32_T duration_ms, AI_BASE_FLOAT64_RND_ENUM rnd_mode, INT32_T seed)
{
    AI_BASE_FLOAT64_MP_RANDSTATE_T rnd_state;
    AI_BASE_FLOAT64_BF_T a, b, a_rounded, c;
    AI_BASE_FLOAT64_LIMB_T prec1, k;
    INT32_T res, it, i, res1, test_loop;
    INT64_T start_time;
    
    AiBaseFloat64MpRandinit(&rnd_state, seed);
    AiBaseFloat64BfInit(&a);
    AiBaseFloat64BfInit(&a_rounded);
    AiBaseFloat64BfInit(&b);
    AiBaseFloat64BfInit(&c);
    start_time = AiBaseFloat64GetClockMsec();
    test_loop = 1;
    it = 0;
    for(;;) {
        prec1 = AiBaseFloat64MpRandom64(&rnd_state) % (3 * prec) + 2;
        AiBaseFloat64BfRrandom(&a, prec1, &rnd_state);
        a.sign = AiBaseFloat64MpRandom64(&rnd_state) & 1;

        k = prec + (AiBaseFloat64MpRandom64(&rnd_state) % 10);
        AiBaseFloat64BfSet(&a_rounded, &a);
        AiBaseFloat64BfRound(&a_rounded, prec, rnd_mode);
        res = AiBaseFloat64BfCanRound(&a, prec, rnd_mode, k);
        if (res) {
            for(i = 0; i < 100; i++) {
                AiBaseFloat64BfRrandom(&c, prec1, &rnd_state);
                c.sign = AiBaseFloat64MpRandom64(&rnd_state) & 1;
                if (c.expn != AI_BASE_FLOAT64_BF_EXP_ZERO)
                    c.expn += a.expn - k;
                
                AiBaseFloat64BfAdd(&b, &a, &c, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_RND_Z);
                AiBaseFloat64BfRound(&b, prec, rnd_mode);
                res1 = !AiBaseFloat64BfIsSame(&b, &a_rounded);
                if (res1) {
                    printf("\nERROR (%d):\n", it);
                    printf("k=%" AI_BASE_FLOAT64_PRID64 "\n", (INT64_T)k);
                    AiBaseFloat64BfPrintStr("a    ", &a);
                    AiBaseFloat64BfPrintStr("a_rnd", &a_rounded);
                    AiBaseFloat64BfPrintStr("e    ", &c);
                    AiBaseFloat64BfPrintStr("b    ", &b);
                    exit(1);
                }
            }
        }
        it++;
        if ((it & (test_loop - 1)) == 0) {
            if ((AiBaseFloat64GetClockMsec() - start_time) >= duration_ms)
                break;
            test_loop *= 2;
        }
    }
    AiBaseFloat64BfDelete(&a);
    AiBaseFloat64BfDelete(&a_rounded);
    AiBaseFloat64BfDelete(&b);
    AiBaseFloat64BfDelete(&c);
    printf(" %8u\n", it);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64TestMulLog2(INT32_T duration_ms, BOOL_T is_inv, BOOL_T is_ceil, INT32_T seed)
{
    AI_BASE_FLOAT64_MP_RANDSTATE_T rnd_state;
    INT32_T it, radix, err, test_loop;
    AI_BASE_FLOAT64_SLIMB_T a, v_max, r, r_ref, prec, d;
    AI_BASE_FLOAT64_MPFR_T a1, log2_radix[AI_BASE_FLOAT64_BF_RADIX_MAX - 1];
    INT64_T start_time;
    
    AiBaseFloat64MpRandinit(&rnd_state, seed);
    prec = 256;
    AiBaseFloat64MpfrInit2(a1, prec);

    for(radix = 2; radix <= AI_BASE_FLOAT64_BF_RADIX_MAX; radix++) {
        AiBaseFloat64MpfrInit2(log2_radix[radix - 2], prec);
        mpfr_set_ui(a1, radix, AI_BASE_FLOAT64_MPFR_RNDN);
        mpfr_log2(log2_radix[radix - 2], a1, AI_BASE_FLOAT64_MPFR_RNDN);
    }

    if (is_inv)
        v_max = BF_PREC_MAX;
    else
        v_max = BF_PREC_MAX / 6;
    start_time = AiBaseFloat64GetClockMsec();
    test_loop = 1;
    it = 0;
    for(;;) {
        for(radix = 2; radix <= AI_BASE_FLOAT64_BF_RADIX_MAX; radix++) {
            a = (AiBaseFloat64MpRandom64(&rnd_state) % (2 * v_max + 1)) - v_max;
            r = AiBaseFloat64BfMul_log2_radix(a, radix, is_inv, is_ceil);
            
            mpfr_set_si(a1, a, AI_BASE_FLOAT64_MPFR_RNDN);
            if (is_inv)
                mpfr_div(a1, a1, log2_radix[radix - 2], AI_BASE_FLOAT64_MPFR_RNDN);
            else
                mpfr_mul(a1, a1, log2_radix[radix - 2], AI_BASE_FLOAT64_MPFR_RNDN);
            if (is_ceil)
                mpfr_ceil(a1, a1);
            else
                mpfr_floor(a1, a1);
            r_ref = mpfr_get_si(a1, AI_BASE_FLOAT64_MPFR_RNDN);
            if (is_inv) {
                err = (r != r_ref);
            } else {
                d = r - r_ref;
                err = (d > 1 || d < -1);
            }
            if (err) {
                printf("\nERROR (%d):\n", it);
                printf("a=%" AI_BASE_FLOAT64_PRID64 " radix=%d inv=%d AiBaseFloat64Ceil=%d res=%" AI_BASE_FLOAT64_PRID64 " ref=%" AI_BASE_FLOAT64_PRID64 "\n",
                       (INT64_T)a, radix, is_inv, is_ceil,
                       (INT64_T)r, (INT64_T)r_ref);
                exit(1);
            }
        }
        it++;
        if ((it & (test_loop - 1)) == 0) {
            if ((AiBaseFloat64GetClockMsec() - start_time) >= duration_ms)
                break;
            test_loop *= 2;
        }
    }

    for(radix = 2; radix <= AI_BASE_FLOAT64_BF_RADIX_MAX; radix++)
        AiBaseFloat64MpfrClear(log2_radix[radix - 2]);
    AiBaseFloat64MpfrClear(a1);
    printf(" %8u\n", it);
}
#endif

#if 1
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64TestOpRm(AI_BASE_FLOAT64_OP_ENUM op, AI_BASE_FLOAT64_LIMB_T rprec, INT32_T exp_bits, AI_BASE_FLOAT64_RND_ENUM rnd_mode, INT32_T seed)
{
    AI_BASE_FLOAT64_BF_T a, b, r, r_ref;
    INT32_T op_count, status, ref_status=0, test_loop, it;
    INT32_T  nb_limbs;
    AI_BASE_FLOAT64_MP_RANDSTATE_T rnd_state;
    BOOL_T res;
    AI_BASE_FLOAT64_RND_ENUM rnd_mode1;
    AI_BASE_FLOAT64_BF_FLAGS_T bf_flags;
    AI_BASE_FLOAT64_LIMB_T prec;
	UINT8_T display_str[1024];

#if 0
    switch(op) {
    case AI_BASE_FLOAT64_OP_ATOF:
        AiBaseFloat64TestAtof(rprec, duration_ms, exp_bits, rnd_mode, seed);
        return;
    case AI_BASE_FLOAT64_OP_FTOA:
        AiBaseFloat64TestFtoa(rprec, duration_ms, exp_bits, rnd_mode, seed);
        return;
    case AI_BASE_FLOAT64_OP_CAN_ROUND:
        AiBaseFloat64TestCanRound(rprec, duration_ms, rnd_mode, seed);
        return;
    case AI_BASE_FLOAT64_OP_MUL_L2RADIX:
    case AI_BASE_FLOAT64_OP_DIV_L2RADIX:
        AiBaseFloat64TestMulLog2(duration_ms, (op == AI_BASE_FLOAT64_OP_DIV_L2RADIX), rnd_mode == AI_BASE_FLOAT64_RND_U, seed);
        return;
    default:
        break;
    }
#endif

    bf_flags = rnd_mode | AiBaseFloat64BfSetExpBits(exp_bits);
    
    AiBaseFloat64MpRandinit(&rnd_state, seed);
    AiBaseFloat64BfInit(&a);
    AiBaseFloat64BfInit(&b);
    AiBaseFloat64BfInit(&r);
    AiBaseFloat64BfInit(&r_ref);
    AiBaseFloat64BfSetUi(&b, 0);
    AiBaseFloat64BfSetUi(&r, 0);
    AiBaseFloat64BfSetUi(&r_ref, 0);
    test_loop = 1;
    it = 0;
    for(;;) {
        if (rprec == 0) {
            prec = (AiBaseFloat64MpRandom64(&rnd_state) % 1000) + 24;
        } else {
            prec = rprec;
        }
        switch(op) {
        case AI_BASE_FLOAT64_OP_RINT:
        case AI_BASE_FLOAT64_OP_RECIP:
        case AI_BASE_FLOAT64_OP_RSQRT:
        case AI_BASE_FLOAT64_OP_SQRT:
        case AI_BASE_FLOAT64_OP_EXP:
        case AI_BASE_FLOAT64_OP_LOG:
        case AI_BASE_FLOAT64_OP_COS:
        case AI_BASE_FLOAT64_OP_SIN:
        case AI_BASE_FLOAT64_OP_TAN:
        case AI_BASE_FLOAT64_OP_ATAN:
        case AI_BASE_FLOAT64_OP_ASIN:
        case AI_BASE_FLOAT64_OP_ACOS:
            op_count = 1;
            break;
        default:
            op_count = 2;
            break;
        }

        if (op_count == 1) {
            if (it < AI_BASE_FLOAT64_SPECIAL_COUNT) {
                AiBaseFloat64SetSpecial(&a, it);
            } else {
                AI_BASE_FLOAT64_LIMB_T prec1;
                
                    prec1 = AiBaseFloat64MpRandom64(&rnd_state) % (3 * prec) + 1;
                    AiBaseFloat64BfRrandom(&a, prec1, &rnd_state);
                    if (op == AI_BASE_FLOAT64_OP_COS || op == AI_BASE_FLOAT64_OP_SIN || op == AI_BASE_FLOAT64_OP_TAN) {
                        INT32_T k;
                        AI_BASE_FLOAT64_BF_T c_s, *c = &c_s;
                        if (a.expn != AI_BASE_FLOAT64_BF_EXP_ZERO)
                            a.expn++;
                        k = (AiBaseFloat64MpRandom64(&rnd_state) % 2000) - 1000;
                        AiBaseFloat64BfInit(c);
#if 0
                        bf_const_pi(c, prec1 + 1, AI_BASE_FLOAT64_RND_N);
#endif
                        c->expn--; /* pi/2 */
                        AiBaseFloat64BfMulSi(c, c, k, prec1 + 1, AI_BASE_FLOAT64_RND_N);
                        AiBaseFloat64BfAdd(&a, &a, c, prec1, AI_BASE_FLOAT64_RND_N);
                        AiBaseFloat64BfDelete(c);
                    } else if (op == AI_BASE_FLOAT64_OP_ACOS || op == AI_BASE_FLOAT64_OP_ASIN) {
                    } else {
                        if (a.expn != AI_BASE_FLOAT64_BF_EXP_ZERO)
                            a.expn += prec1 / 2;
                }
                if (op == AI_BASE_FLOAT64_OP_RSQRT || op == AI_BASE_FLOAT64_OP_SQRT || op == AI_BASE_FLOAT64_OP_LOG) {
                    a.sign = 0;
                } else {
                    a.sign = AiBaseFloat64MpRandom64(&rnd_state) & 1;
                }
            }
        } else if (op == AI_BASE_FLOAT64_OP_OR ||
                   op == AI_BASE_FLOAT64_OP_XOR ||
                   op == AI_BASE_FLOAT64_OP_AND) {
            AiBaseFloat64BfRrandomInit(&a, prec, &rnd_state);
            AiBaseFloat64BfRrandomInit(&b, prec, &rnd_state);
        } else {
            if (it < AI_BASE_FLOAT64_SPECIAL_COUNT * AI_BASE_FLOAT64_SPECIAL_COUNT) {
                AiBaseFloat64SetSpecial(&a, it % AI_BASE_FLOAT64_SPECIAL_COUNT);
                AiBaseFloat64SetSpecial(&b, it / AI_BASE_FLOAT64_SPECIAL_COUNT);
            } else {
                if (op == AI_BASE_FLOAT64_OP_POW) {
                    AiBaseFloat64BfRrandomLarge(&a, prec, &rnd_state);
                    if ((it % 10) == 0) {
                        AiBaseFloat64BfSetSi(&b, (int32_t)AiBaseFloat64MpRandom64(&rnd_state));
                    } else {
                        AiBaseFloat64BfRrandomLarge(&b, prec, &rnd_state);
                    }
                } else {
                    AiBaseFloat64BfRrandomLarge(&a, prec, &rnd_state);
                    AiBaseFloat64BfRrandomLarge(&b, prec, &rnd_state);
                }
            }
        }
        
        status = AiBaseFloat64BfExecOp(op, &r, &a, &b, prec, bf_flags);
        
        rnd_mode1 = rnd_mode;
        if (rnd_mode == AI_BASE_FLOAT64_RND_F)
            rnd_mode1 = AI_BASE_FLOAT64_RND_D;
#if 0
		ref_status = AiBaseFloat64MpfrExecOp(op, &r_ref, &a, &b, prec, rnd_mode1, &ti_ref);
#endif
        //        AiBaseFloat64BfPrintStr("r_ref", &r_ref);

        if (op == AI_BASE_FLOAT64_OP_CMP_EQ ||
            op == AI_BASE_FLOAT64_OP_CMP_LE ||
            op == AI_BASE_FLOAT64_OP_CMP_LT) {
            res = (status != ref_status);
        } else {
            res = !AiBaseFloat64BfIsSame(&r, &r_ref);
            if (rnd_mode == AI_BASE_FLOAT64_RND_F) {
                if (res) {
#if 0
                    AiBaseFloat64MpfrExecOp(op, &r_ref, &a, &b, prec, AI_BASE_FLOAT16_RND_U, &ti_dummy);
#endif
                    res = !AiBaseFloat64BfIsSame(&r, &r_ref);
                }
            } else {
                if ((status & AI_BASE_FLOAT64_BF_ST_INEXACT) !=
                    (ref_status & AI_BASE_FLOAT64_BF_ST_INEXACT))
                    res = 1;
            }
        }
        
        if (1) {
            sprintf(display_str, "\nTest(%d) ERROR (%d):\n", it, res);
			AiServerLogFp64(display_str);

            AiBaseFloat64BfPrintStr("a  ", &a);
            if (op_count > 1) {
                AiBaseFloat64BfPrintStr("b  ", &b);
            }
            AiBaseFloat64BfPrintStr("r  ", &r);
            AiBaseFloat64BfPrintStr("ref", &r_ref);
            printf("st    ="); AiBaseFloat64PrintStatus(status); printf("\n");
            printf("ref_st="); AiBaseFloat64PrintStatus(ref_status); printf("\n");
        }
        it++;
        if ((it & (test_loop - 1)) == 0) {
            test_loop *= 2;
        }
    }

    nb_limbs = (INT32_T)(prec + 63) / 64;
    AiBaseFloat64BfDelete(&a);
    AiBaseFloat64BfDelete(&b);
    AiBaseFloat64BfDelete(&r);
    AiBaseFloat64BfDelete(&r_ref);
}
#endif

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64TestOp(AI_BASE_FLOAT64_OP_ENUM op, AI_BASE_FLOAT64_LIMB_T prec, INT32_T duration_ms, INT32_T exp_bits,
             INT32_T seed)
{
    uint8_t rm_allowed[AI_BASE_FLOAT64_RND_F + 1];
	AI_BASE_FLOAT64_RND_ENUM rnd_mode;

    memset(rm_allowed, 0, sizeof(rm_allowed));
    switch(op) {
        case AI_BASE_FLOAT64_OP_ADD:
        case AI_BASE_FLOAT64_OP_MUL:
        case AI_BASE_FLOAT64_OP_DIV:
        case AI_BASE_FLOAT64_OP_FMOD:
        case AI_BASE_FLOAT64_OP_REM:
        case AI_BASE_FLOAT64_OP_RINT:
        case AI_BASE_FLOAT64_OP_ROUND:
        case AI_BASE_FLOAT64_OP_SQRT:
        case AI_BASE_FLOAT64_OP_ATOF:
        case AI_BASE_FLOAT64_OP_EXP:
        case AI_BASE_FLOAT64_OP_LOG:
        case AI_BASE_FLOAT64_OP_COS:
        case AI_BASE_FLOAT64_OP_SIN:
        case AI_BASE_FLOAT64_OP_TAN:
        case AI_BASE_FLOAT64_OP_ATAN:
        case AI_BASE_FLOAT64_OP_ATAN2:
        case AI_BASE_FLOAT64_OP_ASIN:
        case AI_BASE_FLOAT64_OP_ACOS:
        case AI_BASE_FLOAT64_OP_POW:
            rm_allowed[AI_BASE_FLOAT64_RND_N] = 1;
            rm_allowed[AI_BASE_FLOAT64_RND_Z] = 1;
            rm_allowed[AI_BASE_FLOAT64_RND_U] = 1;
            rm_allowed[AI_BASE_FLOAT64_RND_D] = 1;
            rm_allowed[AI_BASE_FLOAT64_RND_F] = 1;
            break;
        case AI_BASE_FLOAT64_OP_CAN_ROUND:
            rm_allowed[AI_BASE_FLOAT64_RND_N] = 1;
            rm_allowed[AI_BASE_FLOAT64_RND_Z] = 1;
            rm_allowed[AI_BASE_FLOAT64_RND_U] = 1;
            rm_allowed[AI_BASE_FLOAT64_RND_D] = 1;
            rm_allowed[AI_BASE_FLOAT64_RND_NA] = 1;
            break;
        case AI_BASE_FLOAT64_OP_FTOA:
            rm_allowed[AI_BASE_FLOAT64_RND_N] = 1;
            rm_allowed[AI_BASE_FLOAT64_RND_Z] = 1;
            rm_allowed[AI_BASE_FLOAT64_RND_U] = 1;
            rm_allowed[AI_BASE_FLOAT64_RND_D] = 1;
            break;
        case AI_BASE_FLOAT64_OP_SUB:
            /* minimal test for SUB which is like ADD */
            rm_allowed[AI_BASE_FLOAT64_RND_N] = 1;
            break;
        case AI_BASE_FLOAT64_OP_RECIP:
        case AI_BASE_FLOAT64_OP_RSQRT:
            rm_allowed[AI_BASE_FLOAT64_RND_F] = 1;
            break;
        case AI_BASE_FLOAT64_OP_MUL_L2RADIX:
        case AI_BASE_FLOAT64_OP_DIV_L2RADIX:
            rm_allowed[AI_BASE_FLOAT64_RND_U] = 1;
            rm_allowed[AI_BASE_FLOAT64_RND_D] = 1;
            break;
        default:
            rm_allowed[AI_BASE_FLOAT64_RND_Z] = 1;
            break;
    }

    for(rnd_mode = 0; rnd_mode < 6; rnd_mode++) {
        if (rm_allowed[rnd_mode]) {
            AiBaseFloat64TestOpRm(op, prec, exp_bits, rnd_mode, seed);
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
AI_BASE_FLOAT64_OP_ENUM AiBaseFloat64GetOpFromStr(const UINT8_T *str)
{
    AI_BASE_FLOAT64_OP_ENUM op;
    for(op = 0; op < AI_BASE_FLOAT64_OP_COUNT; op++) {
        if (!strcmp(str, g_ai_base_float64_op_str[op]))
            break;
        }
    if (op == AI_BASE_FLOAT64_OP_COUNT) {
        fprintf(stderr, "Unknown operation: %s\n", str);
        exit(1);
    }
    return op;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64Help(void)
{
#if 0
    printf("usage: bftest [options] [first_op [last_op]]\n"
           "\n"
           "Options:\n"
           "-h         this AiBaseFloat64Help\n"
           "-s seed    set the initial seed\n");
	printf("%-20s %5s %3s %3s %5s %8s %8s %8s\n", "OP", "PREC", "EXP", "RND", "SEED", "CNT", "c/64bit", "ref");
    exit(1);
#endif
	return;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
#if 0
INT32_T AiBaseFloat64TestMain(void)
{
    INT32_T seed, duration_ms /*, c*/;
    AI_BASE_FLOAT64_OP_ENUM op, op_start, op_last;
                        
    seed = 1234;
    duration_ms = 100;
    op_start = (AI_BASE_FLOAT64_OP_ENUM)0;
    op_last = (AI_BASE_FLOAT64_OP_ENUM)(AI_BASE_FLOAT64_OP_COUNT - 1);

#if 0
	AiBaseFloat64TestMpfrExecInit();
#endif

	for(;;) {
        for(op = op_start; op <= op_last; op++) {
            if (op == AI_BASE_FLOAT64_OP_MUL_L2RADIX || op == AI_BASE_FLOAT64_OP_DIV_L2RADIX) {
                AiBaseFloat64TestOp(op, AI_BASE_FLOAT64_LIMB_BITS, duration_ms, 0, seed);
            } else if (op == AI_BASE_FLOAT64_OP_CAN_ROUND) {
                AiBaseFloat64TestOp(op, 8, duration_ms, AI_BASE_FLOAT64_BF_EXP_BITS_MAX, seed);
                AiBaseFloat64TestOp(op, 53, duration_ms, AI_BASE_FLOAT64_BF_EXP_BITS_MAX, seed);
                AiBaseFloat64TestOp(op, 256, duration_ms, AI_BASE_FLOAT64_BF_EXP_BITS_MAX, seed);
            } else {
                if (op == AI_BASE_FLOAT64_OP_MUL ||
                    op == AI_BASE_FLOAT64_OP_ADD ||
                    op == AI_BASE_FLOAT64_OP_DIV ||
                    op == AI_BASE_FLOAT64_OP_SQRT ||
                    op == AI_BASE_FLOAT64_OP_CMP_EQ ||
                    op == AI_BASE_FLOAT64_OP_CMP_LT ||
                    op == AI_BASE_FLOAT64_OP_CMP_LE) {
                    AiBaseFloat64TestOp(op, 53, duration_ms, 11, seed);
                }
                AiBaseFloat64TestOp(op, 53, duration_ms, AI_BASE_FLOAT64_BF_EXP_BITS_MAX, seed);
                AiBaseFloat64TestOp(op, 112, duration_ms, AI_BASE_FLOAT64_BF_EXP_BITS_MAX, seed);
                /* mpfr bug ? */
                if (op !=  AI_BASE_FLOAT64_OP_SQRT)
                    AiBaseFloat64TestOp(op, 256, duration_ms, AI_BASE_FLOAT64_BF_EXP_BITS_MAX, seed);
                AiBaseFloat64TestOp(op, 3000, duration_ms, AI_BASE_FLOAT64_BF_EXP_BITS_MAX, seed);
                if (op == AI_BASE_FLOAT64_OP_RECIP) {
                    AiBaseFloat64TestOp(op, 0, duration_ms, AI_BASE_FLOAT64_BF_EXP_BITS_MAX, seed);
                }
            }
        }
        seed++;
        duration_ms = 1000;
    }

    return 0;
}
#endif
#if 0
INT32_T AiBaseFloat64TestMain(void) {
	float a, b, r, r1;
	AI_BASE_FLOAT64_ICVT_UINT32 a_cvt, b_cvt, r_cvt;
	AI_BASE_FLOAT64_F_UINT a_unit, b_unit, r_unit, r1_unit;
    AI_BASE_FLOAT64_BF_FLAGS_T bf_flags;

    bf_flags = 0;

	a = 0.11f;
	b = 0.12f;
	r = 0.132f;

	memcpy((char *)&a_cvt, (char *)&a, sizeof(float));
	memcpy((char *)&b_cvt, (char *)&b, sizeof(float));
	memcpy((char *)&r_cvt, (char *)&r, sizeof(float));
	a_unit = AiBaseFloat64CvtU32Sf32(a_cvt, AI_BASE_FLOAT64_RM_RNE, &bf_flags);
	b_unit = AiBaseFloat64CvtU32Sf32(b_cvt, AI_BASE_FLOAT64_RM_RNE, &bf_flags);
	r1 = a*b;
	r_unit = AiBaseFloat64CvtU32Sf32(r_cvt, AI_BASE_FLOAT64_RM_RNE, &bf_flags);
	r1_unit = AiBaseFloat64CvtSf32U32(r_unit, AI_BASE_FLOAT64_RM_RNE, &bf_flags);
	memcpy((char *)&r1, (char *)&r1_unit, sizeof(float));
	if (r1 == r) {
		return 0;
	}
	return 1;
}
#endif
#if 1
INT32_T AiBaseFloat64TestMainBfFloat64(void) {
	FLOAT64_T a;
	AI_BASE_FLOAT64_ICVT_UINT64 a_cvt;
	AI_BASE_FLOAT64_BF_T a_bf;
	FLOAT64_T a_pres;
	AI_BASE_FLOAT64_BF_RND_T rnd_mode;
	AI_BASE_FLOAT64_ICVT_UINT64 a_pres_cvt;
	INT32_T result;

	a = 0.11;
	memcpy((char *)&a_cvt, (char *)&a, sizeof(FLOAT64_T));

	AiBaseFloat64BfSetFloat64(&a_bf, a);
	
	rnd_mode = AI_BASE_FLOAT64_BF_RNDNA;
	result =  AiBaseFloat64BfGetFloat64(&a_bf, &a_pres, rnd_mode);

	memcpy((char *)&a_pres_cvt, (char *)&a_pres, sizeof(FLOAT64_T));
	if (a != a_pres) {
		return 1;
	}
	if (a_cvt != a_pres_cvt) {
		return 1;
	}
	return 0;
}

ERROR_FLOAT64_T AiBaseFloat64TestMainBfFloat64LoopAll(void) {
	FLOAT64_T a;
	AI_BASE_FLOAT64_ICVT_UINT64 a_cvt;
	AI_BASE_FLOAT64_BF_T a_bf;
	FLOAT64_T a_pres;
	AI_BASE_FLOAT64_BF_RND_T rnd_mode;
	AI_BASE_FLOAT64_ICVT_UINT64 a_pres_cvt;
	ERROR_FLOAT64_T result = AI_BASE_FLOAT64_ERROR_SUCCESS;
	UINT64_T index;

	for(index = 0; index < 0x10000; index++) {
		a_cvt = ((AI_BASE_FLOAT64_ICVT_UINT64)(index | (index << 16) | (index << 32) | (index << 48)));

		memcpy((char *)&a, (char *)&a_cvt, sizeof(FLOAT64_T));

		AiBaseFloat64BfSetFloat64(&a_bf, a);
	
		rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;
		result =  AiBaseFloat64BfGetFloat64(&a_bf, &a_pres, rnd_mode);
		if (result != AI_BASE_FLOAT64_ERROR_SUCCESS) {
			while(1);
		}
		memcpy((char *)&a_pres_cvt, (char *)&a_pres, sizeof(FLOAT64_T));
		while(1) {
			if ((a == a_pres) && (a_cvt == a_pres_cvt)) 
				break;
			if (AiBaseFloat64BfIsNan(&a_bf) == AI_BASE_FLOAT64_ERROR_SUCCESS)
				break;
		}
	}

	return AI_BASE_FLOAT64_ERROR_SUCCESS;
}

INT32_T AiBaseFloat64TestMain(void) {
#if 0
	AiBaseFloat64TestMainBfFloat64();
	AiBaseFloat64TestMainBfFloat64LoopAll();
#endif
	return 0;
}
#endif

/*------------------------- End ---------------------------------------------*/
#endif
