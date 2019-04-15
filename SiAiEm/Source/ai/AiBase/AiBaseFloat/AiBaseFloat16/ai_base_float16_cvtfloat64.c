/**
 ******************************************************************************
 * file    ai_base_float16_cvtdouble.c
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
INT32_T AiBaseFloat16BfGetFloat64(const AI_BASE_FLOAT16_BF_T *a, FLOAT64_T *pres, AI_BASE_FLOAT16_BF_RND_T rnd_mode)
{
	AI_BASE_FLOAT64_BF_T d_temp_bf;

	d_temp_bf.sign = (AI_BASE_FLOAT64_LIMB_T)(a->sign);
	d_temp_bf.expn = (AI_BASE_FLOAT64_SLIMB_T)(a->expn);
	d_temp_bf.tab[0] = (((AI_BASE_FLOAT64_DLIMB_T)a->tab[0]) << (AI_BASE_FLOAT32_LIMB_BITS + AI_BASE_FLOAT16_LIMB_BITS));
	d_temp_bf.len = (AI_BASE_FLOAT64_LIMB_T)(a->len);
	AiBaseFloat64BfGetFloat64(&d_temp_bf, pres, (AI_BASE_FLOAT64_BF_RND_T)rnd_mode);

	return 0;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat16BfSetFloat64(AI_BASE_FLOAT16_BF_T *a, FLOAT64_T d)
{
	FLOAT64_T d_temp;
	AI_BASE_FLOAT64_ICVT_UINT64 d_temp_cvt;
	AI_BASE_FLOAT64_BF_T d_temp_bf;
	AI_BASE_FLOAT64_LIMB_T prec = (AI_BASE_FLOAT16_FP16_MANT_LENGTH + 1);
	AI_BASE_FLOAT64_BF_RND_T rnd_mode = AI_BASE_FLOAT64_BF_RNDU;
	AI_BASE_FLOAT64_BF_FLAGS_T bf_flags = (rnd_mode | AI_BASE_FLOAT64_BF_FLAG_SUBNORMAL | AiBaseFloat64BfSetExpBits(AI_BASE_FLOAT16_FP16_EXP_LENGTH));

	d_temp = d;
	memcpy((char *)&d_temp_cvt, (char *)&d_temp, sizeof(FLOAT64_T));
	AiBaseFloat64BfSetFloat64(&d_temp_bf, d_temp);
	AiBaseFloat64BfRound(&d_temp_bf, prec, bf_flags);

	a->sign = (AI_BASE_FLOAT16_LIMB_T)(d_temp_bf.sign);
	a->expn = (AI_BASE_FLOAT16_SLIMB_T)(d_temp_bf.expn);
	a->tab[0] = (AI_BASE_FLOAT16_LIMB_T)(d_temp_bf.tab[0] >> (AI_BASE_FLOAT32_LIMB_BITS + AI_BASE_FLOAT16_LIMB_BITS));
	a->len = (AI_BASE_FLOAT16_LIMB_T)(d_temp_bf.len);

	return;
}

/*------------------------- End ---------------------------------------------*/
#endif

