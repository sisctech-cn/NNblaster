/**
 ******************************************************************************
 * file    ai_base_float64_cvtfloat64.h
 * author  sunlingge
 * version V100
 * brief   the ai base float 32 convert float 64 driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT64_CVTFLOAT64_H
#define AI_BASE_FLOAT64_CVTFLOAT64_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP64 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Pre Type Definition -----------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT64_FP64_SIGN_SHIFT			(63)
#define AI_BASE_FLOAT64_FP64_SIGN_LENGTH		(1)
#define AI_BASE_FLOAT64_FP64_EXP_SHIFT			(52)
#define AI_BASE_FLOAT64_FP64_EXP_LENGTH			(11)
#define AI_BASE_FLOAT64_FP64_MANT_SHIFT			(0)
#define AI_BASE_FLOAT64_FP64_MANT_LENGTH		(52)
#define AI_BASE_FLOAT64_FP64_BIAS				(1023)
/* sunlingge modify to linsen float consist */
#define AI_BASE_FLOAT64_FP64_QUIET_NAN			(0xFFF8000000000000)
#define AI_BASE_FLOAT64_FP64_ROUND_PREC			(53)

/*------------------------- Type Definition----------------------------------*/
/* conversion between fixed size types */
typedef union {
    FLOAT64_T d;
    UINT64_T u;
} AI_BASE_FLOAT64_UNION64;

/*------------------------- Function Definition -----------------------------*/
ERROR_FLOAT64_T AiBaseFloat64BfGetFloat64(const AI_BASE_FLOAT64_BF_T *a, FLOAT64_T *pres, AI_BASE_FLOAT64_BF_RND_T rnd_mode);
ERROR_FLOAT64_T AiBaseFloat64BfSetFloat64(AI_BASE_FLOAT64_BF_T *a, FLOAT64_T d);


/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/


