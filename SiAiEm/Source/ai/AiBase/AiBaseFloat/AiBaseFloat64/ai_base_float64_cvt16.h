/**
 ******************************************************************************
 * file    ai_base_float64_cvt16.h
 * author  sunlingge
 * version V100
 * brief   the ai base float 32 convet 16 driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT64_CVT16_H
#define AI_BASE_FLOAT64_CVT16_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP64 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Pre Type Definition -----------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT64_ICVT_SIZE16		(16)
#define AI_BASE_FLOAT64_ICVT_UINT16		UINT16_T
#define AI_BASE_FLOAT64_ICVT_INT16		INT16_T

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
AI_BASE_FLOAT64_ICVT_INT16 AiBaseFloat64InternalCvtSf16I16(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags, BOOL_T is_unsigned);
AI_BASE_FLOAT64_ICVT_INT16 AiBaseFloat64CvtSf16I16(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT64_ICVT_UINT16 AiBaseFloat64CvtSf16U16(AI_BASE_FLOAT64_F_UINT a, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT64_F_UINT AiBaseFloat64InternalCvtI16Sf16(AI_BASE_FLOAT64_ICVT_INT16 a, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags, BOOL_T is_unsigned);
AI_BASE_FLOAT64_F_UINT AiBaseFloat64CvtI16Sf16(AI_BASE_FLOAT64_ICVT_INT16 a, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT64_F_UINT AiBaseFloat64CvtU16Sf16(AI_BASE_FLOAT64_ICVT_UINT16 a, AI_BASE_FLOAT64_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);

/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/


