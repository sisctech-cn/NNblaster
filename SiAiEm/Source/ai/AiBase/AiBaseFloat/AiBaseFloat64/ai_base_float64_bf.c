/**
 ******************************************************************************
 * file    ai_base_float_bf32.c
 * author  sunlingge
 * version V100
 * brief   the ai base float bf 32 driver.
 * description
 *	1. FLOAT64_T: sign(63 bit), exponent(52-62 bit), fraction(00-51 bit)
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
/* (128.0 / sqrt((i + 64) / 256)) & 0xff */
const UINT8_T g_ai_base_float64_rsqrt_table[192] = {
  0,254,252,250,248,247,245,243,241,240,238,236,235,233,232,230,
229,228,226,225,223,222,221,220,218,217,216,215,214,212,211,210,
209,208,207,206,205,204,203,202,201,200,199,198,197,196,195,194,
194,193,192,191,190,189,189,188,187,186,185,185,184,183,182,182,
181,180,180,179,178,178,177,176,176,175,174,174,173,172,172,171,
171,170,169,169,168,168,167,167,166,166,165,164,164,163,163,162,
162,161,161,160,160,159,159,158,158,158,157,157,156,156,155,155,
154,154,154,153,153,152,152,151,151,151,150,150,149,149,149,148,
148,147,147,147,146,146,146,145,145,144,144,144,143,143,143,142,
142,142,141,141,141,140,140,140,139,139,139,138,138,138,137,137,
137,137,136,136,136,135,135,135,134,134,134,134,133,133,133,132,
132,132,132,131,131,131,131,130,130,130,130,129,129,129,129,128,
};

/* base conversion from radix */
const UINT8_T g_ai_base_float64_digits_per_limb_table[AI_BASE_FLOAT64_BF_RADIX_MAX - 1] = {
32,20,16,13,12,11,10,10, 9, 9, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
};

/* base conversion to radix */
const UINT32_T g_ai_base_float64_inv_log2_radix[AI_BASE_FLOAT64_BF_RADIX_MAX - 1][AI_BASE_FLOAT64_LIMB_BITS / 2 + 1] = {
{ 0x80000000, 0x00000000,},
{ 0x50c24e60, 0xd4d4f4a7,},
{ 0x40000000, 0x00000000,},
{ 0x372068d2, 0x0a1ee5ca,},
{ 0x3184648d, 0xb8153e7a,},
{ 0x2d983275, 0x9d5369c4,},
{ 0x2aaaaaaa, 0xaaaaaaab,},
{ 0x28612730, 0x6a6a7a54,},
{ 0x268826a1, 0x3ef3fde6,},
{ 0x25001383, 0xbac8a744,},
{ 0x23b46706, 0x82c0c709,},
{ 0x229729f1, 0xb2c83ded,},
{ 0x219e7ffd, 0xa5ad572b,},
{ 0x20c33b88, 0xda7c29ab,},
{ 0x20000000, 0x00000000,},
{ 0x1f50b57e, 0xac5884b3,},
{ 0x1eb22cc6, 0x8aa6e26f,},
{ 0x1e21e118, 0x0c5daab2,},
{ 0x1d9dcd21, 0x439834e4,},
{ 0x1d244c78, 0x367a0d65,},
{ 0x1cb40589, 0xac173e0c,},
{ 0x1c4bd95b, 0xa8d72b0d,},
{ 0x1bead768, 0x98f8ce4c,},
{ 0x1b903469, 0x050f72e5,},
{ 0x1b3b433f, 0x2eb06f15,},
{ 0x1aeb6f75, 0x9c46fc38,},
{ 0x1aa038eb, 0x0e3bfd17,},
{ 0x1a593062, 0xb38d8c56,},
{ 0x1a15f4c3, 0x2b95a2e6,},
{ 0x19d630dc, 0xcc7ddef9,},
{ 0x19999999, 0x9999999a,},
{ 0x195fec80, 0x8a609431,},
{ 0x1928ee7b, 0x0b4f22f9,},
{ 0x18f46acf, 0x8c06e318,},
{ 0x18c23246, 0xdc0a9f3d,},
};

const AI_BASE_FLOAT64_LIMB_T g_ai_base_float64_log2_radix[AI_BASE_FLOAT64_BF_RADIX_MAX - 1] = {
0x20000000,
0x32b80347,
0x40000000,
0x4a4d3c26,
0x52b80347,
0x59d5d9fd,
0x60000000,
0x6570068e,
0x6a4d3c26,
0x6eb3a9f0,
0x72b80347,
0x766a008e,
0x79d5d9fd,
0x7d053f6d,
0x80000000,
0x82cc7edf,
0x8570068e,
0x87ef05ae,
0x8a4d3c26,
0x8c8ddd45,
0x8eb3a9f0,
0x90c10501,
0x92b80347,
0x949a784c,
0x966a008e,
0x982809d6,
0x99d5d9fd,
0x9b74948f,
0x9d053f6d,
0x9e88c6b3,
0xa0000000,
0xa16bad37,
0xa2cc7edf,
0xa4231623,
0xa570068e,
};

AI_BASE_FLOAT64_BF_CONST_CACHE g_ai_base_float64_log2_cache;
AI_BASE_FLOAT64_BF_CONST_CACHE g_ai_base_float64_pi_cache;

#ifdef AI_BASE_FLOAT64_FFT_MUL
#if defined(__AVX2__)
const INT32_T g_ai_base_float64_ntt_int_bits[NB_MODS] = { 254, 203, 152, 101, 50, };
const AI_BASE_FLOAT64_LIMB_T g_ai_base_float64_ntt_mods[NB_MODS] = { 0x00073a8000000001, 0x0007858000000001, 0x0007a38000000001, 0x0007a68000000001, 0x0007fd8000000001,};
const AI_BASE_FLOAT64_LIMB_T g_ai_base_float64_ntt_proot[2][NB_MODS] = {
    { 0x00056198d44332c8, 0x0002eb5d640aad39, 0x00047e31eaa35fd0, 0x0005271ac118a150, 0x00075e0ce8442bd5, },
    { 0x000461169761bcc5, 0x0002dac3cb2da688, 0x0004abc97751e3bf, 0x000656778fc8c485, 0x0000dc6469c269fa, },
};
const AI_BASE_FLOAT64_LIMB_T g_ai_base_float64_ntt_mods_cr[NB_MODS * (NB_MODS - 1) / 2] = {
 0x00020e4da740da8e, 0x0004c3dc09c09c1d, 0x000063bd097b4271, 0x000799d8f18f18fd,
 0x0005384222222264, 0x000572b07c1f07fe, 0x00035cd08888889a,
 0x00066015555557e3, 0x000725960b60b623,
 0x0002fc1fa1d6ce12,
};
#else
#if AI_BASE_FLOAT64_LIMB_BITS == 32
const INT32_T g_ai_base_float64_ntt_int_bits[NB_MODS] = { 148, 119, 89, 59, 29, };
const AI_BASE_FLOAT64_LIMB_T g_ai_base_float64_ntt_mods[NB_MODS] = { 0x0000000032b00001, 0x0000000033700001, 0x0000000036d00001, 0x0000000037300001, 0x000000003e500001,};
const AI_BASE_FLOAT64_LIMB_T g_ai_base_float64_ntt_proot[2][NB_MODS] = {
    { 0x0000000032525f31, 0x0000000005eb3b37, 0x00000000246eda9f, 0x0000000035f25901, 0x00000000022f5768, },
    { 0x00000000051eba1a, 0x00000000107be10e, 0x000000001cd574e0, 0x00000000053806e6, 0x000000002cd6bf98, },
};
const AI_BASE_FLOAT64_LIMB_T g_ai_base_float64_ntt_mods_cr[NB_MODS * (NB_MODS - 1) / 2] = {
 0x000000000449559a, 0x000000001eba6ca9, 0x000000002ec18e46, 0x000000000860160b,
 0x000000000d321307, 0x000000000bf51120, 0x000000000f662938,
 0x000000000932ab3e, 0x000000002f40eef8,
 0x000000002e760905,
};
#endif 
#endif
/* used for AiBaseFloat64MulModFast() */
AI_BASE_FLOAT64_LIMB_T g_ai_base_float64_ntt_mods_div[NB_MODS];
AI_BASE_FLOAT64_LIMB_T g_ai_base_float64_ntt_proot_pow[NB_MODS][2][NTT_PROOT_2EXP + 1];
AI_BASE_FLOAT64_LIMB_T g_ai_base_float64_ntt_proot_pow_inv[NB_MODS][2][NTT_PROOT_2EXP + 1];
NTTLimb *g_ai_base_float64_ntt_trig[NB_MODS][2][NTT_TRIG_K_MAX + 1];
/* 1/2^n mod m */
AI_BASE_FLOAT64_LIMB_T g_ai_base_float64_ntt_len_inv[NB_MODS][NTT_PROOT_2EXP + 1][2];
#if defined(__AVX2__)
AI_BASE_FLOAT64_M256D g_ai_base_float64_ntt_mods_cr_vec[NB_MODS * (NB_MODS - 1) / 2];
AI_BASE_FLOAT64_M256D g_ai_base_float64_ntt_mods_vec[NB_MODS];
AI_BASE_FLOAT64_M256D g_ai_base_float64_ntt_mods_inv_vec[NB_MODS];
#else
AI_BASE_FLOAT64_LIMB_T g_ai_base_float64_ntt_mods_cr_inv[NB_MODS * (NB_MODS - 1) / 2];
#endif
NTTLimb *AiBaseFloat64GetTrig(INT32_T k, INT32_T inverse, INT32_T m_idx);
#endif

