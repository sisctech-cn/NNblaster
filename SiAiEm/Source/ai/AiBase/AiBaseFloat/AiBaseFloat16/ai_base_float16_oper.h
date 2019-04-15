/**
 ******************************************************************************
 * file    ai_base_float16_oper.h
 * author  sunlingge
 * version V100
 * brief   the ai base float16 oper driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT16_OPER_H
#define AI_BASE_FLOAT16_OPER_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP16 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT16_SPECIAL_COUNT			(7)
#define AI_BASE_FLOAT16_F64_MANT_SIZE			(52)
#define AI_BASE_FLOAT16_F64_EXP_MASK			((1 << 11) - 1)

/*------------------------- Type Definition----------------------------------*/
typedef enum {
    AI_BASE_FLOAT16_OP_MUL,					/* mul				*/
    AI_BASE_FLOAT16_OP_ADD,					/* add				*/
    AI_BASE_FLOAT16_OP_SUB,					/* sub				*/
    AI_BASE_FLOAT16_OP_RINT,				/* rint				*/
    AI_BASE_FLOAT16_OP_ROUND,				/* round			*/
    AI_BASE_FLOAT16_OP_CMP_EQ,				/* compare eq		*/
    AI_BASE_FLOAT16_OP_CMP_LT,				/* compare lt		*/
    AI_BASE_FLOAT16_OP_CMP_LE,				/* compare le		*/
    AI_BASE_FLOAT16_OP_RECIP,				/* recip			*/
    AI_BASE_FLOAT16_OP_DIV,					/* div				*/
    AI_BASE_FLOAT16_OP_FMOD,				/* fmod				*/
    AI_BASE_FLOAT16_OP_REM,					/* rem				*/
    AI_BASE_FLOAT16_OP_RSQRT,				/* rsqrt			*/
    AI_BASE_FLOAT16_OP_SQRT,				/* sqrt				*/
    AI_BASE_FLOAT16_OP_OR,					/* or				*/
    AI_BASE_FLOAT16_OP_XOR,					/* xor				*/
    AI_BASE_FLOAT16_OP_AND,					/* and				*/
    AI_BASE_FLOAT16_OP_CAN_ROUND,			/* can round		*/
    AI_BASE_FLOAT16_OP_MUL_L2RADIX,			/* mul l2radix		*/
    AI_BASE_FLOAT16_OP_DIV_L2RADIX,			/* div l2radix		*/
    AI_BASE_FLOAT16_OP_ATOF,				/* atof				*/
    AI_BASE_FLOAT16_OP_FTOA,				/* ftoa				*/
    AI_BASE_FLOAT16_OP_EXP,					/* exp				*/
    AI_BASE_FLOAT16_OP_LOG,					/* log				*/
    AI_BASE_FLOAT16_OP_COS,					/* cos				*/
    AI_BASE_FLOAT16_OP_SIN,					/* sin				*/
    AI_BASE_FLOAT16_OP_TAN,					/* tan				*/
    AI_BASE_FLOAT16_OP_ATAN,				/* atan				*/
    AI_BASE_FLOAT16_OP_ATAN2,				/* atan2			*/
    AI_BASE_FLOAT16_OP_ASIN,				/* asin				*/
    AI_BASE_FLOAT16_OP_ACOS,				/* acos				*/
    AI_BASE_FLOAT16_OP_POW,					/* pow				*/
    AI_BASE_FLOAT16_OP_COUNT,				/* support oper num */
} AI_BASE_FLOAT16_OP_ENUM;

typedef enum {
    AI_BASE_FLOAT16_RND_N,					/* round to nearest, ties to even	*/
    AI_BASE_FLOAT16_RND_Z,					/* round to zero					*/
    AI_BASE_FLOAT16_RND_D,					/* round to -inf					*/
    AI_BASE_FLOAT16_RND_U,					/* round to +inf					*/
    AI_BASE_FLOAT16_RND_NA,					/* round to nearest, away from zero */
    AI_BASE_FLOAT16_RND_F,					/* faithful rounding (nondeterministic, either RNDD or RNDU, inexact flag is always set)  */
	AI_BASE_FLOAT16_RND_COUNT,				/* support rnd num					*/
} AI_BASE_FLOAT16_RND_ENUM;

typedef struct ai_base_float16_mp_randstate_t {
    UINT64_T val;
} AI_BASE_FLOAT16_MP_RANDSTATE_T;

/*------------------------- Function Definition -----------------------------*/
INT32_T AiBaseFloat16CmpEq(FLOAT16_T a, FLOAT16_T b);
INT32_T AiBaseFloat16CmpLt(FLOAT16_T a, FLOAT16_T b);
INT32_T AiBaseFloat16CmpLe(FLOAT16_T a, FLOAT16_T b);
INT32_T AiBaseFloat16CmpGt(FLOAT16_T a, FLOAT16_T b);
INT32_T AiBaseFloat16CmpGe(FLOAT16_T a, FLOAT16_T b);
FLOAT16_T AiBaseFloat16Add(FLOAT16_T a, FLOAT16_T b);
FLOAT16_T AiBaseFloat16Sub(FLOAT16_T a, FLOAT16_T b);
FLOAT16_T AiBaseFloat16Mul(FLOAT16_T a, FLOAT16_T b);
FLOAT16_T AiBaseFloat16Div(FLOAT16_T a, FLOAT16_T b);
FLOAT16_T AiBaseFloat16Exp(FLOAT16_T a);
FLOAT16_T AiBaseFloat16Sqrt(FLOAT16_T a);

/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/


