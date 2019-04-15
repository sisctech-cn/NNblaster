/**
 ******************************************************************************
 * file    ai_base_float_bf32_general.h
 * author  sunlingge
 * version V100
 * brief   the ai base float bf 32 general driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT32_BF_GENERAL_H
#define AI_BASE_FLOAT32_BF_GENERAL_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP32 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT32_FALSE								(0)
#define AI_BASE_FLOAT32_TRUE								(1)

/*------------------------- Macro Definition --------------------------------*/
/* allow subnormal numbers (only available if the number of exponent bits is < BF_EXP_BITS_MAX and prec != BF_PREC_INF) */
/* bf_flag: expt_bits:111111, subnormal:1, rnd: 111 */
#define AI_BASE_FLOAT32_BF_FLAG_SUBNORMAL					(1 << 3)
#define AI_BASE_FLOAT32_BF_RND_MASK							(0x7)
#define AI_BASE_FLOAT32_BF_EXP_BITS_SHIFT					(4)
#define AI_BASE_FLOAT32_BF_EXP_BITS_MASK					(0x3F)

/* maximum radix for AiBaseFloatBfatof() and AiBaseFloatBfftoa() */
#define AI_BASE_FLOAT32_BF_RADIX_MAX						(36) 

/* returned status */
#define AI_BASE_FLOAT32_ERROR_SUCCESS						(0)
#define AI_BASE_FLOAT32_ERROR_GENERAL						((ERROR_FLOAT32_T)(-1))
#define AI_BASE_FLOAT32_BF_ST_INVALID_OP					(1 << 0)
#define AI_BASE_FLOAT32_BF_ST_DIVIDE_ZERO					(1 << 1)
#define AI_BASE_FLOAT32_BF_ST_OVERFLOW						(1 << 2)
#define AI_BASE_FLOAT32_BF_ST_UNDERFLOW						(1 << 3)
#define AI_BASE_FLOAT32_BF_ST_INEXACT						(1 << 4) 
/* not used yet, indicate that a memory allocation error occured. NaN is returned */
#define AI_BASE_FLOAT32_BF_ST_MEM_ERROR						(1 << 5)
/* additional flags for AiBaseFloatBfatof */
/* do not accept hex radix prefix (0x or 0X) if radix = 0 or radix = 16 */
#define AI_BASE_FLOAT32_BF_ATOF_NO_HEX						(1 << 16)
/* accept binary (0b or 0B) or octal (0o or 0O) radix prefix if radix = 0 */
#define AI_BASE_FLOAT32_BF_ATOF_BIN_OCT						(1 << 17)
/* Only accept integers (no decimal point, no exponent, no infinity nor NaN */
#define AI_BASE_FLOAT32_BF_ATOF_INT_ONLY					(1 << 18)
/* Do not accept radix prefix after sign  */
#define AI_BASE_FLOAT32_BF_ATOF_NO_PREFIX_AFTER_SIGN		(1 << 19)
/* Do not parse NaN and parse case sensitive 'Infinity' */
#define AI_BASE_FLOAT32_BF_ATOF_JS_QUIRKS					(1 << 20)
/* Do not round integers to the indicated precision */
#define AI_BASE_FLOAT32_BF_ATOF_INT_PREC_INF				(1 << 21)

/* one more return flag: indicate that the parsed number contains no
   decimal point nor exponent */
#define AI_BASE_FLOAT32_BF_ATOF_ST_INTEGER				(1 << 5)
#define AI_BASE_FLOAT32_BF_FTOA_FORMAT_MASK				(3 << 16)
/* fixed format: prec significant digits rounded with (flags &
   BF_RND_MASK). Exponential notation is used if too many zeros are
   needed. */
#define AI_BASE_FLOAT32_BF_FTOA_FORMAT_FIXED			(0 << 16)
/* fractional format: prec digits after the decimal point rounded with
   (flags & BF_RND_MASK) */
#define AI_BASE_FLOAT32_BF_FTOA_FORMAT_FRAC				(1 << 16)
/* free format: use as many digits as necessary so that AiBaseFloatBfatof()
   return the same number when using precision 'prec', rounding to
   nearest and the subnormal+exponent configuration of 'flags'. The
   result is meaningful only if 'a' is already rounded to the wanted
   precision.
   
   Infinite precision (BF_PREC_INF) is supported when the radix is a
   power of two. */
#define AI_BASE_FLOAT32_BF_FTOA_FORMAT_FREE				(2 << 16)
/* same as BF_FTOA_FORMAT_FREE but uses the minimum number of digits
   (takes more computation time). */
#define AI_BASE_FLOAT32_BF_FTOA_FORMAT_FREE_MIN			(3 << 16)

/* force exponential notation for fixed or free format */
#define AI_BASE_FLOAT32_BF_FTOA_FORCE_EXP				(1 << 20)
/* add 0x prefix for base 16, 0o prefix for base 8 or 0b prefix for
   base 2 if non zero value */
#define AI_BASE_FLOAT32_BF_FTOA_ADD_PREFIX				(1 << 21)
#define AI_BASE_FLOAT32_BF_FTOA_JS_QUIRKS				(1 << 22)

#define AI_BASE_FLOAT32_BF_DIVREM_EUCLIDIAN				(5)

/* modulo 2^n instead of saturation. NaN and infinity return 0 */
#define AI_BASE_FLOAT32_BF_GET_INT_MOD					(1 << 0) 

/* enable it to check the multiplication result */
//#define AI_BASE_FLOAT32_USE_MUL_CHECK
/* enable it to use FFT/NTT multiplication */
#define AI_BASE_FLOAT32_USE_FFT_MUL

/* in limbs of the smallest factor */
#define AI_BASE_FLOAT32_FFT_MUL_THRESHOLD					(100) 

/* XXX: adjust */
#define AI_BASE_FLOAT32_BASECASE_DIV_THRESHOLD_B			(300)
#define AI_BASE_FLOAT32_BASECASE_DIV_THRESHOLD_Q			(300)

#ifdef AI_BASE_FLOAT32_USE_FFT_MUL
#define AI_BASE_FLOAT32_FFT_MUL_R_OVERLAP_A				(1 << 0)
#define AI_BASE_FLOAT32_FFT_MUL_R_OVERLAP_B				(1 << 1)
#endif

#define AI_BASE_FLOAT32_BF_CMP_EQ							(1)
#define AI_BASE_FLOAT32_BF_CMP_LT							(2)
#define AI_BASE_FLOAT32_BF_CMP_LE							(3)

/* logical operations */
#define AI_BASE_FLOAT32_BF_LOGIC_OR						(0)
#define AI_BASE_FLOAT32_BF_LOGIC_XOR						(1)
#define AI_BASE_FLOAT32_BF_LOGIC_AND						(2)

/* base conversion to radix */
#define AI_BASE_FLOAT32_RADIXL_10							(UINT64_C(1000000000))

/* PI constant */
#define AI_BASE_FLOAT32_CHUD_A							(13591409)
#define AI_BASE_FLOAT32_CHUD_B							(545140134)
#define AI_BASE_FLOAT32_CHUD_C							(640320)
#define AI_BASE_FLOAT32_CHUD_BITS_PER_TERM				(47)
/* print */
#define AI_BASE_FLOAT32_FMT_LIMB1							("%x")
#define AI_BASE_FLOAT32_FMT_LIMB							("%08x")
#define AI_BASE_FLOAT32_PRID_LIMB							"d"
#define AI_BASE_FLOAT32_PRIU_LIMB							("u")

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/

/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

