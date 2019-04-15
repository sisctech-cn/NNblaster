/**
 ******************************************************************************
 * file    ai_base_float32_cvt32.h
 * author  sunlingge
 * version V100
 * brief   the ai base float 32 convert 32 driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT32_CVT32_H
#define AI_BASE_FLOAT32_CVT32_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP32 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Pre Type Definition -----------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT32_ICVT_SIZE32		(32)
#define AI_BASE_FLOAT32_ICVT_UINT32		UINT32_T
#define AI_BASE_FLOAT32_ICVT_INT32		INT32_T

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
AI_BASE_FLOAT32_ICVT_INT32 AiBaseFloat32InternalCvtSf32I32(AI_BASE_FLOAT32_F_UINT a, AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags, BOOL_T is_unsigned);
AI_BASE_FLOAT32_ICVT_INT32 AiBaseFloat32CvtSf32I32(AI_BASE_FLOAT32_F_UINT a, AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT32_ICVT_UINT32 AiBaseFloat32CvtSf32U32(AI_BASE_FLOAT32_F_UINT a, AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT32_F_UINT AiBaseFloat32InternalCvtI32Sf32(AI_BASE_FLOAT32_ICVT_INT32 a, AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags, BOOL_T is_unsigned);
AI_BASE_FLOAT32_F_UINT AiBaseFloat32CvtI32Sf32(AI_BASE_FLOAT32_ICVT_INT32 a, AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT32_F_UINT AiBaseFloat32CvtU32Sf32(AI_BASE_FLOAT32_ICVT_UINT32 a, AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);

/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/


