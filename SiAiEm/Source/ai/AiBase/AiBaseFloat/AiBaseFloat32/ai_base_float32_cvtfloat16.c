/**
 ******************************************************************************
 * file    ai_base_float32_cvtfloat16.c
 * author  sunlingge
 * version V100
 * brief   the ai base float 32 driver.
 * description
 *	1. double: sign(63 bit), exponent(52-62 bit), fraction(00-51 bit)
 *	2. float:  sign(31 bit), exponent(23-30 bit), fraction(00-22 bit)
 *  3. fp16:   sign(15 bit), exponent(10-14 bit), fraction(00-09 bit)
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"

#if (AI_BASE_FLOAT_FP32 == 1)
/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/
typedef unsigned short                          UBIT16;

/*------------------------- External Variable Definition --------------------*/
ERROR_T AiServerLogFp16(UINT8_T *str);
int tna_singles2halfp(void *target, void *source, int numel);
int tna_halfp2singles(void *target, void *source, int numel);
UBIT16 tna_func_RND_eval(UBIT16 RND, UBIT16 Sign, UBIT16 l, UBIT16 r, UBIT16 STK);
UBIT16 tna_SISC_fp_add(UBIT16 a, UBIT16 b, UBIT16 rnd);
UBIT16 tna_func_RND_eval_u(UBIT16 RND, UBIT16 Sign, UBIT16 l, UBIT16 r, UBIT16 STK);
UBIT16 tna_SISC_fp_mult(UBIT16 a, UBIT16 b, UBIT16 rnd);
UBIT16 tna_SISC_fp_div(UBIT16 a, UBIT16 b, UBIT16 rnd);
UBIT16 tna_SISC_fp_exp(UBIT16 s_data);
UBIT16 tna_SISC_fp_sigmoid(UBIT16 s_data);

/*------------------------- Variable Definition------------------------------*/

