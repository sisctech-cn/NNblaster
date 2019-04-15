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

#if (AI_BASE_FLOAT_FP32 == 1)
/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/
ERROR_T AiServerLogFp32(UINT8_T *str);

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
INT32_T AiBaseFloat32TestMain(void)
{
    INT32_T seed, duration_ms /*, c*/;
    AI_BASE_FLOAT32_OP_ENUM op, op_start, op_last;
                        
    seed = 1234;
    duration_ms = 100;
    op_start = (AI_BASE_FLOAT32_OP_ENUM)0;
    op_last = (AI_BASE_FLOAT32_OP_ENUM)(AI_BASE_FLOAT32_OP_COUNT - 1);

#if 0
	AiBaseFloat32TestMpfrExecInit();
#endif

	for(;;) {
        for(op = op_start; op <= op_last; op++) {
            if (op == AI_BASE_FLOAT32_OP_MUL_L2RADIX || op == AI_BASE_FLOAT32_OP_DIV_L2RADIX) {
                AiBaseFloat32TestOp(op, AI_BASE_FLOAT32_LIMB_BITS, duration_ms, 0, seed);
            } else if (op == AI_BASE_FLOAT32_OP_CAN_ROUND) {
                AiBaseFloat32TestOp(op, 8, duration_ms, AI_BASE_FLOAT32_BF_EXP_BITS_MAX, seed);
                AiBaseFloat32TestOp(op, 53, duration_ms, AI_BASE_FLOAT32_BF_EXP_BITS_MAX, seed);
                AiBaseFloat32TestOp(op, 256, duration_ms, AI_BASE_FLOAT32_BF_EXP_BITS_MAX, seed);
            } else {
                if (op == AI_BASE_FLOAT32_OP_MUL ||
                    op == AI_BASE_FLOAT32_OP_ADD ||
                    op == AI_BASE_FLOAT32_OP_DIV ||
                    op == AI_BASE_FLOAT32_OP_SQRT ||
                    op == AI_BASE_FLOAT32_OP_CMP_EQ ||
                    op == AI_BASE_FLOAT32_OP_CMP_LT ||
                    op == AI_BASE_FLOAT32_OP_CMP_LE) {
                    AiBaseFloat32TestOp(op, 53, duration_ms, 11, seed);
                }
                AiBaseFloat32TestOp(op, 53, duration_ms, AI_BASE_FLOAT32_BF_EXP_BITS_MAX, seed);
                AiBaseFloat32TestOp(op, 112, duration_ms, AI_BASE_FLOAT32_BF_EXP_BITS_MAX, seed);
                /* mpfr bug ? */
                if (op !=  AI_BASE_FLOAT32_OP_SQRT)
                    AiBaseFloat32TestOp(op, 256, duration_ms, AI_BASE_FLOAT32_BF_EXP_BITS_MAX, seed);
                AiBaseFloat32TestOp(op, 3000, duration_ms, AI_BASE_FLOAT32_BF_EXP_BITS_MAX, seed);
                if (op == AI_BASE_FLOAT32_OP_RECIP) {
                    AiBaseFloat32TestOp(op, 0, duration_ms, AI_BASE_FLOAT32_BF_EXP_BITS_MAX, seed);
                }
            }
        }
        seed++;
        duration_ms = 1000;
    }

    return 0;
}
#endif
#if 0
INT32_T AiBaseFloat32TestMain(void) {
	float a, b, r, r1;
	AI_BASE_FLOAT32_ICVT_UINT32 a_cvt, b_cvt, r_cvt;
	AI_BASE_FLOAT32_F_UINT a_unit, b_unit, r_unit, r1_unit;
    AI_BASE_FLOAT32_BF_FLAGS_T bf_flags;

    bf_flags = 0;

	a = 0.11f;
	b = 0.12f;
	r = 0.132f;

	memcpy((char *)&a_cvt, (char *)&a, sizeof(float));
	memcpy((char *)&b_cvt, (char *)&b, sizeof(float));
	memcpy((char *)&r_cvt, (char *)&r, sizeof(float));
	a_unit = AiBaseFloat32CvtU32Sf32(a_cvt, AI_BASE_FLOAT32_RM_RNE, &bf_flags);
	b_unit = AiBaseFloat32CvtU32Sf32(b_cvt, AI_BASE_FLOAT32_RM_RNE, &bf_flags);
	r1 = a*b;
	r_unit = AiBaseFloat32CvtU32Sf32(r_cvt, AI_BASE_FLOAT32_RM_RNE, &bf_flags);
	r1_unit = AiBaseFloat32CvtSf32U32(r_unit, AI_BASE_FLOAT32_RM_RNE, &bf_flags);
	memcpy((char *)&r1, (char *)&r1_unit, sizeof(float));
	if (r1 == r) {
		return 0;
	}
	return 1;
}
#endif

