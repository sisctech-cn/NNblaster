/**
 ******************************************************************************
 * file    ai_base_float64_bf.h
 * author  sunlingge
 * version V100
 * brief   the ai base float 64 bf driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT64_BF_H
#define AI_BASE_FLOAT64_BF_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP64 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT64_LIMB_LOG2_BITS				(6)
#define AI_BASE_FLOAT64_LIMB_BITS					(1 << AI_BASE_FLOAT64_LIMB_LOG2_BITS)

/*------------------------- Pre Type Definition -----------------------------*/
typedef INT64_T AI_BASE_FLOAT64_SLIMB_T, *AI_BASE_FLOAT64_SLIMB_T_PTR;
typedef UINT64_T AI_BASE_FLOAT64_LIMB_T, *AI_BASE_FLOAT64_LIMB_T_PTR;
typedef UINT64_T AI_BASE_FLOAT64_DLIMB_T, *AI_BASE_FLOAT64_DLIMB_T_PTR;

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT64_INT32_MAX					(0x7FFFFFFFL)
#define AI_BASE_FLOAT64_INT32_MIN					(-AI_BASE_FLOAT64_INT32_MAX - 1L)	
#define AI_BASE_FLOAT64_EXP_MIN						(AI_BASE_FLOAT64_INT32_MIN)
#define AI_BASE_FLOAT64_EXP_MAX						(AI_BASE_FLOAT64_INT32_MAX)

/* in bits */
#define AI_BASE_FLOAT64_BF_EXP_BITS_MIN				(3)
#define AI_BASE_FLOAT64_BF_EXP_BITS_MAX				(AI_BASE_FLOAT64_LIMB_BITS - 2)

#define AI_BASE_FLOAT64_BF_PREC_MIN					(2)
#define AI_BASE_FLOAT64_BF_PREC_MAX					(((AI_BASE_FLOAT64_LIMB_T)1 << AI_BASE_FLOAT64_BF_EXP_BITS_MAX) - 2)
/* infinite precision */
#define AI_BASE_FLOAT64_BF_PREC_INF					(AI_BASE_FLOAT64_BF_PREC_MAX + 1)	

#define AI_BASE_FLOAT64_BF_CHKSUM_MOD				(975620677U)

#define AI_BASE_FLOAT64_BF_EXP_ZERO					(AI_BASE_FLOAT64_EXP_MIN)
#define AI_BASE_FLOAT64_BF_EXP_INF					(AI_BASE_FLOAT64_EXP_MAX - 1)
#define AI_BASE_FLOAT64_BF_EXP_NAN					(AI_BASE_FLOAT64_EXP_MAX)

/* maximum radix for AiBaseFloat64Bfatof() and AiBaseFloat64Bfftoa() */
#define AI_BASE_FLOAT64_BF_RADIX_MAX				(36) 

#ifdef AI_BASE_FLOAT64_FFT_MUL
#define AI_BASE_FLOAT64_STRIP_LEN					(16)
#if defined(__AVX2__)
/* we must have: modulo >= 1 << NTT_MOD_LOG2_MIN */
#define AI_BASE_FLOAT64_NTT_MOD_LOG2_MIN			(50)
#define AI_BASE_FLOAT64_NTT_MOD_LOG2_MAX			(51)
#define AI_BASE_FLOAT64_NB_MODS					(5)
#define AI_BASE_FLOAT64_NTT_PROOT_2EXP			(39)
#define AI_BASE_FLOAT64_VEC_LEN					(4)
#else
#if AI_BASE_FLOAT64_LIMB_BITS == 64
#define AI_BASE_FLOAT64_NTT_MOD_LOG2_MIN			(61)
#define AI_BASE_FLOAT64_NTT_MOD_LOG2_MAX			(62)
#define AI_BASE_FLOAT64_NB_MODS					(5)
#define AI_BASE_FLOAT64_NTT_PROOT_2EXP			(51)
#endif
#if AI_BASE_FLOAT64_LIMB_BITS == 32
/* we must have: modulo >= 1 << NTT_MOD_LOG2_MIN */
#define AI_BASE_FLOAT64_NTT_MOD_LOG2_MIN			(29)
#define AI_BASE_FLOAT64_NTT_MOD_LOG2_MAX			(30)
#define AI_BASE_FLOAT64_NB_MODS					(5)
#define AI_BASE_FLOAT64_NTT_PROOT_2EXP			(20)
#endif
#endif
#if defined(__AVX2__)
#define AI_BASE_FLOAT64_NTT_TRIG_K_MAX			(18)
#else
#define AI_BASE_FLOAT64_NTT_TRIG_K_MAX			(19)
#endif
#endif

