/**
 ******************************************************************************
 * file    ai_base_board_x86_aux.c
 * author  sunlingge
 * version V100
 * brief   the ai base board x86 basp aux driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"
#if defined(_OPENMP)
#include <omp.h>
#endif
#pragma warning(disable:4996)

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/
/*  Misc. */
static INT32_T HW_MMX, HW_x64, HW_RDRAND, HW_BMI1, HW_BMI2, HW_ADX, HW_PREFETCHWT1;
/* Advanced Bit Manipulation */
static INT32_T HW_ABM;      
/*  SIMD: 128-bit */
static INT32_T HW_SSE, HW_SSE2, HW_SSE3, HW_SSSE3, HW_SSE41, HW_SSE42, HW_SSE4a, HW_AES, HW_SHA;
/*  SIMD: 256-bit */
static INT32_T HW_AVX, HW_XOP, HW_FMA3, HW_FMA4, HW_AVX2;
/*  SIMD: 512-bit */
/*  AVX512 Foundation */
static INT32_T HW_AVX512F; 
/*  AVX512 Conflict Detection */  
static INT32_T HW_AVX512CD; 
/*  AVX512 Prefetch */ 
static INT32_T HW_AVX512PF;
/*  AVX512 Exponential + Reciprocal */ 
static INT32_T HW_AVX512ER;   
/*  AVX512 Vector Length Extensions */
static INT32_T HW_AVX512VL;   
/*  AVX512 Byte + Word */
static INT32_T HW_AVX512BW;   
/*  AVX512 Doubleword + Quadword */
static INT32_T HW_AVX512DQ;   
/*  AVX512 Integer 52-bit Fused Multiply-Add */
static INT32_T HW_AVX512IFMA; 
/*  AVX512 Vector Byte Manipulation Instructions */
static INT32_T HW_AVX512VBMI; 

/*------------------------- Function Definition -----------------------------*/
/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseBoardX86CheckCpuFeatures(void) 
{
    INT32_T info[4];
	INT32_T nIds;
	UINT32_T nExIds;
    
	AiBaseBoardX86Cpuid(info, 0);
	nIds = info[0];
    AiBaseBoardX86Cpuid(info, 0x80000000);
    nExIds = info[0];

    if (nIds >= 0x00000001) {
        AiBaseBoardX86Cpuid(info, 0x00000001);
        HW_MMX = (info[3] & ((INT32_T)1 << 23)) != 0;
        HW_SSE = (info[3] & ((INT32_T)1 << 25)) != 0;
        HW_SSE2 = (info[3] & ((INT32_T)1 << 26)) != 0;
        HW_SSE3 = (info[2] & ((INT32_T)1 << 0)) != 0;

        HW_SSSE3 = (info[2] & ((INT32_T)1 << 9)) != 0;
        HW_SSE41 = (info[2] & ((INT32_T)1 << 19)) != 0;
        HW_SSE42 = (info[2] & ((INT32_T)1 << 20)) != 0;
        HW_AES = (info[2] & ((INT32_T)1 << 25)) != 0;

        HW_AVX = (info[2] & ((INT32_T)1 << 28)) != 0;
        HW_FMA3 = (info[2] & ((INT32_T)1 << 12)) != 0;

        HW_RDRAND = (info[2] & ((INT32_T)1 << 30)) != 0;
    }
    if (nIds >= 0x00000007) {
        AiBaseBoardX86Cpuid(info, 0x00000007);
        HW_AVX2 = (info[1] & ((INT32_T)1 << 5)) != 0;

        HW_BMI1 = (info[1] & ((INT32_T)1 << 3)) != 0;
        HW_BMI2 = (info[1] & ((INT32_T)1 << 8)) != 0;
        HW_ADX = (info[1] & ((INT32_T)1 << 19)) != 0;
        HW_SHA = (info[1] & ((INT32_T)1 << 29)) != 0;
        HW_PREFETCHWT1 = (info[2] & ((INT32_T)1 << 0)) != 0;

        HW_AVX512F = (info[1] & ((INT32_T)1 << 16)) != 0;
        HW_AVX512CD = (info[1] & ((INT32_T)1 << 28)) != 0;
        HW_AVX512PF = (info[1] & ((INT32_T)1 << 26)) != 0;
        HW_AVX512ER = (info[1] & ((INT32_T)1 << 27)) != 0;
        HW_AVX512VL = (info[1] & ((INT32_T)1 << 31)) != 0;
        HW_AVX512BW = (info[1] & ((INT32_T)1 << 30)) != 0;
        HW_AVX512DQ = (info[1] & ((INT32_T)1 << 17)) != 0;
        HW_AVX512IFMA = (info[1] & ((INT32_T)1 << 21)) != 0;
        HW_AVX512VBMI = (info[2] & ((INT32_T)1 << 1)) != 0;
    }
    if (nExIds >= 0x80000001) {
        AiBaseBoardX86Cpuid(info, 0x80000001);
        HW_x64 = (info[3] & ((INT32_T)1 << 29)) != 0;
        HW_ABM = (info[2] & ((INT32_T)1 << 5)) != 0;
        HW_SSE4a = (info[2] & ((INT32_T)1 << 6)) != 0;
        HW_FMA4 = (info[2] & ((INT32_T)1 << 16)) != 0;
        HW_XOP = (info[2] & ((INT32_T)1 << 11)) != 0;
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
INT32_T AiBaseBoardX86IsAvx(void) 
{
    static INT32_T result = -1;
    if (result == -1) {
        AiBaseBoardX86CheckCpuFeatures();
        result = HW_AVX;
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
INT32_T AiBaseBoardX86IsFmaAvx2(void) 
{
    static INT32_T result = -1;
    if (result == -1) {
        AiBaseBoardX86CheckCpuFeatures();
        result = HW_FMA3 && HW_AVX2;
    }
    return result;
}

/*------------------------- End ---------------------------------------------*/