/*------------------------- Function Definition -----------------------------*/
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfGetExpBits(AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    return (AI_BASE_FLOAT64_BF_EXP_BITS_MAX - ((flags >> AI_BASE_FLOAT64_BF_EXP_BITS_SHIFT) & AI_BASE_FLOAT64_BF_EXP_BITS_MASK));
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_BF_FLAGS_T AiBaseFloat64BfSetExpBits(INT32_T n)
{
#if 0
    return ((AI_BASE_FLOAT64_BF_EXP_BITS_MAX - n) << AI_BASE_FLOAT64_BF_EXP_BITS_SHIFT);
#endif
#if 1
	INT32_T exp_bits = n;
	INT32_T exp_bits_max = AI_BASE_FLOAT64_BF_EXP_BITS_MAX;
	INT32_T exp_bits_shift = AI_BASE_FLOAT64_BF_EXP_BITS_SHIFT;
	AI_BASE_FLOAT64_BF_FLAGS_T bf_flags;

	bf_flags = (exp_bits_max - exp_bits) << exp_bits_shift;
	return bf_flags;
#endif
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_SLIMB_T AiBaseFloat64BfMax(AI_BASE_FLOAT64_SLIMB_T a, AI_BASE_FLOAT64_SLIMB_T b)
{
    if (a > b)
        return a;
    else
        return b;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_SLIMB_T AiBaseFloat64BfMin(AI_BASE_FLOAT64_SLIMB_T a, AI_BASE_FLOAT64_SLIMB_T b)
{
    if (a < b)
        return a;
    else
        return b;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void *AiBaseFloat64BfRealloc(void *ptr, UINT32_T size)
{
    return NULL;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64BfDelete(AI_BASE_FLOAT64_BF_T *r)
{
    return;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64BfNeg(AI_BASE_FLOAT64_BF_T *r)
{
    r->sign ^= 1;
	return;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfIsFinite(const AI_BASE_FLOAT64_BF_T *a)
{
    return (a->expn < AI_BASE_FLOAT64_BF_EXP_INF);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_FLOAT64_T AiBaseFloat64BfIsNan(const AI_BASE_FLOAT64_BF_T *a)
{
    if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
		return AI_BASE_FLOAT64_ERROR_SUCCESS;
	}

	return AI_BASE_FLOAT64_ERROR_GENERAL;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfIsZero(const AI_BASE_FLOAT64_BF_T *a)
{
    return (a->expn == AI_BASE_FLOAT64_BF_EXP_ZERO);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* could leading zeros */
INT32_T AiBaseFloat64Clz(AI_BASE_FLOAT64_LIMB_T a)
{
    if (a == 0) {
        return AI_BASE_FLOAT64_LIMB_BITS;
    } else {
        return AiBaseFloat64ClzU64(a);
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64Ctz(AI_BASE_FLOAT64_LIMB_T a)
{
    if (a == 0) {
        return AI_BASE_FLOAT64_LIMB_BITS;
    } else {
        return AiBaseFloat64CtzU64(a);
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64CeilLog2(AI_BASE_FLOAT64_LIMB_T a)
{
    if (a <= 1)
        return 0;
    else
        return AI_BASE_FLOAT64_LIMB_BITS - AiBaseFloat64Clz(a - 1);
}

#if 0
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_SLIMB_T AiBaseFloat64CeilDiv(AI_BASE_FLOAT64_SLIMB_T a, AI_BASE_FLOAT64_LIMB_T b)
{
    if (a >= 0)
        return (a + b - 1) / b;
    else
        return a / (AI_BASE_FLOAT64_SLIMB_T)b;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_SLIMB_T AiBaseFloat64FloorDiv(AI_BASE_FLOAT64_SLIMB_T a, AI_BASE_FLOAT64_LIMB_T b)
{
    if (a >= 0) {
        return a / b;
    } else {
        return (a - b + 1) / (AI_BASE_FLOAT64_SLIMB_T)b;
    }
}
#endif

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* 'size' must be > 0 */
void *AiBaseFloat64BfMalloc(UINT32_T size)
{
    return AiBaseFloat64BfRealloc(NULL, size);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64BfFree(void *ptr)
{
    AiBaseFloat64BfRealloc(ptr, 0);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64BfInit(AI_BASE_FLOAT64_BF_T *r)
{
    r->sign = 0;
    r->expn = AI_BASE_FLOAT64_BF_EXP_ZERO;
    r->len = 0;
	memset(r->tab, 0, sizeof(AI_BASE_FLOAT64_DLIMB_T)*2);

	return;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_FLOAT64_T AiBaseFloat64BfResize(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T len)
{
	if (len > 2)
		return AI_BASE_FLOAT64_ERROR_GENERAL;
	
    if (len != r->len) {
		memset(r->tab, 0, sizeof(AI_BASE_FLOAT64_DLIMB_T)*2);
        r->len = len;
    }
	return AI_BASE_FLOAT64_ERROR_SUCCESS;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64BfSetUi(AI_BASE_FLOAT64_BF_T *r, UINT64_T a)
{
    r->sign = 0;
    if (a == 0) {
        r->expn = AI_BASE_FLOAT64_BF_EXP_ZERO;
        AiBaseFloat64BfResize(r, 0);
    } 
    else if (a <= 0xffffffff)
    {
        INT32_T shift;
        AiBaseFloat64BfResize(r, 1);
        shift = AiBaseFloat64Clz((AI_BASE_FLOAT64_LIMB_T)a);
        r->tab[0] = (AI_BASE_FLOAT64_DLIMB_T)(a << shift);
        r->expn = AI_BASE_FLOAT64_LIMB_BITS - shift;
    }
    else {
        UINT32_T a1, a0;
        INT32_T shift;
        AiBaseFloat64BfResize(r, 2);
        a0 = (UINT32_T)a;
        a1 = a >> 32;
        shift = AiBaseFloat64Clz(a1);
        r->tab[0] = a0 << shift;
        r->tab[1] = (a1 << shift) | (a0 >> (AI_BASE_FLOAT64_LIMB_BITS - shift));
        r->expn = 2 * AI_BASE_FLOAT64_LIMB_BITS - shift;
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64BfSetSi(AI_BASE_FLOAT64_BF_T *r, INT64_T a)
{
    if (a < 0) {
        AiBaseFloat64BfSetUi(r, -a);
        r->sign = 1;
    } else {
        AiBaseFloat64BfSetUi(r, a);
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64BfSetNan(AI_BASE_FLOAT64_BF_T *r)
{
    AiBaseFloat64BfResize(r, 0);
    r->expn = AI_BASE_FLOAT64_BF_EXP_NAN;
	/* sunlingge modify to linsen float consist */
    r->sign = 1;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64BfSetZero(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T is_neg)
{
    AiBaseFloat64BfResize(r, 0);
    r->expn = AI_BASE_FLOAT64_BF_EXP_ZERO;
    r->sign = is_neg;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64BfSetInf(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T is_neg)
{
    AiBaseFloat64BfResize(r, 0);
    r->expn = AI_BASE_FLOAT64_BF_EXP_INF;
    r->sign = is_neg;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64BfSet(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a)
{
    if (r == a)
        return;
    r->sign = a->sign;
    r->expn = a->expn;
    AiBaseFloat64BfResize(r, a->len);
    memcpy(r->tab, a->tab, (size_t)(a->len * sizeof(AI_BASE_FLOAT64_DLIMB_T)));
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* equivalent to AiBaseFloat64BfSet(r, a); AiBaseFloat64BfDelete(a) */ 
void AiBaseFloat64BfMove(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_BF_T *a)
{
    if (r == a)
        return;
    AiBaseFloat64BfFree(r->tab);
    *r = *a;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64GetLimbz(const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T idx)
{
    if (idx >= a->len)
        return 0;
    else
        return (AI_BASE_FLOAT64_LIMB_T)(a->tab[idx]);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* get AI_BASE_FLOAT64_LIMB_BITS at bit position 'pos' in tab */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64GetBits(const AI_BASE_FLOAT64_LIMB_T *tab, AI_BASE_FLOAT64_LIMB_T len, AI_BASE_FLOAT64_SLIMB_T pos)
{
    AI_BASE_FLOAT64_LIMB_T i, a0, a1;
    INT32_T p;

    i = pos >> AI_BASE_FLOAT64_LIMB_LOG2_BITS;
    p = pos & (AI_BASE_FLOAT64_LIMB_BITS - 1);
    if (i < len)
        a0 = tab[i];
    else
        a0 = 0;
    if (p == 0) {
        return a0;
    } else {
        i++;
        if (i < len)
            a1 = tab[i];
        else
            a1 = 0;
        return (a0 >> p) | (a1 << (AI_BASE_FLOAT64_LIMB_BITS - p));
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64GetBit(const AI_BASE_FLOAT64_LIMB_T *tab, AI_BASE_FLOAT64_LIMB_T len, AI_BASE_FLOAT64_SLIMB_T pos)
{
    AI_BASE_FLOAT64_SLIMB_T i;
    i = pos >> AI_BASE_FLOAT64_LIMB_LOG2_BITS;
    if (i < 0 || i >= (AI_BASE_FLOAT64_SLIMB_T)len)
        return 0;
    return (tab[i] >> (pos & (AI_BASE_FLOAT64_LIMB_BITS - 1))) & 1;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64LimbMask(INT32_T start, INT32_T last)
{
    AI_BASE_FLOAT64_LIMB_T v;
    INT32_T n;
    n = last - start + 1;
    if (n == AI_BASE_FLOAT64_LIMB_BITS)
        v = -1;
    else
        v = (((AI_BASE_FLOAT64_LIMB_T)1 << n) - 1) << start;
    return v;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* return != 0 if one bit between 0 and bit_pos inclusive is not zero. */
 AI_BASE_FLOAT64_LIMB_T AiBaseFloat64ScanBitNz(const AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_SLIMB_T bit_pos)
{
    AI_BASE_FLOAT64_SLIMB_T pos;
    AI_BASE_FLOAT64_LIMB_T v;
    
    pos = bit_pos >> AI_BASE_FLOAT64_LIMB_LOG2_BITS;
    if (pos < 0)
        return 0;
    v = r->tab[pos] & AiBaseFloat64LimbMask(0, bit_pos & (AI_BASE_FLOAT64_LIMB_BITS - 1));
    if (v != 0)
        return 1;
    pos--;
    while (pos >= 0) {
        if (r->tab[pos] != 0)
            return 1;
        pos--;
    }
    return 0;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* return the addend for rounding. Note that prec can be <= 0 for AiBaseFloat64BfRint() */
INT32_T AiBaseFloat64BfGetRndAdd(INT32_T *pret, const AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T l_value, AI_BASE_FLOAT64_SLIMB_T prec, INT32_T rnd_mode)
{
    INT32_T add_one, inexact;
    AI_BASE_FLOAT64_LIMB_T bit1, bit0;
    
    if (rnd_mode == AI_BASE_FLOAT64_BF_RNDF) {
        bit0 = 1; /* faithful rounding does not honor the INEXACT flag */
    } else {
        /* starting limb for bit 'prec + 1' */
        bit0 = AiBaseFloat64ScanBitNz(r, l_value * AI_BASE_FLOAT64_LIMB_BITS - 1 - AiBaseFloat64BfMax(0, prec + 1));
    }

    /* get the bit at 'prec' */
    bit1 = AiBaseFloat64GetBit((const AI_BASE_FLOAT64_LIMB_T *)(r->tab), l_value, l_value * AI_BASE_FLOAT64_LIMB_BITS - 1 - prec);
    inexact = (bit1 | bit0) != 0;
    
    add_one = 0;
    switch(rnd_mode) {
    case AI_BASE_FLOAT64_BF_RNDZ:
        break;
    case AI_BASE_FLOAT64_BF_RNDN:
        if (bit1) {
            if (bit0) {
                add_one = 1;
            } else {
                /* round to even */
                add_one = (INT32_T)AiBaseFloat64GetBit((const AI_BASE_FLOAT64_LIMB_T *)(r->tab), l_value, (l_value * AI_BASE_FLOAT64_LIMB_BITS - 1 - (prec - 1)));
            }
        }
        break;
    case AI_BASE_FLOAT64_BF_RNDD:
    case AI_BASE_FLOAT64_BF_RNDU:
        if (r->sign == (rnd_mode == AI_BASE_FLOAT64_BF_RNDD))
            add_one = inexact;
        break;
    case AI_BASE_FLOAT64_BF_RNDNA:
    case AI_BASE_FLOAT64_BF_RNDF:
        add_one = (INT32_T)bit1;
        break;
    default:
        abort();
    }
    
    if (inexact)
        *pret |= AI_BASE_FLOAT64_BF_ST_INEXACT;
    return add_one;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfSetOverFlow(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T sign, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    AI_BASE_FLOAT64_SLIMB_T i, l_value, e_max;
    INT32_T rnd_mode;
    
    rnd_mode = flags & AI_BASE_FLOAT64_BF_RND_MASK;
    if (prec == AI_BASE_FLOAT64_BF_PREC_INF ||
        rnd_mode == AI_BASE_FLOAT64_BF_RNDN ||
        rnd_mode == AI_BASE_FLOAT64_BF_RNDNA ||
        (rnd_mode == AI_BASE_FLOAT64_BF_RNDD && sign == 1) ||
        (rnd_mode == AI_BASE_FLOAT64_BF_RNDU && sign == 0)) {
        AiBaseFloat64BfSetInf(r, sign);
    } else {
        /* set to maximum finite number */
        l_value = (prec + AI_BASE_FLOAT64_LIMB_BITS - 1) / AI_BASE_FLOAT64_LIMB_BITS;
        AiBaseFloat64BfResize(r, l_value);
        r->tab[0] = AiBaseFloat64LimbMask(((AI_BASE_FLOAT64_LIMB_T)0-prec) & (AI_BASE_FLOAT64_LIMB_BITS - 1), AI_BASE_FLOAT64_LIMB_BITS - 1);
        for(i = 1; i < l_value; i++)
            r->tab[i] = (AI_BASE_FLOAT64_LIMB_T)-1;
        e_max = (AI_BASE_FLOAT64_LIMB_T)1 << (AiBaseFloat64BfGetExpBits(flags) - 1);
        r->expn = e_max;
        r->sign = sign;
    }
    return AI_BASE_FLOAT64_BF_ST_OVERFLOW | AI_BASE_FLOAT64_BF_ST_INEXACT;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* round to prec1 bits assuming 'r' is non zero and finite. 'r' is
   assumed to have length 'l_value'. Note: 'prec1' can be negative or
   infinite (AI_BASE_FLOAT64_BF_PREC_INF). */
INT32_T __AiBaseFloat64BfRound(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T prec1, AI_BASE_FLOAT64_BF_FLAGS_T flags, AI_BASE_FLOAT64_LIMB_T l_value)
{
    AI_BASE_FLOAT64_LIMB_T v, a;
    INT32_T shift, add_one, ret, rnd_mode;
    AI_BASE_FLOAT64_SLIMB_T i, bit_pos, pos, e_min, e_max, e_range, prec;

    /* e_min and e_max are computed to match the IEEE 754 conventions */
    e_range = (AI_BASE_FLOAT64_LIMB_T)1 << (AiBaseFloat64BfGetExpBits(flags) - 1);
    e_min = -e_range + 3;
    e_max = e_range;
    
    if ((r->expn < e_min) && (flags & AI_BASE_FLOAT64_BF_FLAG_SUBNORMAL)) {
        /* restrict the precision in case of potentially subnormal result */
        prec = prec1 - (e_min - r->expn);
    } else {
        prec = prec1;
    }
    
    /* round to prec bits */
    rnd_mode = flags & AI_BASE_FLOAT64_BF_RND_MASK;
    ret = 0;
    add_one = AiBaseFloat64BfGetRndAdd(&ret, r, l_value, prec, rnd_mode);
    
    if (prec <= 0) {
        if (add_one) {
            AiBaseFloat64BfResize(r, 1);
            r->tab[0] = (AI_BASE_FLOAT64_LIMB_T)1 << (AI_BASE_FLOAT64_LIMB_BITS - 1);
            r->expn += 1 - prec;
            ret |= AI_BASE_FLOAT64_BF_ST_UNDERFLOW | AI_BASE_FLOAT64_BF_ST_INEXACT;
            return ret;
        } else {
            /* goto underflow; */
			ret |= AI_BASE_FLOAT64_BF_ST_UNDERFLOW | AI_BASE_FLOAT64_BF_ST_INEXACT;
            AiBaseFloat64BfSetZero(r, r->sign);
            return ret;
        }
    } else if (add_one) {
        AI_BASE_FLOAT64_LIMB_T carry;
        
        /* add one starting at digit 'prec - 1' */
        bit_pos = l_value * AI_BASE_FLOAT64_LIMB_BITS - 1 - (prec - 1);
        pos = bit_pos >> AI_BASE_FLOAT64_LIMB_LOG2_BITS;
        carry = (AI_BASE_FLOAT64_LIMB_T)1 << (bit_pos & (AI_BASE_FLOAT64_LIMB_BITS - 1));
        
        for(i = pos; i < (AI_BASE_FLOAT64_SLIMB_T)l_value; i++) {
            v = (AI_BASE_FLOAT64_LIMB_T)(r->tab[i] + carry);
            carry = (v < carry);
            r->tab[i] = v;
            if (carry == 0)
                break;
        }
        if (carry) {
            /* shift right by one digit */
            v = 1;
            for(i = l_value - 1; i >= pos; i--) {
                a = (AI_BASE_FLOAT64_LIMB_T)r->tab[i];
                r->tab[i] = (a >> 1) | (v << (AI_BASE_FLOAT64_LIMB_BITS - 1));
                v = a;
            }
            r->expn++;
        }
    }
    
    /* check underflow */
    if ((r->expn < e_min)) {
        if (flags & AI_BASE_FLOAT64_BF_FLAG_SUBNORMAL) {
            /* if inexact, also set the underflow flag */
            if (ret & AI_BASE_FLOAT64_BF_ST_INEXACT)
                ret |= AI_BASE_FLOAT64_BF_ST_UNDERFLOW;
        } else {
        	/*underflow: */
            ret |= AI_BASE_FLOAT64_BF_ST_UNDERFLOW | AI_BASE_FLOAT64_BF_ST_INEXACT;
            AiBaseFloat64BfSetZero(r, r->sign);
            return ret;
        }
    }
    
    /* check overflow */
    if ((r->expn > e_max))
        return AiBaseFloat64BfSetOverFlow(r, r->sign, prec1, flags);
    
    /* keep the bits starting at 'prec - 1' */
    bit_pos = l_value * AI_BASE_FLOAT64_LIMB_BITS - 1 - (prec - 1);
    i = bit_pos >> AI_BASE_FLOAT64_LIMB_LOG2_BITS;
    if (i >= 0) {
        shift = bit_pos & (AI_BASE_FLOAT64_LIMB_BITS - 1);
        if (shift != 0)
            r->tab[i] &= AiBaseFloat64LimbMask(shift, AI_BASE_FLOAT64_LIMB_BITS - 1);
    } else {
        i = 0;
    }
    /* remove trailing zeros */
    while (r->tab[i] == 0)
        i++;
    if (i > 0) {
        l_value -= i;
        memmove(r->tab, r->tab + i, (size_t)(l_value * sizeof(AI_BASE_FLOAT64_LIMB_T)));
    }
    AiBaseFloat64BfResize(r, l_value);
    return ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* 'r' must be a finite number */
INT32_T AiBaseFloat64BfNormalizeAndRound(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T prec1, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    AI_BASE_FLOAT64_LIMB_T l_value, v, a;
    INT32_T shift, ret;
    AI_BASE_FLOAT64_SLIMB_T i;
    
    //    AiBaseFloat64BfPrintStr("bf_renorm", r);
    l_value = r->len;
    while (l_value > 0 && r->tab[l_value - 1] == 0)
        l_value--;
    if (l_value == 0) {
        /* zero */
        r->expn = AI_BASE_FLOAT64_BF_EXP_ZERO;
        AiBaseFloat64BfResize(r, 0);
        ret = 0;
    } else {
        r->expn -= (r->len - l_value) * AI_BASE_FLOAT64_LIMB_BITS;
        /* shift to have the MSB set to '1' */
        v = (AI_BASE_FLOAT64_LIMB_T)(r->tab[l_value - 1]);
        shift = AiBaseFloat64Clz(v);
        if (shift != 0) {
            v = 0;
            for(i = 0; i < (AI_BASE_FLOAT64_SLIMB_T)l_value; i++) {
                a = (AI_BASE_FLOAT64_LIMB_T)(r->tab[i]);
                r->tab[i] = (a << shift) | (v >> (AI_BASE_FLOAT64_LIMB_BITS - shift));
                v = a;
            }
            r->expn -= shift;
        }
        ret = __AiBaseFloat64BfRound(r, prec1, flags, l_value);
    }
    //    AiBaseFloat64BfPrintStr("r_final", r);
    return ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* return true if rounding can be done at precision 'prec' assuming
   the exact result r is such that |r-a| <= 2^(EXP(a)-k). */
/* XXX: check the case where the exponent would be incremented by the
   rounding */
INT32_T AiBaseFloat64BfCanRound(const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_SLIMB_T prec, AI_BASE_FLOAT64_BF_RND_T rnd_mode, AI_BASE_FLOAT64_SLIMB_T k)
{
    BOOL_T is_rndn;
    AI_BASE_FLOAT64_SLIMB_T bit_pos, n;
    AI_BASE_FLOAT64_LIMB_T bit;
    
    if (a->expn == AI_BASE_FLOAT64_BF_EXP_INF || a->expn == AI_BASE_FLOAT64_BF_EXP_NAN)
        return AI_BASE_FLOAT64_FALSE;
    if (rnd_mode == AI_BASE_FLOAT64_BF_RNDF) {
        return (k >= (prec + 1));
    }
    if (a->expn == AI_BASE_FLOAT64_BF_EXP_ZERO)
        return AI_BASE_FLOAT64_FALSE;
    is_rndn = (rnd_mode == AI_BASE_FLOAT64_BF_RNDN || rnd_mode == AI_BASE_FLOAT64_BF_RNDNA);
    if (k < (prec + 2))
        return AI_BASE_FLOAT64_FALSE;
    bit_pos = a->len * AI_BASE_FLOAT64_LIMB_BITS - 1 - prec;
    n = k - prec;
    /* bit pattern for RNDN or RNDNA: 0111.. or 1000...
       for other rounding modes: 000... or 111... 
    */
    bit = AiBaseFloat64GetBit((const AI_BASE_FLOAT64_LIMB_T *)(a->tab), a->len, bit_pos);
    bit_pos--;
    n--;
    bit ^= is_rndn;
    /* XXX: slow, but a few iterations on average */
    while (n != 0) {
        if (AiBaseFloat64GetBit((const AI_BASE_FLOAT64_LIMB_T *)(a->tab), a->len, bit_pos) != bit)
            return AI_BASE_FLOAT64_TRUE;
        bit_pos--;
        n--;
    }
    return AI_BASE_FLOAT64_FALSE;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfRound(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    if (r->len == 0)
        return 0;
    return __AiBaseFloat64BfRound(r, prec, flags, r->len);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* for debugging */
 void AiBaseFloat64DumpLimbs(const UINT8_T *str, const AI_BASE_FLOAT64_LIMB_T *tab, AI_BASE_FLOAT64_LIMB_T n)
{
#if 0
    AI_BASE_FLOAT64_LIMB_T i;
    printf("%s: len=%" AI_BASE_FLOAT64_PRID_LIMB "\n", str, n);
    for(i = 0; i < n; i++) {
        printf("%" AI_BASE_FLOAT64_PRID_LIMB ": " FMT_LIMB "\n",
               i, tab[i]);
    }
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
/* for debugging */
void AiBaseFloat64BfPrintStr(const UINT8_T *str, const AI_BASE_FLOAT64_BF_T *a)
{
    AI_BASE_FLOAT64_SLIMB_T i;
	UINT8_T display_str[1024];

    sprintf(display_str, "%s=", str);
	AiServerLogFp64(display_str);

    if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
        sprintf(display_str, "%s", "NaN");
		AiServerLogFp64(display_str);
    } else {
        if (a->sign) {
            sprintf(display_str, "%s","-");
			AiServerLogFp64(display_str);
		}
        if (a->expn == AI_BASE_FLOAT64_BF_EXP_ZERO) {
            sprintf(display_str, "%s","0");
			AiServerLogFp64(display_str);
        } else if (a->expn == AI_BASE_FLOAT64_BF_EXP_INF) {
            sprintf(display_str, "%s","inf");
			AiServerLogFp64(display_str);
        } else {
			sprintf(display_str, "%s","0x0.");
			AiServerLogFp64(display_str);
            for(i = a->len - 1; i >= 0; i--) {
                sprintf(display_str, AI_BASE_FLOAT64_FMT_LIMB, (unsigned int)(a->tab[i]));
				AiServerLogFp64(display_str);
			}
            sprintf(display_str, "p%" AI_BASE_FLOAT64_PRID_LIMB, (int)(a->expn));
			AiServerLogFp64(display_str);
        }
    }
    sprintf(display_str,"%s", "\n");
	AiServerLogFp64(display_str);

	// double
	sprintf(display_str, "[%s=", str);
	AiServerLogFp64(display_str);

    if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
        sprintf(display_str, "%s", "NaN");
		AiServerLogFp64(display_str);
    } else {
        if (a->sign) {
            sprintf(display_str, "%s","-");
			AiServerLogFp64(display_str);
		}
        if (a->expn == AI_BASE_FLOAT64_BF_EXP_ZERO) {
            sprintf(display_str, "%s","0");
			AiServerLogFp64(display_str);
        } else if (a->expn == AI_BASE_FLOAT64_BF_EXP_INF) {
            sprintf(display_str, "%s","inf");
			AiServerLogFp64(display_str);
        } else {
			double temp_float;
			temp_float = 1.0f;
            for(i = a->len - 1; i >= 0; i--) {
				temp_float *= (float)(a->tab[i]);
				temp_float *= pow(2, -32);
			}
			temp_float *= pow(2, (double)a->expn);
            sprintf(display_str, "%f", temp_float);
			AiServerLogFp64(display_str);
        }
    }
    sprintf(display_str,"]%s", "\n");
	AiServerLogFp64(display_str);
	return;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* compare the absolute value of 'a' and 'b'. Return < 0 if a < b, 0
   if a = b and > 0 otherwise. */
INT32_T AiBaseFloat64BfCmpu(const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b)
{
    AI_BASE_FLOAT64_SLIMB_T i;
    AI_BASE_FLOAT64_LIMB_T len, v1, v2;
    
    if (a->expn != b->expn) {
        if (a->expn < b->expn)
            return -1;
        else
            return 1;
    }
    len = AiBaseFloat64BfMax(a->len, b->len);
    for(i = len - 1; i >= 0; i--) {
        v1 = AiBaseFloat64GetLimbz(a, a->len - len + i);
        v2 = AiBaseFloat64GetLimbz(b, b->len - len + i);
        if (v1 != v2) {
            if (v1 < v2)
                return -1;
            else
                return 1;
        }
    }
    return 0;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* Full order: -0 < 0, NaN == NaN and NaN is larger than all other numbers */
INT32_T AiBaseFloat64BfCmpFull(const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b)
{
    INT32_T res;
    
    if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN || b->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
        if (a->expn == b->expn)
            res = 0;
        else if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN)
            res = 1;
        else
            res = -1;
    } else if (a->sign != b->sign) {
        res = (INT32_T)(1 - 2 * a->sign);
    } else {
        res = AiBaseFloat64BfCmpu(a, b);
        if (a->sign)
            res = -res;
    }
    return res;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfCmp(const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, INT32_T op)
{
    BOOL_T is_both_zero;
    INT32_T res;
    
    if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN || b->expn == AI_BASE_FLOAT64_BF_EXP_NAN)
        return 0;
    if (a->sign != b->sign) {
        is_both_zero = (a->expn == AI_BASE_FLOAT64_BF_EXP_ZERO && b->expn == AI_BASE_FLOAT64_BF_EXP_ZERO);
        if (is_both_zero) {
            return op & AI_BASE_FLOAT64_BF_CMP_EQ;
        } else if (op & AI_BASE_FLOAT64_BF_CMP_LT) {
            return (INT32_T)(a->sign);
        } else {
            return AI_BASE_FLOAT64_FALSE;
        }
    } else {
        res = AiBaseFloat64BfCmpu(a, b);
        if (res == 0) {
            return op & AI_BASE_FLOAT64_BF_CMP_EQ;
        } else if (op & AI_BASE_FLOAT64_BF_CMP_LT) {
            return (INT32_T)((res < 0) ^ a->sign);
        } else {
            return AI_BASE_FLOAT64_FALSE;
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
INT32_T AiBaseFloat64BfCmpEq(const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b)
{
    return AiBaseFloat64BfCmp(a, b, AI_BASE_FLOAT64_BF_CMP_EQ);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfCmpLe(const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b)
{
    return AiBaseFloat64BfCmp(a, b, AI_BASE_FLOAT64_BF_CMP_LE);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfCmpLt(const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b)
{
    return AiBaseFloat64BfCmp(a, b, AI_BASE_FLOAT64_BF_CMP_LT);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* Compute the number of bits 'n' matching the pattern:
   a= X1000..0
   b= X0111..1
              
   When computing a-b, the result will have at least n leading zero
   bits.

   Precondition: a > b and a.expn - b.expn = 0 or 1
*/
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64CountCancelledBits(const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b)
{
    AI_BASE_FLOAT64_SLIMB_T bit_offset, b_offset, n;
    INT32_T p, p1;
    AI_BASE_FLOAT64_LIMB_T v1, v2, mask;

    bit_offset = a->len * AI_BASE_FLOAT64_LIMB_BITS - 1;
    b_offset = (b->len - a->len) * AI_BASE_FLOAT64_LIMB_BITS - (AI_BASE_FLOAT64_LIMB_BITS - 1) +
        a->expn - b->expn;
    n = 0;

    /* first search the equals bits */
    for(;;) {
        v1 = AiBaseFloat64GetLimbz(a, bit_offset >> AI_BASE_FLOAT64_LIMB_LOG2_BITS);
        v2 = AiBaseFloat64GetBits((const AI_BASE_FLOAT64_LIMB_T *)(b->tab), b->len, bit_offset + b_offset);
        //        printf("v1=" FMT_LIMB " v2=" FMT_LIMB "\n", v1, v2);
        if (v1 != v2)
            break;
        n += AI_BASE_FLOAT64_LIMB_BITS;
        bit_offset -= AI_BASE_FLOAT64_LIMB_BITS;
    }
    /* find the position of the first different bit */
    p = AiBaseFloat64Clz(v1 ^ v2) + 1;
    n += p;
    /* then search for '0' in a and '1' in b */
    p = AI_BASE_FLOAT64_LIMB_BITS - p;
    if (p > 0) {
        /* search in the trailing p bits of v1 and v2 */
        mask = AiBaseFloat64LimbMask(0, p - 1);
        p1 = (INT32_T)AiBaseFloat64BfMin(AiBaseFloat64Clz(v1 & mask), AiBaseFloat64Clz((~v2) & mask)) - (AI_BASE_FLOAT64_LIMB_BITS - p);
        n += p1;
        if (p1 != p)
            goto done;
    }
    bit_offset -= AI_BASE_FLOAT64_LIMB_BITS;
    for(;;) {
        v1 = AiBaseFloat64GetLimbz(a, bit_offset >> AI_BASE_FLOAT64_LIMB_LOG2_BITS);
        v2 = AiBaseFloat64GetBits((const AI_BASE_FLOAT64_LIMB_T *)(b->tab), b->len, bit_offset + b_offset);
        //        printf("v1=" FMT_LIMB " v2=" FMT_LIMB "\n", v1, v2);
        if (v1 != 0 || v2 != -1) {
            /* different: count the matching bits */
            p1 = (INT32_T)AiBaseFloat64BfMin(AiBaseFloat64Clz(v1), AiBaseFloat64Clz(~v2));
            n += p1;
            break;
        }
        n += AI_BASE_FLOAT64_LIMB_BITS;
        bit_offset -= AI_BASE_FLOAT64_LIMB_BITS;
    }
 done:
    return n;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfAddInternal(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec,
                           AI_BASE_FLOAT64_BF_FLAGS_T flags, INT32_T b_neg)
{
    const AI_BASE_FLOAT64_BF_T *tmp;
    INT32_T is_sub, ret, cmp_res, a_sign, b_sign;

    a_sign = (INT32_T)a->sign;
    b_sign = (INT32_T)(b->sign ^ b_neg);
    is_sub = a_sign ^ b_sign;
    cmp_res = AiBaseFloat64BfCmpu(a, b);
    if (cmp_res < 0) {
        tmp = a;
        a = b;
        b = tmp;
        a_sign = b_sign; /* b_sign is never used later */
    }
    /* abs(a) >= abs(b) */
    if (cmp_res == 0 && is_sub && a->expn < AI_BASE_FLOAT64_BF_EXP_INF) {
        /* zero result */
        AiBaseFloat64BfSetZero(r, (flags & AI_BASE_FLOAT64_BF_RND_MASK) == AI_BASE_FLOAT64_BF_RNDD);
        ret = 0;
    } else if (a->len == 0 || b->len == 0) {
        ret = 0;
        if (a->expn >= AI_BASE_FLOAT64_BF_EXP_INF) {
            if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
                /* at least one operand is NaN */
                AiBaseFloat64BfSetNan(r);
            } else if (b->expn == AI_BASE_FLOAT64_BF_EXP_INF && is_sub) {
                /* infinities with different signs */
                AiBaseFloat64BfSetNan(r);
                ret = AI_BASE_FLOAT64_BF_ST_INVALID_OP;
            } else {
                AiBaseFloat64BfSetInf(r, a_sign);
            }
        } else {
            /* at least one zero and not subtract */
            AiBaseFloat64BfSet(r, a);
            r->sign = a_sign;
            goto renorm;
        }
    } else {
        AI_BASE_FLOAT64_SLIMB_T d, a_offset, b_bit_offset, i, cancelled_bits;
        AI_BASE_FLOAT64_LIMB_T carry, v1, v2, u, r_len, carry1, precl, tot_len, z, sub_mask;

        r->sign = a_sign;
        r->expn = a->expn;
        d = a->expn - b->expn;
        /* must add more precision for the leading cancelled bits in
           subtraction */
        if (is_sub) {
            if (d <= 1)
                cancelled_bits = AiBaseFloat64CountCancelledBits(a, b);
            else
                cancelled_bits = 1;
        } else {
            cancelled_bits = 0;
        }
        
        /* add two extra bits for rounding */
        precl = (cancelled_bits + prec + 2 + AI_BASE_FLOAT64_LIMB_BITS - 1) / AI_BASE_FLOAT64_LIMB_BITS;
        tot_len = AiBaseFloat64BfMax(a->len, b->len + (d + AI_BASE_FLOAT64_LIMB_BITS - 1) / AI_BASE_FLOAT64_LIMB_BITS);
        r_len = AiBaseFloat64BfMin(precl, tot_len);
        AiBaseFloat64BfResize(r, r_len);
        a_offset = a->len - r_len;
        b_bit_offset = (b->len - r_len) * AI_BASE_FLOAT64_LIMB_BITS + d;

        /* compute the bits before for the rounding */
        carry = is_sub;
        z = 0;
        sub_mask = -is_sub;
        i = r_len - tot_len;
        while (i < 0) {
            AI_BASE_FLOAT64_SLIMB_T ap, bp;
            BOOL_T inflag;
            
            ap = a_offset + i;
            bp = b_bit_offset + i * AI_BASE_FLOAT64_LIMB_BITS;
            inflag = AI_BASE_FLOAT64_FALSE;
            if (ap >= 0 && ap < (AI_BASE_FLOAT64_SLIMB_T)(a->len)) {
                v1 = (AI_BASE_FLOAT64_LIMB_T)(a->tab[ap]);
                inflag = AI_BASE_FLOAT64_TRUE;
            } else {
                v1 = 0;
            }
            if (bp + AI_BASE_FLOAT64_LIMB_BITS > 0 && bp < (AI_BASE_FLOAT64_SLIMB_T)(b->len * AI_BASE_FLOAT64_LIMB_BITS)) {
                v2 = AiBaseFloat64GetBits((const AI_BASE_FLOAT64_LIMB_T *)(b->tab), b->len, bp);
                inflag = AI_BASE_FLOAT64_TRUE;
            } else {
                v2 = 0;
            }
            if (!inflag) {
                /* outside 'a' and 'b': go directly to the next value
                   inside a or b so that the running time does not
                   depend on the exponent difference */
                i = 0;
                if (ap < 0)
                    i = AiBaseFloat64BfMin(i, -a_offset);
                /* b_bit_offset + i * AI_BASE_FLOAT64_LIMB_BITS + AI_BASE_FLOAT64_LIMB_BITS >= 1
                   equivalent to 
                   i >= ceil(-b_bit_offset + 1 - AI_BASE_FLOAT64_LIMB_BITS) / AI_BASE_FLOAT64_LIMB_BITS)
                */
                if (bp + AI_BASE_FLOAT64_LIMB_BITS <= 0)
                    i = AiBaseFloat64BfMin(i, (-b_bit_offset) >> AI_BASE_FLOAT64_LIMB_LOG2_BITS);
            } else {
                i++;
            }
            v2 ^= sub_mask;
            u = v1 + v2;
            carry1 = u < v1;
            u += carry;
            carry = (u < carry) | carry1;
            z |= u;
        }
        /* and the result */
        for(i = 0; i < (AI_BASE_FLOAT64_SLIMB_T)r_len; i++) {
            v1 = AiBaseFloat64GetLimbz(a, a_offset + i);
            v2 = AiBaseFloat64GetBits((const AI_BASE_FLOAT64_LIMB_T *)(b->tab), b->len, b_bit_offset + i * AI_BASE_FLOAT64_LIMB_BITS);
            v2 ^= sub_mask;
            u = v1 + v2;
            carry1 = u < v1;
            u += carry;
            carry = (u < carry) | carry1;
            r->tab[i] = u;
        }
        /* set the extra bits for the rounding */
        r->tab[0] |= (z != 0);

        /* carry is only possible in add case */
        if (!is_sub && carry) {
            AiBaseFloat64BfResize(r, r_len + 1);
            r->tab[r_len] = 1;
            r->expn += AI_BASE_FLOAT64_LIMB_BITS;
        }
    renorm:
        ret = AiBaseFloat64BfNormalizeAndRound(r, prec, flags);
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
INT32_T __AiBaseFloat64BfAdd(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec,
                     AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    return AiBaseFloat64BfAddInternal(r, a, b, prec, flags, 0);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T __AiBaseFloat64BfSub(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec,
                     AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    return AiBaseFloat64BfAddInternal(r, a, b, prec, flags, 1);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64MpAdd(AI_BASE_FLOAT64_LIMB_T *res, const AI_BASE_FLOAT64_LIMB_T *op1, const AI_BASE_FLOAT64_LIMB_T *op2, 
                     AI_BASE_FLOAT64_LIMB_T n, AI_BASE_FLOAT64_LIMB_T carry)
{
    AI_BASE_FLOAT64_SLIMB_T i;
    AI_BASE_FLOAT64_LIMB_T k, a, v, k1;
    
    k = carry;
    for(i=0;i<(AI_BASE_FLOAT64_SLIMB_T)n;i++) {
        v = op1[i];
        a = v + op2[i];
        k1 = a < v;
        a = a + k;
        k = (a < k) | k1;
        res[i] = a;
    }
    return k;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* tabr[] += taba[] * b, return the high word. */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64MpAddMul1(AI_BASE_FLOAT64_LIMB_T *tabr, const AI_BASE_FLOAT64_LIMB_T *taba, AI_BASE_FLOAT64_LIMB_T n,
                              AI_BASE_FLOAT64_LIMB_T b)
{
    AI_BASE_FLOAT64_LIMB_T i, l_value;
    AI_BASE_FLOAT64_DLIMB_T t;
    
    l_value = 0;
    for(i = 0; i < n; i++) {
        t = (AI_BASE_FLOAT64_DLIMB_T)taba[i] * (AI_BASE_FLOAT64_DLIMB_T)b + l_value + tabr[i];
        tabr[i] = (AI_BASE_FLOAT64_LIMB_T)t;
        /* l_value = t >> AI_BASE_FLOAT64_LIMB_BITS; */
    }
    return l_value;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* tabr[] -= taba[] * b. Return the value to substract to the high
   word. */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64MpSubMul1(AI_BASE_FLOAT64_LIMB_T *tabr, const AI_BASE_FLOAT64_LIMB_T *taba, AI_BASE_FLOAT64_LIMB_T n,
                          AI_BASE_FLOAT64_LIMB_T b)
{
    AI_BASE_FLOAT64_LIMB_T i, l_value;
    AI_BASE_FLOAT64_DLIMB_T t;
    
    l_value = 0;
    for(i = 0; i < n; i++) {
        t = tabr[i] - (AI_BASE_FLOAT64_DLIMB_T)taba[i] * (AI_BASE_FLOAT64_DLIMB_T)b - l_value;
        tabr[i] = (AI_BASE_FLOAT64_LIMB_T)t;
        /* l_value = (AI_BASE_FLOAT64_LIMB_T)((AI_BASE_FLOAT64_DLIMB_T)0-(t >> AI_BASE_FLOAT64_LIMB_BITS)); */
    }
    return l_value;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* WARNING: d must be >= 2^(AI_BASE_FLOAT64_LIMB_BITS-1) */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64Udiv1normInit(AI_BASE_FLOAT64_LIMB_T d)
{
    AI_BASE_FLOAT64_LIMB_T a0, a1;
    a1 = (AI_BASE_FLOAT64_LIMB_T)0-d - 1;
    a0 = -1;
    return (((AI_BASE_FLOAT64_DLIMB_T)0/*a1 << AI_BASE_FLOAT64_LIMB_BITS*/) | a0) / d;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* return the quotient and the remainder in '*pr'of 'a1*2^AI_BASE_FLOAT64_LIMB_BITS+a0
   / d' with 0 <= a1 < d. */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64Udiv1norm(AI_BASE_FLOAT64_LIMB_T *pr, AI_BASE_FLOAT64_LIMB_T a1, AI_BASE_FLOAT64_LIMB_T a0,
                                AI_BASE_FLOAT64_LIMB_T d, AI_BASE_FLOAT64_LIMB_T d_inv)
{
    AI_BASE_FLOAT64_LIMB_T n1m, n_adj, q, r, ah;
    AI_BASE_FLOAT64_DLIMB_T a;
    n1m = ((AI_BASE_FLOAT64_SLIMB_T)a0 >> (AI_BASE_FLOAT64_LIMB_BITS - 1));
    n_adj = a0 + (n1m & d);
    a = (AI_BASE_FLOAT64_DLIMB_T)d_inv * (a1 - n1m) + n_adj;
    q = /* (a >> AI_BASE_FLOAT64_LIMB_BITS) + */ a1;
    /* compute a - q * r and update q so that the remainder is\
       between 0 and d - 1 */
    a = ((AI_BASE_FLOAT64_DLIMB_T)0 /* a1 << AI_BASE_FLOAT64_LIMB_BITS */) | a0;
    a = a - (AI_BASE_FLOAT64_DLIMB_T)q * d - d;
    ah = 0 /* a >> AI_BASE_FLOAT64_LIMB_BITS */;
    q += 1 + ah;
    r = (AI_BASE_FLOAT64_LIMB_T)a + (ah & d);
    *pr = r;
    return q;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* b must be >= 1 << (AI_BASE_FLOAT64_LIMB_BITS - 1) */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64MpDiv1norm(AI_BASE_FLOAT64_LIMB_T *tabr, const AI_BASE_FLOAT64_LIMB_T *taba, AI_BASE_FLOAT64_LIMB_T n,
                          AI_BASE_FLOAT64_LIMB_T b, AI_BASE_FLOAT64_LIMB_T r)
{
    AI_BASE_FLOAT64_SLIMB_T i;

    if (n >= 3) {
        AI_BASE_FLOAT64_LIMB_T b_inv;
        b_inv = AiBaseFloat64Udiv1normInit(b);
        for(i = n - 1; i >= 0; i--) {
            tabr[i] = AiBaseFloat64Udiv1norm(&r, r, taba[i], b, b_inv);
        }
    } else {
        AI_BASE_FLOAT64_DLIMB_T a1;
        for(i = n - 1; i >= 0; i--) {
            a1 = ((AI_BASE_FLOAT64_DLIMB_T)0 /* r << AI_BASE_FLOAT64_LIMB_BITS */) | taba[i];
            tabr[i] = (AI_BASE_FLOAT64_LIMB_T)(a1 / b);
            r = a1 % b;
        }
    }
    return r;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* base case division: divides taba[0..na-1] by tabb[0..nb-1]. tabb[nb
   - 1] must be >= 1 << (AI_BASE_FLOAT64_LIMB_BITS - 1). na - nb must be >= 0. 'taba'
   is modified and contains the remainder (nb limbs). tabq[0..na-nb]
   contains the quotient. taba[na] is modified. */
void AiBaseFloat64MpDivnorm(AI_BASE_FLOAT64_LIMB_T *tabq, 
                       AI_BASE_FLOAT64_LIMB_T *taba, AI_BASE_FLOAT64_LIMB_T na, 
                       const AI_BASE_FLOAT64_LIMB_T *tabb, AI_BASE_FLOAT64_LIMB_T nb)
{
    AI_BASE_FLOAT64_LIMB_T r, a, c, q, v, b1, b1_inv, n, dummy_r;
    AI_BASE_FLOAT64_SLIMB_T i;

    b1 = tabb[nb - 1];
    if (nb == 1) {
        taba[0] = AiBaseFloat64MpDiv1norm(tabq, taba, na, b1, 0);
        return;
    }
    taba[na] = 0;
    n = na - nb;
    if (n >= 3)
        b1_inv = AiBaseFloat64Udiv1normInit(b1);
    else
        b1_inv = 0;

    /* XXX: could simplify the first iteration */
    for(i = n; i >= 0; i--) {
        if ((taba[i + nb] >= b1)) {
            q = -1;
        } else if (b1_inv) {
            q = AiBaseFloat64Udiv1norm(&dummy_r, taba[i + nb], taba[i + nb - 1], b1, b1_inv);
        } else {
            AI_BASE_FLOAT64_DLIMB_T al;
            al = ((AI_BASE_FLOAT64_DLIMB_T)0 /* taba[i + nb] << AI_BASE_FLOAT64_LIMB_BITS */) | taba[i + nb - 1];
            q = (AI_BASE_FLOAT64_LIMB_T)(al / b1);
            r = al % b1;
        }
        r = AiBaseFloat64MpSubMul1(taba + i, tabb, nb, q);

        v = taba[i + nb];
        a = v - r;
        c = (a > v);
        taba[i + nb] = a;

        if (c != 0) {
            /* negative result */
            for(;;) {
                q--;
                c = AiBaseFloat64MpAdd(taba + i, taba + i, tabb, nb, 0);
                /* propagate carry and test if positive result */
                if (c != 0) {
                    if (++taba[i + nb] == 0) {
                        break;
                    }
                }
            }
        }
        tabq[i] = q;
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfMul(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec,
           AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    AI_BASE_FLOAT64_LIMB_T i;
    INT32_T ret, r_sign;

    if (a->len < b->len) {
        const AI_BASE_FLOAT64_BF_T *tmp = a;
        a = b;
        b = tmp;
    }
    r_sign = (INT32_T)(a->sign ^ b->sign);
    /* here b->len <= a->len */
    if (b->len == 0) {
        if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN || b->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
            AiBaseFloat64BfSetNan(r);
            ret = 0;
        } else if (a->expn == AI_BASE_FLOAT64_BF_EXP_INF || b->expn == AI_BASE_FLOAT64_BF_EXP_INF) {
            if ((a->expn == AI_BASE_FLOAT64_BF_EXP_INF && b->expn == AI_BASE_FLOAT64_BF_EXP_ZERO) ||
                (a->expn == AI_BASE_FLOAT64_BF_EXP_ZERO && b->expn == AI_BASE_FLOAT64_BF_EXP_INF)) {
                AiBaseFloat64BfSetNan(r);
                ret = AI_BASE_FLOAT64_BF_ST_INVALID_OP;
            } else {
                AiBaseFloat64BfSetInf(r, r_sign);
                ret = 0;
            }
        } else {
            AiBaseFloat64BfSetZero(r, r_sign);
            ret = 0;
        }
    } else {
        AI_BASE_FLOAT64_BF_T tmp, *r1 = NULL;
        AI_BASE_FLOAT64_LIMB_T a_len, b_len, precl;
        AI_BASE_FLOAT64_LIMB_T *a_tab, *b_tab;
            
        a_len = a->len;
        b_len = b->len;
        
        if ((flags & AI_BASE_FLOAT64_BF_RND_MASK) == AI_BASE_FLOAT64_BF_RNDF) {
            /* faithful rounding does not require using the full inputs */
            precl = (prec + 2 + AI_BASE_FLOAT64_LIMB_BITS - 1) / AI_BASE_FLOAT64_LIMB_BITS;
            a_len = AiBaseFloat64BfMin(a_len, precl);
            b_len = AiBaseFloat64BfMin(b_len, precl);
        }
        a_tab = (AI_BASE_FLOAT64_LIMB_T *)(a->tab + a->len - a_len);
        b_tab = (AI_BASE_FLOAT64_LIMB_T *)(b->tab + b->len - b_len);
        
#ifdef AI_BASE_FLOAT64_FFT_MUL
        if (b_len >= AI_BASE_FLOAT64_FFT_MUL_THRESHOLD) {
            INT32_T mul_flags = 0;
            if (r == a)
                mul_flags |= AI_BASE_FLOAT64_FFT_MUL_R_OVERLAP_A;
            if (r == b)
                mul_flags |= AI_BASE_FLOAT64_FFT_MUL_R_OVERLAP_B;
            AiBaseFloat64FftMul(r, a_tab, a_len, b_tab, b_len, mul_flags);
        } else
#endif
        {
            if (r == a || r == b) {
                AiBaseFloat64BfInit(&tmp);
                r1 = r;
                r = &tmp;
            }
            AiBaseFloat64BfResize(r, a_len + b_len);
            memset(r->tab, 0, (size_t)(sizeof(AI_BASE_FLOAT64_LIMB_T) * a_len));
            for(i = 0; i < b_len; i++) {
                r->tab[i + a_len] = AiBaseFloat64MpAddMul1((AI_BASE_FLOAT64_LIMB_T *)(r->tab + i), a_tab, a_len,
                                                b_tab[i]);
            }
        }
        r->sign = r_sign;
        r->expn = a->expn + b->expn;
        ret = AiBaseFloat64BfNormalizeAndRound(r, prec, flags);
        if (r == &tmp)
            AiBaseFloat64BfMove(r1, &tmp);
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
/* multiply 'r' by 2^e */
INT32_T AiBaseFloat64BfMul2Exp(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_SLIMB_T e, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    AI_BASE_FLOAT64_SLIMB_T e_max;
    if (r->len == 0)
        return 0;
    e_max = ((AI_BASE_FLOAT64_LIMB_T)1 << AI_BASE_FLOAT64_BF_EXP_BITS_MAX) - 1;
    e = AiBaseFloat64BfMax(e, -e_max);
    e = AiBaseFloat64BfMin(e, e_max);
    r->expn += e;
    return __AiBaseFloat64BfRound(r, prec, flags, r->len);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64BfRecipRec(AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *x, AI_BASE_FLOAT64_LIMB_T prec1)
{
    AI_BASE_FLOAT64_BF_T t0;
    AI_BASE_FLOAT64_LIMB_T prec;

    AiBaseFloat64BfInit(&t0);

    if (prec1 <= AI_BASE_FLOAT64_LIMB_BITS - 3) {
        AI_BASE_FLOAT64_LIMB_T v;
        /* initial approximation */
        v = (AI_BASE_FLOAT64_LIMB_T)(x->tab[x->len - 1]);
        /* 2^(L-1) <= v <= 2^L-1 (L=AI_BASE_FLOAT64_LIMB_BITS) */
        v = 0 /* (AI_BASE_FLOAT64_LIMB_T)(((AI_BASE_FLOAT64_DLIMB_T)1 << (2 * AI_BASE_FLOAT64_LIMB_BITS - 2)) / v) */;
        /* 2^(L-2) <= v <= 2^(L-1) */
        AiBaseFloat64BfResize(a, 1);
        a->sign = x->sign;
        a->expn = 2 - x->expn;
        if (v == ((AI_BASE_FLOAT64_LIMB_T)1 << (AI_BASE_FLOAT64_LIMB_BITS - 1))) {
            a->tab[0] = v;
        } else {
            a->tab[0] = v << 1;
            a->expn--;
        }
        a->tab[0] &= AiBaseFloat64LimbMask((INT32_T)(AI_BASE_FLOAT64_LIMB_BITS - prec1), AI_BASE_FLOAT64_LIMB_BITS - 1);
    } else {
        /* XXX: prove the added precision */
        AiBaseFloat64BfRecipRec(a, x, (prec1 / 2) + 8);
        prec = prec1 + 8;

        /* a = a + a * (1 - x * a) */
        AiBaseFloat64BfMul(&t0, x, a, prec, AI_BASE_FLOAT64_BF_RNDF);
        t0.sign ^= 1;
        AiBaseFloat64BfAddSi(&t0, &t0, 1, prec, AI_BASE_FLOAT64_BF_RNDF);
        AiBaseFloat64BfMul(&t0, &t0, a, prec, AI_BASE_FLOAT64_BF_RNDF);
        AiBaseFloat64BfAdd(a, a, &t0, prec1, AI_BASE_FLOAT64_BF_RNDF);
    }
    //    AiBaseFloat64BfPrintStr("r", a);
    AiBaseFloat64BfDelete(&t0);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* Note: only faithful rounding is supported */
void AiBaseFloat64BfRecip(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec)
{
    if (r == a)
		return;
    if (a->len == 0) {
        if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
            AiBaseFloat64BfSetNan(r);
        } else if (a->expn == AI_BASE_FLOAT64_BF_EXP_INF) {
            AiBaseFloat64BfSetZero(r, a->sign);
        } else {
            AiBaseFloat64BfSetInf(r, a->sign);
        }
    } else {
        //        AiBaseFloat64BfPrintStr("a", a);
        AiBaseFloat64BfRecipRec(r, a, prec);
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* add zero limbs if necessary to have at least precl limbs */
void AiBaseFloat64BfAddZeroLimbs(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T precl)
{
    AI_BASE_FLOAT64_LIMB_T l_value = r->len;
    if (l_value < precl) {
        AiBaseFloat64BfResize(r, precl);
        memmove(r->tab + precl - l_value, r->tab, (size_t)(l_value * sizeof(AI_BASE_FLOAT64_LIMB_T)));
        memset(r->tab, 0, (size_t)((precl - l_value) * sizeof(AI_BASE_FLOAT64_LIMB_T)));
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* set a bit to 1 at bit position >= (precl * AI_BASE_FLOAT64_LIMB_BITS - 1) */
void AiBaseFloat64BfOrOne(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T precl)
{
    AiBaseFloat64BfAddZeroLimbs(r, precl);
    r->tab[0] |= 1;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* Return e such as a=m*2^e with m odd integer. return 0 if a is zero,
   Infinite or Nan. */
AI_BASE_FLOAT64_SLIMB_T AiBaseFloat64BfGetExpMin(const AI_BASE_FLOAT64_BF_T *a)
{
    AI_BASE_FLOAT64_SLIMB_T i;
    AI_BASE_FLOAT64_LIMB_T v;
    INT32_T k;
    
    for(i = 0; i < (AI_BASE_FLOAT64_SLIMB_T)(a->len); i++) {
        v = (AI_BASE_FLOAT64_LIMB_T)(a->tab[i]);
        if (v != 0) {
            k = AiBaseFloat64Ctz(v);
            return a->expn - (a->len - i) * AI_BASE_FLOAT64_LIMB_BITS + k;
        }
    }
    return 0;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* a and b must be finite numbers with a >= 0 and b > 0. 'q' is the
   integer defined as floor(a/b) and r = a - q * b. */
void AiBaseFloat64BfTdivremu(AI_BASE_FLOAT64_BF_T *q, AI_BASE_FLOAT64_BF_T *r,
                        const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b)
{
    if (a->expn < b->expn) {
        AiBaseFloat64BfSetUi(q, 0);
        AiBaseFloat64BfSet(r, a);
    } else {
        /* for large numbers, use the floating point division in
           faithful mode */
        AiBaseFloat64BfDiv(q, a, b, AiBaseFloat64BfMax(a->expn - b->expn + 1, 2), AI_BASE_FLOAT64_BF_RNDF);
        AiBaseFloat64BfRint(q, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        AiBaseFloat64BfMul(r, q, b, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfSub(r, a, r, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDN);
        if (r->len != 0 && r->sign) {
            AiBaseFloat64BfAddSi(q, q, -1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
            AiBaseFloat64BfAdd(r, r, b, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
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
INT32_T __AiBaseFloat64BfDiv(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec,
                    AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    INT32_T ret, r_sign;
    AI_BASE_FLOAT64_LIMB_T n, nb, precl;
    
    r_sign = (INT32_T)(a->sign ^ b->sign);
    if (a->expn >= AI_BASE_FLOAT64_BF_EXP_INF || b->expn >= AI_BASE_FLOAT64_BF_EXP_INF) {
        if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN || b->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
            AiBaseFloat64BfSetNan(r);
            return 0;
        } else if (a->expn == AI_BASE_FLOAT64_BF_EXP_INF && b->expn == AI_BASE_FLOAT64_BF_EXP_INF) {
            AiBaseFloat64BfSetNan(r);
            return AI_BASE_FLOAT64_BF_ST_INVALID_OP;
        } else if (a->expn == AI_BASE_FLOAT64_BF_EXP_INF) {
            AiBaseFloat64BfSetInf(r, r_sign);
            return 0;
        } else {
            AiBaseFloat64BfSetZero(r, r_sign);
            return 0;
        }
    } else if (a->expn == AI_BASE_FLOAT64_BF_EXP_ZERO) {
        if (b->expn == AI_BASE_FLOAT64_BF_EXP_ZERO) {
            AiBaseFloat64BfSetNan(r);
            return AI_BASE_FLOAT64_BF_ST_INVALID_OP;
        } else {
            AiBaseFloat64BfSetZero(r, r_sign);
            return 0;
        }
    } else if (b->expn == AI_BASE_FLOAT64_BF_EXP_ZERO) {
        AiBaseFloat64BfSetInf(r, r_sign);
        return AI_BASE_FLOAT64_BF_ST_DIVIDE_ZERO;
    }

    /* number of limbs of the quotient (2 extra bits for rounding) */
    precl = (prec + 2 + AI_BASE_FLOAT64_LIMB_BITS - 1) / AI_BASE_FLOAT64_LIMB_BITS;
    nb = b->len;
    n = AiBaseFloat64BfMax(a->len, precl);
    
    if (nb <= AI_BASE_FLOAT64_BASECASE_DIV_THRESHOLD_B ||
        (AI_BASE_FLOAT64_SLIMB_T)n <= (AI_BASE_FLOAT64_BASECASE_DIV_THRESHOLD_Q - 1)) {
        AI_BASE_FLOAT64_LIMB_T *taba, na, i;
        AI_BASE_FLOAT64_SLIMB_T d;
        
        na = n + nb;
        taba = AiBaseFloat64BfMalloc((UINT32_T)((na + 1) * sizeof(AI_BASE_FLOAT64_LIMB_T)));
        d = na - a->len;
        memset(taba, 0, (size_t)(d * sizeof(AI_BASE_FLOAT64_LIMB_T)));
        memcpy(taba + d, a->tab, (size_t)(a->len * sizeof(AI_BASE_FLOAT64_LIMB_T)));
        AiBaseFloat64BfResize(r, n + 1);
        AiBaseFloat64MpDivnorm((AI_BASE_FLOAT64_LIMB_T *)(r->tab), taba, na, (const AI_BASE_FLOAT64_LIMB_T *)(b->tab), nb);
        
        /* see if non zero remainder */
        for(i = 0; i < nb; i++) {
            if (taba[i] != 0) {
                r->tab[0] |= 1;
                break;
            }
        }
        AiBaseFloat64BfFree(taba);
        r->expn = a->expn - b->expn + AI_BASE_FLOAT64_LIMB_BITS;
        r->sign = r_sign;
        ret = AiBaseFloat64BfNormalizeAndRound(r, prec, flags);
    } else if ((flags & AI_BASE_FLOAT64_BF_RND_MASK) == AI_BASE_FLOAT64_BF_RNDF) {
        AI_BASE_FLOAT64_BF_T b_inv;
        AiBaseFloat64BfInit(&b_inv);
        AiBaseFloat64BfRecip(&b_inv, b, prec + 3);
        ret = AiBaseFloat64BfMul(r, a, &b_inv, prec, flags);
        AiBaseFloat64BfDelete(&b_inv);
    } else {
        AI_BASE_FLOAT64_BF_T a1_s, *a1 = &a1_s;
        AI_BASE_FLOAT64_BF_T b1_s, *b1 = &b1_s;
        AI_BASE_FLOAT64_BF_T rem_s, *rem = &rem_s;
        
        /* convert the mantissa of 'a' and 'b' to integers and generate
           a quotient with at least prec + 2 bits */
        a1->expn = (n + nb) * AI_BASE_FLOAT64_LIMB_BITS;
        memcpy(a1->tab, a->tab, sizeof(AI_BASE_FLOAT64_DLIMB_T)*2);
        a1->len = a->len;
        a1->sign = 0;

        b1->expn = nb * AI_BASE_FLOAT64_LIMB_BITS;
        memcpy(b1->tab, b->tab, sizeof(AI_BASE_FLOAT64_DLIMB_T)*2);
        b1->len = nb;
        b1->sign = 0;

        AiBaseFloat64BfInit(rem);
        AiBaseFloat64BfTdivremu(r, rem, a1, b1);
        /* the remainder is not zero: put it in the rounding bits */
        if (rem->len != 0) {
            AiBaseFloat64BfOrOne(r, precl);
        }
        AiBaseFloat64BfDelete(rem);
        r->expn += a->expn - b->expn - n * AI_BASE_FLOAT64_LIMB_BITS;
        r->sign = r_sign;
        ret = AiBaseFloat64BfRound(r, prec, flags);
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
/* division and remainder. 
   
   rnd_mode is the rounding mode for the quotient. The additional
   rounding mode AI_BASE_FLOAT64_BF_RND_EUCLIDIAN is supported.

   'q' is an integer. 'r' is rounded with prec and flags (prec can be
   AI_BASE_FLOAT64_BF_PREC_INF).
*/
INT32_T AiBaseFloat64BfDivrem(AI_BASE_FLOAT64_BF_T *q, AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b,
              AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags, INT32_T rnd_mode)
{
    AI_BASE_FLOAT64_BF_T a1_s, *a1 = &a1_s;
    AI_BASE_FLOAT64_BF_T b1_s, *b1 = &b1_s;
    INT32_T q_sign;
    BOOL_T is_ceil, is_rndn;
    
    if (!(q != a && q != b))
		return 0;
    if (!(r != a && r != b))
		return 0;;
    if (!(q != r))
		return 0;
    
    if (a->len == 0 || b->len == 0) {
        AiBaseFloat64BfSetZero(q, 0);
        if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN || b->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
            AiBaseFloat64BfSetNan(r);
            return 0;
        } else if (a->expn == AI_BASE_FLOAT64_BF_EXP_INF || b->expn == AI_BASE_FLOAT64_BF_EXP_ZERO) {
            AiBaseFloat64BfSetNan(r);
            return AI_BASE_FLOAT64_BF_ST_INVALID_OP;
        } else {
            AiBaseFloat64BfSet(r, a);
            return AiBaseFloat64BfRound(r, prec, flags);
        }
    }

    q_sign = (INT32_T)(a->sign ^ b->sign);
    is_rndn = (rnd_mode == AI_BASE_FLOAT64_BF_RNDN || rnd_mode == AI_BASE_FLOAT64_BF_RNDNA);
    switch(rnd_mode) {
    default:
    case AI_BASE_FLOAT64_BF_RNDZ:
    case AI_BASE_FLOAT64_BF_RNDN:
    case AI_BASE_FLOAT64_BF_RNDNA:
        is_ceil = AI_BASE_FLOAT64_FALSE;
        break;
    case AI_BASE_FLOAT64_BF_RNDD:
        is_ceil = q_sign;
        break;
    case AI_BASE_FLOAT64_BF_RNDU:
        is_ceil = q_sign ^ 1;
        break;
    case AI_BASE_FLOAT64_BF_DIVREM_EUCLIDIAN:
        is_ceil = (BOOL_T)(a->sign);
        break;
    }

    a1->expn = a->expn;
    memcpy(a1->tab, a->tab, sizeof(AI_BASE_FLOAT64_DLIMB_T)*2);
    a1->len = a->len;
    a1->sign = 0;
    
    b1->expn = b->expn;
    memcpy(b1->tab, b->tab, sizeof(AI_BASE_FLOAT64_DLIMB_T)*2);
    b1->len = b->len;
    b1->sign = 0;

    /* XXX: could improve to avoid having a large 'q' */
    AiBaseFloat64BfTdivremu(q, r, a1, b1);

    if (r->len != 0) {
        if (is_rndn) {
            INT32_T res;
            b1->expn--;
            res = AiBaseFloat64BfCmpu(r, b1);
            b1->expn++;
            if (res > 0 ||
                (res == 0 &&
                 (rnd_mode == AI_BASE_FLOAT64_BF_RNDNA ||
                  AiBaseFloat64GetBit((const AI_BASE_FLOAT64_LIMB_T *)(q->tab), q->len, q->len * AI_BASE_FLOAT64_LIMB_BITS - q->expn)))) {
                goto do_sub_r;
            }
        } else if (is_ceil) {
        do_sub_r:
            AiBaseFloat64BfAddSi(q, q, 1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
            AiBaseFloat64BfSub(r, r, b1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        }
    }

    r->sign ^= a->sign;
    q->sign = q_sign;
    return AiBaseFloat64BfRound(r, prec, flags);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfFmod(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec,
            AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    AI_BASE_FLOAT64_BF_T q_s, *q = &q_s;
    INT32_T ret;
    
    AiBaseFloat64BfInit(q);
    ret = AiBaseFloat64BfDivrem(q, r, a, b, prec, flags, AI_BASE_FLOAT64_BF_RNDZ);
    AiBaseFloat64BfDelete(q);
    return ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfRemainder(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec,
                 AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    AI_BASE_FLOAT64_BF_T q_s, *q = &q_s;
    INT32_T ret;
    
    AiBaseFloat64BfInit(q);
    ret = AiBaseFloat64BfDivrem(q, r, a, b, prec, flags, AI_BASE_FLOAT64_BF_RNDN);
    AiBaseFloat64BfDelete(q);
    return ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
 INT32_T AiBaseFloat64BfGetLimb(AI_BASE_FLOAT64_SLIMB_T *pres, const AI_BASE_FLOAT64_BF_T *a, INT32_T flags)
{
    return AiBaseFloat64BfGetInt32((INT32_T *)pres, a, flags);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfRemquo(AI_BASE_FLOAT64_SLIMB_T *pq, AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec,
              AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    AI_BASE_FLOAT64_BF_T q_s, *q = &q_s;
    INT32_T ret;
    
    AiBaseFloat64BfInit(q);
    ret = AiBaseFloat64BfDivrem(q, r, a, b, prec, flags, AI_BASE_FLOAT64_BF_RNDN);
    AiBaseFloat64BfGetLimb(pq, q, AI_BASE_FLOAT64_BF_GET_INT_MOD);
    AiBaseFloat64BfDelete(q);
    return ret;
}

 /**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
 AI_BASE_FLOAT64_LIMB_T AiBaseFloat64MulMod(AI_BASE_FLOAT64_LIMB_T a, AI_BASE_FLOAT64_LIMB_T b, AI_BASE_FLOAT64_LIMB_T m)
{
    AI_BASE_FLOAT64_DLIMB_T t;
    t = (AI_BASE_FLOAT64_DLIMB_T)a * (AI_BASE_FLOAT64_DLIMB_T)b;
    return t % m;
}

#if defined(AI_BASE_FLOAT64_USE_MUL_CHECK)
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64MpMod1(const AI_BASE_FLOAT64_LIMB_T *tab, AI_BASE_FLOAT64_LIMB_T n, AI_BASE_FLOAT64_LIMB_T m, AI_BASE_FLOAT64_LIMB_T r)
{
    AI_BASE_FLOAT64_SLIMB_T i;
    AI_BASE_FLOAT64_DLIMB_T t;

    for(i = n - 1; i >= 0; i--) {
        t = ((AI_BASE_FLOAT64_DLIMB_T)r << AI_BASE_FLOAT64_LIMB_BITS) | tab[i];
        r = t % m;
    }
    return r;
}
#endif

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void __AiBaseFloat64BfRsqrt(AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *x, AI_BASE_FLOAT64_LIMB_T prec1)
{
    AI_BASE_FLOAT64_BF_T t0;
    AI_BASE_FLOAT64_LIMB_T prec;

    if (prec1 <= 7) {
        AI_BASE_FLOAT64_SLIMB_T e;
        AI_BASE_FLOAT64_LIMB_T v;
        /* initial approximation using 8 mantissa bits */
        v = (AI_BASE_FLOAT64_LIMB_T)(x->tab[x->len - 1]);
        e = x->expn;
        if (e & 1) {
            v >>= 1;
            e++;
        }
        v = g_ai_base_float64_rsqrt_table[(v >> (AI_BASE_FLOAT64_LIMB_BITS - 8)) - 64];
        e = 1 - (e / 2);
        if (v == 0) {
            v = 128; /* real table value is 256 */
            e++;
        }
        AiBaseFloat64BfResize(a, 1);
        a->tab[0] = (v << (AI_BASE_FLOAT64_LIMB_BITS - 8)) &
            AiBaseFloat64LimbMask((INT32_T)(AI_BASE_FLOAT64_LIMB_BITS - prec1), AI_BASE_FLOAT64_LIMB_BITS - 1);
        a->expn = e;
        a->sign = 0;
    } else {
        /* XXX: prove rounding */
        __AiBaseFloat64BfRsqrt(a, x, (prec1 / 2) + 2);

        prec = prec1 + 3;

        /* x' = x + (x/2) * (1 - a * x^2) */
        AiBaseFloat64BfInit(&t0);

        AiBaseFloat64BfMul(&t0, a, a, prec, AI_BASE_FLOAT64_BF_RNDF);
        AiBaseFloat64BfMul(&t0, &t0, x, prec, AI_BASE_FLOAT64_BF_RNDF);
        t0.sign ^= 1;
        AiBaseFloat64BfAddSi(&t0, &t0, 1, prec, AI_BASE_FLOAT64_BF_RNDF);
        AiBaseFloat64BfMul(&t0, &t0, a, prec, AI_BASE_FLOAT64_BF_RNDF);
        if (t0.len != 0)
            t0.expn--;
        AiBaseFloat64BfAdd(a, a, &t0, prec1, AI_BASE_FLOAT64_BF_RNDF);

        AiBaseFloat64BfDelete(&t0);
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T __AiBaseFloat64BfSqrt(AI_BASE_FLOAT64_BF_T *x, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec1, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    AI_BASE_FLOAT64_BF_T t0, t1;
    AI_BASE_FLOAT64_LIMB_T prec;
    INT32_T ret;
    
    /* XXX: prove rounding */
    __AiBaseFloat64BfRsqrt(x, a, (prec1 / 2) + 2);
    prec = prec1 + 3;

    /* x' = a * x + (x/2) * (a - (a * x)^2) */
    
    AiBaseFloat64BfInit(&t0);
    AiBaseFloat64BfInit(&t1);
    AiBaseFloat64BfMul(&t1, x, a, prec, AI_BASE_FLOAT64_BF_RNDF);
    AiBaseFloat64BfMul(&t0, &t1, &t1, prec, AI_BASE_FLOAT64_BF_RNDF);
    t0.sign ^= 1;
    AiBaseFloat64BfAdd(&t0, &t0, a, prec, AI_BASE_FLOAT64_BF_RNDF);
    AiBaseFloat64BfMul(&t0, &t0, x, prec, AI_BASE_FLOAT64_BF_RNDF);
    if (t0.len != 0)
        t0.expn--;
    ret = AiBaseFloat64BfAdd(x, &t1, &t0, prec1, flags);
    
    AiBaseFloat64BfDelete(&t0);
    AiBaseFloat64BfDelete(&t1);
    return ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* Note: only faithful rounding is supported */
void AiBaseFloat64BfRsqrt(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec)
{
    if (a->len == 0) {
        if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN ||
            (a->sign && a->expn != AI_BASE_FLOAT64_BF_EXP_ZERO)) {
            AiBaseFloat64BfSetNan(r);
        } else if (a->expn == AI_BASE_FLOAT64_BF_EXP_INF) {
            AiBaseFloat64BfSetZero(r, a->sign);
        } else {
            AiBaseFloat64BfSetInf(r, 0);
        }
    } else if (a->sign) {
        AiBaseFloat64BfSetNan(r);
    } else {
        //        AiBaseFloat64BfPrintStr("a", a);
        __AiBaseFloat64BfRsqrt(r, a, prec);
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* return floor(sqrt(a)) */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64BfIsqrt(AI_BASE_FLOAT64_LIMB_T a)
{
    UINT32_T l_value;
    AI_BASE_FLOAT64_LIMB_T u, s;
    
    if (a == 0)
        return 0;
    l_value = AiBaseFloat64CeilLog2(a);
    u = (AI_BASE_FLOAT64_LIMB_T)1 << ((l_value + 1) / 2);
    /* u >= floor(sqrt(a)) */
    for(;;) {
        s = u;
        u = ((a / s) + s) / 2;
        if (u >= s)
            break;
    }
    return s;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* Integer square root with remainder. 'a' must be an integer. r =
   floor(sqrt(a)) and rem = a - r^2.  AI_BASE_FLOAT64_BF_ST_INEXACT is set if the result
   is inexact. 'rem' can be NULL if the remainder is not needed. */
INT32_T AiBaseFloat64BfSqrtrem(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_BF_T *rem1, const AI_BASE_FLOAT64_BF_T *a)
{
    INT32_T ret;
    
    if (a->len == 0) {
        if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
            AiBaseFloat64BfSetNan(r);
        } else if (a->expn == AI_BASE_FLOAT64_BF_EXP_INF && a->sign) {
            goto invalid_op;
        } else {
            AiBaseFloat64BfSet(r, a);
        }
        if (rem1)
            AiBaseFloat64BfSetUi(rem1, 0);
        ret = 0;
    } else if (a->sign) {
 invalid_op:
        AiBaseFloat64BfSetNan(r);
        if (rem1)
            AiBaseFloat64BfSetUi(rem1, 0);
        ret = AI_BASE_FLOAT64_BF_ST_INVALID_OP;
    } else {
        AI_BASE_FLOAT64_BF_T rem_s, *rem;
        INT32_T res;
        
        AiBaseFloat64BfSqrt(r, a, (a->expn + 1) / 2, AI_BASE_FLOAT64_BF_RNDF);
        AiBaseFloat64BfRint(r, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        /* see if the result is exact by computing the remainder */
        if (rem1) {
            rem = rem1;
        } else {
            rem = &rem_s;
            AiBaseFloat64BfInit(rem);
        }
        AiBaseFloat64BfMul(rem, r, r, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        ret = 0;
        if (rem1) {
            AiBaseFloat64BfNeg(rem);
            AiBaseFloat64BfAdd(rem, rem, a, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
            if (rem->len != 0) {
                ret = AI_BASE_FLOAT64_BF_ST_INEXACT;
                if (rem->sign) {
                    AI_BASE_FLOAT64_BF_T a1_s, *a1 = &a1_s;
                    AiBaseFloat64BfAddSi(r, r, -1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
                    memcpy(a1->tab, a->tab, sizeof(AI_BASE_FLOAT64_DLIMB_T)*2);
                    a1->len = a->len;
                    a1->sign = a->sign;
                    a1->expn = a->expn + 1;
                    AiBaseFloat64BfAdd(rem, rem, r, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
                    AiBaseFloat64BfAddSi(rem, rem, 1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
                }
            } else {
                ret = 0;
            }
        } else {
            res = AiBaseFloat64BfCmpu(rem, a);
            AiBaseFloat64BfDelete(rem);
            //            printf("res2=%d\n", res2);
            if (res > 0) {
                /* need to correct the result */
                AiBaseFloat64BfAddSi(r, r, -1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
            }
            ret = (res != 0 ? AI_BASE_FLOAT64_BF_ST_INEXACT : 0);
        }
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
INT32_T AiBaseFloat64BfSqrt(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    INT32_T rnd_mode, ret;

    if (r == a)
		return 0;

    if (a->len == 0) {
        if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
            AiBaseFloat64BfSetNan(r);
        } else if (a->expn == AI_BASE_FLOAT64_BF_EXP_INF && a->sign) {
            goto invalid_op;
        } else {
            AiBaseFloat64BfSet(r, a);
        }
        ret = 0;
    } else if (a->sign) {
 invalid_op:
        AiBaseFloat64BfSetNan(r);
        ret = AI_BASE_FLOAT64_BF_ST_INVALID_OP;
    } else {
        rnd_mode = flags & AI_BASE_FLOAT64_BF_RND_MASK;
        if (rnd_mode == AI_BASE_FLOAT64_BF_RNDF) {
            ret = __AiBaseFloat64BfSqrt(r, a, prec, flags);
        } else {
            AI_BASE_FLOAT64_BF_T a1_s, *a1 = &a1_s;
            AI_BASE_FLOAT64_SLIMB_T d, prec2;
            INT32_T res1, res2;

            AiBaseFloat64BfInit(a1);
            AiBaseFloat64BfSet(a1, a);
            /* convert the mantissa to an integer with at most 2 *
               prec + 4 bits */
            prec2 = prec + 2;
            /* make '-a->expn + d' divisible by two */
            d = prec2 * 2 - (a->expn & 1);
            a1->expn = d;
            res1 = AiBaseFloat64BfRint(a1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
            res2 = AiBaseFloat64BfSqrtrem(r, NULL, a1);
            AiBaseFloat64BfDelete(a1);
            if ((res2 | res1) != 0) {
                AiBaseFloat64BfOrOne(r, (prec2 + AI_BASE_FLOAT64_LIMB_BITS - 1) / AI_BASE_FLOAT64_LIMB_BITS);
            }
            /* update the exponent */
            r->expn -= (-a->expn + d) >> 1;
            ret = AiBaseFloat64BfRound(r, prec, flags);
        }
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
INT32_T AiBaseFloat64BfOp2(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec,
                            AI_BASE_FLOAT64_BF_FLAGS_T flags, AI_BASE_FLOAT64_BF_OP2_FUNC_T *func)
{
    AI_BASE_FLOAT64_BF_T tmp;
    INT32_T ret;
    
    if (r == a || r == b) {
        AiBaseFloat64BfInit(&tmp);
        ret = func(&tmp, a, b, prec, flags);
        AiBaseFloat64BfMove(r, &tmp);
    } else {
        ret = func(r, a, b, prec, flags);
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
INT32_T AiBaseFloat64BfAdd(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec,
            AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    return AiBaseFloat64BfOp2(r, a, b, prec, flags, __AiBaseFloat64BfAdd);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfSub(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec,
            AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    return AiBaseFloat64BfOp2(r, a, b, prec, flags, __AiBaseFloat64BfSub);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfDiv(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec,
           AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    return AiBaseFloat64BfOp2(r, a, b, prec, flags, __AiBaseFloat64BfDiv);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfMulUi(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, UINT64_T b1, AI_BASE_FLOAT64_LIMB_T prec,
               AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    AI_BASE_FLOAT64_BF_T b;
    INT32_T ret;
    AiBaseFloat64BfInit(&b);
    AiBaseFloat64BfSetUi(&b, b1);
    ret = AiBaseFloat64BfMul(r, a, &b, prec, flags);
    AiBaseFloat64BfDelete(&b);
    return ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfMulSi(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, INT64_T b1, AI_BASE_FLOAT64_LIMB_T prec,
               AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    AI_BASE_FLOAT64_BF_T b;
    INT32_T ret;
    AiBaseFloat64BfInit(&b);
    AiBaseFloat64BfSetSi(&b, b1);
    ret = AiBaseFloat64BfMul(r, a, &b, prec, flags);
    AiBaseFloat64BfDelete(&b);
    return ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfAddSi(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, INT64_T b1, AI_BASE_FLOAT64_LIMB_T prec,
              AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    AI_BASE_FLOAT64_BF_T b;
    INT32_T ret;
    
    AiBaseFloat64BfInit(&b);
    AiBaseFloat64BfSetSi(&b, b1);
    ret = AiBaseFloat64BfAdd(r, a, &b, prec, flags);
    AiBaseFloat64BfDelete(&b);
    return ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfPowUi(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T b, AI_BASE_FLOAT64_LIMB_T prec,
              AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    INT32_T ret, n_bits, i;
    
    if (r == a)
		return 0;

    if (b == 0) {
        AiBaseFloat64BfSetUi(r, 1);
        return 0;
    }
    AiBaseFloat64BfSet(r, a);
    ret = 0;
    n_bits = AI_BASE_FLOAT64_LIMB_BITS - AiBaseFloat64Clz(b);
    for(i = n_bits - 2; i >= 0; i--) {
        ret |= AiBaseFloat64BfMul(r, r, r, prec, flags);
        if ((b >> i) & 1)
            ret |= AiBaseFloat64BfMul(r, r, a, prec, flags);
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
INT32_T AiBaseFloat64BfPowUiUi(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T a1, AI_BASE_FLOAT64_LIMB_T b, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    AI_BASE_FLOAT64_BF_T a;
    INT32_T ret;
    
    AiBaseFloat64BfInit(&a);
    AiBaseFloat64BfSetUi(&a, a1);
    ret = AiBaseFloat64BfPowUi(r, &a, b, prec, flags);
    AiBaseFloat64BfDelete(&a);
    return ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* convert to integer (single rounding) */
INT32_T AiBaseFloat64BfRint(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    INT32_T ret;
    if (r->len == 0)
        return 0;
    if (r->expn <= 0) {
        ret = __AiBaseFloat64BfRound(r, r->expn, flags & ~AI_BASE_FLOAT64_BF_FLAG_SUBNORMAL, r->len) &
            ~AI_BASE_FLOAT64_BF_ST_UNDERFLOW;
    } else {
        ret = __AiBaseFloat64BfRound(r, AiBaseFloat64BfMin(r->expn, prec), flags, r->len);
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
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64BfLogicOp1(AI_BASE_FLOAT64_LIMB_T a, AI_BASE_FLOAT64_LIMB_T b, INT32_T op)
{
    switch(op) {
    case AI_BASE_FLOAT64_BF_LOGIC_OR:
        return a | b;
    case AI_BASE_FLOAT64_BF_LOGIC_XOR:
        return a ^ b;
    default:
    case AI_BASE_FLOAT64_BF_LOGIC_AND:
        return a & b;
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64BfLogicOp(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a1, const AI_BASE_FLOAT64_BF_T *b1, INT32_T op)
{
    AI_BASE_FLOAT64_BF_T b1_s, a1_s, *a, *b;
    AI_BASE_FLOAT64_LIMB_T a_sign, b_sign, r_sign;
    AI_BASE_FLOAT64_SLIMB_T l_value, i, a_bit_offset, b_bit_offset;
    AI_BASE_FLOAT64_LIMB_T v1, v2, v1_mask, v2_mask, r_mask;
    
    if (!(r != a1 && r != b1))
		return;

    if (a1->expn <= 0)
        a_sign = 0; /* minus zero is considered as positive */
    else
        a_sign = a1->sign;

    if (b1->expn <= 0)
        b_sign = 0; /* minus zero is considered as positive */
    else
        b_sign = b1->sign;
    
    if (a_sign) {
        a = &a1_s;
        AiBaseFloat64BfInit(a);
        AiBaseFloat64BfAddSi(a, a1, 1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
    } else {
        a = (AI_BASE_FLOAT64_BF_T *)a1;
    }

    if (b_sign) {
        b = &b1_s;
        AiBaseFloat64BfInit(b);
        AiBaseFloat64BfAddSi(b, b1, 1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
    } else {
        b = (AI_BASE_FLOAT64_BF_T *)b1;
    }
    
    r_sign = AiBaseFloat64BfLogicOp1(a_sign, b_sign, op);
    if (op == AI_BASE_FLOAT64_BF_LOGIC_AND && r_sign == 0) {
        /* no need to compute extra zeros for and */
        if (a_sign == 0 && b_sign == 0)
            l_value = AiBaseFloat64BfMin(a->expn, b->expn);
        else if (a_sign == 0)
            l_value = a->expn;
        else
            l_value = b->expn;
    } else {
        l_value = AiBaseFloat64BfMax(a->expn, b->expn);
    }
    /* Note: a or b can be zero */
    l_value = (AiBaseFloat64BfMax(l_value, 1) + AI_BASE_FLOAT64_LIMB_BITS - 1) / AI_BASE_FLOAT64_LIMB_BITS;
    AiBaseFloat64BfResize(r, l_value);
    a_bit_offset = a->len * AI_BASE_FLOAT64_LIMB_BITS - a->expn;
    b_bit_offset = b->len * AI_BASE_FLOAT64_LIMB_BITS - b->expn;
    v1_mask = (AI_BASE_FLOAT64_LIMB_T)0-a_sign;
    v2_mask = (AI_BASE_FLOAT64_LIMB_T)0-b_sign;
    r_mask = (AI_BASE_FLOAT64_LIMB_T)0-r_sign;
    for(i = 0; i < l_value; i++) {
        v1 = AiBaseFloat64GetBits((const AI_BASE_FLOAT64_LIMB_T *)(a->tab), a->len, a_bit_offset + i * AI_BASE_FLOAT64_LIMB_BITS) ^ v1_mask;
        v2 = AiBaseFloat64GetBits((const AI_BASE_FLOAT64_LIMB_T *)(b->tab), b->len, b_bit_offset + i * AI_BASE_FLOAT64_LIMB_BITS) ^ v2_mask;
        r->tab[i] = AiBaseFloat64BfLogicOp1(v1, v2, op) ^ r_mask;
    }
    r->expn = l_value * AI_BASE_FLOAT64_LIMB_BITS;
    r->sign = r_sign;
    AiBaseFloat64BfNormalizeAndRound(r, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
    if (r_sign)
        AiBaseFloat64BfAddSi(r, r, -1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
    if (a == &b1_s)
        AiBaseFloat64BfDelete(a);
    if (b == &b1_s)
        AiBaseFloat64BfDelete(b);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* 'a' and 'b' must be integers */
void AiBaseFloat64BfLogicOr(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b)
{
    AiBaseFloat64BfLogicOp(r, a, b, AI_BASE_FLOAT64_BF_LOGIC_OR);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* 'a' and 'b' must be integers */
void AiBaseFloat64BfLogicXor(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b)
{
    AiBaseFloat64BfLogicOp(r, a, b, AI_BASE_FLOAT64_BF_LOGIC_XOR);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* 'a' and 'b' must be integers */
void AiBaseFloat64BfLogicAnd(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b)
{
    AiBaseFloat64BfLogicOp(r, a, b, AI_BASE_FLOAT64_BF_LOGIC_AND);
}
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* The rounding mode is always AI_BASE_FLOAT64_BF_RNDZ. Return AI_BASE_FLOAT64_BF_ST_OVERFLOW if there
   is an overflow and 0 otherwise. */
INT32_T AiBaseFloat64BfGetInt32(INT32_T *pres, const AI_BASE_FLOAT64_BF_T *a, INT32_T flags)
{
    UINT32_T v;
    INT32_T ret;
    if (a->expn >= AI_BASE_FLOAT64_BF_EXP_INF) {
        ret = 0;
        if (flags & AI_BASE_FLOAT64_BF_GET_INT_MOD) {
            v = 0;
        } else if (a->expn == AI_BASE_FLOAT64_BF_EXP_INF) {
            v = (UINT32_T)(INT32_MAX + a->sign);
        } else {
            v = INT32_MAX;
        }
    } else if (a->expn <= 0) {
        v = 0;
        ret = 0;
    } else if (a->expn <= 31) {
        v = (UINT32_T)(a->tab[a->len - 1] >> (AI_BASE_FLOAT64_LIMB_BITS - a->expn));
        if (a->sign)
            v = (UINT32_T)((UINT32_T)0-v);
        ret = 0;
    } else if (!(flags & AI_BASE_FLOAT64_BF_GET_INT_MOD)) {
        ret = AI_BASE_FLOAT64_BF_ST_OVERFLOW;
        if (a->sign) {
            v = (UINT32_T)INT32_MAX + 1;
#if 0
            if (a->expn == 32 && 
                ((a->tab[a->len - 1]) >> (UINT32_T)(AI_BASE_FLOAT64_LIMB_BITS - 32)) == v) {
                ret = 0;
            }
#endif
        } else {
            v = INT32_MAX;
        }
    } else {
        v = (UINT32_T)AiBaseFloat64GetBits((const AI_BASE_FLOAT64_LIMB_T *)(a->tab), a->len, a->len * AI_BASE_FLOAT64_LIMB_BITS - a->expn); 
        if (a->sign)
            v = (UINT32_T)((UINT32_T)0-v);
        ret = 0;
    }
    *pres = v;
    return ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* The rounding mode is always AI_BASE_FLOAT64_BF_RNDZ. Return AI_BASE_FLOAT64_BF_ST_OVERFLOW if there
   is an overflow and 0 otherwise. */
INT32_T AiBaseFloat64BfGetInt16(INT16_T *pres, const AI_BASE_FLOAT64_BF_T *a, INT32_T flags)
{
    UINT64_T v;
    INT32_T ret;
    if (a->expn >= AI_BASE_FLOAT64_BF_EXP_INF) {
        ret = 0;
        if (flags & AI_BASE_FLOAT64_BF_GET_INT_MOD) {
            v = 0;
        } else if (a->expn == AI_BASE_FLOAT64_BF_EXP_INF) {
            v = (UINT64_T)INT64_MAX + a->sign;
        } else {
            v = INT64_MAX;
        }
    } else if (a->expn <= 0) {
        v = 0;
        ret = 0;
    } else if (a->expn <= 63) {

        if (a->expn <= 32)
            v = a->tab[a->len - 1] >> (AI_BASE_FLOAT64_LIMB_BITS - a->expn);
        else
            v = (((UINT64_T)a->tab[a->len - 1] << 32) |
                 AiBaseFloat64GetLimbz(a, a->len - 2)) >> (64 - a->expn);
        if (a->sign)
            v = (UINT64_T)((UINT64_T)0-v);
        ret = 0;
    } else if (!(flags & AI_BASE_FLOAT64_BF_GET_INT_MOD)) {
        ret = AI_BASE_FLOAT64_BF_ST_OVERFLOW;
        if (a->sign) {
            UINT64_T v1;
            v = (UINT64_T)INT64_MAX + 1;
            if (a->expn == 64) {
                v1 = a->tab[a->len - 1];
                v1 |= (v1 << 32) | AiBaseFloat64GetLimbz(a, a->len - 2);
                if (v1 == v)
                    ret = 0;
            }
        } else {
            v = INT64_MAX;
        }
    } else {
        AI_BASE_FLOAT64_SLIMB_T bit_pos = a->len * AI_BASE_FLOAT64_LIMB_BITS - a->expn;
        v = AiBaseFloat64GetBits((const AI_BASE_FLOAT64_LIMB_T *)(a->tab), a->len, bit_pos); 
        v |= (UINT64_T)AiBaseFloat64GetBits((const AI_BASE_FLOAT64_LIMB_T *)(a->tab), a->len, bit_pos + 32) << 32;
        if (a->sign)
            v = (UINT64_T)((UINT64_T)0-v);
        ret = 0;
    }
    *pres = (INT16_T)v;
    return ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64GetLimbRadix(INT32_T radix)
{
    INT32_T i, k;
    AI_BASE_FLOAT64_LIMB_T radixl;
    
    k = g_ai_base_float64_digits_per_limb_table[radix - 2];
    radixl = radix;
    for(i = 1; i < k; i++)
        radixl *= radix;
    return radixl;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64BfIntegerFromRadixRec(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_LIMB_T *tab,
                                      AI_BASE_FLOAT64_LIMB_T n, INT32_T level, AI_BASE_FLOAT64_LIMB_T n0,
                                      AI_BASE_FLOAT64_LIMB_T radix, AI_BASE_FLOAT64_BF_T *pow_tab)
{
    if (n == 1) {
        AiBaseFloat64BfSetUi(r, tab[0]);
    } else {
        AI_BASE_FLOAT64_BF_T T_s, *T = &T_s, *B;
        AI_BASE_FLOAT64_LIMB_T n1, n2;
        
        n2 = (((n0 * 2) >> (level + 1)) + 1) / 2;
        n1 = n - n2;
        //        printf("level=%d n0=%ld n1=%ld n2=%ld\n", level, n0, n1, n2);
        B = &pow_tab[level];
        if (B->len == 0) {
            AiBaseFloat64BfPowUiUi(B, radix, n2, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        }
        AiBaseFloat64BfIntegerFromRadixRec(r, tab + n2, n1, level + 1, n0,
                                  radix, pow_tab);
        AiBaseFloat64BfMul(r, r, B, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        AiBaseFloat64BfInit(T);
        AiBaseFloat64BfIntegerFromRadixRec(T, tab, n2, level + 1, n0,
                                  radix, pow_tab);
        AiBaseFloat64BfAdd(r, r, T, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        AiBaseFloat64BfDelete(T);
    }
    //    AiBaseFloat64BfPrintStr("  r=", r);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64BfIntegerFromRadix(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_LIMB_T *tab,
                                 AI_BASE_FLOAT64_LIMB_T n, AI_BASE_FLOAT64_LIMB_T radix)
{
    INT32_T pow_tab_len, i;
    AI_BASE_FLOAT64_LIMB_T radixl;
    AI_BASE_FLOAT64_BF_T *pow_tab;
    
    radixl = AiBaseFloat64GetLimbRadix((INT32_T)radix);
    pow_tab_len = AiBaseFloat64CeilLog2(n) + 2; /* XXX: check */
    pow_tab = AiBaseFloat64BfMalloc(sizeof(pow_tab[0]) * pow_tab_len);
    memset(pow_tab, 0, sizeof(pow_tab[0]) * pow_tab_len);
    AiBaseFloat64BfIntegerFromRadixRec(r, tab, n, 0, n, radixl, pow_tab);
    for(i = 0; i < pow_tab_len; i++) {
        AiBaseFloat64BfDelete(&pow_tab[i]);
    }
    AiBaseFloat64BfFree(pow_tab);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* compute and round T * radix^expn, assuming T is a non zero integer. */
INT32_T AiBaseFloat64BfConvertFromRadix(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *T, AI_BASE_FLOAT64_LIMB_T radix,
                                 AI_BASE_FLOAT64_SLIMB_T expn, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    INT32_T ret, expn_sign;
    AI_BASE_FLOAT64_SLIMB_T e, extra_bits, prec1, ziv_extra_bits;
    AI_BASE_FLOAT64_BF_T B_s, *B = &B_s;

    e = expn;
    expn_sign = 0;
    if (e < 0) {
        e = -e;
        expn_sign = 1;
    }
    AiBaseFloat64BfInit(B);
    if (prec == AI_BASE_FLOAT64_BF_PREC_INF) {
        /* infinite precision: if the result is inexact, 'r' is set to 'T'. */
        if (expn_sign) {
            AI_BASE_FLOAT64_LIMB_T prec1;
            /* we do the division and see if the result can be exact */
            prec1 = T->len * AI_BASE_FLOAT64_LIMB_BITS;
            if (AiBaseFloat64BfPowUiUi(B, radix, e, prec1, AI_BASE_FLOAT64_BF_RNDN) != 0)
                goto inexact_result;
            ret = AiBaseFloat64BfDiv(r, T, B, prec1, AI_BASE_FLOAT64_BF_RNDN);
            if (ret != 0) {
            inexact_result:
                AiBaseFloat64BfSet(r, T);
                ret = AI_BASE_FLOAT64_BF_ST_INEXACT;
            }
        } else {
            AiBaseFloat64BfPowUiUi(B, radix, e, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDN);
            AiBaseFloat64BfMul(r, T, B, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDN);
            ret = 0;
        }
    } else {
        ziv_extra_bits = 16;
        for(;;) {
            prec1 = prec + ziv_extra_bits;
            /* XXX: rigorous error analysis needed */
            extra_bits = AiBaseFloat64CeilLog2(e) * 2 + 1;
            ret = AiBaseFloat64BfPowUiUi(B, radix, e, prec1 + extra_bits, AI_BASE_FLOAT64_BF_RNDN);
            /* XXX: if AiBaseFloat64BfPowUiUi returns an exact result, can stop
               after the next operation */
            if (expn_sign)
                ret |= AiBaseFloat64BfDiv(r, T, B, prec1 + extra_bits, AI_BASE_FLOAT64_BF_RNDN);
            else
                ret |= AiBaseFloat64BfMul(r, T, B, prec1 + extra_bits, AI_BASE_FLOAT64_BF_RNDN);
            if ((ret & AI_BASE_FLOAT64_BF_ST_INEXACT) &&
                !AiBaseFloat64BfCanRound(r, prec, flags & AI_BASE_FLOAT64_BF_RND_MASK, prec1)) {
                /* and more precision and retry */
                ziv_extra_bits = ziv_extra_bits  + (ziv_extra_bits / 2);
            } else {
                ret = AiBaseFloat64BfRound(r, prec, flags) | (ret & AI_BASE_FLOAT64_BF_ST_INEXACT);
                break;
            }
        }
    }
    AiBaseFloat64BfDelete(B);
    return ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64ToDigit(INT32_T c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    else if (c >= 'A' && c <= 'Z')
        return c - 'A' + 10;
    else if (c >= 'a' && c <= 'z')
        return c - 'a' + 10;
    else
        return 36;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* add a limb at 'pos' and decrement pos. new space is created if needed */
void AiBaseFloat64BfAddLimb(AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_SLIMB_T *ppos, AI_BASE_FLOAT64_LIMB_T v)
{
    AI_BASE_FLOAT64_SLIMB_T pos;
    pos = *ppos;
    if ((pos < 0)) {
        AI_BASE_FLOAT64_LIMB_T new_size, d;
        new_size = AiBaseFloat64BfMax(a->len + 1, a->len * 3 / 2);
        memset(a->tab, 0, sizeof(AI_BASE_FLOAT64_DLIMB_T)*2);
        d = new_size - a->len;
        memmove(a->tab + d, a->tab, (size_t)(a->len * sizeof(AI_BASE_FLOAT64_LIMB_T)));
        a->len = new_size;
        pos += d;
    }
    a->tab[pos--] = v;
    *ppos = pos;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfTolower(INT32_T c)
{
    if (c >= 'A' && c <= 'Z')
        c = c - 'A' + 'a';
    return c;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64Strcasestart(const UINT8_T *str, const UINT8_T *val, const UINT8_T **ptr)
{
    const UINT8_T *p, *q;
    p = str;
    q = val;
    while (*q != '\0') {
        if (AiBaseFloat64BfTolower(*p) != *q)
            return 0;
        p++;
        q++;
    }
    if (ptr)
        *ptr = p;
    return 1;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* 
   Return (status, n, exp). 'status' is the floating point status. 'n'
   is the parsed number. 'exp' is set to zero if prec != AI_BASE_FLOAT64_BF_PREC_INF
   or if the result can be exactly represented as binary floating
   point number. Otherwise 'exp' is set to the exponent and 'n' is an
   integer so that the returned result is exactly n * radix ^ exp.
*/
INT32_T AiBaseFloat64BfAtof2(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_SLIMB_T *pexponent,
             const UINT8_T *str, const UINT8_T **pnext, INT32_T radix,
             AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    const UINT8_T *p, *p_start;
    INT32_T is_neg, radix_bits, exp_is_neg, ret, digits_per_limb, shift;
    AI_BASE_FLOAT64_LIMB_T cur_limb;
    AI_BASE_FLOAT64_SLIMB_T pos, expn, int_len;
    BOOL_T has_decpt, is_bin_exp, is_float;
    AI_BASE_FLOAT64_BF_T a_s, *a;
    
    *pexponent = 0;
    p = str;
    if (!(flags & (AI_BASE_FLOAT64_BF_ATOF_INT_ONLY | AI_BASE_FLOAT64_BF_ATOF_JS_QUIRKS)) &&
        radix <= 16 &&
        AiBaseFloat64Strcasestart(p, "nan", &p)) {
        AiBaseFloat64BfSetNan(r);
        ret = 0;
        goto done;
    }
    is_neg = 0;
    
    if (p[0] == '+') {
        p++;
        if (flags & AI_BASE_FLOAT64_BF_ATOF_NO_PREFIX_AFTER_SIGN)
            goto no_radix_prefix;
    } else if (p[0] == '-') {
        is_neg = 1;
        p++;
        if (flags & AI_BASE_FLOAT64_BF_ATOF_NO_PREFIX_AFTER_SIGN)
            goto no_radix_prefix;
    }
    if (p[0] == '0') {
        if ((p[1] == 'x' || p[1] == 'X') &&
            (radix == 0 || radix == 16) &&
            !(flags & AI_BASE_FLOAT64_BF_ATOF_NO_HEX)) {
            radix = 16;
            p += 2;
        } else if ((p[1] == 'o' || p[1] == 'O') &&
                   radix == 0 && (flags & AI_BASE_FLOAT64_BF_ATOF_BIN_OCT)) {
            p += 2;
            radix = 8;
        } else if ((p[1] == 'b' || p[1] == 'B') &&
                   radix == 0 && (flags & AI_BASE_FLOAT64_BF_ATOF_BIN_OCT)) {
            p += 2;
            radix = 2;
        }
    } else {
    no_radix_prefix:
        if (!(flags & AI_BASE_FLOAT64_BF_ATOF_INT_ONLY) && radix <= 16 &&
            ((!(flags & AI_BASE_FLOAT64_BF_ATOF_JS_QUIRKS) && AiBaseFloat64Strcasestart(p, "inf", &p)) ||
             ((flags & AI_BASE_FLOAT64_BF_ATOF_JS_QUIRKS) && AiBaseFloat64Strstart(p, "Infinity", &p)))) {
            AiBaseFloat64BfSetInf(r, is_neg);
            ret = 0;
            goto done;
        }
    }
    
    if (radix == 0)
        radix = 10;
    if ((radix & (radix - 1)) != 0) {
        radix_bits = 0; /* base is not a power of two */
        a = &a_s;
        AiBaseFloat64BfInit(a);
    } else {
        radix_bits = AiBaseFloat64CeilLog2(radix);
        a = r;
    }

    /* skip leading zeros */
    /* XXX: could also skip zeros after the decimal point */
    while (*p == '0')
        p++;

    if (radix_bits) {
        shift = digits_per_limb = AI_BASE_FLOAT64_LIMB_BITS;
    } else {
        radix_bits = 0;
        shift = digits_per_limb = g_ai_base_float64_digits_per_limb_table[radix - 2];
    }
    cur_limb = 0;
    AiBaseFloat64BfResize(a, 1);
    pos = 0;
    p_start = p;
    has_decpt = AI_BASE_FLOAT64_FALSE;
    is_float = AI_BASE_FLOAT64_FALSE;
    for(;;) {
        AI_BASE_FLOAT64_LIMB_T c;
        if (*p == '.') {
            if (flags & AI_BASE_FLOAT64_BF_ATOF_INT_ONLY)
                break;
            if (has_decpt)
                break;
            is_float = AI_BASE_FLOAT64_TRUE;
            has_decpt = AI_BASE_FLOAT64_TRUE;
            int_len = p - p_start;
            p++;
            p_start = p;
        }
        c = AiBaseFloat64ToDigit(*p);
        if (c >= (AI_BASE_FLOAT64_LIMB_T)radix)
            break;
        p++;
        if (radix_bits) {
            shift -= radix_bits;
            if (shift <= 0) {
                cur_limb |= c >> (-shift);
                AiBaseFloat64BfAddLimb(a, &pos, cur_limb);
                if (shift < 0)
                    cur_limb = c << (AI_BASE_FLOAT64_LIMB_BITS + shift);
                else
                    cur_limb = 0;
                shift += AI_BASE_FLOAT64_LIMB_BITS;
            } else {
                cur_limb |= c << shift;
            }
        } else {
            cur_limb = cur_limb * radix + c;
            shift--;
            if (shift == 0) {
                AiBaseFloat64BfAddLimb(a, &pos, cur_limb);
                shift = digits_per_limb;
                cur_limb = 0;
            }
        }
    }
    if (!has_decpt)
        int_len = p - p_start;

    /* add the last limb and pad with zeros */
    if (shift != digits_per_limb) {
        if (radix_bits == 0) {
            while (shift != 0) {
                cur_limb *= radix;
                shift--;
            }
        }
        AiBaseFloat64BfAddLimb(a, &pos, cur_limb);
    }
            
    /* reset the next limbs to zero (we prefer to reallocate in the
       renormalization) */
    memset(a->tab, 0, (size_t)((pos + 1) * sizeof(AI_BASE_FLOAT64_LIMB_T)));

    /* parse the exponent, if any */
    expn = 0;
    is_bin_exp = AI_BASE_FLOAT64_FALSE;
    if (!(flags & AI_BASE_FLOAT64_BF_ATOF_INT_ONLY) && 
        ((radix == 10 && (*p == 'e' || *p == 'E')) ||
         (radix != 10 && (*p == '@' ||
                          (radix_bits && (*p == 'p' || *p == 'P')))))) {
            is_bin_exp = (*p == 'p' || *p == 'P');
        p++;
        is_float = AI_BASE_FLOAT64_TRUE;
        exp_is_neg = 0;
        if (*p == '+') {
            p++;
        } else if (*p == '-') {
            exp_is_neg = 1;
            p++;
        }
        for(;;) {
            INT32_T c;
            c = AiBaseFloat64ToDigit(*p);
            if (c >= 10)
                break;
            if ((expn > ((AI_BASE_FLOAT64_EXP_MAX - 2 - 9) / 10))) {
                /* exponent overflow */
                if (exp_is_neg) {
                    AiBaseFloat64BfSetZero(r, is_neg);
                    ret = AI_BASE_FLOAT64_BF_ST_UNDERFLOW | AI_BASE_FLOAT64_BF_ST_INEXACT;
                } else {
                    AiBaseFloat64BfSetInf(r, is_neg);
                    ret = AI_BASE_FLOAT64_BF_ST_OVERFLOW | AI_BASE_FLOAT64_BF_ST_INEXACT;
                }
                goto done;
            }
            p++;
            expn = expn * 10 + c;
        }
        if (exp_is_neg)
            expn = -expn;
    }
    if (!is_float && (flags & AI_BASE_FLOAT64_BF_ATOF_INT_PREC_INF))
        prec = AI_BASE_FLOAT64_BF_PREC_INF;
    if (radix_bits) {
        /* XXX: may overflow */
        if (!is_bin_exp)
            expn *= radix_bits; 
        a->expn = expn + (int_len * radix_bits);
        a->sign = is_neg;
        ret = AiBaseFloat64BfNormalizeAndRound(a, prec, flags);
    } else {
        AI_BASE_FLOAT64_LIMB_T l_value;
        pos++;
        l_value = a->len - pos; /* number of limbs */
        if (l_value == 0) {
            AiBaseFloat64BfSetZero(r, is_neg);
            ret = 0;
        } else {
            AI_BASE_FLOAT64_BF_T T_s, *T = &T_s;

            expn -= l_value * digits_per_limb - int_len;
            AiBaseFloat64BfInit(T);
            AiBaseFloat64BfIntegerFromRadix(T, (const AI_BASE_FLOAT64_LIMB_T *)(a->tab + pos), l_value, radix);
            T->sign = is_neg;
            ret = AiBaseFloat64BfConvertFromRadix(r, T, radix, expn, prec, flags);
            if (prec == AI_BASE_FLOAT64_BF_PREC_INF && (ret & AI_BASE_FLOAT64_BF_ST_INEXACT)) {
                /* an exact result was requested but the result is
                   inexact: return the exponent and 'r' is an
                   integer */
                *pexponent = expn;
            }
            AiBaseFloat64BfDelete(T);
        }
        AiBaseFloat64BfDelete(a);
    }
    if (!is_float)
        ret |= AI_BASE_FLOAT64_BF_ATOF_ST_INTEGER;
 done:
    if (pnext)
        *pnext = p;
    return ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfAtof(AI_BASE_FLOAT64_BF_T *r, const UINT8_T *str, const UINT8_T **pnext, INT32_T radix,
            AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    AI_BASE_FLOAT64_SLIMB_T dummy_exp;
    return AiBaseFloat64BfAtof2(r, &dummy_exp, str, pnext, radix, prec, flags);
}


/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* compute floor(a*b) or ceil(a*b) with b = log2(radix) or
   b=1/log2(radix). For is_inv = 0, strict accuracy is not guaranteed
   when radix is not a power of two. */
AI_BASE_FLOAT64_SLIMB_T AiBaseFloat64BfMulLog2Radix(AI_BASE_FLOAT64_SLIMB_T a1, UINT32_T radix, INT32_T is_inv,
                          INT32_T is_ceil1)
{
    INT32_T is_neg;
    AI_BASE_FLOAT64_LIMB_T a;
    BOOL_T is_ceil;

    is_ceil = is_ceil1;
    a = a1;
    if (a1 < 0) {
        a = (AI_BASE_FLOAT64_LIMB_T)0-a;
        is_neg = 1;
    } else {
        is_neg = 0;
    }
    is_ceil ^= is_neg;
    if ((radix & (radix - 1)) == 0) {
        INT32_T radix_bits;
        /* radix is a power of two */
        radix_bits = AiBaseFloat64CeilLog2(radix);
        if (is_inv) {
            if (is_ceil)
                a += radix_bits - 1;
            a = a / radix_bits;
        } else {
            a = a * radix_bits;
        }
    } else {
        const UINT32_T *tab;
        AI_BASE_FLOAT64_LIMB_T b0, b1;
        AI_BASE_FLOAT64_DLIMB_T t;
        
        if (is_inv) {
            tab = g_ai_base_float64_inv_log2_radix[radix - 2];
            b1 = tab[0];
            b0 = tab[1];
            t = (AI_BASE_FLOAT64_DLIMB_T)b0 * (AI_BASE_FLOAT64_DLIMB_T)a;
            t = (AI_BASE_FLOAT64_DLIMB_T)b1 * (AI_BASE_FLOAT64_DLIMB_T)a /* + (t >> AI_BASE_FLOAT64_LIMB_BITS) */;
            a = (AI_BASE_FLOAT64_LIMB_T)(t >> (AI_BASE_FLOAT64_LIMB_BITS - 1));
        } else {
            b0 = g_ai_base_float64_log2_radix[radix - 2];
            t = (AI_BASE_FLOAT64_DLIMB_T)b0 * (AI_BASE_FLOAT64_DLIMB_T)a;
            a = (AI_BASE_FLOAT64_LIMB_T)(t >> (AI_BASE_FLOAT64_LIMB_BITS - 3));
        }
        /* a = floor(result) and 'result' cannot be an integer */
        a += is_ceil;
    }
    if (is_neg)
        a = (AI_BASE_FLOAT64_LIMB_T)0-a;
    return a;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* 'n' is the number of output limbs */
void AiBaseFloat64BfIntegerToRadixRec(AI_BASE_FLOAT64_BF_T *pow_tab,
                                    AI_BASE_FLOAT64_LIMB_T *out, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T n,
                                    INT32_T level, AI_BASE_FLOAT64_LIMB_T n0, AI_BASE_FLOAT64_LIMB_T radixl,
                                    UINT32_T radixl_bits)
{
    AI_BASE_FLOAT64_LIMB_T n1, n2, q_prec;
    if (!(n >= 1))
		return;
    if (n == 1) {
        out[0] = AiBaseFloat64GetBits((const AI_BASE_FLOAT64_LIMB_T *)(a->tab), a->len, a->len * AI_BASE_FLOAT64_LIMB_BITS - a->expn);
    } else if (n == 2) {
        AI_BASE_FLOAT64_DLIMB_T t;
        AI_BASE_FLOAT64_SLIMB_T pos;
        pos = a->len * AI_BASE_FLOAT64_LIMB_BITS - a->expn;
        t = /* ((AI_BASE_FLOAT64_DLIMB_T)AiBaseFloat64GetBits((const AI_BASE_FLOAT64_LIMB_T *)(a->tab), a->len, pos + AI_BASE_FLOAT64_LIMB_BITS) << AI_BASE_FLOAT64_LIMB_BITS) | */
            AiBaseFloat64GetBits((const AI_BASE_FLOAT64_LIMB_T *)(a->tab), a->len, pos);
        if ((radixl == AI_BASE_FLOAT64_RADIXL_10)) {
            /* use division by a constant when possible */
            out[0] = (AI_BASE_FLOAT64_LIMB_T)(t % AI_BASE_FLOAT64_RADIXL_10);
            out[1] = (AI_BASE_FLOAT64_LIMB_T)(t / AI_BASE_FLOAT64_RADIXL_10);
        } else {
            out[0] = t % radixl;
            out[1] = (AI_BASE_FLOAT64_LIMB_T)(t / radixl);
        }
    } else {
        AI_BASE_FLOAT64_BF_T Q, R, *B, *B_inv;
        INT32_T q_add;
        AiBaseFloat64BfInit(&Q);
        AiBaseFloat64BfInit(&R);
        n2 = (((n0 * 2) >> (level + 1)) + 1) / 2;
        n1 = n - n2;
        B = &pow_tab[2 * level];
        B_inv = &pow_tab[2 * level + 1];
        if (B->len == 0) {
            /* compute BASE^n2 */
            AiBaseFloat64BfPowUiUi(B, radixl, n2, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
            /* we use enough bits for the maximum possible 'n1' value,
               i.e. n2 + 1 */
            AiBaseFloat64BfRecip(B_inv, B, (n2 + 1) * radixl_bits + 2);
        }
        //        printf("%d: n1=% " PRId64 " n2=%" PRId64 "\n", level, n1, n2);
        q_prec = n1 * radixl_bits;
        AiBaseFloat64BfMul(&Q, a, B_inv, q_prec, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfRint(&Q, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        
        AiBaseFloat64BfMul(&R, &Q, B, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        AiBaseFloat64BfSub(&R, a, &R, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        /* adjust if necessary */
        q_add = 0;
        while (R.sign && R.len != 0) {
            AiBaseFloat64BfAdd(&R, &R, B, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
            q_add--;
        }
        while (AiBaseFloat64BfCmpu(&R, B) >= 0) {
            AiBaseFloat64BfSub(&R, &R, B, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
            q_add++;
        }
        if (q_add != 0) {
            AiBaseFloat64BfAddSi(&Q, &Q, q_add, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        }
        AiBaseFloat64BfIntegerToRadixRec(pow_tab, out + n2, &Q, n1, level + 1, n0,
                                radixl, radixl_bits);
        AiBaseFloat64BfIntegerToRadixRec(pow_tab, out, &R, n2, level + 1, n0,
                                radixl, radixl_bits);
        AiBaseFloat64BfDelete(&Q);
        AiBaseFloat64BfDelete(&R);
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64BfIntegerToRadix(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T radixl)
{
    AI_BASE_FLOAT64_LIMB_T r_len;
    AI_BASE_FLOAT64_BF_T *pow_tab;
    INT32_T i, pow_tab_len;
    
    r_len = r->len;
    pow_tab_len = (AiBaseFloat64CeilLog2(r_len) + 2) * 2; /* XXX: check */
    pow_tab = AiBaseFloat64BfMalloc(sizeof(pow_tab[0]) * pow_tab_len);
    /* could do AiBaseFloat64BfInit(), but memset() has the same result here */
    memset(pow_tab, 0, sizeof(pow_tab[0]) * pow_tab_len);

    AiBaseFloat64BfIntegerToRadixRec(pow_tab, (AI_BASE_FLOAT64_LIMB_T *)(r->tab), a, r_len, 0, r_len, radixl,
                            AiBaseFloat64CeilLog2(radixl));

    for(i = 0; i < pow_tab_len; i++) {
        AiBaseFloat64BfDelete(&pow_tab[i]);
    }
    AiBaseFloat64BfFree(pow_tab);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* a must be >= 0. 'P' is the wanted number of digits in radix
   'radix'. 'r' is the mantissa represented as an integer. *pE
   contains the exponent. */
void AiBaseFloat64BfConvertToRadix(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_SLIMB_T *pE,
                                const AI_BASE_FLOAT64_BF_T *a, INT32_T radix,
                                AI_BASE_FLOAT64_LIMB_T P, AI_BASE_FLOAT64_BF_RND_T rnd_mode,
                                BOOL_T is_fixed_exponent)
{
    AI_BASE_FLOAT64_SLIMB_T E, e, prec, extra_bits, ziv_extra_bits, prec0;
    AI_BASE_FLOAT64_BF_T B_s, *B = &B_s;
    INT32_T e_sign, ret, res;
    
    if (a->len == 0) {
        /* zero case */
        *pE = 0;
        AiBaseFloat64BfSet(r, a);
        return;
    }

    if (is_fixed_exponent) {
        E = *pE;
    } else {
        /* compute the new exponent */
        E = 1 + AiBaseFloat64BfMulLog2Radix(a->expn - 1, radix, AI_BASE_FLOAT64_TRUE, AI_BASE_FLOAT64_FALSE);
    }
    //    AiBaseFloat64BfPrintStr("a", a);
    //    printf("E=%ld P=%ld radix=%d\n", E, P, radix);
    
    for(;;) {
        e = P - E;
        e_sign = 0;
        if (e < 0) {
            e = -e;
            e_sign = 1;
        }
        /* Note: precision for log2(radix) is not critical here */
        prec0 = AiBaseFloat64BfMulLog2Radix(P, radix, AI_BASE_FLOAT64_FALSE, AI_BASE_FLOAT64_TRUE);
        ziv_extra_bits = 16;
        for(;;) {
            prec = prec0 + ziv_extra_bits;
            /* XXX: rigorous error analysis needed */
            extra_bits = AiBaseFloat64CeilLog2(e) * 2 + 1;
            ret = AiBaseFloat64BfPowUiUi(r, radix, e, prec + extra_bits, AI_BASE_FLOAT64_BF_RNDN);
            if (!e_sign)
                ret |= AiBaseFloat64BfMul(r, r, a, prec + extra_bits, AI_BASE_FLOAT64_BF_RNDN);
            else
                ret |= AiBaseFloat64BfDiv(r, a, r, prec + extra_bits, AI_BASE_FLOAT64_BF_RNDN);
            /* if the result is not exact, check that it can be safely
               rounded to an integer */
            if ((ret & AI_BASE_FLOAT64_BF_ST_INEXACT) &&
                !AiBaseFloat64BfCanRound(r, r->expn, rnd_mode, prec)) {
                /* and more precision and retry */
                ziv_extra_bits = ziv_extra_bits  + (ziv_extra_bits / 2);
                continue;
            } else {
                AiBaseFloat64BfRint(r, AI_BASE_FLOAT64_BF_PREC_INF, rnd_mode);
                break;
            }
        }
        if (is_fixed_exponent)
            break;
        /* check that the result is < B^P */
        /* XXX: do an fast approximate test first ? */
        AiBaseFloat64BfInit(B);
        AiBaseFloat64BfPowUiUi(B, radix, P, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        res = AiBaseFloat64BfCmpu(r, B);
        AiBaseFloat64BfDelete(B);
        if (res < 0)
            break;
        /* try a larger exponent */
        E++;
    }
    *pE = E;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64LimbToA(UINT8_T *buf, AI_BASE_FLOAT64_LIMB_T n, UINT32_T radix, INT32_T len)
{
    INT32_T digit, i;

    if (radix == 10) {
        /* specific case with constant divisor */
        for(i = len - 1; i >= 0; i--) {
            digit = (AI_BASE_FLOAT64_LIMB_T)n % 10;
            n = (AI_BASE_FLOAT64_LIMB_T)n / 10;
            buf[i] = digit + '0';
        }
    } else {
        for(i = len - 1; i >= 0; i--) {
            digit = (AI_BASE_FLOAT64_LIMB_T)n % radix;
            n = (AI_BASE_FLOAT64_LIMB_T)n / radix;
            if (digit < 10)
                digit += '0';
            else
                digit += 'a' - 10;
            buf[i] = digit;
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
/* for power of 2 radixes */
void AiBaseFloat64LimbToA2(UINT8_T *buf, AI_BASE_FLOAT64_LIMB_T n, UINT32_T radix_bits, INT32_T len)
{
    INT32_T digit, i;
    UINT32_T mask;

    mask = (1 << radix_bits) - 1;
    for(i = len - 1; i >= 0; i--) {
        digit = n & mask;
        n >>= radix_bits;
        if (digit < 10)
            digit += '0';
        else
            digit += 'a' - 10;
        buf[i] = digit;
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* 'a' must be an integer. A dot is added before the 'dot_pos'
   digit. dot_pos = n_digits does not display the dot. 0 <= dot_pos <=
   n_digits. n_digits >= 1. */
void AiBaseFloat64OutputDigits(AI_BASE_FLOAT64_DYN_BUF *s, const AI_BASE_FLOAT64_BF_T *a1, INT32_T radix, AI_BASE_FLOAT64_LIMB_T n_digits,
                          AI_BASE_FLOAT64_LIMB_T dot_pos)
{
    AI_BASE_FLOAT64_LIMB_T i, v, l_value;
    AI_BASE_FLOAT64_SLIMB_T pos, pos_incr;
    INT32_T digits_per_limb, buf_pos, radix_bits, first_buf_pos;
    UINT8_T buf[65];
    AI_BASE_FLOAT64_BF_T a_s, *a;
    
    if ((radix & (radix - 1)) != 0) {
        AI_BASE_FLOAT64_LIMB_T n, radixl;

        digits_per_limb = g_ai_base_float64_digits_per_limb_table[radix - 2];
        radixl = AiBaseFloat64GetLimbRadix(radix);
        a = &a_s;
        AiBaseFloat64BfInit(a);
        n = (n_digits + digits_per_limb - 1) / digits_per_limb;
        AiBaseFloat64BfResize(a, n);
        AiBaseFloat64BfIntegerToRadix(a, a1, radixl);
        radix_bits = 0;
        pos = n;
        pos_incr = 1;
        first_buf_pos = (INT32_T)(pos * digits_per_limb - n_digits);
    } else {
        a = (AI_BASE_FLOAT64_BF_T *)a1;
        radix_bits = AiBaseFloat64CeilLog2(radix);
        digits_per_limb = AI_BASE_FLOAT64_LIMB_BITS / radix_bits;
        pos_incr = digits_per_limb * radix_bits;
        pos = a->len * AI_BASE_FLOAT64_LIMB_BITS - a->expn + n_digits * radix_bits;
        first_buf_pos = 0;
    }
    buf_pos = digits_per_limb;
    i = 0;
    while (i < n_digits) {
        if (buf_pos == digits_per_limb) {
            pos -= pos_incr;
            if (radix_bits == 0) {
                v = AiBaseFloat64GetLimbz(a, pos);
                AiBaseFloat64LimbToA(buf, v, radix, digits_per_limb);
            } else {
                v = AiBaseFloat64GetBits((const AI_BASE_FLOAT64_LIMB_T *)(a->tab), a->len, pos);
                AiBaseFloat64LimbToA2(buf, v, radix_bits, digits_per_limb);
            }
            buf_pos = first_buf_pos;
            first_buf_pos = 0;
        }
        if (i < dot_pos) {
            l_value = dot_pos;
        } else {
            if (i == dot_pos)
                AiBaseFloat64DbufPutc(s, '.');
            l_value = n_digits;
        }
        l_value = AiBaseFloat64BfMin(digits_per_limb - buf_pos, l_value - i);
        AiBaseFloat64DbufPut(s, (UINT8_T *)(buf + buf_pos), (UINT32_T)l_value);
        buf_pos += (INT32_T)l_value;
        i += l_value;
    }
    if (a != a1)
        AiBaseFloat64BfDelete(a);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void *AiBaseFloat64BfDbufRealloc(void *opaque, void *ptr, UINT32_T size)
{
    return AiBaseFloat64BfRealloc(ptr, size);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* return the length in bytes. A trailing '\0' is added */
UINT32_T AiBaseFloat64BfFtoa(UINT8_T **pbuf, const AI_BASE_FLOAT64_BF_T *a2, INT32_T radix, AI_BASE_FLOAT64_LIMB_T prec,
               AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    AI_BASE_FLOAT64_DYN_BUF s_s, *s = &s_s;
    INT32_T radix_bits;
    
    //    AiBaseFloat64BfPrintStr("ftoa", a2);
    //    printf("radix=%d\n", radix);
    AiBaseFloat64DbufInit2(s, NULL, AiBaseFloat64BfDbufRealloc);
    if (a2->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
        AiBaseFloat64DbufPutstr(s, "NaN");
    } else {
        if (a2->sign)
            AiBaseFloat64DbufPutc(s, '-');
        if (a2->expn == AI_BASE_FLOAT64_BF_EXP_INF) {
            if (flags & AI_BASE_FLOAT64_BF_FTOA_JS_QUIRKS)
                AiBaseFloat64DbufPutstr(s, "Infinity");
            else
                AiBaseFloat64DbufPutstr(s, "Inf");
        } else {
            INT32_T fmt;
            AI_BASE_FLOAT64_SLIMB_T n_digits, n, i, n_max, n1;
            AI_BASE_FLOAT64_BF_T a1_s, *a1;
            AI_BASE_FLOAT64_BF_T a_s, *a = &a_s;

            /* make a positive number */
            memcpy(a->tab, a2->tab, sizeof(AI_BASE_FLOAT64_DLIMB_T)*2);
            a->len = a2->len;
            a->expn = a2->expn;
            a->sign = 0;
            
            if ((radix & (radix - 1)) != 0)
                radix_bits = 0;
            else
                radix_bits = AiBaseFloat64CeilLog2(radix);

            fmt = flags & AI_BASE_FLOAT64_BF_FTOA_FORMAT_MASK;
                
            a1 = &a1_s;
            AiBaseFloat64BfInit(a1);
            if (fmt == AI_BASE_FLOAT64_BF_FTOA_FORMAT_FRAC) {
                UINT32_T pos, start;
                /* one more digit for the rounding */
                n = 1 + AiBaseFloat64BfMulLog2Radix(AiBaseFloat64BfMax(a->expn, 0), radix, AI_BASE_FLOAT64_TRUE, AI_BASE_FLOAT64_TRUE);
                n_digits = n + prec;
                n1 = n;
                AiBaseFloat64BfConvertToRadix(a1, &n1, a, radix, n_digits,
                                    flags & AI_BASE_FLOAT64_BF_RND_MASK, AI_BASE_FLOAT64_TRUE);
                start = s->size;
                AiBaseFloat64OutputDigits(s, a1, radix, n_digits, n);
                /* remove leading zeros because we allocated one more digit */
                pos = start;
                while ((pos + 1) < s->size && s->buf[pos] == '0' &&
                       s->buf[pos + 1] != '.')
                    pos++;
                if (pos > start) {
                    memmove(s->buf + start, s->buf + pos, s->size - pos);
                    s->size -= (pos - start);
                }
            } else {
                if (fmt == AI_BASE_FLOAT64_BF_FTOA_FORMAT_FIXED) {
                    n_digits = prec;
                    n_max = n_digits;
                } else {
                    AI_BASE_FLOAT64_SLIMB_T n_digits_max, n_digits_min;
                    
                    if (prec == AI_BASE_FLOAT64_BF_PREC_INF) {
                        if (!(radix_bits != 0))
							return 0;
                        /* XXX: could use the exact number of bits */
                        prec = a->len * AI_BASE_FLOAT64_LIMB_BITS;
                    }
                    n_digits = 1 + AiBaseFloat64BfMulLog2Radix(prec, radix, AI_BASE_FLOAT64_TRUE, AI_BASE_FLOAT64_TRUE);
                    /* max number of digits for non exponential
                       notation. The rational is to have the same rule
                       as JS i.e. n_max = 21 for 64 bit float in base 10. */
                    n_max = n_digits + 4;
                    if (fmt == AI_BASE_FLOAT64_BF_FTOA_FORMAT_FREE_MIN) {
                        AI_BASE_FLOAT64_BF_T b_s, *b = &b_s;
                        
                        /* find the minimum number of digits by
                           dichotomy. */
                        n_digits_max = n_digits;
                        n_digits_min = 1;
                        AiBaseFloat64BfInit(b);
                        while (n_digits_min < n_digits_max) {
                            n_digits = (n_digits_min + n_digits_max) / 2;
                            AiBaseFloat64BfConvertToRadix(a1, &n, a, radix, n_digits,
                                                flags & AI_BASE_FLOAT64_BF_RND_MASK, AI_BASE_FLOAT64_FALSE);
                            /* convert back to a number and compare */
                            AiBaseFloat64BfConvertFromRadix(b, a1, radix, n - n_digits,
                                                  prec,
                                                  (flags & ~AI_BASE_FLOAT64_BF_RND_MASK) |
                                                  AI_BASE_FLOAT64_BF_RNDN);
                            if (AiBaseFloat64BfCmpu(b, a) == 0) {
                                n_digits_max = n_digits;
                            } else {
                                n_digits_min = n_digits + 1;
                            }
                        }
                        AiBaseFloat64BfDelete(b);
                        n_digits = n_digits_max;
                    }
                }
                AiBaseFloat64BfConvertToRadix(a1, &n, a, radix, n_digits,
                                    flags & AI_BASE_FLOAT64_BF_RND_MASK, AI_BASE_FLOAT64_FALSE);
                if (a1->expn == AI_BASE_FLOAT64_BF_EXP_ZERO &&
                    fmt != AI_BASE_FLOAT64_BF_FTOA_FORMAT_FIXED &&
                    !(flags & AI_BASE_FLOAT64_BF_FTOA_FORCE_EXP)) {
                    /* just output zero */
                    AiBaseFloat64DbufPutstr(s, "0");
                } else {
                    if (flags & AI_BASE_FLOAT64_BF_FTOA_ADD_PREFIX) {
                        if (radix == 16)
                            AiBaseFloat64DbufPutstr(s, "0x");
                        else if (radix == 8)
                            AiBaseFloat64DbufPutstr(s, "0o");
                        else if (radix == 2)
                            AiBaseFloat64DbufPutstr(s, "0b");
                    }
                    if ((flags & AI_BASE_FLOAT64_BF_FTOA_FORCE_EXP) ||
                        n <= -6 || n > n_max) {
                        const UINT8_T *fmt;
                        /* exponential notation */
                        AiBaseFloat64OutputDigits(s, a1, radix, n_digits, 1);
                        if (radix_bits != 0 && radix <= 16) {
                            if (flags & AI_BASE_FLOAT64_BF_FTOA_JS_QUIRKS)
                                fmt = "p%+" AI_BASE_FLOAT64_PRID_LIMB;
                            else
                                fmt = "p%" AI_BASE_FLOAT64_PRID_LIMB;
                            AiBaseFloat64DbufPrintf(s, fmt, (n - 1) * radix_bits);
                        } else {
                            if (flags & AI_BASE_FLOAT64_BF_FTOA_JS_QUIRKS)
                                fmt = "%c%+" AI_BASE_FLOAT64_PRID_LIMB;
                            else
                                fmt = "%c%" AI_BASE_FLOAT64_PRID_LIMB;
                            AiBaseFloat64DbufPrintf(s, fmt,
                                        radix <= 10 ? 'e' : '@', n - 1);
                        }
                    } else if (n <= 0) {
                        /* 0.x */
                        AiBaseFloat64DbufPutstr(s, "0.");
                        for(i = 0; i < -n; i++) {
                            AiBaseFloat64DbufPutc(s, '0');
                        }
                        AiBaseFloat64OutputDigits(s, a1, radix, n_digits, n_digits);
                    } else {
                        if (n_digits <= n) {
                            /* no dot */
                            AiBaseFloat64OutputDigits(s, a1, radix, n_digits, n_digits);
                            for(i = 0; i < (n - n_digits); i++)
                                AiBaseFloat64DbufPutc(s, '0');
                        } else {
                            AiBaseFloat64OutputDigits(s, a1, radix, n_digits, n);
                        }
                    }
                }
            }
            AiBaseFloat64BfDelete(a1);
        }
    }
    AiBaseFloat64DbufPutc(s, '\0');
    *pbuf = (UINT8_T *)s->buf;
    return s->size - 1;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/***************************************************************/
/* transcendental functions */

/* Note: the algorithm is from MPFR */
void AiBaseFloat64BfConstLog2Rec(AI_BASE_FLOAT64_BF_T *T, AI_BASE_FLOAT64_BF_T *P, AI_BASE_FLOAT64_BF_T *Q, AI_BASE_FLOAT64_LIMB_T n1,
                              AI_BASE_FLOAT64_LIMB_T n2, BOOL_T need_P)
{
    if ((n2 - n1) == 1) {
        if (n1 == 0) {
            AiBaseFloat64BfSetUi(P, 3);
        } else {
            AiBaseFloat64BfSetUi(P, n1);
            P->sign = 1;
        }
        AiBaseFloat64BfSetUi(Q, 2 * n1 + 1);
        Q->expn += 2;
        AiBaseFloat64BfSet(T, P);
    } else {
        AI_BASE_FLOAT64_LIMB_T m;
        AI_BASE_FLOAT64_BF_T T1_s, *T1 = &T1_s;
        AI_BASE_FLOAT64_BF_T P1_s, *P1 = &P1_s;
        AI_BASE_FLOAT64_BF_T Q1_s, *Q1 = &Q1_s;
        
        m = n1 + ((n2 - n1) >> 1);
        AiBaseFloat64BfConstLog2Rec(T, P, Q, n1, m, AI_BASE_FLOAT64_TRUE);
        AiBaseFloat64BfInit(T1);
        AiBaseFloat64BfInit(P1);
        AiBaseFloat64BfInit(Q1);
        AiBaseFloat64BfConstLog2Rec(T1, P1, Q1, m, n2, need_P);
        AiBaseFloat64BfMul(T, T, Q1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        AiBaseFloat64BfMul(T1, T1, P, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        AiBaseFloat64BfAdd(T, T, T1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        if (need_P)
            AiBaseFloat64BfMul(P, P, P1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        AiBaseFloat64BfMul(Q, Q, Q1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        AiBaseFloat64BfDelete(T1);
        AiBaseFloat64BfDelete(P1);
        AiBaseFloat64BfDelete(Q1);
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* compute log(2) with faithful rounding at precision 'prec' */
void AiBaseFloat64BfConstLog2Internal(AI_BASE_FLOAT64_BF_T *T, AI_BASE_FLOAT64_LIMB_T prec)
{
    AI_BASE_FLOAT64_LIMB_T w, N;
    AI_BASE_FLOAT64_BF_T P_s, *P = &P_s;
    AI_BASE_FLOAT64_BF_T Q_s, *Q = &Q_s;

    w = prec + 15;
    N = w / 3 + 1;
    AiBaseFloat64BfInit(P);
    AiBaseFloat64BfInit(Q);
    AiBaseFloat64BfConstLog2Rec(T, P, Q, 0, N, AI_BASE_FLOAT64_FALSE);
    AiBaseFloat64BfDiv(T, T, Q, prec, AI_BASE_FLOAT64_BF_RNDN);
    AiBaseFloat64BfDelete(P);
    AiBaseFloat64BfDelete(Q);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64ChudBs(AI_BASE_FLOAT64_BF_T *P, AI_BASE_FLOAT64_BF_T *Q, AI_BASE_FLOAT64_BF_T *G, INT64_T a, INT64_T b, INT32_T need_g,
                    AI_BASE_FLOAT64_LIMB_T prec)
{
    INT64_T c;

    if (a == (b - 1)) {
        AI_BASE_FLOAT64_BF_T T0, T1;
        
        AiBaseFloat64BfInit(&T0);
        AiBaseFloat64BfInit(&T1);
        AiBaseFloat64BfSetUi(G, 2 * b - 1);
        AiBaseFloat64BfMulUi(G, G, 6 * b - 1, prec, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfMulUi(G, G, 6 * b - 5, prec, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfSetUi(&T0, AI_BASE_FLOAT64_CHUD_B);
        AiBaseFloat64BfMulUi(&T0, &T0, b, prec, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfSetUi(&T1, AI_BASE_FLOAT64_CHUD_A);
        AiBaseFloat64BfAdd(&T0, &T0, &T1, prec, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfMul(P, G, &T0, prec, AI_BASE_FLOAT64_BF_RNDN);
        P->sign = b & 1;

        AiBaseFloat64BfSetUi(Q, b);
        AiBaseFloat64BfMulUi(Q, Q, b, prec, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfMulUi(Q, Q, b, prec, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfMulUi(Q, Q, (UINT64_T)AI_BASE_FLOAT64_CHUD_C * AI_BASE_FLOAT64_CHUD_C * AI_BASE_FLOAT64_CHUD_C / 24, prec, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfDelete(&T0);
        AiBaseFloat64BfDelete(&T1);
    } else {
        AI_BASE_FLOAT64_BF_T P2, Q2, G2;
        
        AiBaseFloat64BfInit(&P2);
        AiBaseFloat64BfInit(&Q2);
        AiBaseFloat64BfInit(&G2);

        c = (a + b) / 2;
        AiBaseFloat64ChudBs(P, Q, G, a, c, 1, prec);
        AiBaseFloat64ChudBs(&P2, &Q2, &G2, c, b, need_g, prec);
        
        /* Q = Q1 * Q2 */
        /* G = G1 * G2 */
        /* P = P1 * Q2 + P2 * G1 */
        AiBaseFloat64BfMul(&P2, &P2, G, prec, AI_BASE_FLOAT64_BF_RNDN);
        if (!need_g)
            AiBaseFloat64BfSetUi(G, 0);
        AiBaseFloat64BfMul(P, P, &Q2, prec, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfAdd(P, P, &P2, prec, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfDelete(&P2);

        AiBaseFloat64BfMul(Q, Q, &Q2, prec, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfDelete(&Q2);
        if (need_g)
            AiBaseFloat64BfMul(G, G, &G2, prec, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfDelete(&G2);
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* compute Pi with faithful rounding at precision 'prec' using the
   Chudnovsky formula */
void AiBaseFloat64BfConstPiInternal(AI_BASE_FLOAT64_BF_T *Q, AI_BASE_FLOAT64_LIMB_T prec)
{
    INT64_T n, prec1;
    AI_BASE_FLOAT64_BF_T P, G;

    /* number of serie terms */
    n = prec / AI_BASE_FLOAT64_CHUD_BITS_PER_TERM + 1;
    /* XXX: precision analysis */
    prec1 = prec + 32;

    AiBaseFloat64BfInit(&P);
    AiBaseFloat64BfInit(&G);

    AiBaseFloat64ChudBs(&P, Q, &G, 0, n, 0, AI_BASE_FLOAT64_BF_PREC_INF);
    
    AiBaseFloat64BfMulUi(&G, Q, AI_BASE_FLOAT64_CHUD_A, (AI_BASE_FLOAT64_LIMB_T)prec1, AI_BASE_FLOAT64_BF_RNDN);
    AiBaseFloat64BfAdd(&P, &G, &P, (AI_BASE_FLOAT64_LIMB_T)prec1, AI_BASE_FLOAT64_BF_RNDN);
    AiBaseFloat64BfDiv(Q, Q, &P, (AI_BASE_FLOAT64_LIMB_T)prec1, AI_BASE_FLOAT64_BF_RNDF);
 
    AiBaseFloat64BfSetUi(&P, AI_BASE_FLOAT64_CHUD_C / 64);
    AiBaseFloat64BfRsqrt(&G, &P, (AI_BASE_FLOAT64_LIMB_T)prec1);
    AiBaseFloat64BfMulUi(&G, &G, (UINT64_T)AI_BASE_FLOAT64_CHUD_C * AI_BASE_FLOAT64_CHUD_C / (8 * 12), (AI_BASE_FLOAT64_LIMB_T)prec1, AI_BASE_FLOAT64_BF_RNDF);
    AiBaseFloat64BfMul(Q, Q, &G, prec, AI_BASE_FLOAT64_BF_RNDN);
    AiBaseFloat64BfDelete(&P);
    AiBaseFloat64BfDelete(&G);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfConstGet(AI_BASE_FLOAT64_BF_T *T, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags,
                        AI_BASE_FLOAT64_BF_CONST_CACHE *c,
                        void (*func)(AI_BASE_FLOAT64_BF_T *res, AI_BASE_FLOAT64_LIMB_T prec))
{
    AI_BASE_FLOAT64_LIMB_T ziv_extra_bits, prec1;

    ziv_extra_bits = 32;
    for(;;) {
        prec1 = prec + ziv_extra_bits;
        if (c->prec < prec1) {
            /* no AiBaseFloat64BfInit is needed since g_ai_base_float64_log2_cache is initialized
               to zero */
            func(&c->val, prec1);
            c->prec = prec1;
        } else {
            prec1 = c->prec;
        }
        AiBaseFloat64BfSet(T, &c->val);
        if (!AiBaseFloat64BfCanRound(T, prec, flags & AI_BASE_FLOAT64_BF_RND_MASK, prec1)) {
            /* and more precision and retry */
            ziv_extra_bits = ziv_extra_bits  + (ziv_extra_bits / 2);
        } else {
            break;
        }
    }
    return AiBaseFloat64BfRound(T, prec, flags);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfConstLog2(AI_BASE_FLOAT64_BF_T *T, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    return AiBaseFloat64BfConstGet(T, prec, flags, &g_ai_base_float64_log2_cache, AiBaseFloat64BfConstLog2Internal);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfConstPi(AI_BASE_FLOAT64_BF_T *T, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    return AiBaseFloat64BfConstGet(T, prec, flags, &g_ai_base_float64_pi_cache, AiBaseFloat64BfConstPiInternal);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfZivRounding(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a,
                           AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags,
                           AI_BASE_FLOAT64_ZIV_FUNC *f, void *opaque)
{
    INT32_T rnd_mode, ret;
    AI_BASE_FLOAT64_SLIMB_T prec1, ziv_extra_bits;
    
    rnd_mode = flags & AI_BASE_FLOAT64_BF_RND_MASK;
    if (rnd_mode == AI_BASE_FLOAT64_BF_RNDF) {
        /* no need to iterate */
        f(r, a, prec, opaque);
        ret = 0;
    } else {
        ziv_extra_bits = 32;
        for(;;) {
            prec1 = prec + ziv_extra_bits;
            ret = f(r, a, prec1, opaque);
            if (ret & (AI_BASE_FLOAT64_BF_ST_OVERFLOW | AI_BASE_FLOAT64_BF_ST_UNDERFLOW)) {
                /* should never happen because it indicates the
                   rounding cannot be done correctly, but we do not
                   catch all the cases */
                return ret;
            }
            /* if the result is exact, we can stop */
            if (!(ret & AI_BASE_FLOAT64_BF_ST_INEXACT)) {
                ret = 0;
                break;
            }
            if (AiBaseFloat64BfCanRound(r, prec, rnd_mode, prec1)) {
                ret = AI_BASE_FLOAT64_BF_ST_INEXACT;
                break;
            }
            ziv_extra_bits = ziv_extra_bits * 2;
        }
    }
    return AiBaseFloat64BfRound(r, prec, flags) | ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* Compute the exponential using faithful rounding at precision 'prec'.
   Note: the algorithm is from MPFR */
INT32_T AiBaseFloat64BfExpInternal(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, void *opaque)
{
    AI_BASE_FLOAT64_BF_T T_s, *T = &T_s;
    AI_BASE_FLOAT64_SLIMB_T n, K, l_value, i, prec1;
    
    if (!(r != a))
		return 0;

    /* argument reduction:
       T = a - n*log(2) with 0 <= T < log(2) and n integer.
    */
    AiBaseFloat64BfInit(T);
    if (a->expn <= -1) {
        /* 0 <= abs(a) <= 0.5 */
        if (a->sign)
            n = -1;
        else
            n = 0;
    } else {
        AiBaseFloat64BfConstLog2(T, AI_BASE_FLOAT64_LIMB_BITS, AI_BASE_FLOAT64_BF_RNDZ);
        AiBaseFloat64BfDiv(T, a, T, AI_BASE_FLOAT64_LIMB_BITS, AI_BASE_FLOAT64_BF_RNDD);
        AiBaseFloat64BfGetLimb(&n, T, 0);
    }

    K = AiBaseFloat64BfIsqrt((prec + 1) / 2);
    l_value = (prec - 1) / K + 1;
    /* XXX: precision analysis ? */
    prec1 = prec + (K + 2 * l_value + 18) + K + 8;
    if (a->expn > 0)
        prec1 += a->expn;
    //    printf("n=%ld K=%ld prec1=%ld\n", n, K, prec1);

    AiBaseFloat64BfConstLog2(T, prec1, AI_BASE_FLOAT64_BF_RNDF);
    AiBaseFloat64BfMulSi(T, T, n, prec1, AI_BASE_FLOAT64_BF_RNDN);
    AiBaseFloat64BfSub(T, a, T, prec1, AI_BASE_FLOAT64_BF_RNDN);

    /* reduce the range of T */
    AiBaseFloat64BfMul2Exp(T, -K, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
    
    /* Taylor expansion around zero :
     1 + x + x^2/2 + ... + x^n/n! 
     = (1 + x * (1 + x/2 * (1 + ... (x/n))))
    */
    {
        AI_BASE_FLOAT64_BF_T U_s, *U = &U_s;
        
        AiBaseFloat64BfInit(U);
        AiBaseFloat64BfSetUi(r, 1);
        for(i = l_value ; i >= 1; i--) {
            AiBaseFloat64BfSetUi(U, i);
            AiBaseFloat64BfDiv(U, T, U, prec1, AI_BASE_FLOAT64_BF_RNDN);
            AiBaseFloat64BfMul(r, r, U, prec1, AI_BASE_FLOAT64_BF_RNDN);
            AiBaseFloat64BfAddSi(r, r, 1, prec1, AI_BASE_FLOAT64_BF_RNDN);
        }
        AiBaseFloat64BfDelete(U);
    }
    AiBaseFloat64BfDelete(T);
    
    /* undo the range reduction */
    for(i = 0; i < K; i++) {
        AiBaseFloat64BfMul(r, r, r, prec1, AI_BASE_FLOAT64_BF_RNDN);
    }

    /* undo the argument reduction */
    AiBaseFloat64BfMul2Exp(r, n, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);

    return AI_BASE_FLOAT64_BF_ST_INEXACT;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfExp(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    if (!(r != a))
		return 0;
    if (a->len == 0) {
        if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
            AiBaseFloat64BfSetNan(r);
        } else if (a->expn == AI_BASE_FLOAT64_BF_EXP_INF) {
            if (a->sign)
                AiBaseFloat64BfSetZero(r, 0);
            else
                AiBaseFloat64BfSetInf(r, 0);
        } else {
            AiBaseFloat64BfSetUi(r, 1);
        }
        return 0;
    }

    /* crude overflow and underflow tests */
    if (a->expn > 0) {
        AI_BASE_FLOAT64_BF_T T_s, *T = &T_s;
        AI_BASE_FLOAT64_BF_T log2_s, *log2 = &log2_s;
        AI_BASE_FLOAT64_SLIMB_T e_min, e_max;
        e_max = (AI_BASE_FLOAT64_LIMB_T)1 << (AiBaseFloat64BfGetExpBits(flags) - 1);
        e_min = -e_max + 3;
        if (flags & AI_BASE_FLOAT64_BF_FLAG_SUBNORMAL)
            e_min -= (prec - 1);

        AiBaseFloat64BfInit(T);
        AiBaseFloat64BfInit(log2);
        AiBaseFloat64BfConstLog2(log2, AI_BASE_FLOAT64_LIMB_BITS, AI_BASE_FLOAT64_BF_RNDU);
        AiBaseFloat64BfMulUi(T, log2, e_max, AI_BASE_FLOAT64_LIMB_BITS, AI_BASE_FLOAT64_BF_RNDU);
        /* a > e_max * log(2) implies exp(a) > e_max */
        if (AiBaseFloat64BfCmpLt(T, a) > 0) {
            /* overflow */
            AiBaseFloat64BfDelete(T);
            AiBaseFloat64BfDelete(log2);
            return AiBaseFloat64BfSetOverFlow(r, 0, prec, flags);
        }
        /* a < e_min * log(2) implies exp(a) < e_min */
        AiBaseFloat64BfMulSi(T, log2, e_min, AI_BASE_FLOAT64_LIMB_BITS, AI_BASE_FLOAT64_BF_RNDD);
        if (AiBaseFloat64BfCmpLt(a, T)) {
            INT32_T rnd_mode = flags & AI_BASE_FLOAT64_BF_RND_MASK;

            /* underflow */
            AiBaseFloat64BfDelete(T);
            AiBaseFloat64BfDelete(log2);
            if (rnd_mode == AI_BASE_FLOAT64_BF_RNDU) {
                /* set the smallest value */
                AiBaseFloat64BfSetUi(r, 1);
                r->expn = e_min;
            } else {
                AiBaseFloat64BfSetZero(r, 0);
            }
            return AI_BASE_FLOAT64_BF_ST_UNDERFLOW | AI_BASE_FLOAT64_BF_ST_INEXACT;
        }
        AiBaseFloat64BfDelete(log2);
        AiBaseFloat64BfDelete(T);
    }

    return AiBaseFloat64BfZivRounding(r, a, prec, flags, AiBaseFloat64BfExpInternal, NULL);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfLogInternal(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, void *opaque)
{
    AI_BASE_FLOAT64_BF_T T_s, *T = &T_s;
    AI_BASE_FLOAT64_BF_T U_s, *U = &U_s;
    AI_BASE_FLOAT64_BF_T V_s, *V = &V_s;
    AI_BASE_FLOAT64_SLIMB_T n, prec1, l_value, i, K;
    
    if (!(r != a))
		return 0;

    AiBaseFloat64BfInit(T);
    /* argument reduction 1 */
    /* T=a*2^n with 2/3 <= T <= 4/3 */
    {
        AI_BASE_FLOAT64_BF_T U_s, *U = &U_s;
        AiBaseFloat64BfSet(T, a);
        n = T->expn;
        T->expn = 0;
        /* U= ~ 2/3 */
        AiBaseFloat64BfInit(U);
        AiBaseFloat64BfSetUi(U, 0xaaaaaaaa); 
        U->expn = 0;
        if (AiBaseFloat64BfCmpLt(T, U)) {
            T->expn++;
            n--;
        }
        AiBaseFloat64BfDelete(U);
    }
    //    printf("n=%ld\n", n);
    //    AiBaseFloat64BfPrintStr("T", T);

    /* XXX: precision analysis */
    /* number of iterations for argument reduction 2 */
    K = AiBaseFloat64BfIsqrt((prec + 1) / 2); 
    /* order of Taylor expansion */
    l_value = prec / (2 * K) + 1; 
    /* precision of the intermediate computations */
    prec1 = prec + K + 2 * l_value + 32;

    AiBaseFloat64BfInit(U);
    AiBaseFloat64BfInit(V);
    
    /* Note: cancellation occurs here, so we use more precision (XXX:
       reduce the precision by computing the exact cancellation) */
    AiBaseFloat64BfAddSi(T, T, -1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDN); 

    /* argument reduction 2 */
    for(i = 0; i < K; i++) {
        /* T = T / (1 + sqrt(1 + T)) */
        AiBaseFloat64BfAddSi(U, T, 1, prec1, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfSqrt(V, U, prec1, AI_BASE_FLOAT64_BF_RNDF);
        AiBaseFloat64BfAddSi(U, V, 1, prec1, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfDiv(T, T, U, prec1, AI_BASE_FLOAT64_BF_RNDN);
    }

    {
        AI_BASE_FLOAT64_BF_T Y_s, *Y = &Y_s;
        AI_BASE_FLOAT64_BF_T Y2_s, *Y2 = &Y2_s;
        AiBaseFloat64BfInit(Y);
        AiBaseFloat64BfInit(Y2);

        /* compute ln(1+x) = ln((1+y)/(1-y)) with y=x/(2+x)
           = y + y^3/3 + ... + y^(2*l_value + 1) / (2*l_value+1) 
           with Y=Y^2
           = y*(1+Y/3+Y^2/5+...) = y*(1+Y*(1/3+Y*(1/5 + ...)))
        */
        AiBaseFloat64BfAddSi(Y, T, 2, prec1, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfDiv(Y, T, Y, prec1, AI_BASE_FLOAT64_BF_RNDN);

        AiBaseFloat64BfMul(Y2, Y, Y, prec1, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfSetUi(r, 0);
        for(i = l_value; i >= 1; i--) {
            AiBaseFloat64BfSetUi(U, 1);
            AiBaseFloat64BfSetUi(V, 2 * i + 1);
            AiBaseFloat64BfDiv(U, U, V, prec1, AI_BASE_FLOAT64_BF_RNDN);
            AiBaseFloat64BfAdd(r, r, U, prec1, AI_BASE_FLOAT64_BF_RNDN);
            AiBaseFloat64BfMul(r, r, Y2, prec1, AI_BASE_FLOAT64_BF_RNDN);
        }
        AiBaseFloat64BfAddSi(r, r, 1, prec1, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfMul(r, r, Y, prec1, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfDelete(Y);
        AiBaseFloat64BfDelete(Y2);
    }
    AiBaseFloat64BfDelete(V);
    AiBaseFloat64BfDelete(U);

    /* multiplication by 2 for the Taylor expansion and undo the
       argument reduction 2*/
    AiBaseFloat64BfMul2Exp(r, K + 1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
    
    /* undo the argument reduction 1 */
    AiBaseFloat64BfConstLog2(T, prec1, AI_BASE_FLOAT64_BF_RNDF);
    AiBaseFloat64BfMulSi(T, T, n, prec1, AI_BASE_FLOAT64_BF_RNDN);
    AiBaseFloat64BfAdd(r, r, T, prec1, AI_BASE_FLOAT64_BF_RNDN);
    
    AiBaseFloat64BfDelete(T);
    return AI_BASE_FLOAT64_BF_ST_INEXACT;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfLog(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    AI_BASE_FLOAT64_BF_T T_s, *T = &T_s;
    
    if (!(r != a))
		return 0;

    if (a->len == 0) {
        if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
            AiBaseFloat64BfSetNan(r);
            return 0;
        } else if (a->expn == AI_BASE_FLOAT64_BF_EXP_INF) {
            if (a->sign) {
                AiBaseFloat64BfSetNan(r);
                return AI_BASE_FLOAT64_BF_ST_INVALID_OP;
            } else {
                AiBaseFloat64BfSetInf(r, 0);
                return 0;
            }
        } else {
            AiBaseFloat64BfSetInf(r, 1);
            return 0;
        }
    }
    if (a->sign) {
        AiBaseFloat64BfSetNan(r);
        return AI_BASE_FLOAT64_BF_ST_INVALID_OP;
    }
    AiBaseFloat64BfInit(T);
    AiBaseFloat64BfSetUi(T, 1);
    if (AiBaseFloat64BfCmpEq(a, T)) {
        AiBaseFloat64BfSetZero(r, 0);
        AiBaseFloat64BfDelete(T);
        return 0;
    }
    AiBaseFloat64BfDelete(T);

    return AiBaseFloat64BfZivRounding(r, a, prec, flags, AiBaseFloat64BfLogInternal, NULL);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* x and y finite and x > 0 */
/* XXX: overflow/underflow handling */
INT32_T AiBaseFloat64BfPowGeneric(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *x, AI_BASE_FLOAT64_LIMB_T prec, void *opaque)
{
    const AI_BASE_FLOAT64_BF_T *y = opaque;
    AI_BASE_FLOAT64_BF_T T_s, *T = &T_s;
    AI_BASE_FLOAT64_LIMB_T prec1;

    AiBaseFloat64BfInit(T);
    /* XXX: proof for the added precision */
    prec1 = prec + 32;
    AiBaseFloat64BfLog(T, x, prec1, AI_BASE_FLOAT64_BF_RNDF);
    AiBaseFloat64BfMul(T, T, y, prec1, AI_BASE_FLOAT64_BF_RNDF);
    AiBaseFloat64BfExp(r, T, prec1, AI_BASE_FLOAT64_BF_RNDF);
    AiBaseFloat64BfDelete(T);
    return AI_BASE_FLOAT64_BF_ST_INEXACT;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* x and y finite, x > 0, y integer and y fits on one limb */
/* XXX: overflow/underflow handling */ 
INT32_T AiBaseFloat64BfPowInt(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *x, AI_BASE_FLOAT64_LIMB_T prec, void *opaque)
{
    const AI_BASE_FLOAT64_BF_T *y = opaque;
    AI_BASE_FLOAT64_BF_T T_s, *T = &T_s;
    AI_BASE_FLOAT64_LIMB_T prec1;
    INT32_T ret;
    AI_BASE_FLOAT64_SLIMB_T y1;
    
    AiBaseFloat64BfGetLimb(&y1, y, 0);
    if (y1 < 0)
        y1 = -y1;
    /* XXX: proof for the added precision */
    prec1 = prec + AiBaseFloat64CeilLog2(y1) * 2 + 8;
    ret = AiBaseFloat64BfPowUi(r, x, y1 < 0 ? -y1 : y1, prec1, AI_BASE_FLOAT64_BF_RNDN);
    if (y->sign) {
        AiBaseFloat64BfInit(T);
        AiBaseFloat64BfSetUi(T, 1);
        ret |= AiBaseFloat64BfDiv(r, T, r, prec1, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfDelete(T);
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
/* x must be a finite non zero float. Return AI_BASE_FLOAT64_TRUE if there is a
   floating point number r such as x=r^(2^n) and return this floating
   point number 'r'. Otherwise return AI_BASE_FLOAT64_FALSE and r is undefined. */
BOOL_T AiBaseFloat64CheckExactPower2n(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *x, AI_BASE_FLOAT64_SLIMB_T n)
{
    AI_BASE_FLOAT64_BF_T T_s, *T = &T_s;
    AI_BASE_FLOAT64_SLIMB_T e, i, er;
    AI_BASE_FLOAT64_LIMB_T v;
    
    /* x = m*2^e with m odd integer */
    e = AiBaseFloat64BfGetExpMin(x);
    /* fast check on the exponent */
    if (n > (AI_BASE_FLOAT64_LIMB_BITS - 1)) {
        if (e != 0)
            return AI_BASE_FLOAT64_FALSE;
        er = 0;
    } else {
        if ((e & (((AI_BASE_FLOAT64_LIMB_T)1 << n) - 1)) != 0)
            return AI_BASE_FLOAT64_FALSE;
        er = e >> n;
    }
    /* every perfect odd square = 1 modulo 8 */
    v = AiBaseFloat64GetBits((const AI_BASE_FLOAT64_LIMB_T *)(x->tab), x->len, x->len * AI_BASE_FLOAT64_LIMB_BITS - x->expn + e);
    if ((v & 7) != 1)
        return AI_BASE_FLOAT64_FALSE;

    AiBaseFloat64BfInit(T);
    AiBaseFloat64BfSet(T, x);
    T->expn -= e;
    for(i = 0; i < n; i++) {
        if (i != 0)
            AiBaseFloat64BfSet(T, r);
        if (AiBaseFloat64BfSqrtrem(r, NULL, T) != 0)
            return AI_BASE_FLOAT64_FALSE;
    }
    r->expn += er;
    return AI_BASE_FLOAT64_TRUE;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* prec = AI_BASE_FLOAT64_BF_PREC_INF is accepted for x and y integers and y >= 0 */
INT32_T AiBaseFloat64BfPow(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *x, const AI_BASE_FLOAT64_BF_T *y, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    AI_BASE_FLOAT64_BF_T T_s, *T = &T_s;
    AI_BASE_FLOAT64_BF_T ytmp_s;
    BOOL_T y_is_int, y_is_odd;
    INT32_T r_sign, ret, rnd_mode;
    AI_BASE_FLOAT64_SLIMB_T y_emin;
    
    if (x->len == 0 || y->len == 0) {
        if (y->expn == AI_BASE_FLOAT64_BF_EXP_ZERO) {
            /* pow(x, 0) = 1 */
            AiBaseFloat64BfSetUi(r, 1);
        } else if (x->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
            AiBaseFloat64BfSetNan(r);
        } else {
            INT32_T cmp_x_abs_1;
            AiBaseFloat64BfSetUi(r, 1);
            cmp_x_abs_1 = AiBaseFloat64BfCmpu(x, r);
            if (cmp_x_abs_1 == 0 && (!x->sign || y->expn != AI_BASE_FLOAT64_BF_EXP_NAN)) {
                /* pow(1, y) = 1 even if y = NaN */
                /* pow(-1, +/-inf) = 1 */
            } else if (y->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
                AiBaseFloat64BfSetNan(r);
            } else if (y->expn == AI_BASE_FLOAT64_BF_EXP_INF) {
                if (y->sign == (cmp_x_abs_1 > 0)) {
                    AiBaseFloat64BfSetZero(r, 0);
                } else {
                    AiBaseFloat64BfSetInf(r, 0);
                }
            } else {
                y_emin = AiBaseFloat64BfGetExpMin(y);
                y_is_odd = (y_emin == 0);
                if (y->sign == (x->expn == AI_BASE_FLOAT64_BF_EXP_ZERO)) {
                    AiBaseFloat64BfSetInf(r, y_is_odd & x->sign);
                    if (y->sign) {
                        /* pow(0, y) with y < 0 */
                        return AI_BASE_FLOAT64_BF_ST_DIVIDE_ZERO;
                    }
                } else {
                    AiBaseFloat64BfSetZero(r, y_is_odd & x->sign);
                }
            }
        }
        return 0;
    }
    AiBaseFloat64BfInit(T);
    AiBaseFloat64BfSet(T, x);
    y_emin = AiBaseFloat64BfGetExpMin(y);
    y_is_int = (y_emin >= 0);
    rnd_mode = flags & AI_BASE_FLOAT64_BF_RND_MASK;
    if (x->sign) {
        if (!y_is_int) {
            AiBaseFloat64BfSetNan(r);
            AiBaseFloat64BfDelete(T);
            return AI_BASE_FLOAT64_BF_ST_INVALID_OP;
        }
        y_is_odd = (y_emin == 0);
        r_sign = y_is_odd;
        /* change the directed rounding mode if the sign of the result
           is changed */
        if (r_sign && (rnd_mode == AI_BASE_FLOAT64_BF_RNDD || rnd_mode == AI_BASE_FLOAT64_BF_RNDU))
            flags ^= 1;
        AiBaseFloat64BfNeg(T);
    } else {
        r_sign = 0;
    }

    AiBaseFloat64BfSetUi(r, 1);
    if (AiBaseFloat64BfCmpEq(T, r)) {
        /* abs(x) = 1: nothing more to do */
        ret = 0;
    } else if (y_is_int) {
        AI_BASE_FLOAT64_SLIMB_T T_bits, e;
    int_pow:
        T_bits = T->expn - AiBaseFloat64BfGetExpMin(T);
        if (T_bits == 1) {
            /* pow(2^b, y) = 2^(b*y) */
            AiBaseFloat64BfMulSi(T, y, T->expn - 1, AI_BASE_FLOAT64_LIMB_BITS, AI_BASE_FLOAT64_BF_RNDZ);
            AiBaseFloat64BfGetLimb(&e, T, 0);
            AiBaseFloat64BfSetUi(r, 1);
            ret = AiBaseFloat64BfMul2Exp(r, e, prec, flags);
        } else if (prec == AI_BASE_FLOAT64_BF_PREC_INF) {
            AI_BASE_FLOAT64_SLIMB_T y1;
            /* specific case for infinite precision (integer case) */
            AiBaseFloat64BfGetLimb(&y1, y, 0);
            if (!(!y->sign))
				return 0;
            /* x must be an integer, so abs(x) >= 2 */
            if (y1 >= ((AI_BASE_FLOAT64_SLIMB_T)1 << AI_BASE_FLOAT64_BF_EXP_BITS_MAX)) {
                AiBaseFloat64BfDelete(T);
                return AiBaseFloat64BfSetOverFlow(r, 0, AI_BASE_FLOAT64_BF_PREC_INF, flags);
            }
            ret = AiBaseFloat64BfPowUi(r, T, y1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        } else {
            if (y->expn <= 31) {
                /* small enough power: use exponentiation in all cases */
            } else if (y->sign) {
                /* cannot be exact */
                goto general_case;
            } else {
                if (rnd_mode == AI_BASE_FLOAT64_BF_RNDF)
                    goto general_case; /* no need to track exact results */
                /* see if the result has a chance to be exact:
                   if x=a*2^b (a odd), x^y=a^y*2^(b*y)
                   x^y needs a precision of at least floor_log2(a)*y bits
                */
                AiBaseFloat64BfMulSi(r, y, T_bits - 1, AI_BASE_FLOAT64_LIMB_BITS, AI_BASE_FLOAT64_BF_RNDZ);
                AiBaseFloat64BfGetLimb(&e, r, 0);
                if (prec < (AI_BASE_FLOAT64_LIMB_T)e)
                    goto general_case;
            }
            ret = AiBaseFloat64BfZivRounding(r, T, prec, flags, AiBaseFloat64BfPowInt, (void *)y);
        }
    } else {
        if (rnd_mode != AI_BASE_FLOAT64_BF_RNDF) {
            AI_BASE_FLOAT64_BF_T *y1;
            if (y_emin < 0 && AiBaseFloat64CheckExactPower2n(r, T, -y_emin)) {
                /* the problem is reduced to a power to an integer */
#if 0
                printf("\nn=%ld\n", -y_emin);
                AiBaseFloat64BfPrintStr("T", T);
                AiBaseFloat64BfPrintStr("r", r);
#endif
                AiBaseFloat64BfSet(T, r);
                y1 = &ytmp_s;
                memcpy(y1->tab, y->tab, sizeof(AI_BASE_FLOAT64_DLIMB_T)*2);
                y1->len = y->len;
                y1->sign = y->sign;
                y1->expn = y->expn - y_emin;
                y = y1;
                goto int_pow;
            }
        }
    general_case:
        ret = AiBaseFloat64BfZivRounding(r, T, prec, flags, AiBaseFloat64BfPowGeneric, (void *)y);
    }
    AiBaseFloat64BfDelete(T);
    r->sign = r_sign;
    return ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* compute sqrt(-2*x-x^2) to get |sin(x)| from cos(x) - 1. */
void AiBaseFloat64BfSqrtSin(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *x, AI_BASE_FLOAT64_LIMB_T prec1)
{
    AI_BASE_FLOAT64_BF_T T_s, *T = &T_s;
    AiBaseFloat64BfInit(T);
    AiBaseFloat64BfSet(T, x);
    AiBaseFloat64BfMul(r, T, T, prec1, AI_BASE_FLOAT64_BF_RNDN);
    AiBaseFloat64BfMul2Exp(T, 1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
    AiBaseFloat64BfAdd(T, T, r, prec1, AI_BASE_FLOAT64_BF_RNDN);
    AiBaseFloat64BfNeg(T);
    AiBaseFloat64BfSqrt(r, T, prec1, AI_BASE_FLOAT64_BF_RNDF);
    AiBaseFloat64BfDelete(T);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfSincos(AI_BASE_FLOAT64_BF_T *s, AI_BASE_FLOAT64_BF_T *c, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec)
{
    AI_BASE_FLOAT64_BF_T T_s, *T = &T_s;
    AI_BASE_FLOAT64_BF_T U_s, *U = &U_s;
    AI_BASE_FLOAT64_BF_T r_s, *r = &r_s;
    AI_BASE_FLOAT64_SLIMB_T K, prec1, i, l_value, mod, prec2;
    INT32_T is_neg;
    
    if (!(c != a && s != a))
		return 0;
    if (a->len == 0) {
        if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
            if (c)
                AiBaseFloat64BfSetNan(c);
            if (s)
                AiBaseFloat64BfSetNan(s);
            return 0;
        } else if (a->expn == AI_BASE_FLOAT64_BF_EXP_INF) {
            if (c)
                AiBaseFloat64BfSetNan(c);
            if (s)
                AiBaseFloat64BfSetNan(s);
            return AI_BASE_FLOAT64_BF_ST_INVALID_OP;
        } else {
            if (c)
                AiBaseFloat64BfSetUi(c, 1);
            if (s)
                AiBaseFloat64BfSetZero(s, a->sign);
            return 0;
        }
    }

    AiBaseFloat64BfInit(T);
    AiBaseFloat64BfInit(U);
    AiBaseFloat64BfInit(r);
    
    /* XXX: precision analysis */
    K = AiBaseFloat64BfIsqrt(prec / 2);
    l_value = prec / (2 * K) + 1;
    prec1 = prec + 2 * K + l_value + 8;
    
    /* after the modulo reduction, -pi/4 <= T <= pi/4 */
    if (a->expn <= -1) {
        /* abs(a) <= 0.25: no modulo reduction needed */
        AiBaseFloat64BfSet(T, a);
        mod = 0;
    } else {
        AI_BASE_FLOAT64_SLIMB_T cancel;
        cancel = 0;
        for(;;) {
            prec2 = prec1 + cancel;
            AiBaseFloat64BfConstPi(U, prec2, AI_BASE_FLOAT64_BF_RNDF);
            AiBaseFloat64BfMul2Exp(U, -1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
            AiBaseFloat64BfRemquo(&mod, T, a, U, prec2, AI_BASE_FLOAT64_BF_RNDN);
            //            printf("T.expn=%ld prec2=%ld\n", T->expn, prec2);
            if (mod == 0 || (T->expn != AI_BASE_FLOAT64_BF_EXP_ZERO &&
                             (T->expn + prec2) >= (prec1 - 1)))
                break;
            /* increase the number of bits until the precision is good enough */
            cancel = AiBaseFloat64BfMax(-T->expn, (cancel + 1) * 3 / 2);
        }
        mod &= 3;
    }
    
    is_neg = (INT32_T)(T->sign);
        
    /* compute cosm1(x) = cos(x) - 1 */
    AiBaseFloat64BfMul(T, T, T, prec1, AI_BASE_FLOAT64_BF_RNDN);
    AiBaseFloat64BfMul2Exp(T, -2 * K, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
    
    /* Taylor expansion:
       -x^2/2 + x^4/4! - x^6/6! + ...
    */
    AiBaseFloat64BfSetUi(r, 1);
    for(i = l_value ; i >= 1; i--) {
        AiBaseFloat64BfSetUi(U, 2 * i - 1);
        AiBaseFloat64BfMulUi(U, U, 2 * i, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        AiBaseFloat64BfDiv(U, T, U, prec1, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfMul(r, r, U, prec1, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfNeg(r);
        if (i != 1)
            AiBaseFloat64BfAddSi(r, r, 1, prec1, AI_BASE_FLOAT64_BF_RNDN);
    }
    AiBaseFloat64BfDelete(U);

    /* undo argument reduction:
       cosm1(2*x)= 2*(2*cosm1(x)+cosm1(x)^2)
    */
    for(i = 0; i < K; i++) {
        AiBaseFloat64BfMul(T, r, r, prec1, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfMul2Exp(r, 1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        AiBaseFloat64BfAdd(r, r, T, prec1, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfMul2Exp(r, 1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
    }
    AiBaseFloat64BfDelete(T);

    if (c) {
        if ((mod & 1) == 0) {
            AiBaseFloat64BfAddSi(c, r, 1, prec1, AI_BASE_FLOAT64_BF_RNDN);
        } else {
            AiBaseFloat64BfSqrtSin(c, r, prec1);
            c->sign = is_neg ^ 1;
        }
        c->sign ^= mod >> 1;
    }
    if (s) {
        if ((mod & 1) == 0) {
            AiBaseFloat64BfSqrtSin(s, r, prec1);
            s->sign = is_neg;
        } else {
            AiBaseFloat64BfAddSi(s, r, 1, prec1, AI_BASE_FLOAT64_BF_RNDN);
        }
        s->sign ^= mod >> 1;
    }
    AiBaseFloat64BfDelete(r);
    return AI_BASE_FLOAT64_BF_ST_INEXACT;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfCosInternal(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, void *opaque)
{
    return AiBaseFloat64BfSincos(NULL, r, a, prec);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfCos(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    return AiBaseFloat64BfZivRounding(r, a, prec, flags, AiBaseFloat64BfCosInternal, NULL);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfSinInternal(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, void *opaque)
{
    return AiBaseFloat64BfSincos(r, NULL, a, prec);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfSin(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    return AiBaseFloat64BfZivRounding(r, a, prec, flags, AiBaseFloat64BfSinInternal, NULL);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfTanInternal(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, void *opaque)
{
    AI_BASE_FLOAT64_BF_T T_s, *T = &T_s;
    AI_BASE_FLOAT64_LIMB_T prec1;
    
    if (a->len == 0) {
        if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
            AiBaseFloat64BfSetNan(r);
            return 0;
        } else if (a->expn == AI_BASE_FLOAT64_BF_EXP_INF) {
            AiBaseFloat64BfSetNan(r);
            return AI_BASE_FLOAT64_BF_ST_INVALID_OP;
        } else {
            AiBaseFloat64BfSetZero(r, a->sign);
            return 0;
        }
    }

    /* XXX: precision analysis */
    prec1 = prec + 8;
    AiBaseFloat64BfInit(T);
    AiBaseFloat64BfSincos(r, T, a, prec1);
    AiBaseFloat64BfDiv(r, r, T, prec1, AI_BASE_FLOAT64_BF_RNDF);
    AiBaseFloat64BfDelete(T);
    return AI_BASE_FLOAT64_BF_ST_INEXACT;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfTan(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    return AiBaseFloat64BfZivRounding(r, a, prec, flags, AiBaseFloat64BfTanInternal, NULL);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* if add_pi2 is true, add pi/2 to the result (used for acos(x) to
   avoid cancellation) */
INT32_T AiBaseFloat64BfAtanInternal(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec,
                            void *opaque)
{
    BOOL_T add_pi2 = (BOOL_T)(intptr_t)opaque;
    AI_BASE_FLOAT64_BF_T T_s, *T = &T_s;
    AI_BASE_FLOAT64_BF_T U_s, *U = &U_s;
    AI_BASE_FLOAT64_BF_T V_s, *V = &V_s;
    AI_BASE_FLOAT64_BF_T X2_s, *X2 = &X2_s;
    INT32_T cmp_1;
    AI_BASE_FLOAT64_SLIMB_T prec1, i, K, l_value;
    
    if (a->len == 0) {
        if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
            AiBaseFloat64BfSetNan(r);
            return 0;
        } else {
            if (a->expn == AI_BASE_FLOAT64_BF_EXP_INF)
                i = 1 - 2 * a->sign;
            else
                i = 0;
            i += add_pi2;
            /* return i*(pi/2) with -1 <= i <= 2 */
            if (i == 0) {
                AiBaseFloat64BfSetZero(r, add_pi2 ? 0 : a->sign);
                return 0;
            } else {
                /* PI or PI/2 */
                AiBaseFloat64BfConstPi(r, prec, AI_BASE_FLOAT64_BF_RNDF);
                if (i != 2)
                    AiBaseFloat64BfMul2Exp(r, -1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
                r->sign = (i < 0);
                return AI_BASE_FLOAT64_BF_ST_INEXACT;
            }
        }
    }
    
    AiBaseFloat64BfInit(T);
    AiBaseFloat64BfSetUi(T, 1);
    cmp_1 = AiBaseFloat64BfCmpu(a, T);
    if (cmp_1 == 0 && !add_pi2) {
        /* short cut: abs(a) == 1 -> +/-pi/4 */
        AiBaseFloat64BfConstPi(r, prec, AI_BASE_FLOAT64_BF_RNDF);
        AiBaseFloat64BfMul2Exp(r, -2, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        r->sign = a->sign;
        AiBaseFloat64BfDelete(T);
        return AI_BASE_FLOAT64_BF_ST_INEXACT;
    }

    /* XXX: precision analysis */
    K = AiBaseFloat64BfIsqrt((prec + 1) / 2);
    l_value = prec / (2 * K) + 1;
    prec1 = prec + K + 2 * l_value + 32;
    //    printf("prec=%ld K=%ld l_value=%ld prec1=%ld\n", prec, K, l_value, prec1);
    
    if (cmp_1 > 0) {
        AiBaseFloat64BfSetUi(T, 1);
        AiBaseFloat64BfDiv(T, T, a, prec1, AI_BASE_FLOAT64_BF_RNDN);
    } else {
        AiBaseFloat64BfSet(T, a);
    }

    /* abs(T) <= 1 */

    /* argument reduction */

    AiBaseFloat64BfInit(U);
    AiBaseFloat64BfInit(V);
    AiBaseFloat64BfInit(X2);
    for(i = 0; i < K; i++) {
        /* T = T / (1 + sqrt(1 + T^2)) */
        AiBaseFloat64BfMul(U, T, T, prec1, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfAddSi(U, U, 1, prec1, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfSqrt(V, U, prec1, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfAddSi(V, V, 1, prec1, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfDiv(T, T, V, prec1, AI_BASE_FLOAT64_BF_RNDN);
    }

    /* Taylor series: 
       x - x^3/3 + ... + (-1)^ l_value * y^(2*l_value + 1) / (2*l_value+1) 
    */
    AiBaseFloat64BfMul(X2, T, T, prec1, AI_BASE_FLOAT64_BF_RNDN);
    AiBaseFloat64BfSetUi(r, 0);
    for(i = l_value; i >= 1; i--) {
        AiBaseFloat64BfSetSi(U, 1);
        AiBaseFloat64BfSetUi(V, 2 * i + 1);
        AiBaseFloat64BfDiv(U, U, V, prec1, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfNeg(r);
        AiBaseFloat64BfAdd(r, r, U, prec1, AI_BASE_FLOAT64_BF_RNDN);
        AiBaseFloat64BfMul(r, r, X2, prec1, AI_BASE_FLOAT64_BF_RNDN);
    }
    AiBaseFloat64BfNeg(r);
    AiBaseFloat64BfAddSi(r, r, 1, prec1, AI_BASE_FLOAT64_BF_RNDN);
    AiBaseFloat64BfMul(r, r, T, prec1, AI_BASE_FLOAT64_BF_RNDN);

    /* undo the argument reduction */
    AiBaseFloat64BfMul2Exp(r, K, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
    
    AiBaseFloat64BfDelete(U);
    AiBaseFloat64BfDelete(V);
    AiBaseFloat64BfDelete(X2);

    i = add_pi2;
    if (cmp_1 > 0) {
        /* undo the inversion : r = sign(a)*PI/2 - r */
        AiBaseFloat64BfNeg(r);
        i += 1 - 2 * a->sign;
    }
    /* add i*(pi/2) with -1 <= i <= 2 */
    if (i != 0) {
        AiBaseFloat64BfConstPi(T, prec1, AI_BASE_FLOAT64_BF_RNDF);
        if (i != 2)
            AiBaseFloat64BfMul2Exp(T, -1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
        T->sign = (i < 0);
        AiBaseFloat64BfAdd(r, T, r, prec1, AI_BASE_FLOAT64_BF_RNDN);
    }
    
    AiBaseFloat64BfDelete(T);
    return AI_BASE_FLOAT64_BF_ST_INEXACT;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfAtan(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    return AiBaseFloat64BfZivRounding(r, a, prec, flags, AiBaseFloat64BfAtanInternal, (void *)AI_BASE_FLOAT64_FALSE);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfAtan2Internal(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *y, AI_BASE_FLOAT64_LIMB_T prec, void *opaque)
{
    const AI_BASE_FLOAT64_BF_T *x = opaque;
    AI_BASE_FLOAT64_BF_T T_s, *T = &T_s;
    AI_BASE_FLOAT64_LIMB_T prec1;
    INT32_T ret;
    
    if (y->expn == AI_BASE_FLOAT64_BF_EXP_NAN || x->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
        AiBaseFloat64BfSetNan(r);
        return 0;
    }

    /* compute atan(y/x) assumming inf/inf = 1 and 0/0 = 0 */
    AiBaseFloat64BfInit(T);
    prec1 = prec + 32;
    if (y->expn == AI_BASE_FLOAT64_BF_EXP_INF && x->expn == AI_BASE_FLOAT64_BF_EXP_INF) {
        AiBaseFloat64BfSetUi(T, 1);
        T->sign = y->sign ^ x->sign;
    } else if (y->expn == AI_BASE_FLOAT64_BF_EXP_ZERO && x->expn == AI_BASE_FLOAT64_BF_EXP_ZERO) {
        AiBaseFloat64BfSetZero(T, y->sign ^ x->sign);
    } else {
        AiBaseFloat64BfDiv(T, y, x, prec1, AI_BASE_FLOAT64_BF_RNDF);
    }
    ret = AiBaseFloat64BfAtan(r, T, prec1, AI_BASE_FLOAT64_BF_RNDF);

    if (x->sign) {
        /* if x < 0 (it includes -0), return sign(y)*pi + atan(y/x) */
        AiBaseFloat64BfConstPi(T, prec1, AI_BASE_FLOAT64_BF_RNDF);
        T->sign = y->sign;
        AiBaseFloat64BfAdd(r, r, T, prec1, AI_BASE_FLOAT64_BF_RNDN);
        ret |= AI_BASE_FLOAT64_BF_ST_INEXACT;
    }

    AiBaseFloat64BfDelete(T);
    return ret;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfAtan2(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *y, const AI_BASE_FLOAT64_BF_T *x,
             AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    return AiBaseFloat64BfZivRounding(r, y, prec, flags, AiBaseFloat64BfAtan2Internal, (void *)x);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfAsinInternal(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, void *opaque)
{
    BOOL_T is_acos = (BOOL_T)(intptr_t)opaque;
    AI_BASE_FLOAT64_BF_T T_s, *T = &T_s;
    AI_BASE_FLOAT64_LIMB_T prec1, prec2;
    INT32_T res;
    
    if (a->len == 0) {
        if (a->expn == AI_BASE_FLOAT64_BF_EXP_NAN) {
            AiBaseFloat64BfSetNan(r);
            return 0;
        } else if (a->expn == AI_BASE_FLOAT64_BF_EXP_INF) {
            AiBaseFloat64BfSetNan(r);
            return AI_BASE_FLOAT64_BF_ST_INVALID_OP;
        } else {
            if (is_acos) {
                AiBaseFloat64BfConstPi(r, prec, AI_BASE_FLOAT64_BF_RNDF);
                AiBaseFloat64BfMul2Exp(r, -1, AI_BASE_FLOAT64_BF_PREC_INF, AI_BASE_FLOAT64_BF_RNDZ);
                return AI_BASE_FLOAT64_BF_ST_INEXACT;
            } else {
                AiBaseFloat64BfSetZero(r, a->sign);
                return 0;
            }
        }
    }
    AiBaseFloat64BfInit(T);
    AiBaseFloat64BfSetUi(T, 1);
    res = AiBaseFloat64BfCmpu(a, T);
    if (res > 0) {
        AiBaseFloat64BfSetNan(r);
        return AI_BASE_FLOAT64_BF_ST_INVALID_OP;
    } else if (res == 0 && a->sign == 0 && is_acos) {
        AiBaseFloat64BfSetZero(r, 0);
        return 0;
    }
    
    /* asin(x) = atan(x/sqrt(1-x^2)) 
       acos(x) = pi/2 - asin(x) */
    prec1 = prec + 8;
    /* increase the precision in x^2 to compensate the cancellation in
       (1-x^2) if x is close to 1 */
    /* XXX: use less precision when possible */
    if (a->expn >= 0)
        prec2 = AI_BASE_FLOAT64_BF_PREC_INF;
    else
        prec2 = prec1;
    AiBaseFloat64BfMul(T, a, a, prec2, AI_BASE_FLOAT64_BF_RNDN);
    AiBaseFloat64BfNeg(T);
    AiBaseFloat64BfAddSi(T, T, 1, prec2, AI_BASE_FLOAT64_BF_RNDN);

    AiBaseFloat64BfSqrt(r, T, prec1, AI_BASE_FLOAT64_BF_RNDN);
    AiBaseFloat64BfDiv(T, a, r, prec1, AI_BASE_FLOAT64_BF_RNDN);
    if (is_acos)
        AiBaseFloat64BfNeg(T);
    AiBaseFloat64BfAtanInternal(r, T, prec1, (void *)(intptr_t)is_acos);
    AiBaseFloat64BfDelete(T);
    return AI_BASE_FLOAT64_BF_ST_INEXACT;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfAsin(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    return AiBaseFloat64BfZivRounding(r, a, prec, flags, AiBaseFloat64BfAsinInternal, (void *)AI_BASE_FLOAT64_FALSE);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfAcos(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags)
{
    return AiBaseFloat64BfZivRounding(r, a, prec, flags, AiBaseFloat64BfAsinInternal, (void *)AI_BASE_FLOAT64_TRUE);
}

#ifdef AI_BASE_FLOAT64_FFT_MUL
/***************************************************************/
/* Integer multiplication with FFT */
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* or AI_BASE_FLOAT64_LIMB_BITS at bit position 'pos' in tab */
 void AiBaseFloat64PutBits(AI_BASE_FLOAT64_LIMB_T *tab, AI_BASE_FLOAT64_LIMB_T len, AI_BASE_FLOAT64_SLIMB_T pos, AI_BASE_FLOAT64_LIMB_T val)
{
    AI_BASE_FLOAT64_LIMB_T i;
    INT32_T p;

    i = pos >> AI_BASE_FLOAT64_LIMB_LOG2_BITS;
    p = pos & (AI_BASE_FLOAT64_LIMB_BITS - 1);
    if (i < len)
        tab[i] |= val << p;
    if (p != 0) {
        i++;
        if (i < len) {
            tab[i] |= val >> (AI_BASE_FLOAT64_LIMB_BITS - p);
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
/* add modulo with up to (AI_BASE_FLOAT64_LIMB_BITS-1) bit modulo */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64AddMod(AI_BASE_FLOAT64_LIMB_T a, AI_BASE_FLOAT64_LIMB_T b, AI_BASE_FLOAT64_LIMB_T m)
{
    AI_BASE_FLOAT64_LIMB_T r;
    r = a + b;
    if (r >= m)
        r -= m;
    return r;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* sub modulo with up to AI_BASE_FLOAT64_LIMB_BITS bit modulo */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64SubMod(AI_BASE_FLOAT64_LIMB_T a, AI_BASE_FLOAT64_LIMB_T b, AI_BASE_FLOAT64_LIMB_T m)
{
    AI_BASE_FLOAT64_LIMB_T r;
    r = a - b;
    if (r > a)
        r += m;
    return r;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* return (r0+r1*B) mod m 
   precondition: 0 <= r0+r1*B < 2^(64+NTT_MOD_LOG2_MIN) 
*/
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64ModFast(AI_BASE_FLOAT64_DLIMB_T r, AI_BASE_FLOAT64_LIMB_T m, AI_BASE_FLOAT64_LIMB_T m_inv)
{
    AI_BASE_FLOAT64_LIMB_T a1, q, t0, r1, r0;
    
    a1 = r >> NTT_MOD_LOG2_MIN;
    
    q = ((AI_BASE_FLOAT64_DLIMB_T)a1 * m_inv) >> AI_BASE_FLOAT64_LIMB_BITS;
    r = r - (AI_BASE_FLOAT64_DLIMB_T)q * m - m * 2;
    r1 = r >> AI_BASE_FLOAT64_LIMB_BITS;
    t0 = (AI_BASE_FLOAT64_SLIMB_T)r1 >> 1;
    r += m & t0;
    r0 = r;
    r1 = r >> AI_BASE_FLOAT64_LIMB_BITS;
    r0 += m & r1;
    return r0;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* faster version using precomputed modulo inverse. 
   precondition: 0 <= a * b < 2^(64+NTT_MOD_LOG2_MIN) */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64MulModFast(AI_BASE_FLOAT64_LIMB_T a, AI_BASE_FLOAT64_LIMB_T b, 
                                    AI_BASE_FLOAT64_LIMB_T m, AI_BASE_FLOAT64_LIMB_T m_inv)
{
    AI_BASE_FLOAT64_DLIMB_T r;
    r = (AI_BASE_FLOAT64_DLIMB_T)a * (AI_BASE_FLOAT64_DLIMB_T)b;
    return AiBaseFloat64ModFast(r, m, m_inv);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64InitMulModFast(AI_BASE_FLOAT64_LIMB_T m)
{
    AI_BASE_FLOAT64_DLIMB_T t;
    assert(m < (AI_BASE_FLOAT64_LIMB_T)1 << NTT_MOD_LOG2_MAX);
    assert(m >= (AI_BASE_FLOAT64_LIMB_T)1 << NTT_MOD_LOG2_MIN);
    t = (AI_BASE_FLOAT64_DLIMB_T)1 << (AI_BASE_FLOAT64_LIMB_BITS + NTT_MOD_LOG2_MIN);
    return t / m;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* Faster version used when the multiplier is constant. 0 <= a < 2^64,
   0 <= b < m. */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64MulModFast2(AI_BASE_FLOAT64_LIMB_T a, AI_BASE_FLOAT64_LIMB_T b, 
                                     AI_BASE_FLOAT64_LIMB_T m, AI_BASE_FLOAT64_LIMB_T b_inv)
{
    AI_BASE_FLOAT64_LIMB_T r, q;

    q = ((AI_BASE_FLOAT64_DLIMB_T)a * (AI_BASE_FLOAT64_DLIMB_T)b_inv) >> AI_BASE_FLOAT64_LIMB_BITS;
    r = a * b - q * m;
    if (r >= m)
        r -= m;
    return r;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* Faster version used when the multiplier is constant. 0 <= a < 2^64,
   0 <= b < m. Let r = a * b mod m. The return value is 'r' or 'r +
   m'. */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64MulModFast3(AI_BASE_FLOAT64_LIMB_T a, AI_BASE_FLOAT64_LIMB_T b, 
                                     AI_BASE_FLOAT64_LIMB_T m, AI_BASE_FLOAT64_LIMB_T b_inv)
{
    AI_BASE_FLOAT64_LIMB_T r, q;

    q = ((AI_BASE_FLOAT64_DLIMB_T)a * (AI_BASE_FLOAT64_DLIMB_T)b_inv) >> AI_BASE_FLOAT64_LIMB_BITS;
    r = a * b - q * m;
    return r;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64InitMulModFast2(AI_BASE_FLOAT64_LIMB_T b, AI_BASE_FLOAT64_LIMB_T m)
{
    return ((AI_BASE_FLOAT64_DLIMB_T)b << AI_BASE_FLOAT64_LIMB_BITS) / m;
}

#ifdef __AVX2__
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64NttLimbToInt(NTTLimb a, AI_BASE_FLOAT64_LIMB_T m)
{
    AI_BASE_FLOAT64_SLIMB_T v;
    v = a;
    if (v < 0)
        v += m;
    if (v >= m)
        v -= m;
    return v;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
NTTLimb AiBaseFloat64IntToNttLimb(AI_BASE_FLOAT64_LIMB_T a, AI_BASE_FLOAT64_LIMB_T m)
{
    return (AI_BASE_FLOAT64_SLIMB_T)a;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
NTTLimb AiBaseFloat64IntToNttLimb2(AI_BASE_FLOAT64_LIMB_T a, AI_BASE_FLOAT64_LIMB_T m)
{
    if (a >= (m / 2))
        a -= m;
    return (AI_BASE_FLOAT64_SLIMB_T)a;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* return r + m if r < 0 otherwise r. */
 AI_BASE_FLOAT64_M256D AiBaseFloat64NttMod1(AI_BASE_FLOAT64_M256D r, AI_BASE_FLOAT64_M256D m)
{
    return _mm256_blendv_pd(r, r + m, r);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* input: abs(r) < 2 * m. Output: abs(r) < m */
 AI_BASE_FLOAT64_M256D AiBaseFloat64NttMod(AI_BASE_FLOAT64_M256D r, AI_BASE_FLOAT64_M256D mf, AI_BASE_FLOAT64_M256D m2f)
{
    return _mm256_blendv_pd(r, r + m2f, r) - mf;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* input: abs(a*b) < 2 * m^2, output: abs(r) < m */
 AI_BASE_FLOAT64_M256D AiBaseFloat64NttMulMod(AI_BASE_FLOAT64_M256D a, AI_BASE_FLOAT64_M256D b, AI_BASE_FLOAT64_M256D mf,
                                  AI_BASE_FLOAT64_M256D m_inv)
{
    AI_BASE_FLOAT64_M256D r, q, ab1, ab0, qm0, qm1;
    ab1 = a * b;
    q = _mm256_round_pd(ab1 * m_inv, 0); /* round to nearest */
    qm1 = q * mf;
    qm0 = _mm256_fmsub_pd(q, mf, qm1); /* low part */
    ab0 = _mm256_fmsub_pd(a, b, ab1); /* low part */
    r = (ab1 - qm1) + (ab0 - qm0);
    return r;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void *AiBaseFloat64BfAlignedMalloc(UINT32_T size, UINT32_T align)
{
    void *ptr;
    void **ptr1;
    ptr = AiBaseFloat64BfMalloc(size + sizeof(void *) + align - 1);
    if (!ptr)
        return NULL;
    ptr1 = (void **)(((uintptr_t)ptr + sizeof(void *) + align - 1) &
                     ~(align - 1));
    ptr1[-1] = ptr;
    return ptr1;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64BfAlignedFree(void *ptr)
{
    if (!ptr)
        return;
    AiBaseFloat64BfFree(((void **)ptr)[-1]);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void *AiBaseFloat64NttMalloc(UINT32_T size)
{
    return AiBaseFloat64BfAlignedMalloc(size, 64);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64NttFree(void *ptr)
{
    AiBaseFloat64BfAlignedFree(ptr);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64NttFft(NTTLimb *out_buf, NTTLimb *in_buf,
                              NTTLimb *tmp_buf, INT32_T fft_len_log2,
                              INT32_T inverse, INT32_T m_idx)
{
    AI_BASE_FLOAT64_LIMB_T nb_blocks, fft_per_block, p, k, n, stride_in, i, j;
    NTTLimb *tab_in, *tab_out, *tmp, *trig;
    AI_BASE_FLOAT64_M256D m_inv, mf, m2f, c, a0, a1, b0, b1;
    AI_BASE_FLOAT64_LIMB_T m;
    INT32_T l_value;
    
    m = g_ai_base_float64_ntt_mods[m_idx];
    
    m_inv = _mm256_set1_pd(1.0 / (FLOAT64_T)m);
    mf = _mm256_set1_pd(m);
    m2f = _mm256_set1_pd(m * 2);

    n = (AI_BASE_FLOAT64_LIMB_T)1 << fft_len_log2;
    assert(n >= 8);
    stride_in = n / 2;

    tab_in = in_buf;
    tab_out = tmp_buf;
    trig = AiBaseFloat64GetTrig(fft_len_log2, inverse, m_idx);
    p = 0;
    for(k = 0; k < stride_in; k += 4) {
        a0 = _mm256_load_pd(&tab_in[k]);
        a1 = _mm256_load_pd(&tab_in[k + stride_in]);
        c = _mm256_load_pd(trig);
        trig += 4;
        b0 = AiBaseFloat64NttMod(a0 + a1, mf, m2f);
        b1 = AiBaseFloat64NttMulMod(a0 - a1, c, mf, m_inv);
        a0 = _mm256_permute2f128_pd(b0, b1, 0x20);
        a1 = _mm256_permute2f128_pd(b0, b1, 0x31);
        a0 = _mm256_permute4x64_pd(a0, 0xd8);
        a1 = _mm256_permute4x64_pd(a1, 0xd8);
        _mm256_store_pd(&tab_out[p], a0);
        _mm256_store_pd(&tab_out[p + 4], a1);
        p += 2 * 4;
    }
    tmp = tab_in;
    tab_in = tab_out;
    tab_out = tmp;

    trig = AiBaseFloat64GetTrig(fft_len_log2 - 1, inverse, m_idx);
    p = 0;
    for(k = 0; k < stride_in; k += 4) {
        a0 = _mm256_load_pd(&tab_in[k]);
        a1 = _mm256_load_pd(&tab_in[k + stride_in]);
        c = _mm256_setr_pd(trig[0], trig[0], trig[1], trig[1]);
        trig += 2;
        b0 = AiBaseFloat64NttMod(a0 + a1, mf, m2f);
        b1 = AiBaseFloat64NttMulMod(a0 - a1, c, mf, m_inv);
        a0 = _mm256_permute2f128_pd(b0, b1, 0x20);
        a1 = _mm256_permute2f128_pd(b0, b1, 0x31);
        _mm256_store_pd(&tab_out[p], a0);
        _mm256_store_pd(&tab_out[p + 4], a1);
        p += 2 * 4;
    }
    tmp = tab_in;
    tab_in = tab_out;
    tab_out = tmp;
    
    nb_blocks = n / 4;
    fft_per_block = 4;

    l_value = fft_len_log2 - 2;
    while (nb_blocks != 2) {
        nb_blocks >>= 1;
        p = 0;
        k = 0;
        trig = AiBaseFloat64GetTrig(l_value, inverse, m_idx);
        for(i = 0; i < nb_blocks; i++) {
            c = _mm256_set1_pd(trig[0]);
            trig++;
            for(j = 0; j < fft_per_block; j += 4) {
                a0 = _mm256_load_pd(&tab_in[k + j]);
                a1 = _mm256_load_pd(&tab_in[k + j + stride_in]);
                b0 = AiBaseFloat64NttMod(a0 + a1, mf, m2f);
                b1 = AiBaseFloat64NttMulMod(a0 - a1, c, mf, m_inv);
                _mm256_store_pd(&tab_out[p + j], b0);
                _mm256_store_pd(&tab_out[p + j + fft_per_block], b1);
            }
            k += fft_per_block;
            p += 2 * fft_per_block;
        }
        fft_per_block <<= 1;
        l_value--;
        tmp = tab_in;
        tab_in = tab_out;
        tab_out = tmp;
    }

    tab_out = out_buf;
    for(k = 0; k < stride_in; k += 4) {
        a0 = _mm256_load_pd(&tab_in[k]);
        a1 = _mm256_load_pd(&tab_in[k + stride_in]);
        b0 = AiBaseFloat64NttMod(a0 + a1, mf, m2f);
        b1 = AiBaseFloat64NttMod(a0 - a1, mf, m2f);
        _mm256_store_pd(&tab_out[k], b0);
        _mm256_store_pd(&tab_out[k + stride_in], b1);
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64NttVecMul(NTTLimb *tab1, NTTLimb *tab2, AI_BASE_FLOAT64_LIMB_T fft_len_log2,
                        INT32_T k_tot, INT32_T m_idx)
{
    AI_BASE_FLOAT64_LIMB_T i, c_inv, n, m;
    AI_BASE_FLOAT64_M256D m_inv, mf, a, b, c;
    
    m = g_ai_base_float64_ntt_mods[m_idx];
    c_inv = g_ai_base_float64_ntt_len_inv[m_idx][k_tot][0];
    m_inv = _mm256_set1_pd(1.0 / (FLOAT64_T)m);
    mf = _mm256_set1_pd(m);
    c = _mm256_set1_pd(AiBaseFloat64IntToNttLimb(c_inv, m));
    n = (AI_BASE_FLOAT64_LIMB_T)1 << fft_len_log2;
    for(i = 0; i < n; i += 4) {
        a = _mm256_load_pd(&tab1[i]);
        b = _mm256_load_pd(&tab2[i]);
        a = AiBaseFloat64NttMulMod(a, b, mf, m_inv);
        a = AiBaseFloat64NttMulMod(a, c, mf, m_inv);
        _mm256_store_pd(&tab1[i], a);
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64MulTrig(NTTLimb *buf,
                               AI_BASE_FLOAT64_LIMB_T n, AI_BASE_FLOAT64_LIMB_T c1, AI_BASE_FLOAT64_LIMB_T m, AI_BASE_FLOAT64_LIMB_T m_inv1)
{
    AI_BASE_FLOAT64_LIMB_T i, c2, c3, c4;
    AI_BASE_FLOAT64_M256D c, c_mul, a0, mf, m_inv;
    assert(n >= 2);
    
    mf = _mm256_set1_pd(m);
    m_inv = _mm256_set1_pd(1.0 / (FLOAT64_T)m);

    c2 = AiBaseFloat64MulModFast(c1, c1, m, m_inv1);
    c3 = AiBaseFloat64MulModFast(c2, c1, m, m_inv1);
    c4 = AiBaseFloat64MulModFast(c2, c2, m, m_inv1);
    c = _mm256_setr_pd(1, AiBaseFloat64IntToNttLimb(c1, m),
                       AiBaseFloat64IntToNttLimb(c2, m), AiBaseFloat64IntToNttLimb(c3, m));
    c_mul = _mm256_set1_pd(AiBaseFloat64IntToNttLimb(c4, m));
    for(i = 0; i < n; i += 4) {
        a0 = _mm256_load_pd(&buf[i]);
        a0 = AiBaseFloat64NttMulMod(a0, c, mf, m_inv);
        _mm256_store_pd(&buf[i], a0);
        c = AiBaseFloat64NttMulMod(c, c_mul, mf, m_inv);
    }
}

#else

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void *AiBaseFloat64NttMalloc(UINT32_T size)
{
    return AiBaseFloat64BfMalloc(size);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64NttFree(void *ptr)
{
    AiBaseFloat64BfFree(ptr);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64NttLimbToInt(NTTLimb a, AI_BASE_FLOAT64_LIMB_T m)
{
    if (a >= m)
        a -= m;
    return a;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
NTTLimb AiBaseFloat64IntToNttLimb(AI_BASE_FLOAT64_SLIMB_T a, AI_BASE_FLOAT64_LIMB_T m)
{
    return a;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64NttFft(NTTLimb *out_buf, NTTLimb *in_buf,
                              NTTLimb *tmp_buf, INT32_T fft_len_log2,
                              INT32_T inverse, INT32_T m_idx)
{
    AI_BASE_FLOAT64_LIMB_T nb_blocks, fft_per_block, p, k, n, stride_in, i, j, m, m2;
    NTTLimb *tab_in, *tab_out, *tmp, a0, a1, b0, b1, c, *trig, c_inv;
    INT32_T l_value;
    
    m = g_ai_base_float64_ntt_mods[m_idx];
    m2 = 2 * m;
    n = (AI_BASE_FLOAT64_LIMB_T)1 << fft_len_log2;
    nb_blocks = n;
    fft_per_block = 1;
    stride_in = n / 2;
    tab_in = in_buf;
    tab_out = tmp_buf;
    l_value = fft_len_log2;
    while (nb_blocks != 2) {
        nb_blocks >>= 1;
        p = 0;
        k = 0;
        trig = AiBaseFloat64GetTrig(l_value, inverse, m_idx);
        for(i = 0; i < nb_blocks; i++) {
            c = trig[0];
            c_inv = trig[1];
            trig += 2;
            for(j = 0; j < fft_per_block; j++) {
                a0 = tab_in[k + j];
                a1 = tab_in[k + j + stride_in];
                b0 = AiBaseFloat64AddMod(a0, a1, m2);
                b1 = a0 - a1 + m2;
                b1 = AiBaseFloat64MulModFast3(b1, c, m, c_inv);
                tab_out[p + j] = b0;
                tab_out[p + j + fft_per_block] = b1;
            }
            k += fft_per_block;
            p += 2 * fft_per_block;
        }
        fft_per_block <<= 1;
        l_value--;
        tmp = tab_in;
        tab_in = tab_out;
        tab_out = tmp;
    }
    /* no twiddle in last step */
    tab_out = out_buf; 
    for(k = 0; k < stride_in; k++) {
        a0 = tab_in[k];
        a1 = tab_in[k + stride_in];
        b0 = AiBaseFloat64AddMod(a0, a1, m2);
        b1 = AiBaseFloat64SubMod(a0, a1, m2);
        tab_out[k] = b0;
        tab_out[k + stride_in] = b1;
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64NttVecMul(NTTLimb *tab1, NTTLimb *tab2, INT32_T fft_len_log2,
                        INT32_T k_tot, INT32_T m_idx)
{
    AI_BASE_FLOAT64_LIMB_T i, norm, norm_inv, a, n, m, m_inv;
    
    m = g_ai_base_float64_ntt_mods[m_idx];
    m_inv = g_ai_base_float64_ntt_mods_div[m_idx];
    norm = g_ai_base_float64_ntt_len_inv[m_idx][k_tot][0];
    norm_inv = g_ai_base_float64_ntt_len_inv[m_idx][k_tot][1];
    n = (AI_BASE_FLOAT64_LIMB_T)1 << fft_len_log2;
    for(i = 0; i < n; i++) {
        a = tab1[i];
        /* need to reduce the range so that the product is <
           2^(AI_BASE_FLOAT64_LIMB_BITS+NTT_MOD_LOG2_MIN) */
        if (a >= m)
            a -= m;
        a = AiBaseFloat64MulModFast(a, tab2[i], m, m_inv);
        a = AiBaseFloat64MulModFast3(a, norm, m, norm_inv);
        tab1[i] = a;
    }
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64MulTrig(NTTLimb *buf,
                               AI_BASE_FLOAT64_LIMB_T n, AI_BASE_FLOAT64_LIMB_T c_mul, AI_BASE_FLOAT64_LIMB_T m, AI_BASE_FLOAT64_LIMB_T m_inv)
{
    AI_BASE_FLOAT64_LIMB_T i, c0, c_mul_inv;
    
    c0 = 1;
    c_mul_inv = AiBaseFloat64InitMulModFast2(c_mul, m);
    for(i = 0; i < n; i++) {
        buf[i] = AiBaseFloat64MulModFast(buf[i], c0, m, m_inv);
        c0 = AiBaseFloat64MulModFast2(c0, c_mul, m, c_mul_inv);
    }
}

#endif /* !AVX2 */
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
NTTLimb *AiBaseFloat64GetTrig(INT32_T k, INT32_T inverse1, INT32_T m_idx1)
{
    NTTLimb *tab;
    AI_BASE_FLOAT64_LIMB_T i, n2, c, c_mul, m, c_mul_inv;
    INT32_T m_idx, inverse;
    
    if (k > NTT_TRIG_K_MAX)
        return NULL;

    for(;;) {
        tab = g_ai_base_float64_ntt_trig[m_idx1][inverse1][k];
        if (tab)
            return tab;
        n2 = (AI_BASE_FLOAT64_LIMB_T)1 << (k - 1);
        for(m_idx = 0; m_idx < NB_MODS; m_idx++) {
            m = g_ai_base_float64_ntt_mods[m_idx];
            for(inverse = 0; inverse < 2; inverse++) {
#ifdef __AVX2__
                tab = AiBaseFloat64NttMalloc(sizeof(NTTLimb) * n2);
#else
                tab = AiBaseFloat64NttMalloc(sizeof(NTTLimb) * n2 * 2);
#endif
                c = 1;
                c_mul = g_ai_base_float64_ntt_proot_pow[m_idx][inverse][k];
                c_mul_inv = g_ai_base_float64_ntt_proot_pow_inv[m_idx][inverse][k];
                for(i = 0; i < n2; i++) {
#ifdef __AVX2__
                    tab[i] = AiBaseFloat64IntToNttLimb2(c, m);
#else
                    tab[2 * i] = AiBaseFloat64IntToNttLimb(c, m);
                    tab[2 * i + 1] = AiBaseFloat64InitMulModFast2(c, m);
#endif
                    c = AiBaseFloat64MulModFast2(c, c_mul, m, c_mul_inv);
                }
                g_ai_base_float64_ntt_trig[m_idx][inverse][k] = tab;
            }
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
/* dst = buf1, src = buf2 */
void AiBaseFloat64NttFftPartial(NTTLimb *buf1,
                            INT32_T k1, INT32_T k2, AI_BASE_FLOAT64_LIMB_T n1, AI_BASE_FLOAT64_LIMB_T n2, INT32_T inverse,
                            AI_BASE_FLOAT64_LIMB_T m_idx)
{
    AI_BASE_FLOAT64_LIMB_T i, j, c_mul, c0, m, m_inv, strip_len, l_value;
    NTTLimb *buf2, *buf3;
    
    buf3 = AiBaseFloat64NttMalloc(sizeof(NTTLimb) * n1);
    if (k2 == 0) {
        AiBaseFloat64NttFft(buf1, buf1, buf3, k1, inverse, m_idx);
    } else {
        strip_len = AI_BASE_FLOAT64_STRIP_LEN;
        buf2 = AiBaseFloat64NttMalloc(sizeof(NTTLimb) * n1 * strip_len);

        m = g_ai_base_float64_ntt_mods[m_idx];
        m_inv = g_ai_base_float64_ntt_mods_div[m_idx];
        c0 = g_ai_base_float64_ntt_proot_pow[m_idx][inverse][k1 + k2];
        c_mul = 1;
        assert((n2 % strip_len) == 0);
        for(j = 0; j < n2; j += strip_len) {
            for(i = 0; i < n1; i++) {
                for(l_value = 0; l_value < strip_len; l_value++) {
                    buf2[i + l_value * n1] = buf1[i * n2 + (j + l_value)];
                }
            }
            for(l_value = 0; l_value < strip_len; l_value++) {
                if (inverse)
                    AiBaseFloat64MulTrig(buf2 + l_value * n1, n1, c_mul, m, m_inv);
                AiBaseFloat64NttFft(buf2 + l_value * n1, buf2 + l_value * n1, buf3, k1, inverse, m_idx);
                if (!inverse)
                    AiBaseFloat64MulTrig(buf2 + l_value * n1, n1, c_mul, m, m_inv);
                c_mul = AiBaseFloat64MulModFast(c_mul, c0, m, m_inv);
            }
            
            for(i = 0; i < n1; i++) {
                for(l_value = 0; l_value < strip_len; l_value++) {
                    buf1[i * n2 + (j + l_value)] = buf2[i + l_value *n1];
                }
            }
        }
        AiBaseFloat64NttFree(buf2);
    }
    AiBaseFloat64NttFree(buf3);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* dst = buf1, src = buf2, tmp = buf3 */
void AiBaseFloat64NttConv(NTTLimb *buf1, NTTLimb *buf2,
                     INT32_T k, INT32_T k_tot, AI_BASE_FLOAT64_LIMB_T m_idx)
{
    AI_BASE_FLOAT64_LIMB_T n1, n2, i;
    INT32_T k1, k2;
    
    if (k <= NTT_TRIG_K_MAX) {
        k1 = k;
    } else {
        /* recursive split of the FFT */
        k1 = AiBaseFloat64BfMin(k / 2, NTT_TRIG_K_MAX);
    }
    k2 = k - k1;
    n1 = (AI_BASE_FLOAT64_LIMB_T)1 << k1;
    n2 = (AI_BASE_FLOAT64_LIMB_T)1 << k2;
    
    AiBaseFloat64NttFftPartial(buf1, k1, k2, n1, n2, 0, m_idx);
    AiBaseFloat64NttFftPartial(buf2, k1, k2, n1, n2, 0, m_idx);
    if (k2 == 0) {
        AiBaseFloat64NttVecMul(buf1, buf2, k, k_tot, m_idx);
    } else {
        for(i = 0; i < n1; i++) {
            AiBaseFloat64NttConv(buf1 + i * n2, buf2 + i * n2, k2, k_tot, m_idx);
        }
    }
    AiBaseFloat64NttFftPartial(buf1, k1, k2, n1, n2, 1, m_idx);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64LimbToNtt(NTTLimb *tabr, AI_BASE_FLOAT64_LIMB_T fft_len,
                                  const AI_BASE_FLOAT64_LIMB_T *taba, AI_BASE_FLOAT64_LIMB_T a_len, INT32_T dpl,
                                  INT32_T first_m_idx, INT32_T nb_mods)
{
    AI_BASE_FLOAT64_SLIMB_T i, n;
    AI_BASE_FLOAT64_DLIMB_T a, b;
    INT32_T j, shift;
    AI_BASE_FLOAT64_LIMB_T base_mask1, a0, a1, a2, r, m, m_inv;
    
#if 0
    for(i = 0; i < a_len; i++) {
        printf("%" PRId64 ": " FMT_LIMB "\n",
               (INT64_T)i, taba[i]);
    }
#endif   
    memset(tabr, 0, sizeof(NTTLimb) * fft_len * nb_mods);
    shift = dpl & (AI_BASE_FLOAT64_LIMB_BITS - 1);
    if (shift == 0)
        base_mask1 = -1;
    else
        base_mask1 = ((AI_BASE_FLOAT64_LIMB_T)1 << shift) - 1;
    n = AiBaseFloat64BfMin(fft_len, (a_len * AI_BASE_FLOAT64_LIMB_BITS + dpl - 1) / dpl);
    for(i = 0; i < n; i++) {
        a0 = AiBaseFloat64GetBits(taba, a_len, i * dpl);
        if (dpl <= AI_BASE_FLOAT64_LIMB_BITS) {
            a0 &= base_mask1;
            a = a0;
        } else {
            a1 = AiBaseFloat64GetBits(taba, a_len, i * dpl + AI_BASE_FLOAT64_LIMB_BITS);
            if (dpl <= (AI_BASE_FLOAT64_LIMB_BITS + NTT_MOD_LOG2_MIN)) {
                a = a0 | ((AI_BASE_FLOAT64_DLIMB_T)(a1 & base_mask1) << AI_BASE_FLOAT64_LIMB_BITS);
            } else {
                if (dpl > 2 * AI_BASE_FLOAT64_LIMB_BITS) {
                    a2 = AiBaseFloat64GetBits(taba, a_len, i * dpl + AI_BASE_FLOAT64_LIMB_BITS * 2) &
                        base_mask1;
                } else {
                    a1 &= base_mask1;
                    a2 = 0;
                }
                //            printf("a=0x%016lx%016lx%016lx\n", a2, a1, a0);
                a = (a0 >> (AI_BASE_FLOAT64_LIMB_BITS - NTT_MOD_LOG2_MAX + NTT_MOD_LOG2_MIN)) |
                    ((AI_BASE_FLOAT64_DLIMB_T)a1 << (NTT_MOD_LOG2_MAX - NTT_MOD_LOG2_MIN)) |
                    ((AI_BASE_FLOAT64_DLIMB_T)a2 << (AI_BASE_FLOAT64_LIMB_BITS + NTT_MOD_LOG2_MAX - NTT_MOD_LOG2_MIN));
                a0 &= ((AI_BASE_FLOAT64_LIMB_T)1 << (AI_BASE_FLOAT64_LIMB_BITS - NTT_MOD_LOG2_MAX + NTT_MOD_LOG2_MIN)) - 1;
            }
        }
        for(j = 0; j < nb_mods; j++) {
            m = g_ai_base_float64_ntt_mods[first_m_idx + j];
            m_inv = g_ai_base_float64_ntt_mods_div[first_m_idx + j];
            r = AiBaseFloat64ModFast(a, m, m_inv);
            if (dpl > (AI_BASE_FLOAT64_LIMB_BITS + NTT_MOD_LOG2_MIN)) {
                b = ((AI_BASE_FLOAT64_DLIMB_T)r << (AI_BASE_FLOAT64_LIMB_BITS - NTT_MOD_LOG2_MAX + NTT_MOD_LOG2_MIN)) | a0;
                r = AiBaseFloat64ModFast(b, m, m_inv);
            }
            tabr[i + j * fft_len] = AiBaseFloat64IntToNttLimb(r, m);
        }
    }
}

#if defined(__AVX2__)
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64NttToLimb(AI_BASE_FLOAT64_LIMB_T *tabr, AI_BASE_FLOAT64_LIMB_T r_len,
                                  const NTTLimb *buf, INT32_T fft_len_log2, INT32_T dpl,
                                  INT32_T nb_mods)
{
    const AI_BASE_FLOAT64_LIMB_T *mods = g_ai_base_float64_ntt_mods + NB_MODS - nb_mods;
    const AI_BASE_FLOAT64_M256D *mods_cr_vec, *mf, *m_inv;
    AI_BASE_FLOAT64_VEC_UNION y[NB_MODS];
    AI_BASE_FLOAT64_LIMB_T u[NB_MODS], carry[NB_MODS], fft_len, base_mask1, r;
    AI_BASE_FLOAT64_SLIMB_T i, len, pos;
    INT32_T j, k, l_value, shift, n_limb1, p;
    AI_BASE_FLOAT64_DLIMB_T t;
        
    j = NB_MODS * (NB_MODS - 1) / 2 - nb_mods * (nb_mods - 1) / 2;
    mods_cr_vec = g_ai_base_float64_ntt_mods_cr_vec + j;
    mf = g_ai_base_float64_ntt_mods_vec + NB_MODS - nb_mods;
    m_inv = g_ai_base_float64_ntt_mods_inv_vec + NB_MODS - nb_mods;
        
    shift = dpl & (AI_BASE_FLOAT64_LIMB_BITS - 1);
    if (shift == 0)
        base_mask1 = -1;
    else
        base_mask1 = ((AI_BASE_FLOAT64_LIMB_T)1 << shift) - 1;
    n_limb1 = ((unsigned)dpl - 1) / AI_BASE_FLOAT64_LIMB_BITS;
    for(j = 0; j < NB_MODS; j++) 
        carry[j] = 0;
    for(j = 0; j < NB_MODS; j++) 
        u[j] = 0; /* avoid warnings */
    memset(tabr, 0, sizeof(AI_BASE_FLOAT64_LIMB_T) * r_len);
    fft_len = (AI_BASE_FLOAT64_LIMB_T)1 << fft_len_log2;
    len = AiBaseFloat64BfMin(fft_len, (r_len * AI_BASE_FLOAT64_LIMB_BITS + dpl - 1) / dpl);
    len = (len + AI_BASE_FLOAT64_VEC_LEN - 1) & ~(AI_BASE_FLOAT64_VEC_LEN - 1);
    i = 0;
    while (i < len) {
        for(j = 0; j < nb_mods; j++)
            y[j].v = *(AI_BASE_FLOAT64_M256D *)&buf[i + fft_len * j];

        /* Chinese remainder to get mixed radix representation */
        l_value = 0;
        for(j = 0; j < nb_mods - 1; j++) {
            y[j].v = AiBaseFloat64NttMod1(y[j].v, mf[j]);
            for(k = j + 1; k < nb_mods; k++) {
                y[k].v = AiBaseFloat64NttMulMod(y[k].v - y[j].v,
                                     mods_cr_vec[l_value], mf[k], m_inv[k]);
                l_value++;
            }
        }
        y[j].v = AiBaseFloat64NttMod1(y[j].v, mf[j]);
        
        for(p = 0; p < AI_BASE_FLOAT64_VEC_LEN; p++) {
            /* back to normal representation */
            u[0] = (INT64_T)y[nb_mods - 1].d[p];
            l_value = 1;
            for(j = nb_mods - 2; j >= 1; j--) {
                r = (INT64_T)y[j].d[p];
                for(k = 0; k < l_value; k++) {
                    t = (AI_BASE_FLOAT64_DLIMB_T)u[k] * mods[j] + r;
                    r = t >> AI_BASE_FLOAT64_LIMB_BITS;
                    u[k] = t;
                }
                u[l_value] = r;
                l_value++;
            }
            /* XXX: for nb_mods = 5, l_value should be 4 */
            
            /* last step adds the carry */
            r = (INT64_T)y[0].d[p];
            for(k = 0; k < l_value; k++) {
                t = (AI_BASE_FLOAT64_DLIMB_T)u[k] * mods[j] + r + carry[k];
                r = t >> AI_BASE_FLOAT64_LIMB_BITS;
                u[k] = t;
            }
            u[l_value] = r + carry[l_value];

#if 0
            printf("%" PRId64 ": ", i);
            for(j = nb_mods - 1; j >= 0; j--) {
                printf(" %019" PRIu64, u[j]);
            }
            printf("\n");
#endif
            
            /* write the digits */
            pos = i * dpl;
            for(j = 0; j < n_limb1; j++) {
                AiBaseFloat64PutBits(tabr, r_len, pos, u[j]);
                pos += AI_BASE_FLOAT64_LIMB_BITS;
            }
            AiBaseFloat64PutBits(tabr, r_len, pos, u[n_limb1] & base_mask1);
            /* shift by dpl digits and set the carry */
            if (shift == 0) {
                for(j = n_limb1 + 1; j < nb_mods; j++)
                    carry[j - (n_limb1 + 1)] = u[j];
            } else {
                for(j = n_limb1; j < nb_mods - 1; j++) {
                    carry[j - n_limb1] = (u[j] >> shift) |
                        (u[j + 1] << (AI_BASE_FLOAT64_LIMB_BITS - shift));
                }
                carry[nb_mods - 1 - n_limb1] = u[nb_mods - 1] >> shift;
            }
            i++;
        }
    }
}
#else
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64NttToLimb(AI_BASE_FLOAT64_LIMB_T *tabr, AI_BASE_FLOAT64_LIMB_T r_len,
                                  const NTTLimb *buf, INT32_T fft_len_log2, INT32_T dpl,
                                  INT32_T nb_mods)
{
    const AI_BASE_FLOAT64_LIMB_T *mods = g_ai_base_float64_ntt_mods + NB_MODS - nb_mods;
    const AI_BASE_FLOAT64_LIMB_T *mods_cr, *mods_cr_inv;
    AI_BASE_FLOAT64_LIMB_T y[NB_MODS], u[NB_MODS], carry[NB_MODS], fft_len, base_mask1, r;
    AI_BASE_FLOAT64_SLIMB_T i, len, pos;
    INT32_T j, k, l_value, shift, n_limb1;
    AI_BASE_FLOAT64_DLIMB_T t;
        
    j = NB_MODS * (NB_MODS - 1) / 2 - nb_mods * (nb_mods - 1) / 2;
    mods_cr = g_ai_base_float64_ntt_mods_cr + j;
    mods_cr_inv = g_ai_base_float64_ntt_mods_cr_inv + j;

    shift = dpl & (AI_BASE_FLOAT64_LIMB_BITS - 1);
    if (shift == 0)
        base_mask1 = -1;
    else
        base_mask1 = ((AI_BASE_FLOAT64_LIMB_T)1 << shift) - 1;
    n_limb1 = ((unsigned)dpl - 1) / AI_BASE_FLOAT64_LIMB_BITS;
    for(j = 0; j < NB_MODS; j++) 
        carry[j] = 0;
    for(j = 0; j < NB_MODS; j++) 
        u[j] = 0; /* avoid warnings */
    memset(tabr, 0, sizeof(AI_BASE_FLOAT64_LIMB_T) * r_len);
    fft_len = (AI_BASE_FLOAT64_LIMB_T)1 << fft_len_log2;
    len = AiBaseFloat64BfMin(fft_len, (r_len * AI_BASE_FLOAT64_LIMB_BITS + dpl - 1) / dpl);
    for(i = 0; i < len; i++) {
        for(j = 0; j < nb_mods; j++)  {
            y[j] = AiBaseFloat64NttLimbToInt(buf[i + fft_len * j], mods[j]);
        }

        /* Chinese remainder to get mixed radix representation */
        l_value = 0;
        for(j = 0; j < nb_mods - 1; j++) {
            for(k = j + 1; k < nb_mods; k++) {
                AI_BASE_FLOAT64_LIMB_T m;
                m = mods[k];
                /* Note: there is no overflow in the AiBaseFloat64SubMod() because
                   the modulos are sorted by increasing order */
                y[k] = AiBaseFloat64MulModFast2(y[k] - y[j] + m, 
                                     mods_cr[l_value], m, mods_cr_inv[l_value]);
                l_value++;
            }
        }
        
        /* back to normal representation */
        u[0] = y[nb_mods - 1];
        l_value = 1;
        for(j = nb_mods - 2; j >= 1; j--) {
            r = y[j];
            for(k = 0; k < l_value; k++) {
                t = (AI_BASE_FLOAT64_DLIMB_T)u[k] * mods[j] + r;
                r = t >> AI_BASE_FLOAT64_LIMB_BITS;
                u[k] = t;
            }
            u[l_value] = r;
            l_value++;
        }
        
        /* last step adds the carry */
        r = y[0];
        for(k = 0; k < l_value; k++) {
            t = (AI_BASE_FLOAT64_DLIMB_T)u[k] * mods[j] + r + carry[k];
            r = t >> AI_BASE_FLOAT64_LIMB_BITS;
            u[k] = t;
        }
        u[l_value] = r + carry[l_value];

#if 0
        printf("%" PRId64 ": ", (INT64_T)i);
        for(j = nb_mods - 1; j >= 0; j--) {
            printf(" " FMT_LIMB, u[j]);
        }
        printf("\n");
#endif
        
        /* write the digits */
        pos = i * dpl;
        for(j = 0; j < n_limb1; j++) {
            AiBaseFloat64PutBits(tabr, r_len, pos, u[j]);
            pos += AI_BASE_FLOAT64_LIMB_BITS;
        }
        AiBaseFloat64PutBits(tabr, r_len, pos, u[n_limb1] & base_mask1);
        /* shift by dpl digits and set the carry */
        if (shift == 0) {
            for(j = n_limb1 + 1; j < nb_mods; j++)
                carry[j - (n_limb1 + 1)] = u[j];
        } else {
            for(j = n_limb1; j < nb_mods - 1; j++) {
                carry[j - n_limb1] = (u[j] >> shift) |
                    (u[j + 1] << (AI_BASE_FLOAT64_LIMB_BITS - shift));
            }
            carry[nb_mods - 1 - n_limb1] = u[nb_mods - 1] >> shift;
        }
    }
}
#endif

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64NttStaticInit(void)
{
    INT32_T inited;
    INT32_T inverse, i, j, k, l_value;
    AI_BASE_FLOAT64_LIMB_T c, c_inv, c_inv2, m, m_inv;

    if (inited)
        return;
    inited = 1;
    for(j = 0; j < NB_MODS; j++) {
        m = g_ai_base_float64_ntt_mods[j];
        m_inv = AiBaseFloat64InitMulModFast(m);
        g_ai_base_float64_ntt_mods_div[j] = m_inv;
#if defined(__AVX2__)
        g_ai_base_float64_ntt_mods_vec[j] = _mm256_set1_pd(m);
        g_ai_base_float64_ntt_mods_inv_vec[j] = _mm256_set1_pd(1.0 / (FLOAT64_T)m);
#endif
        c_inv2 = (m + 1) / 2; /* 1/2 */
        c_inv = 1;
        for(i = 0; i <= NTT_PROOT_2EXP; i++) {
            g_ai_base_float64_ntt_len_inv[j][i][0] = c_inv;
            g_ai_base_float64_ntt_len_inv[j][i][1] = AiBaseFloat64InitMulModFast2(c_inv, m);
            c_inv = AiBaseFloat64MulModFast(c_inv, c_inv2, m, m_inv);
        }

        for(inverse = 0; inverse < 2; inverse++) {
            c = g_ai_base_float64_ntt_proot[inverse][j];
            for(i = 0; i < NTT_PROOT_2EXP; i++) {
                g_ai_base_float64_ntt_proot_pow[j][inverse][NTT_PROOT_2EXP - i] = c;
                g_ai_base_float64_ntt_proot_pow_inv[j][inverse][NTT_PROOT_2EXP - i] =
                    AiBaseFloat64InitMulModFast2(c, m);
                c = AiBaseFloat64MulModFast(c, c, m, m_inv);
            }
        }
    }

    l_value = 0;
    for(j = 0; j < NB_MODS - 1; j++) {
        for(k = j + 1; k < NB_MODS; k++) {
#if defined(__AVX2__)
            g_ai_base_float64_ntt_mods_cr_vec[l_value] = _mm256_set1_pd(AiBaseFloat64IntToNttLimb2(g_ai_base_float64_ntt_mods_cr[l_value],
                                                                 g_ai_base_float64_ntt_mods[k]));
#else
            g_ai_base_float64_ntt_mods_cr_inv[l_value] = AiBaseFloat64InitMulModFast2(g_ai_base_float64_ntt_mods_cr[l_value],
                                                    g_ai_base_float64_ntt_mods[k]);
#endif
            l_value++;
        }
    }
                
}

INT32_T AiBaseFloat64BfGetFftSize(INT32_T *pdpl, INT32_T *pnb_mods, AI_BASE_FLOAT64_LIMB_T len)
{
    INT32_T dpl, fft_len_log2, n_bits, nb_mods, dpl_found, fft_len_log2_found;
    INT32_T int_bits, nb_mods_found;
    AI_BASE_FLOAT64_LIMB_T cost, min_cost;
    
    min_cost = -1;
    dpl_found = 0;
    nb_mods_found = 4;
    fft_len_log2_found = 0;
    for(nb_mods = 3; nb_mods <= NB_MODS; nb_mods++) {
        int_bits = g_ai_base_float64_ntt_int_bits[NB_MODS - nb_mods];
        dpl = AiBaseFloat64BfMin((int_bits - 4) / 2,
                     2 * AI_BASE_FLOAT64_LIMB_BITS + 2 * NTT_MOD_LOG2_MIN - NTT_MOD_LOG2_MAX);
        for(;;) {
            fft_len_log2 = AiBaseFloat64CeilLog2((len * AI_BASE_FLOAT64_LIMB_BITS + dpl - 1) / dpl);
            if (fft_len_log2 > NTT_PROOT_2EXP)
                goto next;
            n_bits = fft_len_log2 + 2 * dpl;
            if (n_bits <= int_bits) {
                cost = ((AI_BASE_FLOAT64_LIMB_T)(fft_len_log2 + 1) << fft_len_log2) * nb_mods;
                //                printf("n=%d dpl=%d: cost=%" PRId64 "\n", nb_mods, dpl, (INT64_T)cost);
                if (cost < min_cost) {
                    min_cost = cost;
                    dpl_found = dpl;
                    nb_mods_found = nb_mods;
                    fft_len_log2_found = fft_len_log2;
                }
                break;
            }
            dpl--;
            if (dpl == 0)
                break;
        }
    next: ;
    }
    if (!dpl_found)
        abort();
    /* limit dpl if possible to reduce fixed cost of limb/NTT conversion */
    if (dpl_found > (AI_BASE_FLOAT64_LIMB_BITS + NTT_MOD_LOG2_MIN) &&
        ((AI_BASE_FLOAT64_LIMB_T)(AI_BASE_FLOAT64_LIMB_BITS + NTT_MOD_LOG2_MIN) << fft_len_log2_found) >=
        len * AI_BASE_FLOAT64_LIMB_BITS) {
        dpl_found = AI_BASE_FLOAT64_LIMB_BITS + NTT_MOD_LOG2_MIN;
    }
    *pnb_mods = nb_mods_found;
    *pdpl = dpl_found;
    return fft_len_log2_found;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat64FftMul(AI_BASE_FLOAT64_BF_T *res, AI_BASE_FLOAT64_LIMB_T *a_tab, AI_BASE_FLOAT64_LIMB_T a_len,
                              AI_BASE_FLOAT64_LIMB_T *b_tab, AI_BASE_FLOAT64_LIMB_T b_len, INT32_T mul_flags)
{
    INT32_T dpl, fft_len_log2, j, nb_mods, reduced_mem;
    AI_BASE_FLOAT64_SLIMB_T len, fft_len;
    NTTLimb *buf1, *buf2, *ptr;
#if defined(AI_BASE_FLOAT64_USE_MUL_CHECK)
    AI_BASE_FLOAT64_LIMB_T ha, hb, hr, h_ref;
#endif
    
    AiBaseFloat64NttStaticInit();

    /* find the optimal number of digits per limb (dpl) */
    len = a_len + b_len;
    fft_len_log2 = AiBaseFloat64BfGetFftSize(&dpl, &nb_mods, len);
    fft_len = (UINT64_T)1 << fft_len_log2;
    //    printf("len=%" PRId64 " fft_len_log2=%d dpl=%d\n", len, fft_len_log2, dpl);
#if defined(AI_BASE_FLOAT64_USE_MUL_CHECK)
    ha = AiBaseFloat64MpMod1(a_tab, a_len, AI_BASE_FLOAT64_BF_CHKSUM_MOD, 0);
    hb = AiBaseFloat64MpMod1(b_tab, b_len, AI_BASE_FLOAT64_BF_CHKSUM_MOD, 0);
#endif
    if ((mul_flags & (AI_BASE_FLOAT64_FFT_MUL_R_OVERLAP_A | AI_BASE_FLOAT64_FFT_MUL_R_OVERLAP_B)) == 0) {
        AiBaseFloat64BfResize(res, 0);
    } else if (mul_flags & AI_BASE_FLOAT64_FFT_MUL_R_OVERLAP_B) {
        AI_BASE_FLOAT64_LIMB_T *tmp_tab, tmp_len;
        /* it is better to free 'b' first */
        tmp_tab = a_tab;
        a_tab = b_tab;
        b_tab = tmp_tab;
        tmp_len = a_len;
        a_len = b_len;
        b_len = tmp_len;
    }
    buf1 = AiBaseFloat64NttMalloc(sizeof(NTTLimb) * fft_len * nb_mods);
    AiBaseFloat64LimbToNtt(buf1, fft_len, a_tab, a_len, dpl,
                NB_MODS - nb_mods, nb_mods);
    if ((mul_flags & (AI_BASE_FLOAT64_FFT_MUL_R_OVERLAP_A | AI_BASE_FLOAT64_FFT_MUL_R_OVERLAP_B)) == 
        AI_BASE_FLOAT64_FFT_MUL_R_OVERLAP_A) {
        AiBaseFloat64BfResize(res, 0);
    }
    reduced_mem = (fft_len_log2 >= 14);
    if (!reduced_mem) {
        buf2 = AiBaseFloat64NttMalloc(sizeof(NTTLimb) * fft_len * nb_mods);
        AiBaseFloat64LimbToNtt(buf2, fft_len, b_tab, b_len, dpl,
                    NB_MODS - nb_mods, nb_mods);
        AiBaseFloat64BfResize(res, 0); /* in case res == b */
    } else {
        buf2 = AiBaseFloat64NttMalloc(sizeof(NTTLimb) * fft_len);
    }
    for(j = 0; j < nb_mods; j++) {
        if (reduced_mem) {
            AiBaseFloat64LimbToNtt(buf2, fft_len, b_tab, b_len, dpl,
                        NB_MODS - nb_mods + j, 1);
            ptr = buf2;
        } else {
            ptr = buf2 + fft_len * j;
        }
        AiBaseFloat64NttConv(buf1 + fft_len * j, ptr,
                 fft_len_log2, fft_len_log2, j + NB_MODS - nb_mods);
    }
    AiBaseFloat64BfResize(res, 0); /* in case res == b and reduced mem */
    AiBaseFloat64NttFree(buf2);
    AiBaseFloat64BfResize(res, len);
    AiBaseFloat64NttToLimb(res->tab, len, buf1, fft_len_log2, dpl, nb_mods);
    AiBaseFloat64NttFree(buf1);
#if defined(AI_BASE_FLOAT64_USE_MUL_CHECK)
    hr = AiBaseFloat64MpMod1(res->tab, len, AI_BASE_FLOAT64_BF_CHKSUM_MOD, 0);
    h_ref = AiBaseFloat64MulMod(ha, hb, AI_BASE_FLOAT64_BF_CHKSUM_MOD);
    if (hr != h_ref) {
        printf("ntt_mul_error: len=%" AI_BASE_FLOAT64_PRID_LIMB " fft_len_log2=%d dpl=%d nb_mods=%d\n",
               len, fft_len_log2, dpl, nb_mods);
        //        printf("ha=0x" FMT_LIMB" hb=0x" FMT_LIMB " hr=0x" FMT_LIMB " expected=0x" FMT_LIMB "\n", ha, hb, hr, h_ref);
        exit(1);
    }
#endif    
}

#else /* AI_BASE_FLOAT64_FFT_MUL */

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat64BfGetFftSize(INT32_T *pdpl, INT32_T *pnb_mods, AI_BASE_FLOAT64_LIMB_T len)
{
    return 0;
}

#endif /* !AI_BASE_FLOAT64_FFT_MUL */


/*------------------------- End ---------------------------------------------*/
#endif