/*------------------------- Type Definition----------------------------------*/
/* +/-zero is represented with expn = BF_EXP_ZERO and len = 0,
   +/-infinity is represented with expn = BF_EXP_INF and len = 0,
   NaN is represented with expn = BF_EXP_NAN and len = 0 (sign is ignored)
 */
typedef struct ai_base_float64_bf_t {
    AI_BASE_FLOAT64_LIMB_T sign;
    AI_BASE_FLOAT64_SLIMB_T expn;
    AI_BASE_FLOAT64_LIMB_T len;
    AI_BASE_FLOAT64_LIMB_T tab[4];
} AI_BASE_FLOAT64_BF_T, *AI_BASE_FLOAT64_BF_T_PTR;


typedef enum {
    AI_BASE_FLOAT64_BF_RNDN,				/* round to nearest, ties to even */
    AI_BASE_FLOAT64_BF_RNDZ,				/* round to zero */
    AI_BASE_FLOAT64_BF_RNDD,				/* round to -inf */
    AI_BASE_FLOAT64_BF_RNDU,				/* round to +inf */
    AI_BASE_FLOAT64_BF_RNDNA,				/* round to nearest, away from zero */
    AI_BASE_FLOAT64_BF_RNDF,				/* faithful rounding (nondeterministic, either RNDD or RNDU, inexact flag is always set)  */
} AI_BASE_FLOAT64_BF_RND_T;

/* contains the rounding mode and number of exponents bits */
typedef UINT32_T AI_BASE_FLOAT64_BF_FLAGS_T;

typedef INT32_T AI_BASE_FLOAT64_BF_OP2_FUNC_T(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);

typedef struct {
    AI_BASE_FLOAT64_BF_T val;
    AI_BASE_FLOAT64_LIMB_T prec;
} AI_BASE_FLOAT64_BF_CONST_CACHE;

/* AI_BASE_FLOAT64_ZIV_FUNC should compute the result 'r' with faithful rounding at
   precision 'prec'. For efficiency purposes, the final AiBaseFloat64BfRound()
   does not need to be done in the function. */
typedef INT32_T AI_BASE_FLOAT64_ZIV_FUNC(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, void *opaque);

#ifdef AI_BASE_FLOAT64_FFT_MUL
#if defined(__AVX2__)
typedef FLOAT64_T NTTLimb;
typedef __m256d AI_BASE_FLOAT64_M256D;
typedef union {
    AI_BASE_FLOAT64_M256D v;
    FLOAT64_T d[4];
} AI_BASE_FLOAT64_VEC_UNION;
#else
typedef AI_BASE_FLOAT64_LIMB_T NTTLimb;
#endif
#endif

