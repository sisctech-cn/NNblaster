/**
 ******************************************************************************
 * file    ai_base_float32_bf.h
 * author  sunlingge
 * version V100
 * brief   the ai base float 32 bf driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT32_BF_H
#define AI_BASE_FLOAT32_BF_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP32 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT32_LIMB_LOG2_BITS				(5)
#define AI_BASE_FLOAT32_LIMB_BITS					(1 << AI_BASE_FLOAT32_LIMB_LOG2_BITS)

/*------------------------- Pre Type Definition -----------------------------*/
typedef INT32_T AI_BASE_FLOAT32_SLIMB_T, *AI_BASE_FLOAT32_SLIMB_T_PTR;
typedef UINT32_T AI_BASE_FLOAT32_LIMB_T, *AI_BASE_FLOAT32_LIMB_T_PTR;
typedef UINT64_T AI_BASE_FLOAT32_DLIMB_T, *AI_BASE_FLOAT32_DLIMB_T_PTR;

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT32_INT32_MAX					(0x7FFFFFFFL)
#define AI_BASE_FLOAT32_INT32_MIN					(-AI_BASE_FLOAT32_INT32_MAX - 1L)	
#define AI_BASE_FLOAT32_EXP_MIN						(AI_BASE_FLOAT32_INT32_MIN)
#define AI_BASE_FLOAT32_EXP_MAX						(AI_BASE_FLOAT32_INT32_MAX)

/* in bits */
#define AI_BASE_FLOAT32_BF_EXP_BITS_MIN				(3)
#define AI_BASE_FLOAT32_BF_EXP_BITS_MAX				(AI_BASE_FLOAT32_LIMB_BITS - 2)

#define AI_BASE_FLOAT32_BF_PREC_MIN					(2)
#define AI_BASE_FLOAT32_BF_PREC_MAX					(((AI_BASE_FLOAT32_LIMB_T)1 << AI_BASE_FLOAT32_BF_EXP_BITS_MAX) - 2)
/* infinite precision */
#define AI_BASE_FLOAT32_BF_PREC_INF					(AI_BASE_FLOAT32_BF_PREC_MAX + 1)	

#define AI_BASE_FLOAT32_BF_CHKSUM_MOD				(975620677U)

#define AI_BASE_FLOAT32_BF_EXP_ZERO					(AI_BASE_FLOAT32_EXP_MIN)
#define AI_BASE_FLOAT32_BF_EXP_INF					(AI_BASE_FLOAT32_EXP_MAX - 1)
#define AI_BASE_FLOAT32_BF_EXP_NAN					(AI_BASE_FLOAT32_EXP_MAX)

/* maximum radix for AiBaseFloat32Bfatof() and AiBaseFloat32Bfftoa() */
#define AI_BASE_FLOAT32_BF_RADIX_MAX				(36) 

#ifdef AI_BASE_FLOAT32_FFT_MUL
#define AI_BASE_FLOAT32_STRIP_LEN					(16)
#if defined(__AVX2__)
/* we must have: modulo >= 1 << NTT_MOD_LOG2_MIN */
#define AI_BASE_FLOAT32_NTT_MOD_LOG2_MIN			(50)
#define AI_BASE_FLOAT32_NTT_MOD_LOG2_MAX			(51)
#define AI_BASE_FLOAT32_NB_MODS					(5)
#define AI_BASE_FLOAT32_NTT_PROOT_2EXP			(39)
#define AI_BASE_FLOAT32_VEC_LEN					(4)
#else
#if AI_BASE_FLOAT32_LIMB_BITS == 64
#define AI_BASE_FLOAT32_NTT_MOD_LOG2_MIN			(61)
#define AI_BASE_FLOAT32_NTT_MOD_LOG2_MAX			(62)
#define AI_BASE_FLOAT32_NB_MODS					(5)
#define AI_BASE_FLOAT32_NTT_PROOT_2EXP			(51)
#endif
#if AI_BASE_FLOAT32_LIMB_BITS == 32
/* we must have: modulo >= 1 << NTT_MOD_LOG2_MIN */
#define AI_BASE_FLOAT32_NTT_MOD_LOG2_MIN			(29)
#define AI_BASE_FLOAT32_NTT_MOD_LOG2_MAX			(30)
#define AI_BASE_FLOAT32_NB_MODS					(5)
#define AI_BASE_FLOAT32_NTT_PROOT_2EXP			(20)
#endif
#endif
#if defined(__AVX2__)
#define AI_BASE_FLOAT32_NTT_TRIG_K_MAX			(18)
#else
#define AI_BASE_FLOAT32_NTT_TRIG_K_MAX			(19)
#endif
#endif

