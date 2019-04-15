/**
 ******************************************************************************
 * file    ai_base_float16_cvtfloat16.h
 * author  sunlingge
 * version V100
 * brief   the ai base float 16 convert float 16 driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT16_CVTFLOAT16_H
#define AI_BASE_FLOAT16_CVTFLOAT16_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP16 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Pre Type Definition -----------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT16_FP16_SIGN_SHIFT			(15)
#define AI_BASE_FLOAT16_FP16_SIGN_LENGTH		(1)
#define AI_BASE_FLOAT16_FP16_EXP_SHIFT			(10)
#define AI_BASE_FLOAT16_FP16_EXP_LENGTH			(5)
#define AI_BASE_FLOAT16_FP16_MANT_SHIFT			(0)
#define AI_BASE_FLOAT16_FP16_MANT_LENGTH		(10)
#define AI_BASE_FLOAT16_FP16_BIAS				(15)
/* sunlingge modify to linsen float consist */
#define AI_BASE_FLOAT16_FP16_QUIET_NAN			(0xFE00)
#define AI_BASE_FLOAT16_FP16_ROUND_PREC			(11)

/*------------------------- Type Definition----------------------------------*/
/* conversion between fixed size types */
typedef union {
    FLOAT16_T d;
    UINT16_T u;
} AI_BASE_FLOAT16_UNION16;

/*------------------------- Function Definition -----------------------------*/
ERROR_FLOAT16_T AiBaseFloat16BfGetFloat16(const AI_BASE_FLOAT16_BF_T *a, FLOAT16_T *pres, AI_BASE_FLOAT16_BF_RND_T rnd_mode);
ERROR_FLOAT16_T AiBaseFloat16BfSetFloat16(AI_BASE_FLOAT16_BF_T *a, FLOAT16_T d);

/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/