/*------------------------- Function Definition -----------------------------*/
INT32_T AiBaseFloat64BfGetExpBits(AI_BASE_FLOAT64_BF_FLAGS_T flags);
AI_BASE_FLOAT64_BF_FLAGS_T AiBaseFloat64BfSetExpBits(INT32_T n);
AI_BASE_FLOAT64_SLIMB_T AiBaseFloat64BfMax(AI_BASE_FLOAT64_SLIMB_T a, AI_BASE_FLOAT64_SLIMB_T b);
AI_BASE_FLOAT64_SLIMB_T AiBaseFloat64BfMin(AI_BASE_FLOAT64_SLIMB_T a, AI_BASE_FLOAT64_SLIMB_T b);
void *AiBaseFloat64BfRealloc(void *ptr, UINT32_T size);
void AiBaseFloat64BfDelete(AI_BASE_FLOAT64_BF_T *r);
void AiBaseFloat64BfNeg(AI_BASE_FLOAT64_BF_T *r);
INT32_T AiBaseFloat64BfIsFinite(const AI_BASE_FLOAT64_BF_T *a);
ERROR_FLOAT64_T AiBaseFloat64BfIsNan(const AI_BASE_FLOAT64_BF_T *a);
INT32_T AiBaseFloat64BfIsZero(const AI_BASE_FLOAT64_BF_T *a);
INT32_T AiBaseFloat64Clz(AI_BASE_FLOAT64_LIMB_T a);
INT32_T AiBaseFloat64Ctz(AI_BASE_FLOAT64_LIMB_T a);
INT32_T AiBaseFloat64CeilLog2(AI_BASE_FLOAT64_LIMB_T a);
#if 0
AI_BASE_FLOAT64_SLIMB_T AiBaseFloat64CeilDiv(AI_BASE_FLOAT64_SLIMB_T a, AI_BASE_FLOAT64_LIMB_T b);
AI_BASE_FLOAT64_SLIMB_T AiBaseFloat64FloorDiv(AI_BASE_FLOAT64_SLIMB_T a, AI_BASE_FLOAT64_LIMB_T b);
#endif
void *AiBaseFloat64BfMalloc(UINT32_T size);
void AiBaseFloat64BfFree(void *ptr);
void AiBaseFloat64BfInit(AI_BASE_FLOAT64_BF_T *r);
ERROR_FLOAT64_T AiBaseFloat64BfResize(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T len);
void AiBaseFloat64BfSetUi(AI_BASE_FLOAT64_BF_T *r, UINT64_T a);
void AiBaseFloat64BfSetSi(AI_BASE_FLOAT64_BF_T *r, INT64_T a);
void AiBaseFloat64BfSetNan(AI_BASE_FLOAT64_BF_T *r);
void AiBaseFloat64BfSetZero(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T is_neg);
void AiBaseFloat64BfSetInf(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T is_neg);
void AiBaseFloat64BfSet(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a);
void AiBaseFloat64BfMove(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_BF_T *a);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64GetLimbz(const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T idx);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64GetBits(const AI_BASE_FLOAT64_LIMB_T *tab, AI_BASE_FLOAT64_LIMB_T len, AI_BASE_FLOAT64_SLIMB_T pos);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64GetBit(const AI_BASE_FLOAT64_LIMB_T *tab, AI_BASE_FLOAT64_LIMB_T len, AI_BASE_FLOAT64_SLIMB_T pos);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64LimbMask(INT32_T start, INT32_T last);
 AI_BASE_FLOAT64_LIMB_T AiBaseFloat64ScanBitNz(const AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_SLIMB_T bit_pos);
