/**
 ******************************************************************************
 * file    ai_base_float16_bf.h
 * author  sunlingge
 * version V100
 * brief   the ai base float 16 bf driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT16_BF_H
#define AI_BASE_FLOAT16_BF_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP16 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT16_LIMB_LOG2_BITS					(4)
#define AI_BASE_FLOAT16_LIMB_BITS						(1 << AI_BASE_FLOAT16_LIMB_LOG2_BITS)		

/*------------------------- Pre Type Definition -----------------------------*/
typedef INT16_T AI_BASE_FLOAT16_SLIMB_T, *AI_BASE_FLOAT16_SLIMB_T_PTR;
typedef UINT16_T AI_BASE_FLOAT16_LIMB_T, *AI_BASE_FLOAT16_LIMB_T_PTR;
typedef UINT32_T AI_BASE_FLOAT16_DLIMB_T, *AI_BASE_FLOAT16_DLIMB_T_PTR;

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT16_INT16_MAX						(0x7FFF)							
#define AI_BASE_FLOAT16_INT16_MIN						(-AI_BASE_FLOAT16_INT16_MAX - 1)	
#define AI_BASE_FLOAT16_EXP_MIN							(AI_BASE_FLOAT16_INT16_MIN)
#define AI_BASE_FLOAT16_EXP_MAX							(AI_BASE_FLOAT16_INT16_MAX)
/* in bits */
#define AI_BASE_FLOAT16_BF_EXP_BITS_MIN					(3)
#define AI_BASE_FLOAT16_BF_EXP_BITS_MAX					(AI_BASE_FLOAT16_LIMB_BITS - 2)
#define AI_BASE_FLOAT16_BF_PREC_MIN						(2)
#define AI_BASE_FLOAT16_BF_PREC_MAX						(((AI_BASE_FLOAT16_LIMB_T)1 << AI_BASE_FLOAT16_BF_EXP_BITS_MAX) - 2)
/* infinite precision */
#define AI_BASE_FLOAT16_BF_PREC_INF						(AI_BASE_FLOAT16_BF_PREC_MAX + 1)	
#define AI_BASE_FLOAT16_BF_CHKSUM_MOD					(31234)
#define AI_BASE_FLOAT16_BF_EXP_ZERO						(AI_BASE_FLOAT16_EXP_MIN)
#define AI_BASE_FLOAT16_BF_EXP_INF						(AI_BASE_FLOAT16_EXP_MAX - 1)
#define AI_BASE_FLOAT16_BF_EXP_NAN						(AI_BASE_FLOAT16_EXP_MAX)

/* maximum radix for AiBaseFloat16Bfatof() and AiBaseFloat16Bfftoa() */
#define AI_BASE_FLOAT16_BF_RADIX_MAX						(36) 

#ifdef AI_BASE_FLOAT16_FFT_MUL
#define AI_BASE_FLOAT16_STRIP_LEN							(16)
#if defined(__AVX2__)
/* we must have: modulo >= 1 << NTT_MOD_LOG2_MIN */
#define AI_BASE_FLOAT16_NTT_MOD_LOG2_MIN					(50)
#define AI_BASE_FLOAT16_NTT_MOD_LOG2_MAX					(51)
#define AI_BASE_FLOAT16_NB_MODS							(5)
#define AI_BASE_FLOAT16_NTT_PROOT_2EXP					(39)
#define AI_BASE_FLOAT16_VEC_LEN							(4)
#else
#if AI_BASE_FLOAT16_LIMB_BITS == 64
#define AI_BASE_FLOAT16_NTT_MOD_LOG2_MIN					(61)
#define AI_BASE_FLOAT16_NTT_MOD_LOG2_MAX					(62)
#define AI_BASE_FLOAT16_NB_MODS							(5)
#define AI_BASE_FLOAT16_NTT_PROOT_2EXP					(51)
#endif
#if AI_BASE_FLOAT16_LIMB_BITS == 32
/* we must have: modulo >= 1 << NTT_MOD_LOG2_MIN */
#define AI_BASE_FLOAT16_NTT_MOD_LOG2_MIN					(29)
#define AI_BASE_FLOAT16_NTT_MOD_LOG2_MAX					(30)
#define AI_BASE_FLOAT16_NB_MODS							(5)
#define AI_BASE_FLOAT16_NTT_PROOT_2EXP					(20)
#endif
#endif
#if defined(__AVX2__)
#define AI_BASE_FLOAT16_NTT_TRIG_K_MAX					(18)
#else
#define AI_BASE_FLOAT16_NTT_TRIG_K_MAX					(19)
#endif
#endif

