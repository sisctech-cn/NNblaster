/**
 ******************************************************************************
 * file    ai_base_float16_general.h
 * author  sunlingge
 * version V100
 * brief   the ai base float 16 general driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT16_GENERAL_H
#define AI_BASE_FLOAT16_GENERAL_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP16 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT16_FFLAG_INVALID_OP					(1 << 4)
#define AI_BASE_FLOAT16_FFLAG_DIVIDE_ZERO					(1 << 3)
#define AI_BASE_FLOAT16_FFLAG_OVERFLOW					(1 << 2)
#define AI_BASE_FLOAT16_FFLAG_UNDERFLOW					(1 << 1)
#define AI_BASE_FLOAT16_FFLAG_INEXACT						(1 << 0)

#define AI_BASE_FLOAT16_FCLASS_NINF						(1 << 0)
#define AI_BASE_FLOAT16_FCLASS_NNORMAL					(1 << 1)
#define AI_BASE_FLOAT16_FCLASS_NSUBNORMAL					(1 << 2)
#define AI_BASE_FLOAT16_FCLASS_NZERO						(1 << 3)
#define AI_BASE_FLOAT16_FCLASS_PZERO						(1 << 4)
#define AI_BASE_FLOAT16_FCLASS_PSUBNORMAL					(1 << 5)
#define AI_BASE_FLOAT16_FCLASS_PNORMAL					(1 << 6)
#define AI_BASE_FLOAT16_FCLASS_PINF						(1 << 7)
#define AI_BASE_FLOAT16_FCLASS_SNAN						(1 << 8)
#define AI_BASE_FLOAT16_FCLASS_QNAN						(1 << 9)

#define AI_BASE_FLOAT16_XGLUE(x, y)						(x##y)
#define AI_BASE_FLOAT16_GLUE(x, y)						AI_BASE_FLOAT16_XGLUE(x, y)

/*------------------------- Type Definition----------------------------------*/
typedef enum {
    AI_BASE_FLOAT16_RM_RNE,			/* Round to Nearest, ties to Even */
    AI_BASE_FLOAT16_RM_RTZ,			/* Round towards Zero */
    AI_BASE_FLOAT16_RM_RDN,			/* Round Down */
    AI_BASE_FLOAT16_RM_RUP,			/* Round Up */
    AI_BASE_FLOAT16_RM_RMM,			/* Round to Nearest, ties to Max Magnitude */
} AI_BASE_FLOAT16_ROUNDING_MODE_ENUM;

typedef void *AI_BASE_FLOAT16_DYN_BUF_REALLOC_FUNC(void *opaque, void *ptr, UINT32_T size);

typedef struct ai_base_float16_dyn_buf_struct {
    UINT8_T *buf;
    UINT32_T size;
    UINT32_T allocated_size;
    BOOL_T error;						/* true if a memory allocation error occurred */
    AI_BASE_FLOAT16_DYN_BUF_REALLOC_FUNC *realloc_func;
    void *opaque;					/* for realloc_func */
	UINT8_T buffer_s[16];
} AI_BASE_FLOAT16_DYN_BUF, *AI_BASE_FLOAT16_DYN_BUF_PTR;

/*------------------------- Function Definition -----------------------------*/
INT32_T AiBaseFloat16BuiltinClz(UINT32_T x);
INT32_T AiBaseFloat16ClzU16(UINT16_T a);
INT32_T AiBaseFloat16ClzU32(UINT32_T a);
INT32_T AiBaseFloat16ClzU64(UINT64_T a);
INT32_T AiBaseFloat16BuiltinCtz(UINT32_T x);
INT32_T AiBaseFloat16CtzU16(UINT16_T a);
INT32_T AiBaseFloat16CtzU32(UINT32_T a);
INT32_T AiBaseFloat16CtzU64(UINT64_T a);
void AiBaseFloat16Pstrcpy(INT8_T *buf, INT32_T buf_size, const INT8_T *str);
INT8_T *AiBaseFloat16Pstrcat(INT8_T *buf, INT32_T buf_size, const INT8_T *s);
INT32_T AiBaseFloat16Strstart(const INT8_T *str, const INT8_T *val, const INT8_T **ptr);
void AiBaseFloat16DbufInit2(AI_BASE_FLOAT16_DYN_BUF *s, void *opaque, AI_BASE_FLOAT16_DYN_BUF_REALLOC_FUNC *realloc_func);
static void *AiBaseFloat16DbufDefaultRealloc(void *opaque, void *ptr, UINT32_T size);
void AiBaseFloat16DbufInit(AI_BASE_FLOAT16_DYN_BUF *s);
INT32_T AiBaseFloat16DbufRealloc(AI_BASE_FLOAT16_DYN_BUF *s, UINT32_T new_size);
INT32_T AiBaseFloat16DbufWrite(AI_BASE_FLOAT16_DYN_BUF *s, UINT32_T offset, const UINT8_T *data, UINT32_T len);
INT32_T AiBaseFloat16DbufPut(AI_BASE_FLOAT16_DYN_BUF *s, const UINT8_T *data, UINT32_T len);
INT32_T AiBaseFloat16DbufPutc(AI_BASE_FLOAT16_DYN_BUF *s, UINT8_T c);
INT32_T AiBaseFloat16DbufPutstr(AI_BASE_FLOAT16_DYN_BUF *s, const INT8_T *str);
INT32_T AiBaseFloat16DbufPrintf(AI_BASE_FLOAT16_DYN_BUF *s, const INT8_T *fmt, ...);
void AiBaseFloat16DbufFree(AI_BASE_FLOAT16_DYN_BUF *s);

/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/


