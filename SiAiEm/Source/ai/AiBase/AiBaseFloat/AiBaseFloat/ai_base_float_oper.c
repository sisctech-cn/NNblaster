/**
 ******************************************************************************
 * file    ai_base_float_oepr.c
 * author  sunlingge
 * version V100
 * brief   the ai base float oper driver.
 * description
 *	1. double: sign(63 bit), exponent(52-62 bit), fraction(00-51 bit)
 *	2. float:  sign(31 bit), exponent(23-30 bit), fraction(00-22 bit)
 *  3. fp16:   sign(15 bit), exponent(10-14 bit), fraction(00-09 bit)
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"
#pragma warning(disable:4996)

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/

/*------------------------- Function Definition -----------------------------*/
/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseFloatCvtF32Fai(FLOAT32_T value) {
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	return (value);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	return AiBaseFloat16CvtF32F16(value);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP32)
	return (value);
#endif
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
FLOAT32_T AiBaseFloatCvtFaiF32(FLOAT_AI_T value)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	return (value);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	return AiBaseFloat32CvtF16F32(value);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP32)
	return (value);
#endif
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseFloatCvtI32Fai(INT32_T value) {
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	return (FLOAT_AI_T)(value);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	return AiBaseFloat16CvtF32F16((FLOAT32_T)value);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP32)
	return (FLOAT_AI_T)(value);
#endif
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
INT32_T AiBaseFloatCvtFaiI32(FLOAT_AI_T value)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	return (INT32_T)(value);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	return (INT32_T)AiBaseFloat32CvtF16F32(value);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP32)
	return (INT32_T)(value);
#endif
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
INT32_T AiBaseFloatCmpEq(FLOAT_AI_T a, FLOAT_AI_T b)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	return (a == b);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	return AiBaseFloat16CmpEq(a, b);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP32)
	return AiBaseFloat32CmpEq(a, b);
#endif
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
INT32_T AiBaseFloatCmpLt(FLOAT_AI_T a, FLOAT_AI_T b)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	return (a < b);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	return AiBaseFloat16CmpLt(a, b);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP32)
	return AiBaseFloat32CmpLt(a, b);
#endif
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
INT32_T AiBaseFloatCmpLe(FLOAT_AI_T a, FLOAT_AI_T b)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	return (a <= b);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	return AiBaseFloat16CmpLe(a, b);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP32)
	return AiBaseFloat32CmpLe(a, b);
#endif
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
INT32_T AiBaseFloatCmpGt(FLOAT_AI_T a, FLOAT_AI_T b)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	return (a > b);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	return AiBaseFloat16CmpGt(a, b);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP32)
	return AiBaseFloat32CmpGt(a, b);
#endif
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
INT32_T AiBaseFloatCmpGe(FLOAT_AI_T a, FLOAT_AI_T b)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	return (a >= b);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	return AiBaseFloat16CmpGe(a, b);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP32)
	return AiBaseFloat32CmpGe(a, b);
#endif
}


/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseFloatAdd(FLOAT_AI_T a, FLOAT_AI_T b)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	return (a + b);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	return AiBaseFloat16Add(a, b);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP32)
	return AiBaseFloat32Add(a, b);
#endif
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseFloatSub(FLOAT_AI_T a, FLOAT_AI_T b)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	return (a - b);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	return AiBaseFloat16Sub(a, b);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP32)
	return AiBaseFloat32Sub(a, b);
#endif
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseFloatMul(FLOAT_AI_T a, FLOAT_AI_T b)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	return (a * b);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	return AiBaseFloat16Mul(a, b);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP32)
	return AiBaseFloat32Mul(a, b);
#endif
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseFloatDiv(FLOAT_AI_T a, FLOAT_AI_T b)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	return (a / b);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	return AiBaseFloat16Div(a, b);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP32)
	return AiBaseFloat32Div(a, b);
#endif
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseFloatExp(FLOAT_AI_T a)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	return (FLOAT_AI_T)(exp(a));
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	return AiBaseFloat16Exp(a);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP32)
	return AiBaseFloat32Exp(a);
#endif
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseFloatSqrt(FLOAT_AI_T a)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	return (FLOAT_AI_T)(sqrt(a));
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	return AiBaseFloat16Sqrt(a);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP32)
	return AiBaseFloat32Sqrt(a);
#endif
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseFloatFloor(FLOAT_AI_T a)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	return (FLOAT_AI_T)(floor(a));
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	FLOAT32_T x;
	FLOAT_AI_T r;

	x = AiBaseFloatCvtFaiF32(a);
	x = (FLOAT32_T)floor((FLOAT32_T)x);
	r = AiBaseFloatCvtF32Fai(x);
	return r;
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP32)
	return floor(a);
#endif
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseFloatCeil(FLOAT_AI_T a)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	return (FLOAT_AI_T)(ceil(a));
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	FLOAT32_T x;
	FLOAT_AI_T r;

	x = AiBaseFloatCvtFaiF32(a);
	x = (FLOAT32_T)ceil((FLOAT32_T)x);
	r = AiBaseFloatCvtF32Fai(x);
	return r;
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP32)
	return ceil(a);
#endif
}

/*------------------------- End ---------------------------------------------*/

