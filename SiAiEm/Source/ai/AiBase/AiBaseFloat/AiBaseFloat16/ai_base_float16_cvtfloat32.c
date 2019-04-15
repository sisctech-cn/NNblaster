/**
 ******************************************************************************
 * file    ai_base_float16_cvtfloat.c
 * author  sunlingge
 * version V100
 * brief   the ai base float 16 driver.
 * description
 *	1. double: sign(63 bit), exponent(52-62 bit), fraction(00-51 bit)
 *	2. float:  sign(31 bit), exponent(23-30 bit), fraction(00-22 bit)
 *  3. fp16:   sign(15 bit), exponent(10-14 bit), fraction(00-09 bit)
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"

#if (AI_BASE_FLOAT_FP16 == 1)
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
INT32_T AiBaseFloat16BfGetFloat32(const AI_BASE_FLOAT16_BF_T *a, FLOAT32_T *pres, AI_BASE_FLOAT16_BF_RND_T rnd_mode)
{
	AI_BASE_FLOAT32_BF_T d_temp_bf;

	d_temp_bf.sign = (AI_BASE_FLOAT32_LIMB_T)(a->sign);
	while(1) {
		if (a->expn == AI_BASE_FLOAT16_BF_EXP_ZERO) {
			d_temp_bf.expn = AI_BASE_FLOAT32_BF_EXP_ZERO;
			break;
		}
		if (a->expn == AI_BASE_FLOAT16_BF_EXP_INF) {
			d_temp_bf.expn = AI_BASE_FLOAT32_BF_EXP_INF;
			break;
		}
		if (a->expn == AI_BASE_FLOAT16_BF_EXP_NAN) {
			d_temp_bf.expn = AI_BASE_FLOAT32_BF_EXP_NAN;
			break;
		}
		d_temp_bf.expn = (AI_BASE_FLOAT32_SLIMB_T)(a->expn);
		break;
	}
	d_temp_bf.tab[0] = (((AI_BASE_FLOAT32_DLIMB_T)a->tab[0]) << (AI_BASE_FLOAT16_LIMB_BITS));
	d_temp_bf.len = (AI_BASE_FLOAT32_LIMB_T)(a->len);
	AiBaseFloat32BfGetFloat32(&d_temp_bf, pres, (AI_BASE_FLOAT32_BF_RND_T)rnd_mode);

	return 0;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat16BfSetFloat32(AI_BASE_FLOAT16_BF_T *a, FLOAT32_T d)
{
	FLOAT32_T d_temp;
	AI_BASE_FLOAT32_ICVT_UINT32 d_temp_cvt;
	AI_BASE_FLOAT32_BF_T d_temp_bf;
	AI_BASE_FLOAT32_LIMB_T prec = (AI_BASE_FLOAT16_FP16_MANT_LENGTH + 1);
	AI_BASE_FLOAT32_BF_RND_T rnd_mode = AI_BASE_FLOAT32_BF_RNDF;
	AI_BASE_FLOAT32_BF_FLAGS_T bf_flags = (rnd_mode | AI_BASE_FLOAT32_BF_FLAG_SUBNORMAL | AiBaseFloat32BfSetExpBits(AI_BASE_FLOAT16_FP16_EXP_LENGTH));

	d_temp = d;
	memcpy((char *)&d_temp_cvt, (char *)&d_temp, sizeof(FLOAT32_T));
	AiBaseFloat32BfSetFloat32(&d_temp_bf, d_temp);
	AiBaseFloat32BfRound(&d_temp_bf, prec, bf_flags);

	a->sign = (AI_BASE_FLOAT16_LIMB_T)(d_temp_bf.sign);
	while(1) {
		if ((d_temp_bf.expn - 127 + 15) >= 0x1F) {
			a->expn = AI_BASE_FLOAT16_BF_EXP_INF;
			break;
		}
		if (d_temp_bf.expn == AI_BASE_FLOAT32_BF_EXP_ZERO) {
			a->expn = AI_BASE_FLOAT16_BF_EXP_ZERO;
			break;
		}
		if (d_temp_bf.expn == AI_BASE_FLOAT32_BF_EXP_INF) {
			a->expn = AI_BASE_FLOAT16_BF_EXP_INF;
			break;
		}
		if (d_temp_bf.expn == AI_BASE_FLOAT32_BF_EXP_NAN) {
			a->expn = AI_BASE_FLOAT16_BF_EXP_NAN;
			break;
		}
		a->expn = (AI_BASE_FLOAT16_SLIMB_T)(d_temp_bf.expn);
		break;
	}
	a->tab[0] = (AI_BASE_FLOAT16_LIMB_T)(d_temp_bf.tab[0] >> (AI_BASE_FLOAT16_LIMB_BITS));
	a->len = (AI_BASE_FLOAT16_LIMB_T)(d_temp_bf.len);

	return;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
FLOAT16_T AiBaseFloat16CvtF32F16(FLOAT32_T value) 
{
#if (AI_PRODUCT_OPTION_LINSEN_FLOAT_API == 1)
#if (AI_PRODUCT_OPTION_FLOAT_MODEL_CONSIST_CHECK != 1)
	FLOAT32_T source;
	FLOAT16_T dest;

	source = value;
	tna_singles2halfp(&dest, &source, 1);
	return dest;
#endif
#if (AI_PRODUCT_OPTION_FLOAT_MODEL_CONSIST_CHECK == 1)
	FLOAT32_T source;
	FLOAT16_T dest;
	FLOAT16_T dest_ai_base_float;

	source = value;
	tna_singles2halfp(&dest, &source, 1);

	/* check with AiBaseFloat */
	{
		FLOAT32_T source_value;
		AI_BASE_FLOAT16_BF_T a_bf;
		AI_BASE_FLOAT16_BF_RND_T rnd_mode = AI_BASE_FLOAT16_BF_RNDF;
		FLOAT16_T dest_value;

		source_value = value;
		AiBaseFloat16BfSetFloat32(&a_bf, source_value);
		AiBaseFloat16BfGetFloat16(&a_bf, &dest_value, rnd_mode);
		if (dest_value != dest) {
			AiBaseLogErrorCritial();
		}
		dest_ai_base_float = dest_value;
	}
	/* check with linsen */
	{
		FLOAT32_T f32_ai_base_float;
		FLOAT32_T f32_linsen;
		tna_halfp2singles(&f32_linsen, &dest, 1);
		tna_halfp2singles(&f32_ai_base_float, &dest_ai_base_float, 1);
		if ((*((UINT32_T *)&f32_linsen)) != (*((UINT32_T *)&f32_ai_base_float))) {
			AiBaseLogErrorCritial();
		}
	}
	return dest;
#endif
#endif
#if (AI_PRODUCT_OPTION_LINSEN_FLOAT_API != 1)
	FLOAT32_T source_value;
	AI_BASE_FLOAT16_BF_T a_bf;
	AI_BASE_FLOAT16_BF_RND_T rnd_mode = AI_BASE_FLOAT16_BF_RNDNA;
	FLOAT16_T dest_value;

	source_value = value;
	AiBaseFloat16BfSetFloat32(&a_bf, source_value);
	AiBaseFloat16BfGetFloat16(&a_bf, &dest_value, rnd_mode);

	return dest_value;
#endif
}

/*------------------------- End ---------------------------------------------*/
#endif