#define AI_BASE_FLOAT16_TAB_LENGTH_MAX					(1024)

/*------------------------- Type Definition----------------------------------*/
/* +/-zero is represented with expn = BF_EXP_ZERO and len = 0,
   +/-infinity is represented with expn = BF_EXP_INF and len = 0,
   NaN is represented with expn = BF_EXP_NAN and len = 0 (sign is ignored)
 */
typedef struct ai_base_float16_bf_t {
    AI_BASE_FLOAT16_LIMB_T sign;
    AI_BASE_FLOAT16_SLIMB_T expn;
    AI_BASE_FLOAT16_LIMB_T len;
    AI_BASE_FLOAT16_LIMB_T tab[AI_BASE_FLOAT16_TAB_LENGTH_MAX];
} AI_BASE_FLOAT16_BF_T, *AI_BASE_FLOAT16_BF_T_PTR;


typedef enum {
    AI_BASE_FLOAT16_BF_RNDN,				/* round to nearest, ties to even */
    AI_BASE_FLOAT16_BF_RNDZ,				/* round to zero */
    AI_BASE_FLOAT16_BF_RNDD,				/* round to -inf */
    AI_BASE_FLOAT16_BF_RNDU,				/* round to +inf */
    AI_BASE_FLOAT16_BF_RNDNA,				/* round to nearest, away from zero */
    AI_BASE_FLOAT16_BF_RNDF,				/* faithful rounding (nondeterministic, either RNDD or RNDU, inexact flag is always set)  */
} AI_BASE_FLOAT16_BF_RND_T;

/* contains the rounding mode and number of exponents bits */
typedef UINT32_T AI_BASE_FLOAT16_BF_FLAGS_T;

typedef INT32_T AI_BASE_FLOAT16_BF_OP2_FUNC_T(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);

typedef struct {
    AI_BASE_FLOAT16_BF_T val;
    AI_BASE_FLOAT16_LIMB_T prec;
} AI_BASE_FLOAT16_BF_CONST_CACHE;

/* AI_BASE_FLOAT16_ZIV_FUNC should compute the result 'r' with faithful rounding at
   precision 'prec'. For efficiency purposes, the final AiBaseFloat16BfRound()
   does not need to be done in the function. */
typedef INT32_T AI_BASE_FLOAT16_ZIV_FUNC(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec, void *opaque);

#ifdef AI_BASE_FLOAT16_FFT_MUL
#if defined(__AVX2__)
typedef FLOAT64_T NTTLimb;
typedef __m256d AI_BASE_FLOAT16_M256D;
typedef union {
    AI_BASE_FLOAT16_M256D v;
    FLOAT64_T d[4];
} AI_BASE_FLOAT16_VEC_UNION;
#else
typedef AI_BASE_FLOAT16_LIMB_T NTTLimb;
#endif
#endif