/*------------------------- Type Definition----------------------------------*/
/* +/-zero is represented with expn = BF_EXP_ZERO and len = 0,
   +/-infinity is represented with expn = BF_EXP_INF and len = 0,
   NaN is represented with expn = BF_EXP_NAN and len = 0 (sign is ignored)
 */
typedef struct ai_base_float32_bf_t {
    AI_BASE_FLOAT32_LIMB_T sign;
    AI_BASE_FLOAT32_SLIMB_T expn;
    AI_BASE_FLOAT32_LIMB_T len;
    AI_BASE_FLOAT32_LIMB_T tab[4];
} AI_BASE_FLOAT32_BF_T, *AI_BASE_FLOAT32_BF_T_PTR;


typedef enum {
    AI_BASE_FLOAT32_BF_RNDN,				/* round to nearest, ties to even */
    AI_BASE_FLOAT32_BF_RNDZ,				/* round to zero */
    AI_BASE_FLOAT32_BF_RNDD,				/* round to -inf */
    AI_BASE_FLOAT32_BF_RNDU,				/* round to +inf */
    AI_BASE_FLOAT32_BF_RNDNA,				/* round to nearest, away from zero */
    AI_BASE_FLOAT32_BF_RNDF,				/* faithful rounding (nondeterministic, either RNDD or RNDU, inexact flag is always set)  */
} AI_BASE_FLOAT32_BF_RND_T;

/* contains the rounding mode and number of exponents bits */
typedef UINT32_T AI_BASE_FLOAT32_BF_FLAGS_T;

typedef INT32_T AI_BASE_FLOAT32_BF_OP2_FUNC_T(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);

typedef struct {
    AI_BASE_FLOAT32_BF_T val;
    AI_BASE_FLOAT32_LIMB_T prec;
} AI_BASE_FLOAT32_BF_CONST_CACHE;

/* AI_BASE_FLOAT32_ZIV_FUNC should compute the result 'r' with faithful rounding at
   precision 'prec'. For efficiency purposes, the final AiBaseFloat32BfRound()
   does not need to be done in the function. */
typedef INT32_T AI_BASE_FLOAT32_ZIV_FUNC(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, void *opaque);

#ifdef AI_BASE_FLOAT32_FFT_MUL
#if defined(__AVX2__)
typedef FLOAT64_T NTTLimb;
typedef __m256d AI_BASE_FLOAT32_M256D;
typedef union {
    AI_BASE_FLOAT32_M256D v;
    FLOAT64_T d[4];
} AI_BASE_FLOAT32_VEC_UNION;
#else
typedef AI_BASE_FLOAT32_LIMB_T NTTLimb;
#endif
#endif

