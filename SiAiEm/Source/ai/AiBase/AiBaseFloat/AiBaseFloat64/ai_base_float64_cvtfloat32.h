/**
 ******************************************************************************
 * file    ai_base_float64_cvtfloat.h
 * author  sunlingge
 * version V100
 * brief   the ai base float 32 convert float driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT64_CVTFLOAT_H
#define AI_BASE_FLOAT64_CVTFLOAT_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP64 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Pre Type Definition -----------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
INT32_T AiBaseFloat64BfGetFloat32(const AI_BASE_FLOAT64_BF_T *a, FLOAT32_T *pres, AI_BASE_FLOAT64_BF_RND_T rnd_mode);
void AiBaseFloat64BfSetFloat32(AI_BASE_FLOAT64_BF_T *a, FLOAT32_T d);


/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/