/*------------------------- Function Definition -----------------------------*/
INT32_T AiBaseFloat16BfGetExpBits(AI_BASE_FLOAT16_BF_FLAGS_T flags);
AI_BASE_FLOAT16_BF_FLAGS_T AiBaseFloat16BfSetExpBits(INT32_T n);
AI_BASE_FLOAT16_SLIMB_T AiBaseFloat16BfMax(AI_BASE_FLOAT16_SLIMB_T a, AI_BASE_FLOAT16_SLIMB_T b);
AI_BASE_FLOAT16_SLIMB_T AiBaseFloat16BfMin(AI_BASE_FLOAT16_SLIMB_T a, AI_BASE_FLOAT16_SLIMB_T b);
void *AiBaseFloat16BfRealloc(void *ptr, UINT32_T size);
void AiBaseFloat16BfNeg(AI_BASE_FLOAT16_BF_T *r);
INT32_T AiBaseFloat16BfIsFinite(const AI_BASE_FLOAT16_BF_T *a);
ERROR_FLOAT16_T AiBaseFloat16BfIsNan(const AI_BASE_FLOAT16_BF_T *a);
INT32_T AiBaseFloat16BfIsZero(const AI_BASE_FLOAT16_BF_T *a);
INT32_T AiBaseFloat16Clz(AI_BASE_FLOAT16_LIMB_T a);
INT32_T AiBaseFloat16Ctz(AI_BASE_FLOAT16_LIMB_T a);
INT32_T AiBaseFloat16CeilLog2(AI_BASE_FLOAT16_LIMB_T a);
#if 0
AI_BASE_FLOAT16_SLIMB_T AiBaseFloat16CeilDiv(AI_BASE_FLOAT16_SLIMB_T a, AI_BASE_FLOAT16_LIMB_T b);
AI_BASE_FLOAT16_SLIMB_T AiBaseFloat16FloorDiv(AI_BASE_FLOAT16_SLIMB_T a, AI_BASE_FLOAT16_LIMB_T b);
#endif
void *AiBaseFloat16BfMalloc(UINT32_T size);
void AiBaseFloat16BfInit(AI_BASE_FLOAT16_BF_T *r);
ERROR_FLOAT16_T AiBaseFloat16BfResize(AI_BASE_FLOAT16_BF_T *r, AI_BASE_FLOAT16_LIMB_T len);
void AiBaseFloat16BfSetUi(AI_BASE_FLOAT16_BF_T *r, UINT64_T a);
void AiBaseFloat16BfSetSi(AI_BASE_FLOAT16_BF_T *r, INT64_T a);
void AiBaseFloat16BfSetNan(AI_BASE_FLOAT16_BF_T *r);
void AiBaseFloat16BfSetZero(AI_BASE_FLOAT16_BF_T *r, AI_BASE_FLOAT16_LIMB_T is_neg);
void AiBaseFloat16BfSetInf(AI_BASE_FLOAT16_BF_T *r, AI_BASE_FLOAT16_LIMB_T is_neg);
void AiBaseFloat16BfSet(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a);
void AiBaseFloat16BfMove(AI_BASE_FLOAT16_BF_T *r, AI_BASE_FLOAT16_BF_T *a);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16GetLimbz(const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T idx);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16GetBits(const AI_BASE_FLOAT16_LIMB_T *tab, AI_BASE_FLOAT16_LIMB_T len, AI_BASE_FLOAT16_SLIMB_T pos);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16GetBit(const AI_BASE_FLOAT16_LIMB_T *tab, AI_BASE_FLOAT16_LIMB_T len, AI_BASE_FLOAT16_SLIMB_T pos);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16LimbMask(INT32_T start, INT32_T last);
 AI_BASE_FLOAT16_LIMB_T AiBaseFloat16ScanBitNz(const AI_BASE_FLOAT16_BF_T *r, AI_BASE_FLOAT16_SLIMB_T bit_pos);
