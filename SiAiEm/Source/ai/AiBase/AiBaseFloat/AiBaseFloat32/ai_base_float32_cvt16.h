/**
 ******************************************************************************
 * file    ai_base_float32_cvt16.h
 * author  sunlingge
 * version V100
 * brief   the ai base float 32 convet 16 driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT32_CVT16_H
#define AI_BASE_FLOAT32_CVT16_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP32 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Pre Type Definition -----------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT32_ICVT_SIZE16		(16)
#define AI_BASE_FLOAT32_ICVT_UINT16		UINT16_T
#define AI_BASE_FLOAT32_ICVT_INT16		INT16_T

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
AI_BASE_FLOAT32_ICVT_INT16 AiBaseFloat32InternalCvtSf32I16(AI_BASE_FLOAT32_F_UINT a, AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags, BOOL_T is_unsigned);
AI_BASE_FLOAT32_ICVT_INT16 AiBaseFloat32CvtSf32I16(AI_BASE_FLOAT32_F_UINT a, AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT32_ICVT_UINT16 AiBaseFloat32CvtSf32U16(AI_BASE_FLOAT32_F_UINT a, AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT32_F_UINT AiBaseFloat32InternalCvtI16Sf32(AI_BASE_FLOAT32_ICVT_INT16 a, AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags, BOOL_T is_unsigned);
AI_BASE_FLOAT32_F_UINT AiBaseFloat32CvtI16Sf32(AI_BASE_FLOAT32_ICVT_INT16 a, AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);
AI_BASE_FLOAT32_F_UINT AiBaseFloat32CvtU16Sf32(AI_BASE_FLOAT32_ICVT_UINT16 a, AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm, UINT32_T *pfflags);

/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/


