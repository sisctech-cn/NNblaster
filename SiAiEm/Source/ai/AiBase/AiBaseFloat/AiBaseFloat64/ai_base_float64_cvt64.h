/**
 ******************************************************************************
 * file    ai_base_float64_cvt64.h
 * author  sunlingge
 * version V100
 * brief   the ai base float 32 convert 64 driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT64_CVT64_H
#define AI_BASE_FLOAT64_CVT64_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP64 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Pre Type Definition -----------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT64_ICVT_SIZE64		(64)
#define AI_BASE_FLOAT64_ICVT_UINT64		UINT64_T
#define AI_BASE_FLOAT64_ICVT_INT64		INT64_T

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
AI_BASE_FLOAT64_ICVT_INT64 AiBaseFloat64InternalCvt32I64(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags, BOOL_T is_unsigned);
AI_BASE_FLOAT64_ICVT_INT64 AiBaseFloat64CvtSf32I64(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT64_ICVT_UINT64 AiBaseFloat64CvtSf32U64(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT64_F_UINT AiBaseFloat64InternalCvtI64Sf32(AI_BASE_FLOAT64_ICVT_INT64 a, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags, BOOL_T is_unsigned);
AI_BASE_FLOAT64_F_UINT AiBaseFloat64CvtI64Sf32(AI_BASE_FLOAT64_ICVT_INT64 a, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT64_F_UINT AiBaseFloat64CvtU64Sf32(AI_BASE_FLOAT64_ICVT_UINT64 a, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);

/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/


