/**
 ******************************************************************************
 * file    ai_base_float16_cvtfloat.h
 * author  sunlingge
 * version V100
 * brief   the ai base float 16 convert float driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT16_CVTFLOAT_H
#define AI_BASE_FLOAT16_CVTFLOAT_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP16 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Pre Type Definition -----------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
INT32_T AiBaseFloat16BfGetFloat32(const AI_BASE_FLOAT16_BF_T *a, FLOAT32_T *pres, AI_BASE_FLOAT16_BF_RND_T rnd_mode);
void AiBaseFloat16BfSetFloat32(AI_BASE_FLOAT16_BF_T *a, FLOAT32_T d);
FLOAT16_T AiBaseFloat16CvtF32F16(FLOAT32_T value);

/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/


