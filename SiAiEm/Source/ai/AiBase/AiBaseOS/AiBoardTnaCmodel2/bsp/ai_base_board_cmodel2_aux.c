/**
 ******************************************************************************
 * file    ai_base_board_cmodel2_aux.c
 * author  sunlingge
 * version V100
 * brief   the ai base board tna ir basp aux driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"
#if defined(_OPENMP)
#include <omp.h>
#endif
#pragma warning(disable:4996)

#if (AI_PRODUCT_OPTION_ACCELERATOR_TYPE == AI_PRODUCT_ACCELERATOR_SISCTECH_CMODEL2)
/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/
/*  Misc. */
static INT32_T AI_BASE_BOARD_CMODEL2_HW_MMX, AI_BASE_BOARD_CMODEL2_HW_x64, AI_BASE_BOARD_CMODEL2_HW_RDRAND, AI_BASE_BOARD_CMODEL2_HW_BMI1, AI_BASE_BOARD_CMODEL2_HW_BMI2, AI_BASE_BOARD_CMODEL2_HW_ADX, AI_BASE_BOARD_CMODEL2_HW_PREFETCHWT1;
/* Advanced Bit Manipulation */
static INT32_T AI_BASE_BOARD_CMODEL2_HW_ABM;      
/*  SIMD: 128-bit */
static INT32_T AI_BASE_BOARD_CMODEL2_HW_SSE, AI_BASE_BOARD_CMODEL2_HW_SSE2, AI_BASE_BOARD_CMODEL2_HW_SSE3, AI_BASE_BOARD_CMODEL2_HW_SSSE3, AI_BASE_BOARD_CMODEL2_HW_SSE41, AI_BASE_BOARD_CMODEL2_HW_SSE42, AI_BASE_BOARD_CMODEL2_HW_SSE4a, AI_BASE_BOARD_CMODEL2_HW_AES, AI_BASE_BOARD_CMODEL2_HW_SHA;
/*  SIMD: 256-bit */
static INT32_T AI_BASE_BOARD_CMODEL2_HW_AVX, AI_BASE_BOARD_CMODEL2_HW_XOP, AI_BASE_BOARD_CMODEL2_HW_FMA3, AI_BASE_BOARD_CMODEL2_HW_FMA4, AI_BASE_BOARD_CMODEL2_HW_AVX2;
/*  SIMD: 512-bit */
/*  AVX512 Foundation */
static INT32_T AI_BASE_BOARD_CMODEL2_HW_AVX512F; 
/*  AVX512 Conflict Detection */  
static INT32_T AI_BASE_BOARD_CMODEL2_HW_AVX512CD; 
/*  AVX512 Prefetch */ 
static INT32_T AI_BASE_BOARD_CMODEL2_HW_AVX512PF;
/*  AVX512 Exponential + Reciprocal */ 
static INT32_T AI_BASE_BOARD_CMODEL2_HW_AVX512ER;   
/*  AVX512 Vector Length Extensions */
static INT32_T AI_BASE_BOARD_CMODEL2_HW_AVX512VL;   
/*  AVX512 Byte + Word */
static INT32_T AI_BASE_BOARD_CMODEL2_HW_AVX512BW;   
/*  AVX512 Doubleword + Quadword */
static INT32_T AI_BASE_BOARD_CMODEL2_HW_AVX512DQ;   
/*  AVX512 Integer 52-bit Fused Multiply-Add */
static INT32_T AI_BASE_BOARD_CMODEL2_HW_AVX512IFMA; 
/*  AVX512 Vector Byte Manipulation Instructions */
static INT32_T AI_BASE_BOARD_CMODEL2_HW_AVX512VBMI; 

