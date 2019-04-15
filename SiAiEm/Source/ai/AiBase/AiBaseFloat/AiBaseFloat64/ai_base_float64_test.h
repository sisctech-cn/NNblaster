/**
 ******************************************************************************
 * file    ai_base_float64_test.h
 * author  sunlingge
 * version V100
 * brief   the ai base float32 bf driver.
 ******************************************************************************
 */
#ifndef AI_BASE_FLOAT64_TEST_H
#define AI_BASE_FLOAT64_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_BASE_FLOAT_FP64 == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_FLOAT64_SPECIAL_COUNT			(7)
#define AI_BASE_FLOAT64_F64_MANT_SIZE			(52)
#define AI_BASE_FLOAT64_F64_EXP_MASK			((1 << 11) - 1)

/*------------------------- Type Definition----------------------------------*/
typedef enum {
    AI_BASE_FLOAT64_OP_MUL,					/* mul				*/
    AI_BASE_FLOAT64_OP_ADD,					/* add				*/
    AI_BASE_FLOAT64_OP_SUB,					/* sub				*/
    AI_BASE_FLOAT64_OP_RINT,				/* rint				*/
    AI_BASE_FLOAT64_OP_ROUND,				/* round			*/
    AI_BASE_FLOAT64_OP_CMP_EQ,				/* compare eq		*/
    AI_BASE_FLOAT64_OP_CMP_LT,				/* compare lt		*/
    AI_BASE_FLOAT64_OP_CMP_LE,				/* compare le		*/
    AI_BASE_FLOAT64_OP_RECIP,				/* recip			*/
    AI_BASE_FLOAT64_OP_DIV,					/* div				*/
    AI_BASE_FLOAT64_OP_FMOD,				/* fmod				*/
    AI_BASE_FLOAT64_OP_REM,					/* rem				*/
    AI_BASE_FLOAT64_OP_RSQRT,				/* rsqrt			*/
    AI_BASE_FLOAT64_OP_SQRT,				/* sqrt				*/
    AI_BASE_FLOAT64_OP_OR,					/* or				*/
    AI_BASE_FLOAT64_OP_XOR,					/* xor				*/
    AI_BASE_FLOAT64_OP_AND,					/* and				*/
    AI_BASE_FLOAT64_OP_CAN_ROUND,			/* can round		*/
    AI_BASE_FLOAT64_OP_MUL_L2RADIX,			/* mul l2radix		*/
    AI_BASE_FLOAT64_OP_DIV_L2RADIX,			/* div l2radix		*/
    AI_BASE_FLOAT64_OP_ATOF,				/* atof				*/
    AI_BASE_FLOAT64_OP_FTOA,				/* ftoa				*/
    AI_BASE_FLOAT64_OP_EXP,					/* exp				*/
    AI_BASE_FLOAT64_OP_LOG,					/* log				*/
    AI_BASE_FLOAT64_OP_COS,					/* cos				*/
    AI_BASE_FLOAT64_OP_SIN,					/* sin				*/
    AI_BASE_FLOAT64_OP_TAN,					/* tan				*/
    AI_BASE_FLOAT64_OP_ATAN,				/* atan				*/
    AI_BASE_FLOAT64_OP_ATAN2,				/* atan2			*/
    AI_BASE_FLOAT64_OP_ASIN,				/* asin				*/
    AI_BASE_FLOAT64_OP_ACOS,				/* acos				*/
    AI_BASE_FLOAT64_OP_POW,					/* pow				*/
    AI_BASE_FLOAT64_OP_COUNT,				/* support oper num */
} AI_BASE_FLOAT64_OP_ENUM;

typedef enum {
    AI_BASE_FLOAT64_RND_N,					/* round to nearest, ties to even	*/
    AI_BASE_FLOAT64_RND_Z,					/* round to zero					*/
    AI_BASE_FLOAT64_RND_D,					/* round to -inf					*/
    AI_BASE_FLOAT64_RND_U,					/* round to +inf					*/
    AI_BASE_FLOAT64_RND_NA,					/* round to nearest, away from zero */
    AI_BASE_FLOAT64_RND_F,					/* faithful rounding (nondeterministic, either RNDD or RNDU, inexact flag is always set)  */
	AI_BASE_FLOAT64_RND_COUNT,				/* support rnd num					*/
} AI_BASE_FLOAT64_RND_ENUM;

typedef struct ai_base_float64_mp_randstate_t {
    UINT64_T val;
} AI_BASE_FLOAT64_MP_RANDSTATE_T;

/*------------------------- Function Definition -----------------------------*/

/*------------------------- End ---------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/