INT32_T AiBaseFloat16BfGetRndAdd(INT32_T *pret, const AI_BASE_FLOAT16_BF_T *r, AI_BASE_FLOAT16_LIMB_T l, AI_BASE_FLOAT16_SLIMB_T prec, INT32_T rnd_mode);
INT32_T AiBaseFloat16BfSetOverFlow(AI_BASE_FLOAT16_BF_T *r, INT32_T sign, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T __AiBaseFloat16BfRound(AI_BASE_FLOAT16_BF_T *r, AI_BASE_FLOAT16_LIMB_T prec1, AI_BASE_FLOAT16_BF_FLAGS_T flags, AI_BASE_FLOAT16_LIMB_T l);
INT32_T AiBaseFloat16BfNormalizeAndRound(AI_BASE_FLOAT16_BF_T *r, AI_BASE_FLOAT16_LIMB_T prec1, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfCanRound(const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_SLIMB_T prec, AI_BASE_FLOAT16_BF_RND_T rnd_mode, AI_BASE_FLOAT16_SLIMB_T k);
INT32_T AiBaseFloat16BfRound(AI_BASE_FLOAT16_BF_T *r, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
void AiBaseFloat16DumpLimbs(const UINT8_T *str, const AI_BASE_FLOAT16_LIMB_T *tab, AI_BASE_FLOAT16_LIMB_T n);
void AiBaseFloat16BfPrintStr(const UINT8_T *str, const AI_BASE_FLOAT16_BF_T *a);
INT32_T AiBaseFloat16BfCmpu(const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b);
INT32_T AiBaseFloat16BfCmpFull(const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b);
INT32_T AiBaseFloat16BfCmp(const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b, INT32_T op);
INT32_T AiBaseFloat16BfCmpEq(const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b);
INT32_T AiBaseFloat16BfCmpLe(const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b);
INT32_T AiBaseFloat16BfCmpLt(const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16CountCancelledBits(const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b);
INT32_T AiBaseFloat16BfAddInternal(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags, INT32_T b_neg);
INT32_T __AiBaseFloat16BfAdd(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T __AiBaseFloat16BfSub(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16MpAdd(AI_BASE_FLOAT16_LIMB_T *res, const AI_BASE_FLOAT16_LIMB_T *op1, const AI_BASE_FLOAT16_LIMB_T *op2, AI_BASE_FLOAT16_LIMB_T n, AI_BASE_FLOAT16_LIMB_T carry);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16MpAddMul1(AI_BASE_FLOAT16_LIMB_T *tabr, const AI_BASE_FLOAT16_LIMB_T *taba, AI_BASE_FLOAT16_LIMB_T n, AI_BASE_FLOAT16_LIMB_T b);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16MpSubMul1(AI_BASE_FLOAT16_LIMB_T *tabr, const AI_BASE_FLOAT16_LIMB_T *taba, AI_BASE_FLOAT16_LIMB_T n, AI_BASE_FLOAT16_LIMB_T b);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16Udiv1normInit(AI_BASE_FLOAT16_LIMB_T d);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16Udiv1norm(AI_BASE_FLOAT16_LIMB_T *pr, AI_BASE_FLOAT16_LIMB_T a1, AI_BASE_FLOAT16_LIMB_T a0, AI_BASE_FLOAT16_LIMB_T d, AI_BASE_FLOAT16_LIMB_T d_inv);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16MpDiv1norm(AI_BASE_FLOAT16_LIMB_T *tabr, const AI_BASE_FLOAT16_LIMB_T *taba, AI_BASE_FLOAT16_LIMB_T n, AI_BASE_FLOAT16_LIMB_T b, AI_BASE_FLOAT16_LIMB_T r);
void AiBaseFloat16MpDivnorm(AI_BASE_FLOAT16_LIMB_T *tabq, AI_BASE_FLOAT16_LIMB_T *taba, AI_BASE_FLOAT16_LIMB_T na, const AI_BASE_FLOAT16_LIMB_T *tabb, AI_BASE_FLOAT16_LIMB_T nb);
INT32_T AiBaseFloat16BfMul(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfMul2Exp(AI_BASE_FLOAT16_BF_T *r, AI_BASE_FLOAT16_SLIMB_T e, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
void AiBaseFloat16BfRecipRec(AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *x, AI_BASE_FLOAT16_LIMB_T prec1);
void AiBaseFloat16BfRecip(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec);
void AiBaseFloat16BfAddZeroLimbs(AI_BASE_FLOAT16_BF_T *r, AI_BASE_FLOAT16_LIMB_T precl);
void AiBaseFloat16BfOrOne(AI_BASE_FLOAT16_BF_T *r, AI_BASE_FLOAT16_LIMB_T precl);
AI_BASE_FLOAT16_SLIMB_T AiBaseFloat16BfGetExpMin(const AI_BASE_FLOAT16_BF_T *a);
void AiBaseFloat16BfTdivremu(AI_BASE_FLOAT16_BF_T *q, AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b);
INT32_T __AiBaseFloat16BfDiv(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfDivrem(AI_BASE_FLOAT16_BF_T *q, AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags, INT32_T rnd_mode);
INT32_T AiBaseFloat16BfFmod(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfRemainder(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
 INT32_T AiBaseFloat16BfGetLimb(AI_BASE_FLOAT16_SLIMB_T *pres, const AI_BASE_FLOAT16_BF_T *a, INT32_T flags);
INT32_T AiBaseFloat16BfRemquo(AI_BASE_FLOAT16_SLIMB_T *pq, AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
 AI_BASE_FLOAT16_LIMB_T AiBaseFloat16MulMod(AI_BASE_FLOAT16_LIMB_T a, AI_BASE_FLOAT16_LIMB_T b, AI_BASE_FLOAT16_LIMB_T m);
#if defined(AI_BASE_FLOAT16_USE_MUL_CHECK)
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16MpMod1(const AI_BASE_FLOAT16_LIMB_T *tab, AI_BASE_FLOAT16_LIMB_T n, AI_BASE_FLOAT16_LIMB_T m, AI_BASE_FLOAT16_LIMB_T r);
#endif
void __AiBaseFloat16BfRsqrt(AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *x, AI_BASE_FLOAT16_LIMB_T prec1);
INT32_T __AiBaseFloat16BfSqrt(AI_BASE_FLOAT16_BF_T *x, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec1, AI_BASE_FLOAT16_BF_FLAGS_T flags);
void AiBaseFloat16BfRsqrt(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16BfIsqrt(AI_BASE_FLOAT16_LIMB_T a);
INT32_T AiBaseFloat16BfSqrtrem(AI_BASE_FLOAT16_BF_T *r, AI_BASE_FLOAT16_BF_T *rem1, const AI_BASE_FLOAT16_BF_T *a);
INT32_T AiBaseFloat16BfSqrt(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfOp2(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags, AI_BASE_FLOAT16_BF_OP2_FUNC_T *func);
INT32_T AiBaseFloat16BfAdd(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfSub(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfDiv(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfMulUi(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, UINT64_T b1, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfMulSi(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, INT64_T b1, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfAddSi(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, INT64_T b1, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfPowUi(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T b, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfPowUiUi(AI_BASE_FLOAT16_BF_T *r, AI_BASE_FLOAT16_LIMB_T a1, AI_BASE_FLOAT16_LIMB_T b, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfRint(AI_BASE_FLOAT16_BF_T *r, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16BfLogicOp1(AI_BASE_FLOAT16_LIMB_T a, AI_BASE_FLOAT16_LIMB_T b, INT32_T op);
void AiBaseFloat16BfLogicOp(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a1, const AI_BASE_FLOAT16_BF_T *b1, INT32_T op);
void AiBaseFloat16BfLogicOr(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b);
void AiBaseFloat16BfLogicXor(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b);
void AiBaseFloat16BfLogicAnd(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, const AI_BASE_FLOAT16_BF_T *b);
INT32_T AiBaseFloat16BfGetInt32(INT32_T *pres, const AI_BASE_FLOAT16_BF_T *a, INT32_T flags);
INT32_T AiBaseFloat16BfGetInt16(INT16_T *pres, const AI_BASE_FLOAT16_BF_T *a, INT32_T flags);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16GetLimbRadix(INT32_T radix);
void AiBaseFloat16BfIntegerFromRadixRec(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_LIMB_T *tab, AI_BASE_FLOAT16_LIMB_T n, INT32_T level, AI_BASE_FLOAT16_LIMB_T n0, AI_BASE_FLOAT16_LIMB_T radix, AI_BASE_FLOAT16_BF_T *pow_tab);
void AiBaseFloat16BfIntegerFromRadix(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_LIMB_T *tab, AI_BASE_FLOAT16_LIMB_T n, AI_BASE_FLOAT16_LIMB_T radix);
INT32_T AiBaseFloat16BfConvertFromRadix(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *T, AI_BASE_FLOAT16_LIMB_T radix, AI_BASE_FLOAT16_SLIMB_T expn, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16ToDigit(INT32_T c);
void AiBaseFloat16BfAddLimb(AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_SLIMB_T *ppos, AI_BASE_FLOAT16_LIMB_T v);
INT32_T AiBaseFloat16BfTolower(INT32_T c);
INT32_T AiBaseFloat16Strcasestart(const UINT8_T *str, const UINT8_T *val, const UINT8_T **ptr);
INT32_T AiBaseFloat16BfAtof2(AI_BASE_FLOAT16_BF_T *r, AI_BASE_FLOAT16_SLIMB_T *pexponent, const UINT8_T *str, const UINT8_T **pnext, INT32_T radix, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfAtof(AI_BASE_FLOAT16_BF_T *r, const UINT8_T *str, const UINT8_T **pnext, INT32_T radix, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
AI_BASE_FLOAT16_SLIMB_T AiBaseFloat16BfMulLog2Radix(AI_BASE_FLOAT16_SLIMB_T a1, UINT32_T radix, INT32_T is_inv, INT32_T is_ceil1);
void AiBaseFloat16BfIntegerToRadixRec(AI_BASE_FLOAT16_BF_T *pow_tab, AI_BASE_FLOAT16_LIMB_T *out, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T n, INT32_T level, AI_BASE_FLOAT16_LIMB_T n0, AI_BASE_FLOAT16_LIMB_T radixl, UINT32_T radixl_bits);
void AiBaseFloat16BfIntegerToRadix(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T radixl);
void AiBaseFloat16BfConvertToRadix(AI_BASE_FLOAT16_BF_T *r, AI_BASE_FLOAT16_SLIMB_T *pE, const AI_BASE_FLOAT16_BF_T *a, INT32_T radix, AI_BASE_FLOAT16_LIMB_T P, AI_BASE_FLOAT16_BF_RND_T rnd_mode, BOOL_T is_fixed_exponent);
void AiBaseFloat16LimbToA(UINT8_T *buf, AI_BASE_FLOAT16_LIMB_T n, UINT32_T radix, INT32_T len);
void AiBaseFloat16LimbToA2(UINT8_T *buf, AI_BASE_FLOAT16_LIMB_T n, UINT32_T radix_bits, INT32_T len);
//void AiBaseFloat16OutputDigits(AI_BASE_FLOAT16_DYN_BUF *s, const AI_BASE_FLOAT16_BF_T *a1, INT32_T radix, AI_BASE_FLOAT16_LIMB_T n_digits, AI_BASE_FLOAT16_LIMB_T dot_pos);
void *AibaseFloat16BfDbufRealloc(void *opaque, void *ptr, UINT32_T size);
UINT32_T AiBaseFloat16BfFtoa(UINT8_T **pbuf, const AI_BASE_FLOAT16_BF_T *a2, INT32_T radix, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
void AiBaseFloat16BfConstLog2Rec(AI_BASE_FLOAT16_BF_T *T, AI_BASE_FLOAT16_BF_T *P, AI_BASE_FLOAT16_BF_T *Q, AI_BASE_FLOAT16_LIMB_T n1, AI_BASE_FLOAT16_LIMB_T n2, BOOL_T need_P);
void AiBaseFloat16BfConstLog2Internal(AI_BASE_FLOAT16_BF_T *T, AI_BASE_FLOAT16_LIMB_T prec);
void AiBaseFloat16ChudBs(AI_BASE_FLOAT16_BF_T *P, AI_BASE_FLOAT16_BF_T *Q, AI_BASE_FLOAT16_BF_T *G, INT64_T a, INT64_T b, INT32_T need_g, AI_BASE_FLOAT16_LIMB_T prec);
void AiBaseFloat16BfConstPiInternal(AI_BASE_FLOAT16_BF_T *Q, AI_BASE_FLOAT16_LIMB_T prec);
INT32_T AiBaseFloat16BfConstGet(AI_BASE_FLOAT16_BF_T *T, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags, AI_BASE_FLOAT16_BF_CONST_CACHE *c, void (*func)(AI_BASE_FLOAT16_BF_T *res, AI_BASE_FLOAT16_LIMB_T prec));
INT32_T AiBaseFloat16BfConstLog2(AI_BASE_FLOAT16_BF_T *T, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfConstPi(AI_BASE_FLOAT16_BF_T *T, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfZivRounding(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags, AI_BASE_FLOAT16_ZIV_FUNC *f, void *opaque);
INT32_T AiBaseFloat16BfExpInternal(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat16BfExp(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfLogInternal(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat16BfLog(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfPowGeneric(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *x, AI_BASE_FLOAT16_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat16BfPowInt(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *x, AI_BASE_FLOAT16_LIMB_T prec, void *opaque);
BOOL_T AiBaseFloat16CheckExactPower2n(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *x, AI_BASE_FLOAT16_SLIMB_T n);
INT32_T AiBaseFloat16BfPow(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *x, const AI_BASE_FLOAT16_BF_T *y, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
void AiBaseFloat16BfSqrtSin(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *x, AI_BASE_FLOAT16_LIMB_T prec1);
INT32_T AiBaseFloat16BfSincos(AI_BASE_FLOAT16_BF_T *s, AI_BASE_FLOAT16_BF_T *c, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec);
INT32_T AiBaseFloat16BfCosInternal(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat16BfCos(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfSinInternal(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat16BfSin(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfTanInternal(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat16BfTan(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfAtanInternal(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat16BfAtan(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfAtan2Internal(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *y, AI_BASE_FLOAT16_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat16BfAtan2(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *y, const AI_BASE_FLOAT16_BF_T *x, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfAsinInternal(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat16BfAsin(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
INT32_T AiBaseFloat16BfAcos(AI_BASE_FLOAT16_BF_T *r, const AI_BASE_FLOAT16_BF_T *a, AI_BASE_FLOAT16_LIMB_T prec, AI_BASE_FLOAT16_BF_FLAGS_T flags);
#ifdef AI_BASE_FLOAT16_FFT_MUL
void AiBaseFloat16PutBits(AI_BASE_FLOAT16_LIMB_T *tab, AI_BASE_FLOAT16_LIMB_T len, AI_BASE_FLOAT16_SLIMB_T pos, AI_BASE_FLOAT16_LIMB_T val);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16AddMod(AI_BASE_FLOAT16_LIMB_T a, AI_BASE_FLOAT16_LIMB_T b, AI_BASE_FLOAT16_LIMB_T m);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16SubMod(AI_BASE_FLOAT16_LIMB_T a, AI_BASE_FLOAT16_LIMB_T b, AI_BASE_FLOAT16_LIMB_T m);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16ModFast(AI_BASE_FLOAT16_DLIMB_T r, AI_BASE_FLOAT16_LIMB_T m, AI_BASE_FLOAT16_LIMB_T m_inv);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16MulModFast(AI_BASE_FLOAT16_LIMB_T a, AI_BASE_FLOAT16_LIMB_T b, AI_BASE_FLOAT16_LIMB_T m, AI_BASE_FLOAT16_LIMB_T m_inv)
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16InitMulModFast(AI_BASE_FLOAT16_LIMB_T m);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16MulModFast2(AI_BASE_FLOAT16_LIMB_T a, AI_BASE_FLOAT16_LIMB_T b, AI_BASE_FLOAT16_LIMB_T m, AI_BASE_FLOAT16_LIMB_T b_inv);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16MulModFast3(AI_BASE_FLOAT16_LIMB_T a, AI_BASE_FLOAT16_LIMB_T b, AI_BASE_FLOAT16_LIMB_T m, AI_BASE_FLOAT16_LIMB_T b_inv);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16InitMulModFast2(AI_BASE_FLOAT16_LIMB_T b, AI_BASE_FLOAT16_LIMB_T m);
#ifdef __AVX2__
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16NttLimbToInt(NTTLimb a, AI_BASE_FLOAT16_LIMB_T m);
NTTLimb AiBaseFloat16IntToNttLimb(AI_BASE_FLOAT16_LIMB_T a, AI_BASE_FLOAT16_LIMB_T m);
NTTLimb AiBaseFloat16IntToNttLimb2(AI_BASE_FLOAT16_LIMB_T a, AI_BASE_FLOAT16_LIMB_T m);
AI_BASE_FLOAT16_M256D AiBaseFloat16NttMod1(AI_BASE_FLOAT16_M256D r, AI_BASE_FLOAT16_M256D m);
AI_BASE_FLOAT16_M256D AiBaseFloat16NttMod(AI_BASE_FLOAT16_M256D r, AI_BASE_FLOAT16_M256D mf, AI_BASE_FLOAT16_M256D m2f);
AI_BASE_FLOAT16_M256D AiBaseFloat16NttMulMod(AI_BASE_FLOAT16_M256D a, AI_BASE_FLOAT16_M256D b, AI_BASE_FLOAT16_M256D mf, AI_BASE_FLOAT16_M256D m_inv);
void *AiBaseFloat16BfAlignedMalloc(UINT32_T size, UINT32_T align);
void AiBaseFloat16BfAlignedFree(void *ptr);
void *AiBaseFloat16NttMalloc(UINT32_T size);
void AiBaseFloat16NttFree(void *ptr);
void AiBaseFloat16NttFft(NTTLimb *out_buf, NTTLimb *in_buf, NTTLimb *tmp_buf, INT32_T fft_len_log2, INT32_T inverse, INT32_T m_idx);
void AiBaseFloat16NttVecMul(NTTLimb *tab1, NTTLimb *tab2, AI_BASE_FLOAT16_LIMB_T fft_len_log2, INT32_T k_tot, INT32_T m_idx);
void AiBaseFloat16MulTrig(NTTLimb *buf, AI_BASE_FLOAT16_LIMB_T n, AI_BASE_FLOAT16_LIMB_T c1, AI_BASE_FLOAT16_LIMB_T m, AI_BASE_FLOAT16_LIMB_T m_inv1);
#else
void *AiBaseFloat16NttMalloc(UINT32_T size);
void AiBaseFloat16NttFree(void *ptr);
AI_BASE_FLOAT16_LIMB_T AiBaseFloat16NttLimbToInt(NTTLimb a, AI_BASE_FLOAT16_LIMB_T m);
NTTLimb AiBaseFloat16IntToNttLimb(AI_BASE_FLOAT16_SLIMB_T a, AI_BASE_FLOAT16_LIMB_T m);
void AiBaseFloat16NttFft(NTTLimb *out_buf, NTTLimb *in_buf, NTTLimb *tmp_buf, INT32_T fft_len_log2, INT32_T inverse, INT32_T m_idx);
void AiBaseFloat16NttVecMul(NTTLimb *tab1, NTTLimb *tab2, INT32_T fft_len_log2, INT32_T k_tot, INT32_T m_idx);
void AiBaseFloat16MulTrig(NTTLimb *buf, AI_BASE_FLOAT16_LIMB_T n, AI_BASE_FLOAT16_LIMB_T c_mul, AI_BASE_FLOAT16_LIMB_T m, AI_BASE_FLOAT16_LIMB_T m_inv);
#endif /* !AVX2 */
NTTLimb *AiBaseFloat16GetTrig(INT32_T k, INT32_T inverse1, INT32_T m_idx1);
void AiBaseFloat16NttFftPartial(NTTLimb *buf1, INT32_T k1, INT32_T k2, AI_BASE_FLOAT16_LIMB_T n1, AI_BASE_FLOAT16_LIMB_T n2, INT32_T inverse, AI_BASE_FLOAT16_LIMB_T m_idx);
void AiBaseFloat16NttConv(NTTLimb *buf1, NTTLimb *buf2, INT32_T k, INT32_T k_tot, AI_BASE_FLOAT16_LIMB_T m_idx);
void AiBaseFloat16LimbToNtt(NTTLimb *tabr, AI_BASE_FLOAT16_LIMB_T fft_len,  const AI_BASE_FLOAT16_LIMB_T *taba, AI_BASE_FLOAT16_LIMB_T a_len, INT32_T dpl, INT32_T first_m_idx, INT32_T nb_mods);
#if defined(__AVX2__)
void AiBaseFloat16NttToLimb(AI_BASE_FLOAT16_LIMB_T *tabr, AI_BASE_FLOAT16_LIMB_T r_len,  const NTTLimb *buf, INT32_T fft_len_log2, INT32_T dpl, INT32_T nb_mods);
#else
void AiBaseFloat16NttToLimb(AI_BASE_FLOAT16_LIMB_T *tabr, AI_BASE_FLOAT16_LIMB_T r_len, const NTTLimb *buf, INT32_T fft_len_log2, INT32_T dpl, INT32_T nb_mods);
#endif
void AiBaseFloat16NttStaticInit(void);
INT32_T AiBaseFloat16BfGetFftSize(INT32_T *pdpl, INT32_T *pnb_mods, AI_BASE_FLOAT16_LIMB_T len);
void AiBaseFloat16FftMul(AI_BASE_FLOAT16_BF_T *res, AI_BASE_FLOAT16_LIMB_T *a_tab, AI_BASE_FLOAT16_LIMB_T a_len, AI_BASE_FLOAT16_LIMB_T *b_tab, AI_BASE_FLOAT16_LIMB_T b_len, INT32_T mul_flags);
#else /* AI_BASE_FLOAT16_FFT_MUL */
INT32_T AiBaseFloat16BfGetFftSize(INT32_T *pdpl, INT32_T *pnb_mods, AI_BASE_FLOAT16_LIMB_T len);
#endif /* !AI_BASE_FLOAT16_FFT_MUL */

/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/



