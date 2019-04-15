/**
 ******************************************************************************
 * file    ai_base_float32_cvtfloat16.h
 * author  sunlingge
 * version V100
 * brief   the ai base float 32 convert float 16 driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT32_CVTFLOAT16_H
#define AI_BASE_FLOAT32_CVTFLOAT16_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP32 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Pre Type Definition -----------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
INT32_T AiBaseFloat32BfGetFloat16(const AI_BASE_FLOAT32_BF_T *a, FLOAT16_T *pres, AI_BASE_FLOAT32_BF_RND_T rnd_mode);
void AiBaseFloat32BfSetFloat16(AI_BASE_FLOAT32_BF_T *a, FLOAT16_T d);
FLOAT16_T AiBaseFloat32CvtF32F16(FLOAT32_T value);
FLOAT32_T AiBaseFloat32CvtF16F32(FLOAT16_T value);

/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/


