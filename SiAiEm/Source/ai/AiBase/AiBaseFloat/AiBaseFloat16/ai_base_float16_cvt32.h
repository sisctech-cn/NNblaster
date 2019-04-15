/**
 ******************************************************************************
 * file    ai_base_float16_cvt32.h
 * author  sunlingge
 * version V100
 * brief   the ai base float 16 convert 32 driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT16_CVT32_H
#define AI_BASE_FLOAT16_CVT32_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP16 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Pre Type Definition -----------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT16_ICVT_SIZE32		(32)
#define AI_BASE_FLOAT16_ICVT_UINT32		UINT32_T
#define AI_BASE_FLOAT16_ICVT_INT32		INT32_T

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
AI_BASE_FLOAT16_ICVT_INT32 AiBaseFloat16InternalCvtSf16I32(AI_BASE_FLOAT16_F_UINT a, AI_BASE_FLOAT16_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags, BOOL_T is_unsigned);
AI_BASE_FLOAT16_ICVT_INT32 AiBaseFloat16CvtSf16I32(AI_BASE_FLOAT16_F_UINT a, AI_BASE_FLOAT16_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT16_ICVT_UINT32 AiBaseFloat16CvtSf16U32(AI_BASE_FLOAT16_F_UINT a, AI_BASE_FLOAT16_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT16_F_UINT AiBaseFloat16InternalCvtI32Sf16(AI_BASE_FLOAT16_ICVT_INT32 a, AI_BASE_FLOAT16_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags, BOOL_T is_unsigned);
AI_BASE_FLOAT16_F_UINT AiBaseFloat16CvtI32Sf16(AI_BASE_FLOAT16_ICVT_INT32 a, AI_BASE_FLOAT16_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT16_F_UINT AiBaseFloat16CvtU32Sf16(AI_BASE_FLOAT16_ICVT_UINT32 a, AI_BASE_FLOAT16_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
FLOAT16_T AiBaseFloat16CvtI32F16(INT32_T a);

/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/


