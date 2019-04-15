/**
 ******************************************************************************
 * file    ai_base_float_oper.h
 * author  sunlingge
 * version V100
 * brief   the ai base float oper driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT_OPER_H
#define AI_BASE_FLOAT_OPER_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
FLOAT_AI_T AiBaseFloatCvtF32Fai(FLOAT32_T value);
FLOAT32_T AiBaseFloatCvtFaiF32(FLOAT_AI_T value);
FLOAT_AI_T AiBaseFloatCvtI32Fai(INT32_T value);
INT32_T AiBaseFloatCvtFaiI32(FLOAT_AI_T value);
INT32_T AiBaseFloatCmpEq(FLOAT_AI_T a, FLOAT_AI_T b);
INT32_T AiBaseFloatCmpLt(FLOAT_AI_T a, FLOAT_AI_T b);
INT32_T AiBaseFloatCmpLe(FLOAT_AI_T a, FLOAT_AI_T b);
INT32_T AiBaseFloatCmpGt(FLOAT_AI_T a, FLOAT_AI_T b);
INT32_T AiBaseFloatCmpGe(FLOAT_AI_T a, FLOAT_AI_T b);
FLOAT_AI_T AiBaseFloatAdd(FLOAT_AI_T a, FLOAT_AI_T b);
FLOAT_AI_T AiBaseFloatSub(FLOAT_AI_T a, FLOAT_AI_T b);
FLOAT_AI_T AiBaseFloatMul(FLOAT_AI_T a, FLOAT_AI_T b);
FLOAT_AI_T AiBaseFloatDiv(FLOAT_AI_T a, FLOAT_AI_T b);
FLOAT_AI_T AiBaseFloatExp(FLOAT_AI_T a);
FLOAT_AI_T AiBaseFloatSqrt(FLOAT_AI_T a);
FLOAT_AI_T AiBaseFloatFloor(FLOAT_AI_T a);
FLOAT_AI_T AiBaseFloatCeil(FLOAT_AI_T a);

/*------------------------- End ---------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/


