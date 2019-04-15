/**
 ******************************************************************************
 * file    ai_base_float_test.c
 * author  sunlingge
 * version V100
 * brief   the ai base float driver.
 * description
 *	1. double: sign(63 bit), exponent(52-62 bit), fraction(00-51 bit)
 *	2. float:  sign(31 bit), exponent(23-30 bit), fraction(00-22 bit)
 *  3. fp16:   sign(15 bit), exponent(10-14 bit), fraction(00-09 bit)
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"
#pragma warning(disable:4996)

#if (AI_BASE_FLOAT_FP16 == 1)
/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/
ERROR_T AiServerLogFp16(UINT8_T *str);

/*------------------------- Variable Definition------------------------------*/

/*------------------------- Function Definition -----------------------------*/
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
#if 0
INT32_T AiBaseFloat16TestMain(void)
{
    INT32_T seed, duration_ms;
#if 0
	INT32_T c;
#endif
    AI_BASE_FLOAT16_OP_ENUM op, op_start, op_last;
                        
    seed = 1234;
    duration_ms = 100;
    op_start = (AI_BASE_FLOAT16_OP_ENUM)0;
    op_last = (AI_BASE_FLOAT16_OP_ENUM)(AI_BASE_FLOAT16_OP_COUNT - 1);

#if 0
	AiBaseFloatTestMpfrExecInit();
#endif

	for(;;) {
        for(op = op_start; op <= op_last; op++) {
            if (op == AI_BASE_FLOAT16_OP_MUL_L2RADIX || op == AI_BASE_FLOAT16_OP_DIV_L2RADIX) {
                AiBaseFloat16TestOp(op, AI_BASE_FLOAT16_LIMB_BITS, 0, seed);
            } else if (op == AI_BASE_FLOAT16_OP_CAN_ROUND) {
                AiBaseFloat16TestOp(op, 8, AI_BASE_FLOAT16_BF_EXP_BITS_MAX, seed);
                AiBaseFloat16TestOp(op, 53, AI_BASE_FLOAT16_BF_EXP_BITS_MAX, seed);
                AiBaseFloat16TestOp(op, 256, AI_BASE_FLOAT16_BF_EXP_BITS_MAX, seed);
            } else {
                if (op == AI_BASE_FLOAT16_OP_MUL ||
                    op == AI_BASE_FLOAT16_OP_ADD ||
                    op == AI_BASE_FLOAT16_OP_DIV ||
                    op == AI_BASE_FLOAT16_OP_SQRT ||
                    op == AI_BASE_FLOAT16_OP_CMP_EQ ||
                    op == AI_BASE_FLOAT16_OP_CMP_LT ||
                    op == AI_BASE_FLOAT16_OP_CMP_LE) {
                    AiBaseFloat16TestOp(op, 11, 5, seed);
                }
                AiBaseFloat16TestOp(op, 11, AI_BASE_FLOAT16_BF_EXP_BITS_MAX, seed);
#if 0
                AiBaseFloat16TestOp(op, 112, AI_BASE_FLOAT16_BF_EXP_BITS_MAX, seed);
#endif
                /* mpfr bug ? */
                if (op !=  AI_BASE_FLOAT16_OP_SQRT)
                    AiBaseFloat16TestOp(op, 256, AI_BASE_FLOAT16_BF_EXP_BITS_MAX, seed);
                AiBaseFloat16TestOp(op, 3000, AI_BASE_FLOAT16_BF_EXP_BITS_MAX, seed);
                if (op == AI_BASE_FLOAT16_OP_RECIP) {
                    AiBaseFloat16TestOp(op, 0, AI_BASE_FLOAT16_BF_EXP_BITS_MAX, seed);
                }
            }
        }
        seed++;
        duration_ms = 1000;
    }

    return 0;
}
#endif
ERROR_FLOAT16_T AiBaseFloat16TestMainBfFloat16(void) {
	FLOAT16_T a;
	AI_BASE_FLOAT16_ICVT_UINT16 a_cvt;
	AI_BASE_FLOAT16_BF_T a_bf;
	FLOAT16_T a_pres;
	AI_BASE_FLOAT16_BF_RND_T rnd_mode;
	AI_BASE_FLOAT16_ICVT_UINT16 a_pres_cvt;
	ERROR_FLOAT16_T result = AI_BASE_FLOAT16_ERROR_SUCCESS;
	

//	a = 0x0AA0;	/* normal */
	a = 0x7C01;
	memcpy((char *)&a_cvt, (char *)&a, sizeof(FLOAT16_T));

	AiBaseFloat16BfSetFloat16(&a_bf, a);
	
	rnd_mode = AI_BASE_FLOAT16_BF_RNDNA;
	result =  AiBaseFloat16BfGetFloat16(&a_bf, &a_pres, rnd_mode);
	if (result != AI_BASE_FLOAT16_ERROR_SUCCESS) {
		return result;
	}
	memcpy((char *)&a_pres_cvt, (char *)&a_pres, sizeof(FLOAT16_T));

#if 0
	if (AiBaseFloat16BfIsNan(&a_bf) == AI_BASE_FLOAT16_ERROR_SUCCESS) {
		/* need add test code */
	}
#endif
	if (a != a_pres) {
		return AI_BASE_FLOAT16_ERROR_GENERAL;
	}
	if (a_cvt != a_pres_cvt) {
		return AI_BASE_FLOAT16_ERROR_GENERAL;
	}
	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainBfFloat16LoopAll(void) {
	FLOAT16_T a;
	AI_BASE_FLOAT16_ICVT_UINT16 a_cvt;
	AI_BASE_FLOAT16_BF_T a_bf;
	FLOAT16_T a_pres;
	AI_BASE_FLOAT16_BF_RND_T rnd_mode;
	AI_BASE_FLOAT16_ICVT_UINT16 a_pres_cvt;
	ERROR_FLOAT16_T result = AI_BASE_FLOAT16_ERROR_SUCCESS;
	UINT32_T index;

	for(index = 0; index < 0x10000; index++) {
		a_cvt = (AI_BASE_FLOAT16_ICVT_UINT16)index;

		memcpy((char *)&a, (char *)&a_cvt, sizeof(FLOAT16_T));

		AiBaseFloat16BfSetFloat16(&a_bf, a);
	
		rnd_mode = AI_BASE_FLOAT16_BF_RNDNA;
		result =  AiBaseFloat16BfGetFloat16(&a_bf, &a_pres, rnd_mode);
		if (result != AI_BASE_FLOAT16_ERROR_SUCCESS) {
			while(1);
		}
		memcpy((char *)&a_pres_cvt, (char *)&a_pres, sizeof(FLOAT16_T));
		while(1) {
			if ((a == a_pres) && (a_cvt == a_pres_cvt)) 
				break;
			if (AiBaseFloat16BfIsNan(&a_bf) == AI_BASE_FLOAT16_ERROR_SUCCESS)
				break;
		}
	}

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}


ERROR_FLOAT16_T AiBaseFloat16TestMainAdd(void) {
	FLOAT32_T a;
	FLOAT32_T b;
	FLOAT32_T c;
	FLOAT16_T a_16;
	FLOAT16_T b_16;
	FLOAT16_T c_16;
	FLOAT32_T c_32;

	a = 1.1f;
	b = 2.2f;
	c = a + b;
	a_16 = AiBaseFloat16CvtF32F16(a);
	b_16 = AiBaseFloat16CvtF32F16(b);
	c_16 = AiBaseFloat16Add(a_16, b_16);
	c_32 = AiBaseFloat32CvtF16F32(c_16);

	if (c != c_32)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainMul(void) {
	FLOAT32_T a;
	FLOAT32_T b;
	FLOAT32_T c;
	FLOAT16_T a_16;
	FLOAT16_T b_16;
	FLOAT16_T c_16;
	FLOAT32_T c_32;

	a = 1.1f;
	b = 2.2f;
	c = a * b;
	a_16 = AiBaseFloat16CvtF32F16(a);
	b_16 = AiBaseFloat16CvtF32F16(b);
	c_16 = AiBaseFloat16Mul(a_16, b_16);
	c_32 = AiBaseFloat32CvtF16F32(c_16);

	if (c != c_32)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainCmpEq(void) {
	FLOAT32_T a;
	FLOAT32_T b;
	FLOAT16_T a_16;
	FLOAT16_T b_16;
	INT32_T c_16;
	INT32_T c_32;

	a = 0.1f;
	b = -0.1f;
	a_16 = AiBaseFloat16CvtF32F16(a);
	b_16 = AiBaseFloat16CvtF32F16(b);
	c_16 = AiBaseFloat16CmpEq(a_16, b_16);
	c_32 = (a == b);

	if (c_16 != c_32)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainCmpEq1(void) {
	FLOAT32_T a;
	FLOAT32_T b;
	FLOAT16_T a_16;
	FLOAT16_T b_16;
	INT32_T c_16;
	INT32_T c_32;

	a = 0.1f;
	b = 0.1f;
	a_16 = AiBaseFloat16CvtF32F16(a);
	b_16 = AiBaseFloat16CvtF32F16(b);
	c_16 = AiBaseFloat16CmpEq(a_16, b_16);
	c_32 = (a == b);

	if (c_16 != c_32)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainCmpEq2(void) {
	FLOAT32_T a;
	FLOAT32_T b;
	FLOAT16_T a_16;
	FLOAT16_T b_16;
	INT32_T c_16;
	INT32_T c_32;

	a = -0.1f;
	b = 0.1f;
	a_16 = AiBaseFloat16CvtF32F16(a);
	b_16 = AiBaseFloat16CvtF32F16(b);
	c_16 = AiBaseFloat16CmpEq(a_16, b_16);
	c_32 = (a == b);

	if (c_16 != c_32)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainCmpLt(void) {
	FLOAT32_T a;
	FLOAT32_T b;
	FLOAT16_T a_16;
	FLOAT16_T b_16;
	INT32_T c_16;
	INT32_T c_32;

	a = 0.1f;
	b = -0.1f;
	a_16 = AiBaseFloat16CvtF32F16(a);
	b_16 = AiBaseFloat16CvtF32F16(b);
	c_16 = AiBaseFloat16CmpLt(a_16, b_16);
	c_32 = (a < b);

	if (c_16 != c_32)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainCmpLt1(void) {
	FLOAT32_T a;
	FLOAT32_T b;
	FLOAT16_T a_16;
	FLOAT16_T b_16;
	INT32_T c_16;
	INT32_T c_32;

	a = 0.1f;
	b = 0.1f;
	a_16 = AiBaseFloat16CvtF32F16(a);
	b_16 = AiBaseFloat16CvtF32F16(b);
	c_16 = AiBaseFloat16CmpLt(a_16, b_16);
	c_32 = (a < b);

	if (c_16 != c_32)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainCmpLt2(void) {
	FLOAT32_T a;
	FLOAT32_T b;
	FLOAT16_T a_16;
	FLOAT16_T b_16;
	INT32_T c_16;
	INT32_T c_32;

	a = -0.1f;
	b = 0.1f;
	a_16 = AiBaseFloat16CvtF32F16(a);
	b_16 = AiBaseFloat16CvtF32F16(b);
	c_16 = AiBaseFloat16CmpLt(a_16, b_16);
	c_32 = (a < b);

	if (c_16 != c_32)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainCmpLe(void) {
	FLOAT32_T a;
	FLOAT32_T b;
	FLOAT16_T a_16;
	FLOAT16_T b_16;
	INT32_T c_16;
	INT32_T c_32;

	a = 0.1f;
	b = -0.1f;
	a_16 = AiBaseFloat16CvtF32F16(a);
	b_16 = AiBaseFloat16CvtF32F16(b);
	c_16 = AiBaseFloat16CmpLe(a_16, b_16);
	c_32 = (a <= b);

	if (c_16 != c_32)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainCmpLe1(void) {
	FLOAT32_T a;
	FLOAT32_T b;
	FLOAT16_T a_16;
	FLOAT16_T b_16;
	INT32_T c_16;
	INT32_T c_32;

	a = 0.1f;
	b = 0.1f;
	a_16 = AiBaseFloat16CvtF32F16(a);
	b_16 = AiBaseFloat16CvtF32F16(b);
	c_16 = AiBaseFloat16CmpLe(a_16, b_16);
	c_32 = (a <= b);

	if (c_16 != c_32)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainCmpLe2(void) {
	FLOAT32_T a;
	FLOAT32_T b;
	FLOAT16_T a_16;
	FLOAT16_T b_16;
	INT32_T c_16;
	INT32_T c_32;

	a = -0.1f;
	b = 0.1f;
	a_16 = AiBaseFloat16CvtF32F16(a);
	b_16 = AiBaseFloat16CvtF32F16(b);
	c_16 = AiBaseFloat16CmpLe(a_16, b_16);
	c_32 = (a <= b);

	if (c_16 != c_32)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainCmpGt(void) {
	FLOAT32_T a;
	FLOAT32_T b;
	FLOAT16_T a_16;
	FLOAT16_T b_16;
	INT32_T c_16;
	INT32_T c_32;

	a = 0.1f;
	b = -0.1f;
	a_16 = AiBaseFloat16CvtF32F16(a);
	b_16 = AiBaseFloat16CvtF32F16(b);
	c_16 = AiBaseFloat16CmpGt(a_16, b_16);
	c_32 = (a > b);

	if (c_16 != c_32)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainCmpGt1(void) {
	FLOAT32_T a;
	FLOAT32_T b;
	FLOAT16_T a_16;
	FLOAT16_T b_16;
	INT32_T c_16;
	INT32_T c_32;

	a = 0.1f;
	b = 0.1f;
	a_16 = AiBaseFloat16CvtF32F16(a);
	b_16 = AiBaseFloat16CvtF32F16(b);
	c_16 = AiBaseFloat16CmpGt(a_16, b_16);
	c_32 = (a > b);

	if (c_16 != c_32)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainCmpGt2(void) {
	FLOAT32_T a;
	FLOAT32_T b;
	FLOAT16_T a_16;
	FLOAT16_T b_16;
	INT32_T c_16;
	INT32_T c_32;

	a = -0.1f;
	b = 0.1f;
	a_16 = AiBaseFloat16CvtF32F16(a);
	b_16 = AiBaseFloat16CvtF32F16(b);
	c_16 = AiBaseFloat16CmpGt(a_16, b_16);
	c_32 = (a > b);

	if (c_16 != c_32)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainCmpGe(void) {
	FLOAT32_T a;
	FLOAT32_T b;
	FLOAT16_T a_16;
	FLOAT16_T b_16;
	INT32_T c_16;
	INT32_T c_32;

	a = 0.1f;
	b = -0.1f;
	a_16 = AiBaseFloat16CvtF32F16(a);
	b_16 = AiBaseFloat16CvtF32F16(b);
	c_16 = AiBaseFloat16CmpGe(a_16, b_16);
	c_32 = (a >= b);

	if (c_16 != c_32)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainCmpGe1(void) {
	FLOAT32_T a;
	FLOAT32_T b;
	FLOAT16_T a_16;
	FLOAT16_T b_16;
	INT32_T c_16;
	INT32_T c_32;

	a = 0.1f;
	b = 0.1f;
	a_16 = AiBaseFloat16CvtF32F16(a);
	b_16 = AiBaseFloat16CvtF32F16(b);
	c_16 = AiBaseFloat16CmpGe(a_16, b_16);
	c_32 = (a >= b);

	if (c_16 != c_32)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainCmpGe2(void) {
	FLOAT32_T a;
	FLOAT32_T b;
	FLOAT16_T a_16;
	FLOAT16_T b_16;
	INT32_T c_16;
	INT32_T c_32;

	a = -0.1f;
	b = 0.1f;
	a_16 = AiBaseFloat16CvtF32F16(a);
	b_16 = AiBaseFloat16CvtF32F16(b);
	c_16 = AiBaseFloat16CmpGe(a_16, b_16);
	c_32 = (a >= b);

	if (c_16 != c_32)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainCvtInt32(void) {
	INT32_T a;
	INT32_T a1;
	FLOAT16_T b;
	FLOAT16_T b1;
	FLOAT32_T c;
	FLOAT32_T c1;
	FLOAT32_T d;
	FLOAT32_T d1;
	FLOAT16_T temp;

	a = 1;
	a1 = 0;
	b = AiBaseFloat16CvtI32F16(a);
	b1 = AiBaseFloat16CvtI32F16(a1);
	c = AiBaseFloat32CvtF16F32(b);
	c1 = AiBaseFloat32CvtF16F32(b1);
	d = (FLOAT32_T)a;
	temp = AiBaseFloat32CvtF32F16(d);
	d1 = (FLOAT32_T)a1;

	if (c != d)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	if (c1 != d1)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainSqrt(void) {
	FLOAT32_T a;
	FLOAT32_T c;
	FLOAT16_T a_16;
	FLOAT16_T c_16;
	FLOAT32_T c_32;

	a = -1.1f;
	c = (FLOAT32_T)sqrt(a);
	a_16 = AiBaseFloat16CvtF32F16(a);
	c_16 = AiBaseFloat16Sqrt(a_16);
	c_32 = AiBaseFloat32CvtF16F32(c_16);

	if (c != c_32)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT16_T AiBaseFloat16TestMainExp(void) {
	FLOAT32_T a;
	FLOAT32_T c;
	FLOAT16_T a_16;
	FLOAT16_T c_16;
	FLOAT32_T c_32;

	a = -1.1f;
	c = (FLOAT32_T)exp(a);
	a_16 = AiBaseFloat16CvtF32F16(a);
	c_16 = AiBaseFloat16Exp(a_16);
	c_32 = AiBaseFloat32CvtF16F32(c_16);

	if (c != c_32)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}


ERROR_FLOAT16_T AiBaseFloat16TestMainAddLoopAll(void) {
#if 0
	FLOAT16_T a;
	AI_BASE_FLOAT32_ICVT_UINT16 a_cvt;
	AI_BASE_FLOAT16_BF_T a_bf;
	AI_BASE_FLOAT16_BF_RND_T rnd_mode = AI_BASE_FLOAT16_BF_RNDNA;
	AI_BASE_FLOAT32_BF_T a1_bf;
	AI_BASE_FLOAT32_BF_RND_T rnd1_mode = AI_BASE_FLOAT16_BF_RNDNA;
	FLOAT32_T b;
	AI_BASE_FLOAT32_ICVT_UINT32 b_cvt;
	FLOAT32_T c;
	AI_BASE_FLOAT32_ICVT_UINT32 c_cvt;
	ERROR_FLOAT32_T result = AI_BASE_FLOAT32_ERROR_SUCCESS;
	UINT64_T index;

	for (index = 0; index < 0x10000; index++) {
		a_cvt = ((AI_BASE_FLOAT64_ICVT_UINT16)(index));
		memcpy((char *)&a, (char *)&a_cvt, sizeof(FLOAT16_T));
		AiBaseFloat16BfSetFloat16(&a_bf, a);
		AiBaseFloat16BfGetFloat32(&a_bf, &b, rnd_mode);

		AiBaseFloat32BfSetFloat16(&a1_bf, a);
		result = AiBaseFloat32BfGetFloat32(&a1_bf, &c, rnd1_mode);
		if (result != AI_BASE_FLOAT32_ERROR_SUCCESS) {
			while (1);
		}
		memcpy((char *)&b_cvt, (char *)&b, sizeof(FLOAT32_T));
		memcpy((char *)&c_cvt, (char *)&c, sizeof(FLOAT32_T));
		while (1) {
			if ((b == c) && (b_cvt == c_cvt))
				break;
			if (AiBaseFloat16BfIsNan(&a_bf) == AI_BASE_FLOAT16_ERROR_SUCCESS)
				break;
		}
	}
#endif
	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}


INT32_T AiBaseFloat16TestMain(void) {
#if 0
	AiBaseFloat16TestMainBfFloat16();
	AiBaseFloat16TestMainBfFloat16LoopAll();
#endif
#if 1
	AiBaseFloat16TestMainExp();
	AiBaseFloat16TestMainCvtInt32();
	AiBaseFloat16TestMainCmpEq();
	AiBaseFloat16TestMainCmpEq1();
	AiBaseFloat16TestMainCmpEq2();
	AiBaseFloat16TestMainCmpLt();
	AiBaseFloat16TestMainCmpLt1();
	AiBaseFloat16TestMainCmpLt2();
	AiBaseFloat16TestMainCmpLe();
	AiBaseFloat16TestMainCmpLe1();
	AiBaseFloat16TestMainCmpLe2();
	AiBaseFloat16TestMainCmpGt();
	AiBaseFloat16TestMainCmpGt1();
	AiBaseFloat16TestMainCmpGt2();
	AiBaseFloat16TestMainCmpGe();
	AiBaseFloat16TestMainCmpGe1();
	AiBaseFloat16TestMainCmpGe2();
	AiBaseFloat16TestMainAdd();
	AiBaseFloat16TestMainMul();
	AiBaseFloat16TestMainAddLoopAll();
#endif
	return 1;
}

/*------------------------- End ---------------------------------------------*/
#endif