/*------------------------- Function Definition -----------------------------*/
INT32_T AiBaseFloat32BfGetExpBits(AI_BASE_FLOAT32_BF_FLAGS_T flags);
AI_BASE_FLOAT32_BF_FLAGS_T AiBaseFloat32BfSetExpBits(INT32_T n);
AI_BASE_FLOAT32_SLIMB_T AiBaseFloat32BfMax(AI_BASE_FLOAT32_SLIMB_T a, AI_BASE_FLOAT32_SLIMB_T b);
AI_BASE_FLOAT32_SLIMB_T AiBaseFloat32BfMin(AI_BASE_FLOAT32_SLIMB_T a, AI_BASE_FLOAT32_SLIMB_T b);
void *AiBaseFloat32BfRealloc(void *ptr, UINT32_T size);
void AiBaseFloat32BfNeg(AI_BASE_FLOAT32_BF_T *r);
INT32_T AiBaseFloat32BfIsFinite(const AI_BASE_FLOAT32_BF_T *a);
ERROR_FLOAT32_T AiBaseFloat32BfIsNan(const AI_BASE_FLOAT32_BF_T *a);
INT32_T AiBaseFloat32BfIsZero(const AI_BASE_FLOAT32_BF_T *a);
INT32_T AiBaseFloat32Clz(AI_BASE_FLOAT32_LIMB_T a);
INT32_T AiBaseFloat32Ctz(AI_BASE_FLOAT32_LIMB_T a);
INT32_T AiBaseFloat32CeilLog2(AI_BASE_FLOAT32_LIMB_T a);
#if 0
AI_BASE_FLOAT32_SLIMB_T AiBaseFloat32CeilDiv(AI_BASE_FLOAT32_SLIMB_T a, AI_BASE_FLOAT32_LIMB_T b);
AI_BASE_FLOAT32_SLIMB_T AiBaseFloat32FloorDiv(AI_BASE_FLOAT32_SLIMB_T a, AI_BASE_FLOAT32_LIMB_T b);
#endif
void *AiBaseFloat32BfMalloc(UINT32_T size);
void AiBaseFloat32BfInit(AI_BASE_FLOAT32_BF_T *r);
ERROR_FLOAT32_T AiBaseFloat32BfResize(AI_BASE_FLOAT32_BF_T *r, AI_BASE_FLOAT32_LIMB_T len);
void AiBaseFloat32BfSetUi(AI_BASE_FLOAT32_BF_T *r, UINT64_T a);
void AiBaseFloat32BfSetSi(AI_BASE_FLOAT32_BF_T *r, INT64_T a);
void AiBaseFloat32BfSetNan(AI_BASE_FLOAT32_BF_T *r);
void AiBaseFloat32BfSetZero(AI_BASE_FLOAT32_BF_T *r, AI_BASE_FLOAT32_LIMB_T is_neg);
void AiBaseFloat32BfSetInf(AI_BASE_FLOAT32_BF_T *r, AI_BASE_FLOAT32_LIMB_T is_neg);
void AiBaseFloat32BfSet(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a);
void AiBaseFloat32BfMove(AI_BASE_FLOAT32_BF_T *r, AI_BASE_FLOAT32_BF_T *a);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32GetLimbz(const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T idx);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32GetBits(const AI_BASE_FLOAT32_LIMB_T *tab, AI_BASE_FLOAT32_LIMB_T len, AI_BASE_FLOAT32_SLIMB_T pos);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32GetBit(const AI_BASE_FLOAT32_LIMB_T *tab, AI_BASE_FLOAT32_LIMB_T len, AI_BASE_FLOAT32_SLIMB_T pos);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32LimbMask(INT32_T start, INT32_T last);
 AI_BASE_FLOAT32_LIMB_T AiBaseFloat32ScanBitNz(const AI_BASE_FLOAT32_BF_T *r, AI_BASE_FLOAT32_SLIMB_T bit_pos);
