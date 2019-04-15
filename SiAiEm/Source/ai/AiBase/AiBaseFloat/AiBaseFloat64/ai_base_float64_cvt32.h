/**
 ******************************************************************************
 * file    ai_base_float64_cvt32.h
 * author  sunlingge
 * version V100
 * brief   the ai base float 32 convert 32 driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT64_CVT32_H
#define AI_BASE_FLOAT64_CVT32_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP64 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Pre Type Definition -----------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT64_ICVT_SIZE32		(32)
#define AI_BASE_FLOAT64_ICVT_UINT32		UINT32_T
#define AI_BASE_FLOAT64_ICVT_INT32		INT32_T

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
AI_BASE_FLOAT64_ICVT_INT32 AiBaseFloat64InternalCvtSf32I32(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags, BOOL_T is_unsigned);
AI_BASE_FLOAT64_ICVT_INT32 AiBaseFloat64CvtSf32I32(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT64_ICVT_UINT32 AiBaseFloat64CvtSf32U32(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT64_F_UINT AiBaseFloat64InternalCvtI32Sf32(AI_BASE_FLOAT64_ICVT_INT32 a, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags, BOOL_T is_unsigned);
AI_BASE_FLOAT64_F_UINT AiBaseFloat64CvtI32Sf32(AI_BASE_FLOAT64_ICVT_INT32 a, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT64_F_UINT AiBaseFloat64CvtU32Sf32(AI_BASE_FLOAT64_ICVT_UINT32 a, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);

/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/


