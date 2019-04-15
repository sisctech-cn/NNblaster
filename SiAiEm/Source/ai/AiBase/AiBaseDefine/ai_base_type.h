/**
 ******************************************************************************
 * file    ai_base_type.h
 * author  sunlingge
 * version V100
 * brief   the ai base type definition.
 ******************************************************************************
 */
#ifndef AI_BASE_DATA_TYPE_H
#define AI_BASE_DATA_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#ifndef NULL
#define NULL    ((void *)0)
#endif
#ifndef __IO
#define __IO volatile
#endif
#ifndef __I
#define __I volatile
#endif

/*------------------------- Type Definition----------------------------------*/
/* interger define */
typedef int8_t INT8_T, *INT8_T_PTR;
typedef uint8_t UINT8_T, *UINT8_T_PTR;
typedef int16_t INT16_T, *INT16_T_PTR;
typedef uint16_t UINT16_T, *UINT16_T_PTR;
typedef int32_t INT32_T, *INT32_T_PTR;
typedef uint32_t UINT32_T, *UINT32_T_PTR;
typedef int64_t INT64_T, *INT64_T_PTR;
typedef uint64_t UINT64_T, *UINT64_T_PTR;
/* bool define */
typedef int	BOOL_T, *BOOL_T_PTR;
/* float define */
typedef UINT16_T FLOAT16_T, *FLOAT16_T_PTR;
typedef float FLOAT32_T, *FLOAT32_T_PTR;
typedef double FLOAT64_T, *FLOAT64_T_PTR;
/* soft float define */
typedef UINT16_T SFLOAT16_T, *SFLOAT16_T_PTR;
typedef UINT32_T SFLOAT32_T, *SFLOAT32_T_PTR;
typedef UINT64_T SFLOAT64_T, *SFLOAT64_T_PTR;
/* error define */
typedef int32_t ERROR_T;
/* float error define */
typedef int32_t ERROR_FLOAT16_T;
typedef int32_t ERROR_FLOAT32_T;
typedef int32_t ERROR_FLOAT64_T;
typedef FLOAT64_T DOUBLE_AI_T, *DOUBLE_AI_T_PTR;
/* address typedef */
typedef UINT64_T  VOID_T, *VOID_T_PTR;

/*------------------------- Function Definition -----------------------------*/

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
