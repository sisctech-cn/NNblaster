/**
 ******************************************************************************
 * file    ai_base_float32_cvtdouble.h
 * author  sunlingge
 * version V100
 * brief   the ai base float 32 convert double driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT32_CVTDOUBLE_H
#define AI_BASE_FLOAT32_CVTDOUBLE_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP32 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Pre Type Definition -----------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
INT32_T AiBaseFloat32BfGetFloat64(const AI_BASE_FLOAT32_BF_T *a, FLOAT64_T *pres, AI_BASE_FLOAT32_BF_RND_T rnd_mode);
void AiBaseFloat32BfSetFloat64(AI_BASE_FLOAT32_BF_T *a, FLOAT64_T d);

/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/


