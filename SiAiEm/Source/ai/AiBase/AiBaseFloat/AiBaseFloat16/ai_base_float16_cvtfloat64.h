/**
 ******************************************************************************
 * file    ai_base_float16_cvtdouble.h
 * author  sunlingge
 * version V100
 * brief   the ai base float 16 convert double driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT16_CVTDOUBLE_H
#define AI_BASE_FLOAT16_CVTDOUBLE_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP16 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Pre Type Definition -----------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
INT32_T AiBaseFloat16BfGetFloat64(const AI_BASE_FLOAT16_BF_T *a, FLOAT64_T *pres, AI_BASE_FLOAT16_BF_RND_T rnd_mode);
void AiBaseFloat16BfSetFloat64(AI_BASE_FLOAT16_BF_T *a, FLOAT64_T d);

/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/