INT32_T AiBaseFloat64BfGetRndAdd(INT32_T *pret, const AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T l, AI_BASE_FLOAT64_SLIMB_T prec, INT32_T rnd_mode);
INT32_T AiBaseFloat64BfSetOverFlow(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T sign, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T __AiBaseFloat64BfRound(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T prec1, AI_BASE_FLOAT64_BF_FLAGS_T flags, AI_BASE_FLOAT64_LIMB_T l);
INT32_T AiBaseFloat64BfNormalizeAndRound(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T prec1, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfCanRound(const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_SLIMB_T prec, AI_BASE_FLOAT64_BF_RND_T rnd_mode, AI_BASE_FLOAT64_SLIMB_T k);
INT32_T AiBaseFloat64BfRound(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
void AiBaseFloat64DumpLimbs(const UINT8_T *str, const AI_BASE_FLOAT64_LIMB_T *tab, AI_BASE_FLOAT64_LIMB_T n);
void AiBaseFloat64BfPrintStr(const UINT8_T *str, const AI_BASE_FLOAT64_BF_T *a);
INT32_T AiBaseFloat64BfCmpu(const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b);
INT32_T AiBaseFloat64BfCmpFull(const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b);
INT32_T AiBaseFloat64BfCmp(const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, INT32_T op);
INT32_T AiBaseFloat64BfCmpEq(const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b);
INT32_T AiBaseFloat64BfCmpLe(const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b);
INT32_T AiBaseFloat64BfCmpLt(const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64CountCancelledBits(const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b);
INT32_T AiBaseFloat64BfAddInternal(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags, INT32_T b_neg);
INT32_T __AiBaseFloat64BfAdd(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T __AiBaseFloat64BfSub(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64MpAdd(AI_BASE_FLOAT64_LIMB_T *res, const AI_BASE_FLOAT64_LIMB_T *op1, const AI_BASE_FLOAT64_LIMB_T *op2, AI_BASE_FLOAT64_LIMB_T n, AI_BASE_FLOAT64_LIMB_T carry);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64MpAddMul1(AI_BASE_FLOAT64_LIMB_T *tabr, const AI_BASE_FLOAT64_LIMB_T *taba, AI_BASE_FLOAT64_LIMB_T n, AI_BASE_FLOAT64_LIMB_T b);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64MpSubMul1(AI_BASE_FLOAT64_LIMB_T *tabr, const AI_BASE_FLOAT64_LIMB_T *taba, AI_BASE_FLOAT64_LIMB_T n, AI_BASE_FLOAT64_LIMB_T b);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64Udiv1normInit(AI_BASE_FLOAT64_LIMB_T d);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64Udiv1norm(AI_BASE_FLOAT64_LIMB_T *pr, AI_BASE_FLOAT64_LIMB_T a1, AI_BASE_FLOAT64_LIMB_T a0, AI_BASE_FLOAT64_LIMB_T d, AI_BASE_FLOAT64_LIMB_T d_inv);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64MpDiv1norm(AI_BASE_FLOAT64_LIMB_T *tabr, const AI_BASE_FLOAT64_LIMB_T *taba, AI_BASE_FLOAT64_LIMB_T n, AI_BASE_FLOAT64_LIMB_T b, AI_BASE_FLOAT64_LIMB_T r);
void AiBaseFloat64MpDivnorm(AI_BASE_FLOAT64_LIMB_T *tabq, AI_BASE_FLOAT64_LIMB_T *taba, AI_BASE_FLOAT64_LIMB_T na, const AI_BASE_FLOAT64_LIMB_T *tabb, AI_BASE_FLOAT64_LIMB_T nb);
INT32_T AiBaseFloat64BfMul(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfMul2Exp(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_SLIMB_T e, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
void AiBaseFloat64BfRecipRec(AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *x, AI_BASE_FLOAT64_LIMB_T prec1);
void AiBaseFloat64BfRecip(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec);
void AiBaseFloat64BfAddZeroLimbs(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T precl);
void AiBaseFloat64BfOrOne(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T precl);
AI_BASE_FLOAT64_SLIMB_T AiBaseFloat64BfGetExpMin(const AI_BASE_FLOAT64_BF_T *a);
void AiBaseFloat64BfTdivremu(AI_BASE_FLOAT64_BF_T *q, AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b);
INT32_T __AiBaseFloat64BfDiv(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfDivrem(AI_BASE_FLOAT64_BF_T *q, AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags, INT32_T rnd_mode);
INT32_T AiBaseFloat64BfFmod(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfRemainder(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
 INT32_T AiBaseFloat64BfGetLimb(AI_BASE_FLOAT64_SLIMB_T *pres, const AI_BASE_FLOAT64_BF_T *a, INT32_T flags);
INT32_T AiBaseFloat64BfRemquo(AI_BASE_FLOAT64_SLIMB_T *pq, AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
 AI_BASE_FLOAT64_LIMB_T AiBaseFloat64MulMod(AI_BASE_FLOAT64_LIMB_T a, AI_BASE_FLOAT64_LIMB_T b, AI_BASE_FLOAT64_LIMB_T m);
#if defined(AI_BASE_FLOAT64_USE_MUL_CHECK)
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64MpMod1(const AI_BASE_FLOAT64_LIMB_T *tab, AI_BASE_FLOAT64_LIMB_T n, AI_BASE_FLOAT64_LIMB_T m, AI_BASE_FLOAT64_LIMB_T r);
#endif
void __AiBaseFloat64BfRsqrt(AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *x, AI_BASE_FLOAT64_LIMB_T prec1);
INT32_T __AiBaseFloat64BfSqrt(AI_BASE_FLOAT64_BF_T *x, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec1, AI_BASE_FLOAT64_BF_FLAGS_T flags);
void AiBaseFloat64BfRsqrt(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64BfIsqrt(AI_BASE_FLOAT64_LIMB_T a);
INT32_T AiBaseFloat64BfSqrtrem(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_BF_T *rem1, const AI_BASE_FLOAT64_BF_T *a);
INT32_T AiBaseFloat64BfSqrt(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfOp2(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags, AI_BASE_FLOAT64_BF_OP2_FUNC_T *func);
INT32_T AiBaseFloat64BfAdd(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfSub(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfDiv(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfMulUi(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, UINT64_T b1, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfMulSi(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, INT64_T b1, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfAddSi(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, INT64_T b1, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfPowUi(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T b, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfPowUiUi(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T a1, AI_BASE_FLOAT64_LIMB_T b, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfRint(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64BfLogicOp1(AI_BASE_FLOAT64_LIMB_T a, AI_BASE_FLOAT64_LIMB_T b, INT32_T op);
void AiBaseFloat64BfLogicOp(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a1, const AI_BASE_FLOAT64_BF_T *b1, INT32_T op);
void AiBaseFloat64BfLogicOr(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b);
void AiBaseFloat64BfLogicXor(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b);
void AiBaseFloat64BfLogicAnd(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, const AI_BASE_FLOAT64_BF_T *b);
INT32_T AiBaseFloat64BfGetInt32(INT32_T *pres, const AI_BASE_FLOAT64_BF_T *a, INT32_T flags);
INT32_T AiBaseFloat64BfGetInt16(INT16_T *pres, const AI_BASE_FLOAT64_BF_T *a, INT32_T flags);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64GetLimbRadix(INT32_T radix);
void AiBaseFloat64BfIntegerFromRadixRec(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_LIMB_T *tab, AI_BASE_FLOAT64_LIMB_T n, INT32_T level, AI_BASE_FLOAT64_LIMB_T n0, AI_BASE_FLOAT64_LIMB_T radix, AI_BASE_FLOAT64_BF_T *pow_tab);
void AiBaseFloat64BfIntegerFromRadix(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_LIMB_T *tab, AI_BASE_FLOAT64_LIMB_T n, AI_BASE_FLOAT64_LIMB_T radix);
INT32_T AiBaseFloat64BfConvertFromRadix(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *T, AI_BASE_FLOAT64_LIMB_T radix, AI_BASE_FLOAT64_SLIMB_T expn, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64ToDigit(INT32_T c);
void AiBaseFloat64BfAddLimb(AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_SLIMB_T *ppos, AI_BASE_FLOAT64_LIMB_T v);
INT32_T AiBaseFloat64BfTolower(INT32_T c);
INT32_T AiBaseFloat64Strcasestart(const UINT8_T *str, const UINT8_T *val, const UINT8_T **ptr);
INT32_T AiBaseFloat64BfAtof2(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_SLIMB_T *pexponent, const UINT8_T *str, const UINT8_T **pnext, INT32_T radix, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfAtof(AI_BASE_FLOAT64_BF_T *r, const UINT8_T *str, const UINT8_T **pnext, INT32_T radix, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
AI_BASE_FLOAT64_SLIMB_T AiBaseFloat64BfMulLog2Radix(AI_BASE_FLOAT64_SLIMB_T a1, UINT32_T radix, INT32_T is_inv, INT32_T is_ceil1);
void AiBaseFloat64BfIntegerToRadixRec(AI_BASE_FLOAT64_BF_T *pow_tab, AI_BASE_FLOAT64_LIMB_T *out, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T n, INT32_T level, AI_BASE_FLOAT64_LIMB_T n0, AI_BASE_FLOAT64_LIMB_T radixl, UINT32_T radixl_bits);
void AiBaseFloat64BfIntegerToRadix(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T radixl);
void AiBaseFloat64BfConvertToRadix(AI_BASE_FLOAT64_BF_T *r, AI_BASE_FLOAT64_SLIMB_T *pE, const AI_BASE_FLOAT64_BF_T *a, INT32_T radix, AI_BASE_FLOAT64_LIMB_T P, AI_BASE_FLOAT64_BF_RND_T rnd_mode, BOOL_T is_fixed_exponent);
void AiBaseFloat64LimbToA(UINT8_T *buf, AI_BASE_FLOAT64_LIMB_T n, UINT32_T radix, INT32_T len);
void AiBaseFloat64LimbToA2(UINT8_T *buf, AI_BASE_FLOAT64_LIMB_T n, UINT32_T radix_bits, INT32_T len);
//void AiBaseFloat64OutputDigits(AI_BASE_FLOAT64_DYN_BUF *s, const AI_BASE_FLOAT64_BF_T *a1, INT32_T radix, AI_BASE_FLOAT64_LIMB_T n_digits, AI_BASE_FLOAT64_LIMB_T dot_pos);
void *AiBaseFloat64BfDbufRealloc(void *opaque, void *ptr, UINT32_T size);
UINT32_T AiBaseFloat64BfFtoa(UINT8_T **pbuf, const AI_BASE_FLOAT64_BF_T *a2, INT32_T radix, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
void AiBaseFloat64BfConstLog2Rec(AI_BASE_FLOAT64_BF_T *T, AI_BASE_FLOAT64_BF_T *P, AI_BASE_FLOAT64_BF_T *Q, AI_BASE_FLOAT64_LIMB_T n1, AI_BASE_FLOAT64_LIMB_T n2, BOOL_T need_P);
void AiBaseFloat64BfConstLog2Internal(AI_BASE_FLOAT64_BF_T *T, AI_BASE_FLOAT64_LIMB_T prec);
void AiBaseFloat64ChudBs(AI_BASE_FLOAT64_BF_T *P, AI_BASE_FLOAT64_BF_T *Q, AI_BASE_FLOAT64_BF_T *G, INT64_T a, INT64_T b, INT32_T need_g, AI_BASE_FLOAT64_LIMB_T prec);
void AiBaseFloat64BfConstPiInternal(AI_BASE_FLOAT64_BF_T *Q, AI_BASE_FLOAT64_LIMB_T prec);
INT32_T AiBaseFloat64BfConstGet(AI_BASE_FLOAT64_BF_T *T, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags, AI_BASE_FLOAT64_BF_CONST_CACHE *c, void (*func)(AI_BASE_FLOAT64_BF_T *res, AI_BASE_FLOAT64_LIMB_T prec));
INT32_T AiBaseFloat64BfConstLog2(AI_BASE_FLOAT64_BF_T *T, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfConstPi(AI_BASE_FLOAT64_BF_T *T, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfZivRounding(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags, AI_BASE_FLOAT64_ZIV_FUNC *f, void *opaque);
INT32_T AiBaseFloat64BfExpInternal(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat64BfExp(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfLogInternal(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat64BfLog(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfPowGeneric(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *x, AI_BASE_FLOAT64_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat64BfPowInt(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *x, AI_BASE_FLOAT64_LIMB_T prec, void *opaque);
BOOL_T AiBaseFloat64CheckExactPower2n(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *x, AI_BASE_FLOAT64_SLIMB_T n);
INT32_T AiBaseFloat64BfPow(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *x, const AI_BASE_FLOAT64_BF_T *y, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
void AiBaseFloat64BfSqrtSin(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *x, AI_BASE_FLOAT64_LIMB_T prec1);
INT32_T AiBaseFloat64BfSincos(AI_BASE_FLOAT64_BF_T *s, AI_BASE_FLOAT64_BF_T *c, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec);
INT32_T AiBaseFloat64BfCosInternal(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat64BfCos(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfSinInternal(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat64BfSin(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfTanInternal(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat64BfTan(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfAtanInternal(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat64BfAtan(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfAtan2Internal(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *y, AI_BASE_FLOAT64_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat64BfAtan2(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *y, const AI_BASE_FLOAT64_BF_T *x, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfAsinInternal(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat64BfAsin(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
INT32_T AiBaseFloat64BfAcos(AI_BASE_FLOAT64_BF_T *r, const AI_BASE_FLOAT64_BF_T *a, AI_BASE_FLOAT64_LIMB_T prec, AI_BASE_FLOAT64_BF_FLAGS_T flags);
#ifdef AI_BASE_FLOAT64_FFT_MUL
void AiBaseFloat64PutBits(AI_BASE_FLOAT64_LIMB_T *tab, AI_BASE_FLOAT64_LIMB_T len, AI_BASE_FLOAT64_SLIMB_T pos, AI_BASE_FLOAT64_LIMB_T val);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64AddMod(AI_BASE_FLOAT64_LIMB_T a, AI_BASE_FLOAT64_LIMB_T b, AI_BASE_FLOAT64_LIMB_T m);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64SubMod(AI_BASE_FLOAT64_LIMB_T a, AI_BASE_FLOAT64_LIMB_T b, AI_BASE_FLOAT64_LIMB_T m);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64ModFast(AI_BASE_FLOAT64_DLIMB_T r, AI_BASE_FLOAT64_LIMB_T m, AI_BASE_FLOAT64_LIMB_T m_inv);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64MulModFast(AI_BASE_FLOAT64_LIMB_T a, AI_BASE_FLOAT64_LIMB_T b, AI_BASE_FLOAT64_LIMB_T m, AI_BASE_FLOAT64_LIMB_T m_inv)
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64InitMulModFast(AI_BASE_FLOAT64_LIMB_T m);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64MulModFast2(AI_BASE_FLOAT64_LIMB_T a, AI_BASE_FLOAT64_LIMB_T b, AI_BASE_FLOAT64_LIMB_T m, AI_BASE_FLOAT64_LIMB_T b_inv);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64MulModFast3(AI_BASE_FLOAT64_LIMB_T a, AI_BASE_FLOAT64_LIMB_T b, AI_BASE_FLOAT64_LIMB_T m, AI_BASE_FLOAT64_LIMB_T b_inv);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64InitMulModFast2(AI_BASE_FLOAT64_LIMB_T b, AI_BASE_FLOAT64_LIMB_T m);
#ifdef __AVX2__
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64NttLimbToInt(NTTLimb a, AI_BASE_FLOAT64_LIMB_T m);
NTTLimb AiBaseFloat64IntToNttLimb(AI_BASE_FLOAT64_LIMB_T a, AI_BASE_FLOAT64_LIMB_T m);
NTTLimb AiBaseFloat64IntToNttLimb2(AI_BASE_FLOAT64_LIMB_T a, AI_BASE_FLOAT64_LIMB_T m);
AI_BASE_FLOAT64_M256D AiBaseFloat64NttMod1(AI_BASE_FLOAT64_M256D r, AI_BASE_FLOAT64_M256D m);
AI_BASE_FLOAT64_M256D AiBaseFloat64NttMod(AI_BASE_FLOAT64_M256D r, AI_BASE_FLOAT64_M256D mf, AI_BASE_FLOAT64_M256D m2f);
AI_BASE_FLOAT64_M256D AiBaseFloat64NttMulMod(AI_BASE_FLOAT64_M256D a, AI_BASE_FLOAT64_M256D b, AI_BASE_FLOAT64_M256D mf, AI_BASE_FLOAT64_M256D m_inv);
void *AiBaseFloat64BfAlignedMalloc(UINT32_T size, UINT32_T align);
void AiBaseFloat64BfAlignedFree(void *ptr);
void *AiBaseFloat64NttMalloc(UINT32_T size);
void AiBaseFloat64NttFree(void *ptr);
void AiBaseFloat64NttFft(NTTLimb *out_buf, NTTLimb *in_buf, NTTLimb *tmp_buf, INT32_T fft_len_log2, INT32_T inverse, INT32_T m_idx);
void AiBaseFloat64NttVecMul(NTTLimb *tab1, NTTLimb *tab2, AI_BASE_FLOAT64_LIMB_T fft_len_log2, INT32_T k_tot, INT32_T m_idx);
void AiBaseFloat64MulTrig(NTTLimb *buf, AI_BASE_FLOAT64_LIMB_T n, AI_BASE_FLOAT64_LIMB_T c1, AI_BASE_FLOAT64_LIMB_T m, AI_BASE_FLOAT64_LIMB_T m_inv1);
#else
void *AiBaseFloat64NttMalloc(UINT32_T size);
void AiBaseFloat64NttFree(void *ptr);
AI_BASE_FLOAT64_LIMB_T AiBaseFloat64NttLimbToInt(NTTLimb a, AI_BASE_FLOAT64_LIMB_T m);
NTTLimb AiBaseFloat64IntToNttLimb(AI_BASE_FLOAT64_SLIMB_T a, AI_BASE_FLOAT64_LIMB_T m);
void AiBaseFloat64NttFft(NTTLimb *out_buf, NTTLimb *in_buf, NTTLimb *tmp_buf, INT32_T fft_len_log2, INT32_T inverse, INT32_T m_idx);
void AiBaseFloat64NttVecMul(NTTLimb *tab1, NTTLimb *tab2, INT32_T fft_len_log2, INT32_T k_tot, INT32_T m_idx);
void AiBaseFloat64MulTrig(NTTLimb *buf, AI_BASE_FLOAT64_LIMB_T n, AI_BASE_FLOAT64_LIMB_T c_mul, AI_BASE_FLOAT64_LIMB_T m, AI_BASE_FLOAT64_LIMB_T m_inv);
#endif /* !AVX2 */
NTTLimb *AiBaseFloat64GetTrig(INT32_T k, INT32_T inverse1, INT32_T m_idx1);
void AiBaseFloat64NttFftPartial(NTTLimb *buf1, INT32_T k1, INT32_T k2, AI_BASE_FLOAT64_LIMB_T n1, AI_BASE_FLOAT64_LIMB_T n2, INT32_T inverse, AI_BASE_FLOAT64_LIMB_T m_idx);
void AiBaseFloat64NttConv(NTTLimb *buf1, NTTLimb *buf2, INT32_T k, INT32_T k_tot, AI_BASE_FLOAT64_LIMB_T m_idx);
void AiBaseFloat64LimbToNtt(NTTLimb *tabr, AI_BASE_FLOAT64_LIMB_T fft_len,  const AI_BASE_FLOAT64_LIMB_T *taba, AI_BASE_FLOAT64_LIMB_T a_len, INT32_T dpl, INT32_T first_m_idx, INT32_T nb_mods);
#if defined(__AVX2__)
void AiBaseFloat64NttToLimb(AI_BASE_FLOAT64_LIMB_T *tabr, AI_BASE_FLOAT64_LIMB_T r_len,  const NTTLimb *buf, INT32_T fft_len_log2, INT32_T dpl, INT32_T nb_mods);
#else
void AiBaseFloat64NttToLimb(AI_BASE_FLOAT64_LIMB_T *tabr, AI_BASE_FLOAT64_LIMB_T r_len, const NTTLimb *buf, INT32_T fft_len_log2, INT32_T dpl, INT32_T nb_mods);
#endif
void AiBaseFloat64NttStaticInit(void);
INT32_T AiBaseFloat64BfGetFftSize(INT32_T *pdpl, INT32_T *pnb_mods, AI_BASE_FLOAT64_LIMB_T len);
void AiBaseFloat64FftMul(AI_BASE_FLOAT64_BF_T *res, AI_BASE_FLOAT64_LIMB_T *a_tab, AI_BASE_FLOAT64_LIMB_T a_len, AI_BASE_FLOAT64_LIMB_T *b_tab, AI_BASE_FLOAT64_LIMB_T b_len, INT32_T mul_flags);
#else /* AI_BASE_FLOAT64_FFT_MUL */
INT32_T AiBaseFloat64BfGetFftSize(INT32_T *pdpl, INT32_T *pnb_mods, AI_BASE_FLOAT64_LIMB_T len);
#endif /* !AI_BASE_FLOAT64_FFT_MUL */

/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/