#if 1
ERROR_FLOAT16_T AiBaseFloat32TestMainBfSetFloat16(void) {
	FLOAT32_T a;
	AI_BASE_FLOAT16_BF_T a_bf;
	AI_BASE_FLOAT16_BF_RND_T rnd_mode = AI_BASE_FLOAT16_BF_RNDNA;
	FLOAT32_T b;
	FLOAT32_T c;
	ERROR_FLOAT16_T result = AI_BASE_FLOAT16_ERROR_SUCCESS;

//	a = 1.1E-37;
	a = 1.1f;
	b = (FLOAT32_T)a;
	AiBaseFloat16BfSetFloat32(&a_bf, a);
	result = AiBaseFloat16BfGetFloat32(&a_bf, &c, rnd_mode);
	if (result != AI_BASE_FLOAT16_ERROR_SUCCESS)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	if (b != c)
		return AI_BASE_FLOAT16_ERROR_GENERAL;

	return AI_BASE_FLOAT16_ERROR_SUCCESS;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfSetFloat16LoopAll(void) {
	FLOAT16_T a;
	AI_BASE_FLOAT32_ICVT_UINT16 a_cvt;
	AI_BASE_FLOAT16_BF_T a_bf;
	AI_BASE_FLOAT16_BF_RND_T rnd_mode = AI_BASE_FLOAT16_BF_RNDNA;
	AI_BASE_FLOAT32_BF_T a1_bf;
	AI_BASE_FLOAT32_BF_RND_T rnd1_mode = AI_BASE_FLOAT32_BF_RNDNA;
	FLOAT32_T b;
	AI_BASE_FLOAT32_ICVT_UINT32 b_cvt;
	FLOAT32_T c;
	AI_BASE_FLOAT32_ICVT_UINT32 c_cvt;
	ERROR_FLOAT32_T result = AI_BASE_FLOAT32_ERROR_SUCCESS;
	UINT64_T index;

	for(index = 0; index < 0x10000; index++) {
		a_cvt = ((AI_BASE_FLOAT64_ICVT_UINT16)(index));
		memcpy((char *)&a, (char *)&a_cvt, sizeof(FLOAT16_T));
		AiBaseFloat16BfSetFloat16(&a_bf, a);
		AiBaseFloat16BfGetFloat32(&a_bf, &b, rnd_mode);

		AiBaseFloat32BfSetFloat16(&a1_bf, a);
		result = AiBaseFloat32BfGetFloat32(&a1_bf, &c, rnd1_mode);
		if (result != AI_BASE_FLOAT32_ERROR_SUCCESS) {
			while(1);
		}
		memcpy((char *)&b_cvt, (char *)&b, sizeof(FLOAT32_T));
		memcpy((char *)&c_cvt, (char *)&c, sizeof(FLOAT32_T));
		while(1) {
			if ((b == c) && (b_cvt == c_cvt))  
				break;
			if (AiBaseFloat16BfIsNan(&a_bf) == AI_BASE_FLOAT16_ERROR_SUCCESS)
				break;
		}
	}

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfGetFloat16(void) {
	FLOAT32_T a;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;
	AI_BASE_FLOAT16_BF_T a1_bf;
	AI_BASE_FLOAT16_BF_RND_T rnd1_mode = AI_BASE_FLOAT32_BF_RNDNA;
	FLOAT16_T b;
	FLOAT16_T c;
	ERROR_FLOAT32_T result = AI_BASE_FLOAT32_ERROR_SUCCESS;

	a = 1.1f;
	AiBaseFloat16BfSetFloat32(&a1_bf, a);
	AiBaseFloat16BfGetFloat16(&a1_bf, &b, rnd1_mode);
	AiBaseFloat32BfSetFloat32(&a_bf, a);
	result = AiBaseFloat32BfGetFloat16(&a_bf, &c, rnd_mode);
	if (result != AI_BASE_FLOAT32_ERROR_SUCCESS)
		return AI_BASE_FLOAT32_ERROR_GENERAL;

	if (b != c)
		return AI_BASE_FLOAT32_ERROR_GENERAL;

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfGetFloat16LoopAll(void) {
	FLOAT32_T a;
	AI_BASE_FLOAT32_ICVT_UINT32 a_cvt;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;
	AI_BASE_FLOAT16_BF_T a1_bf;
	AI_BASE_FLOAT16_BF_RND_T rnd1_mode = AI_BASE_FLOAT16_BF_RNDNA;
	FLOAT16_T b;
	AI_BASE_FLOAT16_ICVT_UINT16 b_cvt;
	FLOAT16_T c;
	AI_BASE_FLOAT16_ICVT_UINT16 c_cvt;
	ERROR_FLOAT32_T result = AI_BASE_FLOAT32_ERROR_SUCCESS;
	UINT64_T index;

	for(index = 0; index < 0x10000; index++) {
		a_cvt = ((AI_BASE_FLOAT32_ICVT_UINT32)(index | (index << 16)));
		memcpy((char *)&a, (char *)&a_cvt, sizeof(FLOAT32_T));
		AiBaseFloat16BfSetFloat32(&a1_bf, a);
		AiBaseFloat16BfGetFloat16(&a1_bf, &b, rnd1_mode);

		AiBaseFloat32BfSetFloat32(&a_bf, a);
		result = AiBaseFloat32BfGetFloat16(&a_bf, &c, rnd_mode);
		if (result != AI_BASE_FLOAT32_ERROR_SUCCESS) {
			while(1);
		}
		memcpy((char *)&b_cvt, (char *)&b, sizeof(FLOAT16_T));
		memcpy((char *)&c_cvt, (char *)&c, sizeof(FLOAT16_T));
		while(1) {
			if ((b == c) && (b_cvt == c_cvt))  
				break;
			if (AiBaseFloat16BfIsNan(&a1_bf) == AI_BASE_FLOAT16_ERROR_SUCCESS)
				break;
		}
	}

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}


INT32_T AiBaseFloat32TestMainBfFloat32(void) {
	FLOAT32_T a;
	AI_BASE_FLOAT32_ICVT_UINT32 a_cvt;
	AI_BASE_FLOAT32_BF_T a_bf;
	FLOAT32_T a_pres;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode;
	AI_BASE_FLOAT32_ICVT_UINT32 a_pres_cvt;
	INT32_T result;

	a = 0.11f;
	memcpy((char *)&a_cvt, (char *)&a, sizeof(FLOAT32_T));

	AiBaseFloat32BfSetFloat32(&a_bf, a);
	
	rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;
	result =  AiBaseFloat32BfGetFloat32(&a_bf, &a_pres, rnd_mode);

	memcpy((char *)&a_pres_cvt, (char *)&a_pres, sizeof(FLOAT32_T));
	if (a != a_pres) {
		return 1;
	}
	if (a_cvt != a_pres_cvt) {
		return 1;
	}
	return 0;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfFloat32LoopAll(void) {
	FLOAT32_T a;
	AI_BASE_FLOAT32_ICVT_UINT32 a_cvt;
	AI_BASE_FLOAT32_BF_T a_bf;
	FLOAT32_T a_pres;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode;
	AI_BASE_FLOAT32_ICVT_UINT32 a_pres_cvt;
	ERROR_FLOAT32_T result = AI_BASE_FLOAT32_ERROR_SUCCESS;
	UINT32_T index;

	for(index = 0; index < 0x10000; index++) {
		a_cvt = ((AI_BASE_FLOAT32_ICVT_UINT32)(index | (index << 16)));

		memcpy((char *)&a, (char *)&a_cvt, sizeof(FLOAT32_T));

		AiBaseFloat32BfSetFloat32(&a_bf, a);
	
		rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;
		result =  AiBaseFloat32BfGetFloat32(&a_bf, &a_pres, rnd_mode);
		if (result != AI_BASE_FLOAT32_ERROR_SUCCESS) {
			while(1);
		}
		memcpy((char *)&a_pres_cvt, (char *)&a_pres, sizeof(FLOAT32_T));
		while(1) {
			if ((a == a_pres) && (a_cvt == a_pres_cvt)) 
				break;
			if (AiBaseFloat32BfIsNan(&a_bf) == AI_BASE_FLOAT32_ERROR_SUCCESS)
				break;
		}
	}

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfSetFloat64(void) {
	FLOAT64_T a;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;
	FLOAT32_T b;
	FLOAT32_T c;
	ERROR_FLOAT32_T result = AI_BASE_FLOAT32_ERROR_SUCCESS;

//	a = 1.1E-37;
	a = 7.0998266894699357e-046;
	b = (FLOAT32_T)a;
	AiBaseFloat32BfSetFloat64(&a_bf, a);
	result = AiBaseFloat32BfGetFloat32(&a_bf, &c, rnd_mode);
	if (result != AI_BASE_FLOAT32_ERROR_SUCCESS)
		return AI_BASE_FLOAT32_ERROR_GENERAL;

	if (b != c)
		return AI_BASE_FLOAT32_ERROR_GENERAL;

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfSetFloat64LoopAll(void) {
	FLOAT64_T a;
	AI_BASE_FLOAT64_ICVT_UINT64 a_cvt;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;
	FLOAT32_T b;
	AI_BASE_FLOAT32_ICVT_UINT32 b_cvt;
	FLOAT32_T c;
	AI_BASE_FLOAT32_ICVT_UINT32 c_cvt;
	ERROR_FLOAT32_T result = AI_BASE_FLOAT32_ERROR_SUCCESS;
	UINT64_T index;

	for(index = 0; index < 0x10000; index++) {
		a_cvt = ((AI_BASE_FLOAT64_ICVT_UINT64)(index | (index << 16) | (index << 32) | (index << 48)));
		memcpy((char *)&a, (char *)&a_cvt, sizeof(FLOAT64_T));
		b = (FLOAT32_T)a;

		AiBaseFloat32BfSetFloat64(&a_bf, a);
		result = AiBaseFloat32BfGetFloat32(&a_bf, &c, rnd_mode);
		if (result != AI_BASE_FLOAT32_ERROR_SUCCESS) {
			while(1);
		}
		memcpy((char *)&b_cvt, (char *)&b, sizeof(FLOAT32_T));
		memcpy((char *)&c_cvt, (char *)&c, sizeof(FLOAT32_T));
		while(1) {
			if ((b == c) && (b_cvt == c_cvt))  
				break;
			if (AiBaseFloat32BfIsNan(&a_bf) == AI_BASE_FLOAT32_ERROR_SUCCESS)
				break;
		}
	}

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfGetFloat64(void) {
	FLOAT32_T a;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;
	FLOAT64_T b;
	FLOAT64_T c;
	ERROR_FLOAT32_T result = AI_BASE_FLOAT32_ERROR_SUCCESS;

	a = 1.1E-37f;
	b = (FLOAT64_T)a;
	AiBaseFloat32BfSetFloat32(&a_bf, a);
	result = AiBaseFloat32BfGetFloat64(&a_bf, &c, rnd_mode);
	if (result != AI_BASE_FLOAT32_ERROR_SUCCESS)
		return AI_BASE_FLOAT32_ERROR_GENERAL;

	if (b != c)
		return AI_BASE_FLOAT32_ERROR_GENERAL;

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfGetFloat64LoopAll(void) {
	FLOAT32_T a;
	AI_BASE_FLOAT32_ICVT_UINT32 a_cvt;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDNA;
	FLOAT64_T b;
	AI_BASE_FLOAT64_ICVT_UINT64 b_cvt;
	FLOAT64_T c;
	AI_BASE_FLOAT64_ICVT_UINT64 c_cvt;
	ERROR_FLOAT32_T result = AI_BASE_FLOAT32_ERROR_SUCCESS;
	UINT64_T index;

	for(index = 0; index < 0x10000; index++) {
		a_cvt = ((AI_BASE_FLOAT32_ICVT_UINT32)(index | (index << 16)));
		memcpy((char *)&a, (char *)&a_cvt, sizeof(FLOAT32_T));
		b = (FLOAT64_T)a;

		AiBaseFloat32BfSetFloat32(&a_bf, a);
		result = AiBaseFloat32BfGetFloat64(&a_bf, &c, rnd_mode);
		if (result != AI_BASE_FLOAT32_ERROR_SUCCESS) {
			while(1);
		}
		memcpy((char *)&b_cvt, (char *)&b, sizeof(FLOAT64_T));
		memcpy((char *)&c_cvt, (char *)&c, sizeof(FLOAT64_T));
		while(1) {
			if ((b == c) && (b_cvt == c_cvt))  
				break;
			if (AiBaseFloat32BfIsNan(&a_bf) == AI_BASE_FLOAT32_ERROR_SUCCESS)
				break;
		}
	}

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfGetInt32(void) {
	FLOAT32_T a;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm = AI_BASE_FLOAT32_RM_RTZ;
	UINT32_T fflags;
	FLOAT32_T b;
	AI_BASE_FLOAT32_ICVT_INT32 b_icvt;
	AI_BASE_FLOAT32_F_UINT a_unit;
	INT32_T c;

//	a = 1.1f;
	a = 1.401e-045f;
	c = (INT32_T)a;
	AiBaseFloat32BfSetFloat32(&a_bf, a);
	memcpy(&a_unit, &a, sizeof(AI_BASE_FLOAT32_F_UINT));
	b_icvt = AiBaseFloat32CvtSf32I32(a_unit, rm, &fflags);
	memcpy(&b, &b_icvt, sizeof(AI_BASE_FLOAT32_ICVT_INT32));
	if (c != b) {
		return AI_BASE_FLOAT32_ERROR_GENERAL;
	}

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfGetInt32LoopAll(void) {
	FLOAT32_T a;
	AI_BASE_FLOAT32_ICVT_UINT32 a_cvt;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm = AI_BASE_FLOAT32_RM_RTZ;
	UINT32_T fflags;
	FLOAT32_T b;
	AI_BASE_FLOAT32_ICVT_INT32 b_icvt;
	AI_BASE_FLOAT32_F_UINT a_unit;
	INT32_T c;
	UINT64_T index;

	for(index = 0; index < 0x10000; index++) {
		while(1) {
			a_cvt = ((AI_BASE_FLOAT32_ICVT_UINT32)(index | (index << 16)));
			memcpy((char *)&a, (char *)&a_cvt, sizeof(FLOAT32_T));
			c = (INT32_T)a;

			AiBaseFloat32BfSetFloat32(&a_bf, a);
			memcpy(&a_unit, &a, sizeof(AI_BASE_FLOAT32_F_UINT));
			b_icvt = AiBaseFloat32CvtSf32I32(a_unit, rm, &fflags);
			memcpy(&b, &b_icvt, sizeof(AI_BASE_FLOAT32_ICVT_INT32));
			if (b_icvt >= 0) {
				if (b_icvt == 0x00000000) {
					if ((b_icvt == c))  
						break;
				} else {
					if ((b_icvt == c))  
						break;
					if ((b_icvt == (AI_BASE_FLOAT32_ICVT_INT32)(0x7FFFFFFF)) && (c == (INT32_T)0x80000000))
						break;
				}
			} else {
				if (b_icvt == 0x00000000) {
					if ((b_icvt == c))  
						break;
				} else {
					if ((b_icvt == c))  
						break;
					if ((b_icvt == (AI_BASE_FLOAT32_ICVT_INT32)(0x7FFFFFFF)) && (c == (INT32_T)0x80000000))
						break;
				}
			}
		}
	}

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfGetUint32(void) {
	FLOAT32_T a;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm = AI_BASE_FLOAT32_RM_RTZ;
	UINT32_T fflags;
	FLOAT32_T b;
	AI_BASE_FLOAT32_ICVT_UINT32 b_icvt;
	AI_BASE_FLOAT32_F_UINT a_unit;
	UINT32_T c;

//	a = 1.1f;
	a = 1.401e-045f;
	c = (INT32_T)a;
	AiBaseFloat32BfSetFloat32(&a_bf, a);
	memcpy(&a_unit, &a, sizeof(AI_BASE_FLOAT32_F_UINT));
	b_icvt = AiBaseFloat32CvtSf32U32(a_unit, rm, &fflags);
	memcpy(&b, &b_icvt, sizeof(AI_BASE_FLOAT32_ICVT_INT32));
	if (c != b) {
		return AI_BASE_FLOAT32_ERROR_GENERAL;
	}

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfGetUint32LoopAll(void) {
	FLOAT32_T a;
	AI_BASE_FLOAT32_ICVT_UINT32 a_cvt;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm = AI_BASE_FLOAT32_RM_RTZ;
	UINT32_T fflags;
	FLOAT32_T b;
	AI_BASE_FLOAT32_ICVT_UINT32 b_icvt;
	AI_BASE_FLOAT32_F_UINT a_unit;
	UINT32_T c;
	UINT64_T index;

	for(index = 0; index < 0x10000; index++) {
		while(1) {
			a_cvt = ((AI_BASE_FLOAT32_ICVT_UINT32)(index | (index << 16)));
			memcpy((char *)&a, (char *)&a_cvt, sizeof(FLOAT32_T));
			c = (UINT32_T)a;

			AiBaseFloat32BfSetFloat32(&a_bf, a);
			memcpy(&a_unit, &a, sizeof(AI_BASE_FLOAT32_F_UINT));
			b_icvt = AiBaseFloat32CvtSf32U32(a_unit, rm, &fflags);
			memcpy(&b, &b_icvt, sizeof(AI_BASE_FLOAT32_ICVT_INT32));
			if (b_icvt <= 0x7FFFFFFF) {
				if ((b_icvt == c))  
					break;
#if 1
				if ((b_icvt == (AI_BASE_FLOAT32_ICVT_UINT32)(0x7FFFFFFF)) && (c == (UINT32_T)0x80000000))
					break;
				if ((b_icvt == (AI_BASE_FLOAT32_ICVT_UINT32)(0x00000000)))
					break;
#endif
			} else {
				if ((b_icvt == c))  
					break;
#if 1
				if (b_icvt == 0xFFFFFFFF)
					break;
#endif
			}
		}
	}

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfGetInt16(void) {
	FLOAT32_T a;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm = AI_BASE_FLOAT32_RM_RTZ;
	UINT32_T fflags;
	FLOAT32_T b;
	AI_BASE_FLOAT32_ICVT_INT16 b_icvt;
	AI_BASE_FLOAT32_F_UINT a_unit;
	INT16_T c;

//	a = 1.1f;
	a = 1.401e-045f;
	c = (INT16_T)a;
	AiBaseFloat32BfSetFloat32(&a_bf, a);
	memcpy(&a_unit, &a, sizeof(AI_BASE_FLOAT32_F_UINT));
	b_icvt = AiBaseFloat32CvtSf32I16(a_unit, rm, &fflags);
	memcpy(&b, &b_icvt, sizeof(AI_BASE_FLOAT32_ICVT_INT32));
	if (c != b) {
		return AI_BASE_FLOAT32_ERROR_GENERAL;
	}

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfGetInt16LoopAll(void) {
	FLOAT32_T a;
	AI_BASE_FLOAT32_ICVT_UINT32 a_cvt;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm = AI_BASE_FLOAT32_RM_RTZ;
	UINT32_T fflags;
	FLOAT16_T b;
	AI_BASE_FLOAT32_ICVT_INT16 b_icvt;
	AI_BASE_FLOAT32_F_UINT a_unit;
	INT16_T c;
	UINT64_T index;

	for(index = 0; index < 0x10000; index++) {
		while(1) {
			a_cvt = ((AI_BASE_FLOAT32_ICVT_UINT32)(index | (index << 16)));
			memcpy((char *)&a, (char *)&a_cvt, sizeof(FLOAT32_T));
			c = (INT16_T)a;

			AiBaseFloat32BfSetFloat32(&a_bf, a);
			memcpy(&a_unit, &a, sizeof(AI_BASE_FLOAT32_F_UINT));
			b_icvt = AiBaseFloat32CvtSf32I16(a_unit, rm, &fflags);
			memcpy(&b, &b_icvt, sizeof(AI_BASE_FLOAT32_ICVT_INT16));
			if (b_icvt >= 0) {
				if (b_icvt == 0x0000) {
					if ((b_icvt == c))  
						break;
				} else {
					if ((b_icvt == c))  
						break;
					if ((b_icvt == (AI_BASE_FLOAT32_ICVT_INT16)(0x7FFF)))
						break;
				}
			} else {
				if (b_icvt == 0x0000) {
					if ((b_icvt == c))  
						break;
				} else {
					if ((b_icvt == c))  
						break;
					if ((b_icvt == (AI_BASE_FLOAT32_ICVT_INT16)(0x8000)))
						break;
				}
			}
		}
	}

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfGetUint16(void) {
	FLOAT32_T a;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm = AI_BASE_FLOAT32_RM_RTZ;
	UINT32_T fflags;
	FLOAT16_T b;
	AI_BASE_FLOAT32_ICVT_UINT16 b_icvt;
	AI_BASE_FLOAT32_F_UINT a_unit;
	UINT16_T c;

//	a = 1.1f;
	a = 1.401e-045f;
	c = (INT16_T)a;
	AiBaseFloat32BfSetFloat32(&a_bf, a);
	memcpy(&a_unit, &a, sizeof(AI_BASE_FLOAT32_F_UINT));
	b_icvt = AiBaseFloat32CvtSf32U16(a_unit, rm, &fflags);
	memcpy(&b, &b_icvt, sizeof(AI_BASE_FLOAT32_ICVT_INT16));
	if (c != b) {
		return AI_BASE_FLOAT32_ERROR_GENERAL;
	}

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfGetUint16LoopAll(void) {
	FLOAT32_T a;
	AI_BASE_FLOAT32_ICVT_UINT32 a_cvt;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm = AI_BASE_FLOAT32_RM_RTZ;
	UINT32_T fflags;
	FLOAT16_T b;
	AI_BASE_FLOAT32_ICVT_UINT16 b_icvt;
	AI_BASE_FLOAT32_F_UINT a_unit;
	UINT16_T c;
	UINT64_T index;

	for(index = 0; index < 0x10000; index++) {
		while(1) {
			a_cvt = ((AI_BASE_FLOAT32_ICVT_UINT32)(index | (index << 16)));
			memcpy((char *)&a, (char *)&a_cvt, sizeof(FLOAT32_T));
			c = (UINT16_T)a;

			AiBaseFloat32BfSetFloat32(&a_bf, a);
			memcpy(&a_unit, &a, sizeof(AI_BASE_FLOAT32_F_UINT));
			b_icvt = AiBaseFloat32CvtSf32U16(a_unit, rm, &fflags);
			memcpy(&b, &b_icvt, sizeof(AI_BASE_FLOAT32_ICVT_INT16));
			if (b_icvt <= 0x7FFF) {
				if ((b_icvt == c))  
					break;
				if ((b_icvt == (AI_BASE_FLOAT32_ICVT_UINT16)(0x0000)))
					break;
			} else {
				if ((b_icvt == c))  
					break;
				if (b_icvt == 0xFFFF)
					break;
			}
		}
	}

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfGetInt64(void) {
	FLOAT32_T a;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm = AI_BASE_FLOAT32_RM_RTZ;
	UINT32_T fflags;
	FLOAT64_T b;
	AI_BASE_FLOAT32_ICVT_INT64 b_icvt;
	AI_BASE_FLOAT32_F_UINT a_unit;
	INT64_T c;

//	a = 1.1f;
	a = 1.401e-045f;
	c = (INT64_T)a;
	AiBaseFloat32BfSetFloat32(&a_bf, a);
	memcpy(&a_unit, &a, sizeof(AI_BASE_FLOAT32_F_UINT));
	b_icvt = AiBaseFloat32CvtSf32I64(a_unit, rm, &fflags);
	memcpy(&b, &b_icvt, sizeof(AI_BASE_FLOAT32_ICVT_INT64));
	if (c != b) {
		return AI_BASE_FLOAT32_ERROR_GENERAL;
	}

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfGetInt64LoopAll(void) {
	FLOAT32_T a;
	AI_BASE_FLOAT32_ICVT_UINT32 a_cvt;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm = AI_BASE_FLOAT32_RM_RTZ;
	UINT32_T fflags;
	FLOAT64_T b;
	AI_BASE_FLOAT32_ICVT_INT64 b_icvt;
	AI_BASE_FLOAT32_F_UINT a_unit;
	INT64_T c;
	UINT64_T index;

	for(index = 0; index < 0x10000; index++) {
		while(1) {
			a_cvt = ((AI_BASE_FLOAT32_ICVT_UINT32)(index | (index << 16)));
			memcpy((char *)&a, (char *)&a_cvt, sizeof(FLOAT32_T));
			c = (INT64_T)a;

			AiBaseFloat32BfSetFloat32(&a_bf, a);
			memcpy(&a_unit, &a, sizeof(AI_BASE_FLOAT32_F_UINT));
			b_icvt = AiBaseFloat32CvtSf32I64(a_unit, rm, &fflags);
			memcpy(&b, &b_icvt, sizeof(AI_BASE_FLOAT32_ICVT_INT64));
			if (b_icvt >= 0) {
				if (b_icvt == 0x0000000000000000) {
					if ((b_icvt == c))  
						break;
				} else {
					if ((b_icvt == c))  
						break;
					if ((b_icvt == (AI_BASE_FLOAT32_ICVT_INT64)(0x7FFFFFFFFFFFFFFF)))
						break;
				}
			} else {
				if (b_icvt == 0x0000000000000000) {
					if ((b_icvt == c))  
						break;
				} else {
					if ((b_icvt == c))  
						break;
					if ((b_icvt == (AI_BASE_FLOAT32_ICVT_INT64)(0x8000000000000000)))
						break;
				}
			}
		}
	}

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfGetUint64(void) {
	FLOAT32_T a;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm = AI_BASE_FLOAT32_RM_RTZ;
	UINT32_T fflags;
	FLOAT64_T b;
	AI_BASE_FLOAT32_ICVT_UINT64 b_icvt;
	AI_BASE_FLOAT32_F_UINT a_unit;
	UINT64_T c;

//	a = 1.1f;
	a = 1.401e-045f;
	c = (INT64_T)a;
	AiBaseFloat32BfSetFloat32(&a_bf, a);
	memcpy(&a_unit, &a, sizeof(AI_BASE_FLOAT32_F_UINT));
	b_icvt = AiBaseFloat32CvtSf32U64(a_unit, rm, &fflags);
	memcpy(&b, &b_icvt, sizeof(AI_BASE_FLOAT32_ICVT_INT64));
	if (c != b) {
		return AI_BASE_FLOAT32_ERROR_GENERAL;
	}

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}

ERROR_FLOAT32_T AiBaseFloat32TestMainBfGetUint64LoopAll(void) {
	FLOAT32_T a;
	AI_BASE_FLOAT32_ICVT_UINT32 a_cvt;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_ROUNDING_MODE_ENUM rm = AI_BASE_FLOAT32_RM_RTZ;
	UINT32_T fflags;
	FLOAT64_T b;
	AI_BASE_FLOAT32_ICVT_UINT64 b_icvt;
	AI_BASE_FLOAT32_F_UINT a_unit;
	UINT64_T c;
	UINT64_T index;

	for(index = 0; index < 0x10000; index++) {
		while(1) {
			a_cvt = ((AI_BASE_FLOAT32_ICVT_UINT32)(index | (index << 16)));
			memcpy((char *)&a, (char *)&a_cvt, sizeof(FLOAT32_T));
			c = (UINT16_T)a;

			AiBaseFloat32BfSetFloat32(&a_bf, a);
			memcpy(&a_unit, &a, sizeof(AI_BASE_FLOAT32_F_UINT));
			b_icvt = AiBaseFloat32CvtSf32U64(a_unit, rm, &fflags);
			memcpy(&b, &b_icvt, sizeof(AI_BASE_FLOAT32_ICVT_INT64));
			if (b_icvt <= 0x7FFFFFFFFFFFFFFF) {
				if ((b_icvt == c))  
					break;
				if ((b_icvt == (AI_BASE_FLOAT32_ICVT_UINT16)(0x0000000000000000)))
					break;
			} else {
				if ((b_icvt == c))  
					break;
				if (b_icvt == 0xFFFFFFFFFFFFFFFF)
					break;
			}
		}
	}

	return AI_BASE_FLOAT32_ERROR_SUCCESS;
}

INT32_T AiBaseFloat32TestMain(void) {
#if 0
	AiBaseFloat32TestMainBfFloat32();
	AiBaseFloat32TestMainBfFloat32LoopAll();
#endif
#if 1
	AiBaseFloat32TestMainBfSetFloat16();
	AiBaseFloat32TestMainBfSetFloat16LoopAll();
#endif
#if 1
	AiBaseFloat32TestMainBfGetFloat16();
	AiBaseFloat32TestMainBfGetFloat16LoopAll();
#endif
#if 0
	AiBaseFloat32TestMainBfSetFloat64();
	AiBaseFloat32TestMainBfSetFloat64LoopAll();
#endif
#if 0
	AiBaseFloat32TestMainBfGetFloat64();
	AiBaseFloat32TestMainBfGetFloat64LoopAll();
#endif
#if 0
	AiBaseFloat32TestMainBfGetInt32();
	AiBaseFloat32TestMainBfGetInt32LoopAll();
#endif
#if 0
	AiBaseFloat32TestMainBfGetUint32();
	AiBaseFloat32TestMainBfGetUint32LoopAll();
#endif
#if 0
	AiBaseFloat32TestMainBfGetInt16();
	AiBaseFloat32TestMainBfGetInt16LoopAll();
#endif
#if 0
	AiBaseFloat32TestMainBfGetUint16();
	AiBaseFloat32TestMainBfGetUint16LoopAll();
#endif
#if 0
	AiBaseFloat32TestMainBfGetInt64();
	AiBaseFloat32TestMainBfGetInt64LoopAll();
#endif
#if 0
	AiBaseFloat32TestMainBfGetUint64();
	AiBaseFloat32TestMainBfGetUint64LoopAll();
#endif
	return 0;
}
#endif

/*------------------------- End ---------------------------------------------*/
#endif