/*------------------------- Function Definition -----------------------------*/
/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseBoardCmodel2CheckCpuFeatures(void) 
{
    INT32_T info[4];
	INT32_T nIds;
	UINT32_T nExIds;
    
	AiBaseBoardCmodel2Cpuid(info, 0);
	nIds = info[0];
    AiBaseBoardCmodel2Cpuid(info, 0x80000000);
    nExIds = info[0];

    if (nIds >= 0x00000001) {
        AiBaseBoardCmodel2Cpuid(info, 0x00000001);
        AI_BASE_BOARD_CMODEL2_HW_MMX = (info[3] & ((INT32_T)1 << 23)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_SSE = (info[3] & ((INT32_T)1 << 25)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_SSE2 = (info[3] & ((INT32_T)1 << 26)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_SSE3 = (info[2] & ((INT32_T)1 << 0)) != 0;

        AI_BASE_BOARD_CMODEL2_HW_SSSE3 = (info[2] & ((INT32_T)1 << 9)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_SSE41 = (info[2] & ((INT32_T)1 << 19)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_SSE42 = (info[2] & ((INT32_T)1 << 20)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_AES = (info[2] & ((INT32_T)1 << 25)) != 0;

        AI_BASE_BOARD_CMODEL2_HW_AVX = (info[2] & ((INT32_T)1 << 28)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_FMA3 = (info[2] & ((INT32_T)1 << 12)) != 0;

        AI_BASE_BOARD_CMODEL2_HW_RDRAND = (info[2] & ((INT32_T)1 << 30)) != 0;
    }
    if (nIds >= 0x00000007) {
        AiBaseBoardCmodel2Cpuid(info, 0x00000007);
        AI_BASE_BOARD_CMODEL2_HW_AVX2 = (info[1] & ((INT32_T)1 << 5)) != 0;

        AI_BASE_BOARD_CMODEL2_HW_BMI1 = (info[1] & ((INT32_T)1 << 3)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_BMI2 = (info[1] & ((INT32_T)1 << 8)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_ADX = (info[1] & ((INT32_T)1 << 19)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_SHA = (info[1] & ((INT32_T)1 << 29)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_PREFETCHWT1 = (info[2] & ((INT32_T)1 << 0)) != 0;

        AI_BASE_BOARD_CMODEL2_HW_AVX512F = (info[1] & ((INT32_T)1 << 16)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_AVX512CD = (info[1] & ((INT32_T)1 << 28)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_AVX512PF = (info[1] & ((INT32_T)1 << 26)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_AVX512ER = (info[1] & ((INT32_T)1 << 27)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_AVX512VL = (info[1] & ((INT32_T)1 << 31)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_AVX512BW = (info[1] & ((INT32_T)1 << 30)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_AVX512DQ = (info[1] & ((INT32_T)1 << 17)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_AVX512IFMA = (info[1] & ((INT32_T)1 << 21)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_AVX512VBMI = (info[2] & ((INT32_T)1 << 1)) != 0;
    }
    if (nExIds >= 0x80000001) {
        AiBaseBoardCmodel2Cpuid(info, 0x80000001);
        AI_BASE_BOARD_CMODEL2_HW_x64 = (info[3] & ((INT32_T)1 << 29)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_ABM = (info[2] & ((INT32_T)1 << 5)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_SSE4a = (info[2] & ((INT32_T)1 << 6)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_FMA4 = (info[2] & ((INT32_T)1 << 16)) != 0;
        AI_BASE_BOARD_CMODEL2_HW_XOP = (info[2] & ((INT32_T)1 << 11)) != 0;
    }

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
INT32_T AiBaseBoardCmodel2IsAvx(void) 
{
    static INT32_T result = -1;
    if (result == -1) {
        AiBaseBoardCmodel2CheckCpuFeatures();
        result = AI_BASE_BOARD_CMODEL2_HW_AVX;
    }
    return result;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
INT32_T AiBaseBoardCmodel2IsFmaAvx2(void) 
{
    static INT32_T result = -1;
    if (result == -1) {
        AiBaseBoardCmodel2CheckCpuFeatures();
        result = AI_BASE_BOARD_CMODEL2_HW_FMA3 && AI_BASE_BOARD_CMODEL2_HW_AVX2;
    }
    return result;
}

/*------------------------- End ---------------------------------------------*/
#endif