/*------------------------- Function Definition -----------------------------*/
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat32BfGetFloat16(const AI_BASE_FLOAT32_BF_T *a, FLOAT16_T *pres, AI_BASE_FLOAT32_BF_RND_T rnd_mode)
{
	AI_BASE_FLOAT32_BF_T d_temp_bf;
	AI_BASE_FLOAT16_BF_T d16_temp_bf;
	AI_BASE_FLOAT16_LIMB_T prec = (AI_BASE_FLOAT16_FP16_MANT_LENGTH + 1);
	AI_BASE_FLOAT32_BF_RND_T rnd32_mode = AI_BASE_FLOAT32_BF_RNDNA;
	AI_BASE_FLOAT32_BF_FLAGS_T bf_flags = (rnd32_mode | AI_BASE_FLOAT32_BF_FLAG_SUBNORMAL | AiBaseFloat32BfSetExpBits(AI_BASE_FLOAT16_FP16_EXP_LENGTH));
	AI_BASE_FLOAT16_BF_RND_T rnd16_mode = AI_BASE_FLOAT16_BF_RNDNA;

	memcpy((char *)&d_temp_bf, (char *)a, sizeof(AI_BASE_FLOAT32_BF_T));
	AiBaseFloat32BfRound(&d_temp_bf, prec, bf_flags);

	d16_temp_bf.sign = (AI_BASE_FLOAT16_LIMB_T)(d_temp_bf.sign);
	while(1) {
		if (d_temp_bf.expn == AI_BASE_FLOAT32_BF_EXP_ZERO) {
			d16_temp_bf.expn = AI_BASE_FLOAT16_BF_EXP_ZERO;
			break;
		}
		if (d_temp_bf.expn == AI_BASE_FLOAT32_BF_EXP_INF) {
			d16_temp_bf.expn = AI_BASE_FLOAT16_BF_EXP_INF;
			break;
		}
		if (d_temp_bf.expn == AI_BASE_FLOAT32_BF_EXP_NAN) {
			d16_temp_bf.expn = AI_BASE_FLOAT16_BF_EXP_NAN;
			break;
		}
		d16_temp_bf.expn = (AI_BASE_FLOAT16_SLIMB_T)(d_temp_bf.expn);
		break;
	}
	d16_temp_bf.tab[0] = (AI_BASE_FLOAT16_LIMB_T)(d_temp_bf.tab[0] >> (AI_BASE_FLOAT16_LIMB_BITS));
	d16_temp_bf.len = (AI_BASE_FLOAT16_LIMB_T)(d_temp_bf.len);

	AiBaseFloat16BfGetFloat16(&d16_temp_bf, pres, rnd16_mode);

	return 0;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat32BfSetFloat16(AI_BASE_FLOAT32_BF_T *a, FLOAT16_T d)
{
	FLOAT16_T d_temp;
	AI_BASE_FLOAT16_ICVT_UINT16 d_temp_cvt;
	AI_BASE_FLOAT16_BF_T d_temp_bf;

	d_temp = d;
	memcpy((char *)&d_temp_cvt, (char *)&d_temp, sizeof(FLOAT16_T));
	AiBaseFloat16BfSetFloat16(&d_temp_bf, d_temp);

	a->sign = (AI_BASE_FLOAT32_LIMB_T)(d_temp_bf.sign);
	while(1) {
		if (d_temp_bf.expn == AI_BASE_FLOAT16_BF_EXP_ZERO) {
			a->expn = AI_BASE_FLOAT32_BF_EXP_ZERO;
			break;
		}
		if (d_temp_bf.expn == AI_BASE_FLOAT16_BF_EXP_INF) {
			a->expn = AI_BASE_FLOAT32_BF_EXP_INF;
			break;
		}
		if (d_temp_bf.expn == AI_BASE_FLOAT16_BF_EXP_NAN) {
			a->expn = AI_BASE_FLOAT32_BF_EXP_NAN;
			break;
		}
		a->expn = (AI_BASE_FLOAT32_SLIMB_T)(d_temp_bf.expn);
		break;
	}
	a->tab[0] = (((AI_BASE_FLOAT32_DLIMB_T)(d_temp_bf.tab[0])) << (AI_BASE_FLOAT16_LIMB_BITS));
	a->len = (AI_BASE_FLOAT32_LIMB_T)(d_temp_bf.len);

	return;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
FLOAT16_T AiBaseFloat32CvtF32F16(FLOAT32_T value) 
{
#if (AI_PRODUCT_OPTION_LINSEN_FLOAT_API == 1)
	FLOAT16_T dest;
	tna_singles2halfp(&dest, &value, 1);
	return dest;
#endif
#if (AI_PRODUCT_OPTION_LINSEN_FLOAT_API != 1)
	FLOAT32_T source_value;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDU;
	FLOAT16_T dest_value;

	source_value = value;
	AiBaseFloat32BfSetFloat32(&a_bf, source_value);
	AiBaseFloat32BfGetFloat16(&a_bf, &dest_value, rnd_mode);

	return dest_value;
#endif
}

/**
* brief  	none.
* param  	None
* retval 	None
* author	Sunlingge
* comment  V100
*/
FLOAT32_T AiBaseFloat32CvtF16F32(FLOAT16_T value)
{
#if (AI_PRODUCT_OPTION_LINSEN_FLOAT_API == 1)
	FLOAT32_T dest;
	tna_halfp2singles(&dest, &value, 1);
	return dest;
#endif
#if (AI_PRODUCT_OPTION_LINSEN_FLOAT_API != 1)
	FLOAT16_T source_value;
	AI_BASE_FLOAT32_BF_T a_bf;
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDU;
	FLOAT32_T dest_value;

	source_value = value;
	AiBaseFloat32BfSetFloat16(&a_bf, source_value);
	AiBaseFloat32BfGetFloat32(&a_bf, &dest_value, rnd_mode);

	return dest_value;
#endif
}

/*------------------------- End ---------------------------------------------*/
#endif

