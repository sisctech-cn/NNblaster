/**
 ******************************************************************************
 * file    ai_base_float32_cvtfloat32.h
 * author  sunlingge
 * version V100
 * brief   the ai base float 32 convert float 32 driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT32_CVTFLOAT32_H
#define AI_BASE_FLOAT32_CVTFLOAT32_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP32 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Pre Type Definition -----------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT32_FP32_SIGN_SHIFT			(31)
#define AI_BASE_FLOAT32_FP32_SIGN_LENGTH		(1)
#define AI_BASE_FLOAT32_FP32_EXP_SHIFT			(23)
#define AI_BASE_FLOAT32_FP32_EXP_LENGTH			(8)
#define AI_BASE_FLOAT32_FP32_MANT_SHIFT			(0)
#define AI_BASE_FLOAT32_FP32_MANT_LENGTH		(23)
#define AI_BASE_FLOAT32_FP32_BIAS				(127)
/* sunlingge modify to linsen float consist */
#define AI_BASE_FLOAT32_FP32_QUIET_NAN			(0xFFC00000)
#define AI_BASE_FLOAT32_FP32_ROUND_PREC			(24)


/*------------------------- Type Definition----------------------------------*/
/* conversion between fixed size types */
typedef union {
    FLOAT32_T d;
    UINT32_T u;
} AI_BASE_FLOAT32_UNION32;

/*------------------------- Function Definition -----------------------------*/
ERROR_FLOAT32_T AiBaseFloat32BfGetFloat32(const AI_BASE_FLOAT32_BF_T *a, FLOAT32_T *pres, AI_BASE_FLOAT32_BF_RND_T rnd_mode);
ERROR_FLOAT32_T AiBaseFloat32BfSetFloat32(AI_BASE_FLOAT32_BF_T *a, FLOAT32_T d);


/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/


