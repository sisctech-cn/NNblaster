/**
 ******************************************************************************
 * file    ai_base_float16_cvt64.h
 * author  sunlingge
 * version V100
 * brief   the ai base float 16 convert 64 driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT16_CVT64_H
#define AI_BASE_FLOAT16_CVT64_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP16 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Pre Type Definition -----------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT16_ICVT_SIZE64		(64)
#define AI_BASE_FLOAT16_ICVT_UINT64		UINT64_T
#define AI_BASE_FLOAT16_ICVT_INT64		INT64_T

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
AI_BASE_FLOAT16_ICVT_INT64 AiBaseFloat16InternalCvtSf16I64(AI_BASE_FLOAT16_F_UINT a, AI_BASE_FLOAT16_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags, BOOL_T is_unsigned);
AI_BASE_FLOAT16_ICVT_INT64 AiBaseFloat16CvtSf16I64(AI_BASE_FLOAT16_F_UINT a, AI_BASE_FLOAT16_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT16_ICVT_UINT64 AiBaseFloat16CvtSf16U64(AI_BASE_FLOAT16_F_UINT a, AI_BASE_FLOAT16_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT16_F_UINT AiBaseFloat16InternalCvtI64Sf16(AI_BASE_FLOAT16_ICVT_INT64 a, AI_BASE_FLOAT16_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags, BOOL_T is_unsigned);
AI_BASE_FLOAT16_F_UINT AiBaseFloat16CvtI64Sf16(AI_BASE_FLOAT16_ICVT_INT64 a, AI_BASE_FLOAT16_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT16_F_UINT AiBaseFloat16CvtU64Sf16(AI_BASE_FLOAT16_ICVT_UINT64 a, AI_BASE_FLOAT16_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);

/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/