INT32_T AiBaseFloat32BfGetRndAdd(INT32_T *pret, const AI_BASE_FLOAT32_BF_T *r, AI_BASE_FLOAT32_LIMB_T l, AI_BASE_FLOAT32_SLIMB_T prec, INT32_T rnd_mode);
INT32_T AiBaseFloat32BfSetOverFlow(AI_BASE_FLOAT32_BF_T *r, INT32_T sign, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T __AiBaseFloat32BfRound(AI_BASE_FLOAT32_BF_T *r, AI_BASE_FLOAT32_LIMB_T prec1, AI_BASE_FLOAT32_BF_FLAGS_T flags, AI_BASE_FLOAT32_LIMB_T l);
INT32_T AiBaseFloat32BfNormalizeAndRound(AI_BASE_FLOAT32_BF_T *r, AI_BASE_FLOAT32_LIMB_T prec1, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfCanRound(const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_SLIMB_T prec, AI_BASE_FLOAT32_BF_RND_T rnd_mode, AI_BASE_FLOAT32_SLIMB_T k);
INT32_T AiBaseFloat32BfRound(AI_BASE_FLOAT32_BF_T *r, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
void AiBaseFloat32DumpLimbs(const UINT8_T *str, const AI_BASE_FLOAT32_LIMB_T *tab, AI_BASE_FLOAT32_LIMB_T n);
void AiBaseFloat32BfPrintStr(const UINT8_T *str, const AI_BASE_FLOAT32_BF_T *a);
INT32_T AiBaseFloat32BfCmpu(const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b);
INT32_T AiBaseFloat32BfCmpFull(const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b);
INT32_T AiBaseFloat32BfCmp(const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b, INT32_T op);
INT32_T AiBaseFloat32BfCmpEq(const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b);
INT32_T AiBaseFloat32BfCmpLe(const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b);
INT32_T AiBaseFloat32BfCmpLt(const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32CountCancelledBits(const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b);
INT32_T AiBaseFloat32BfAddInternal(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags, INT32_T b_neg);
INT32_T __AiBaseFloat32BfAdd(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T __AiBaseFloat32BfSub(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32MpAdd(AI_BASE_FLOAT32_LIMB_T *res, const AI_BASE_FLOAT32_LIMB_T *op1, const AI_BASE_FLOAT32_LIMB_T *op2, AI_BASE_FLOAT32_LIMB_T n, AI_BASE_FLOAT32_LIMB_T carry);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32MpAddMul1(AI_BASE_FLOAT32_LIMB_T *tabr, const AI_BASE_FLOAT32_LIMB_T *taba, AI_BASE_FLOAT32_LIMB_T n, AI_BASE_FLOAT32_LIMB_T b);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32MpSubMul1(AI_BASE_FLOAT32_LIMB_T *tabr, const AI_BASE_FLOAT32_LIMB_T *taba, AI_BASE_FLOAT32_LIMB_T n, AI_BASE_FLOAT32_LIMB_T b);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32Udiv1normInit(AI_BASE_FLOAT32_LIMB_T d);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32Udiv1norm(AI_BASE_FLOAT32_LIMB_T *pr, AI_BASE_FLOAT32_LIMB_T a1, AI_BASE_FLOAT32_LIMB_T a0, AI_BASE_FLOAT32_LIMB_T d, AI_BASE_FLOAT32_LIMB_T d_inv);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32MpDiv1norm(AI_BASE_FLOAT32_LIMB_T *tabr, const AI_BASE_FLOAT32_LIMB_T *taba, AI_BASE_FLOAT32_LIMB_T n, AI_BASE_FLOAT32_LIMB_T b, AI_BASE_FLOAT32_LIMB_T r);
void AiBaseFloat32MpDivnorm(AI_BASE_FLOAT32_LIMB_T *tabq, AI_BASE_FLOAT32_LIMB_T *taba, AI_BASE_FLOAT32_LIMB_T na, const AI_BASE_FLOAT32_LIMB_T *tabb, AI_BASE_FLOAT32_LIMB_T nb);
INT32_T AiBaseFloat32BfMul(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfMul2Exp(AI_BASE_FLOAT32_BF_T *r, AI_BASE_FLOAT32_SLIMB_T e, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
void AiBaseFloat32BfRecipRec(AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *x, AI_BASE_FLOAT32_LIMB_T prec1);
void AiBaseFloat32BfRecip(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec);
void AiBaseFloat32BfAddZeroLimbs(AI_BASE_FLOAT32_BF_T *r, AI_BASE_FLOAT32_LIMB_T precl);
void AiBaseFloat32BfOrOne(AI_BASE_FLOAT32_BF_T *r, AI_BASE_FLOAT32_LIMB_T precl);
AI_BASE_FLOAT32_SLIMB_T AiBaseFloat32BfGetExpMin(const AI_BASE_FLOAT32_BF_T *a);
void AiBaseFloat32BfTdivremu(AI_BASE_FLOAT32_BF_T *q, AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b);
INT32_T __AiBaseFloat32BfDiv(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfDivrem(AI_BASE_FLOAT32_BF_T *q, AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags, INT32_T rnd_mode);
INT32_T AiBaseFloat32BfFmod(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfRemainder(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
 INT32_T AiBaseFloat32BfGetLimb(AI_BASE_FLOAT32_SLIMB_T *pres, const AI_BASE_FLOAT32_BF_T *a, INT32_T flags);
INT32_T AiBaseFloat32BfRemquo(AI_BASE_FLOAT32_SLIMB_T *pq, AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
 AI_BASE_FLOAT32_LIMB_T AiBaseFloat32MulMod(AI_BASE_FLOAT32_LIMB_T a, AI_BASE_FLOAT32_LIMB_T b, AI_BASE_FLOAT32_LIMB_T m);
#if defined(AI_BASE_FLOAT32_USE_MUL_CHECK)
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32MpMod1(const AI_BASE_FLOAT32_LIMB_T *tab, AI_BASE_FLOAT32_LIMB_T n, AI_BASE_FLOAT32_LIMB_T m, AI_BASE_FLOAT32_LIMB_T r);
#endif
void __AiBaseFloat32BfRsqrt(AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *x, AI_BASE_FLOAT32_LIMB_T prec1);
INT32_T __AiBaseFloat32BfSqrt(AI_BASE_FLOAT32_BF_T *x, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec1, AI_BASE_FLOAT32_BF_FLAGS_T flags);
void AiBaseFloat32BfRsqrt(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32BfIsqrt(AI_BASE_FLOAT32_LIMB_T a);
INT32_T AiBaseFloat32BfSqrtrem(AI_BASE_FLOAT32_BF_T *r, AI_BASE_FLOAT32_BF_T *rem1, const AI_BASE_FLOAT32_BF_T *a);
INT32_T AiBaseFloat32BfSqrt(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfOp2(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags, AI_BASE_FLOAT32_BF_OP2_FUNC_T *func);
INT32_T AiBaseFloat32BfAdd(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfSub(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfDiv(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfMulUi(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, UINT64_T b1, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfMulSi(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, INT64_T b1, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfAddSi(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, INT64_T b1, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfPowUi(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T b, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfPowUiUi(AI_BASE_FLOAT32_BF_T *r, AI_BASE_FLOAT32_LIMB_T a1, AI_BASE_FLOAT32_LIMB_T b, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfRint(AI_BASE_FLOAT32_BF_T *r, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32BfLogicOp1(AI_BASE_FLOAT32_LIMB_T a, AI_BASE_FLOAT32_LIMB_T b, INT32_T op);
void AiBaseFloat32BfLogicOp(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a1, const AI_BASE_FLOAT32_BF_T *b1, INT32_T op);
void AiBaseFloat32BfLogicOr(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b);
void AiBaseFloat32BfLogicXor(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b);
void AiBaseFloat32BfLogicAnd(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, const AI_BASE_FLOAT32_BF_T *b);
INT32_T AiBaseFloat32BfGetInt32(INT32_T *pres, const AI_BASE_FLOAT32_BF_T *a, INT32_T flags);
INT32_T AiBaseFloat32BfGetInt16(INT16_T *pres, const AI_BASE_FLOAT32_BF_T *a, INT32_T flags);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32GetLimbRadix(INT32_T radix);
void AiBaseFloat32BfIntegerFromRadixRec(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_LIMB_T *tab, AI_BASE_FLOAT32_LIMB_T n, INT32_T level, AI_BASE_FLOAT32_LIMB_T n0, AI_BASE_FLOAT32_LIMB_T radix, AI_BASE_FLOAT32_BF_T *pow_tab);
void AiBaseFloat32BfIntegerFromRadix(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_LIMB_T *tab, AI_BASE_FLOAT32_LIMB_T n, AI_BASE_FLOAT32_LIMB_T radix);
INT32_T AiBaseFloat32BfConvertFromRadix(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *T, AI_BASE_FLOAT32_LIMB_T radix, AI_BASE_FLOAT32_SLIMB_T expn, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32ToDigit(INT32_T c);
void AiBaseFloat32BfAddLimb(AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_SLIMB_T *ppos, AI_BASE_FLOAT32_LIMB_T v);
INT32_T AiBaseFloat32BfTolower(INT32_T c);
INT32_T AiBaseFloat32Strcasestart(const UINT8_T *str, const UINT8_T *val, const UINT8_T **ptr);
INT32_T AiBaseFloat32BfAtof2(AI_BASE_FLOAT32_BF_T *r, AI_BASE_FLOAT32_SLIMB_T *pexponent, const UINT8_T *str, const UINT8_T **pnext, INT32_T radix, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfAtof(AI_BASE_FLOAT32_BF_T *r, const UINT8_T *str, const UINT8_T **pnext, INT32_T radix, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
AI_BASE_FLOAT32_SLIMB_T AiBaseFloat32BfMulLog2Radix(AI_BASE_FLOAT32_SLIMB_T a1, UINT32_T radix, INT32_T is_inv, INT32_T is_ceil1);
void AiBaseFloat32BfIntegerToRadixRec(AI_BASE_FLOAT32_BF_T *pow_tab, AI_BASE_FLOAT32_LIMB_T *out, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T n, INT32_T level, AI_BASE_FLOAT32_LIMB_T n0, AI_BASE_FLOAT32_LIMB_T radixl, UINT32_T radixl_bits);
void AiBaseFloat32BfIntegerToRadix(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T radixl);
void AiBaseFloat32BfConvertToRadix(AI_BASE_FLOAT32_BF_T *r, AI_BASE_FLOAT32_SLIMB_T *pE, const AI_BASE_FLOAT32_BF_T *a, INT32_T radix, AI_BASE_FLOAT32_LIMB_T P, AI_BASE_FLOAT32_BF_RND_T rnd_mode, BOOL_T is_fixed_exponent);
void AiBaseFloat32LimbToA(UINT8_T *buf, AI_BASE_FLOAT32_LIMB_T n, UINT32_T radix, INT32_T len);
void AiBaseFloat32LimbToA2(UINT8_T *buf, AI_BASE_FLOAT32_LIMB_T n, UINT32_T radix_bits, INT32_T len);
//void AiBaseFloat32OutputDigits(AI_BASE_FLOAT32_DYN_BUF *s, const AI_BASE_FLOAT32_BF_T *a1, INT32_T radix, AI_BASE_FLOAT32_LIMB_T n_digits, AI_BASE_FLOAT32_LIMB_T dot_pos);
void *AibaseFloat32BfDbufRealloc(void *opaque, void *ptr, UINT32_T size);
UINT32_T AiBaseFloat32BfFtoa(UINT8_T **pbuf, const AI_BASE_FLOAT32_BF_T *a2, INT32_T radix, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
void AiBaseFloat32BfConstLog2Rec(AI_BASE_FLOAT32_BF_T *T, AI_BASE_FLOAT32_BF_T *P, AI_BASE_FLOAT32_BF_T *Q, AI_BASE_FLOAT32_LIMB_T n1, AI_BASE_FLOAT32_LIMB_T n2, BOOL_T need_P);
void AiBaseFloat32BfConstLog2Internal(AI_BASE_FLOAT32_BF_T *T, AI_BASE_FLOAT32_LIMB_T prec);
void AiBaseFloat32ChudBs(AI_BASE_FLOAT32_BF_T *P, AI_BASE_FLOAT32_BF_T *Q, AI_BASE_FLOAT32_BF_T *G, INT64_T a, INT64_T b, INT32_T need_g, AI_BASE_FLOAT32_LIMB_T prec);
void AiBaseFloat32BfConstPiInternal(AI_BASE_FLOAT32_BF_T *Q, AI_BASE_FLOAT32_LIMB_T prec);
INT32_T AiBaseFloat32BfConstGet(AI_BASE_FLOAT32_BF_T *T, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags, AI_BASE_FLOAT32_BF_CONST_CACHE *c, void (*func)(AI_BASE_FLOAT32_BF_T *res, AI_BASE_FLOAT32_LIMB_T prec));
INT32_T AiBaseFloat32BfConstLog2(AI_BASE_FLOAT32_BF_T *T, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfConstPi(AI_BASE_FLOAT32_BF_T *T, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfZivRounding(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags, AI_BASE_FLOAT32_ZIV_FUNC *f, void *opaque);
INT32_T AiBaseFloat32BfExpInternal(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat32BfExp(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfLogInternal(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat32BfLog(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfPowGeneric(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *x, AI_BASE_FLOAT32_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat32BfPowInt(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *x, AI_BASE_FLOAT32_LIMB_T prec, void *opaque);
BOOL_T AiBaseFloat32CheckExactPower2n(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *x, AI_BASE_FLOAT32_SLIMB_T n);
INT32_T AiBaseFloat32BfPow(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *x, const AI_BASE_FLOAT32_BF_T *y, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
void AiBaseFloat32BfSqrtSin(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *x, AI_BASE_FLOAT32_LIMB_T prec1);
INT32_T AiBaseFloat32BfSincos(AI_BASE_FLOAT32_BF_T *s, AI_BASE_FLOAT32_BF_T *c, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec);
INT32_T AiBaseFloat32BfCosInternal(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat32BfCos(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfSinInternal(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat32BfSin(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfTanInternal(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat32BfTan(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfAtanInternal(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat32BfAtan(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfAtan2Internal(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *y, AI_BASE_FLOAT32_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat32BfAtan2(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *y, const AI_BASE_FLOAT32_BF_T *x, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfAsinInternal(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, void *opaque);
INT32_T AiBaseFloat32BfAsin(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
INT32_T AiBaseFloat32BfAcos(AI_BASE_FLOAT32_BF_T *r, const AI_BASE_FLOAT32_BF_T *a, AI_BASE_FLOAT32_LIMB_T prec, AI_BASE_FLOAT32_BF_FLAGS_T flags);
#ifdef AI_BASE_FLOAT32_FFT_MUL
void AiBaseFloat32PutBits(AI_BASE_FLOAT32_LIMB_T *tab, AI_BASE_FLOAT32_LIMB_T len, AI_BASE_FLOAT32_SLIMB_T pos, AI_BASE_FLOAT32_LIMB_T val);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32AddMod(AI_BASE_FLOAT32_LIMB_T a, AI_BASE_FLOAT32_LIMB_T b, AI_BASE_FLOAT32_LIMB_T m);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32SubMod(AI_BASE_FLOAT32_LIMB_T a, AI_BASE_FLOAT32_LIMB_T b, AI_BASE_FLOAT32_LIMB_T m);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32ModFast(AI_BASE_FLOAT32_DLIMB_T r, AI_BASE_FLOAT32_LIMB_T m, AI_BASE_FLOAT32_LIMB_T m_inv);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32MulModFast(AI_BASE_FLOAT32_LIMB_T a, AI_BASE_FLOAT32_LIMB_T b, AI_BASE_FLOAT32_LIMB_T m, AI_BASE_FLOAT32_LIMB_T m_inv)
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32InitMulModFast(AI_BASE_FLOAT32_LIMB_T m);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32MulModFast2(AI_BASE_FLOAT32_LIMB_T a, AI_BASE_FLOAT32_LIMB_T b, AI_BASE_FLOAT32_LIMB_T m, AI_BASE_FLOAT32_LIMB_T b_inv);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32MulModFast3(AI_BASE_FLOAT32_LIMB_T a, AI_BASE_FLOAT32_LIMB_T b, AI_BASE_FLOAT32_LIMB_T m, AI_BASE_FLOAT32_LIMB_T b_inv);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32InitMulModFast2(AI_BASE_FLOAT32_LIMB_T b, AI_BASE_FLOAT32_LIMB_T m);
#ifdef __AVX2__
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32NttLimbToInt(NTTLimb a, AI_BASE_FLOAT32_LIMB_T m);
NTTLimb AiBaseFloat32IntToNttLimb(AI_BASE_FLOAT32_LIMB_T a, AI_BASE_FLOAT32_LIMB_T m);
NTTLimb AiBaseFloat32IntToNttLimb2(AI_BASE_FLOAT32_LIMB_T a, AI_BASE_FLOAT32_LIMB_T m);
AI_BASE_FLOAT32_M256D AiBaseFloat32NttMod1(AI_BASE_FLOAT32_M256D r, AI_BASE_FLOAT32_M256D m);
AI_BASE_FLOAT32_M256D AiBaseFloat32NttMod(AI_BASE_FLOAT32_M256D r, AI_BASE_FLOAT32_M256D mf, AI_BASE_FLOAT32_M256D m2f);
AI_BASE_FLOAT32_M256D AiBaseFloat32NttMulMod(AI_BASE_FLOAT32_M256D a, AI_BASE_FLOAT32_M256D b, AI_BASE_FLOAT32_M256D mf, AI_BASE_FLOAT32_M256D m_inv);
void *AiBaseFloat32BfAlignedMalloc(UINT32_T size, UINT32_T align);
void AiBaseFloat32BfAlignedFree(void *ptr);
void *AiBaseFloat32NttMalloc(UINT32_T size);
void AiBaseFloat32NttFree(void *ptr);
void AiBaseFloat32NttFft(NTTLimb *out_buf, NTTLimb *in_buf, NTTLimb *tmp_buf, INT32_T fft_len_log2, INT32_T inverse, INT32_T m_idx);
void AiBaseFloat32NttVecMul(NTTLimb *tab1, NTTLimb *tab2, AI_BASE_FLOAT32_LIMB_T fft_len_log2, INT32_T k_tot, INT32_T m_idx);
void AiBaseFloat32MulTrig(NTTLimb *buf, AI_BASE_FLOAT32_LIMB_T n, AI_BASE_FLOAT32_LIMB_T c1, AI_BASE_FLOAT32_LIMB_T m, AI_BASE_FLOAT32_LIMB_T m_inv1);
#else
void *AiBaseFloat32NttMalloc(UINT32_T size);
void AiBaseFloat32NttFree(void *ptr);
AI_BASE_FLOAT32_LIMB_T AiBaseFloat32NttLimbToInt(NTTLimb a, AI_BASE_FLOAT32_LIMB_T m);
NTTLimb AiBaseFloat32IntToNttLimb(AI_BASE_FLOAT32_SLIMB_T a, AI_BASE_FLOAT32_LIMB_T m);
void AiBaseFloat32NttFft(NTTLimb *out_buf, NTTLimb *in_buf, NTTLimb *tmp_buf, INT32_T fft_len_log2, INT32_T inverse, INT32_T m_idx);
void AiBaseFloat32NttVecMul(NTTLimb *tab1, NTTLimb *tab2, INT32_T fft_len_log2, INT32_T k_tot, INT32_T m_idx);
void AiBaseFloat32MulTrig(NTTLimb *buf, AI_BASE_FLOAT32_LIMB_T n, AI_BASE_FLOAT32_LIMB_T c_mul, AI_BASE_FLOAT32_LIMB_T m, AI_BASE_FLOAT32_LIMB_T m_inv);
#endif /* !AVX2 */
NTTLimb *AiBaseFloat32GetTrig(INT32_T k, INT32_T inverse1, INT32_T m_idx1);
void AiBaseFloat32NttFftPartial(NTTLimb *buf1, INT32_T k1, INT32_T k2, AI_BASE_FLOAT32_LIMB_T n1, AI_BASE_FLOAT32_LIMB_T n2, INT32_T inverse, AI_BASE_FLOAT32_LIMB_T m_idx);
void AiBaseFloat32NttConv(NTTLimb *buf1, NTTLimb *buf2, INT32_T k, INT32_T k_tot, AI_BASE_FLOAT32_LIMB_T m_idx);
void AiBaseFloat32LimbToNtt(NTTLimb *tabr, AI_BASE_FLOAT32_LIMB_T fft_len,  const AI_BASE_FLOAT32_LIMB_T *taba, AI_BASE_FLOAT32_LIMB_T a_len, INT32_T dpl, INT32_T first_m_idx, INT32_T nb_mods);
#if defined(__AVX2__)
void AiBaseFloat32NttToLimb(AI_BASE_FLOAT32_LIMB_T *tabr, AI_BASE_FLOAT32_LIMB_T r_len,  const NTTLimb *buf, INT32_T fft_len_log2, INT32_T dpl, INT32_T nb_mods);
#else
void AiBaseFloat32NttToLimb(AI_BASE_FLOAT32_LIMB_T *tabr, AI_BASE_FLOAT32_LIMB_T r_len, const NTTLimb *buf, INT32_T fft_len_log2, INT32_T dpl, INT32_T nb_mods);
#endif
void AiBaseFloat32NttStaticInit(void);
INT32_T AiBaseFloat32BfGetFftSize(INT32_T *pdpl, INT32_T *pnb_mods, AI_BASE_FLOAT32_LIMB_T len);
void AiBaseFloat32FftMul(AI_BASE_FLOAT32_BF_T *res, AI_BASE_FLOAT32_LIMB_T *a_tab, AI_BASE_FLOAT32_LIMB_T a_len, AI_BASE_FLOAT32_LIMB_T *b_tab, AI_BASE_FLOAT32_LIMB_T b_len, INT32_T mul_flags);
#else /* AI_BASE_FLOAT32_FFT_MUL */
INT32_T AiBaseFloat32BfGetFftSize(INT32_T *pdpl, INT32_T *pnb_mods, AI_BASE_FLOAT32_LIMB_T len);
#endif /* !AI_BASE_FLOAT32_FFT_MUL */

/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/



