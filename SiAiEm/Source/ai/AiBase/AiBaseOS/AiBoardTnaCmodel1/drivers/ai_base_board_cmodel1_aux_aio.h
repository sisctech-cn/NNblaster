/**
 ******************************************************************************
 * file    ai_board_cmodel_aux_aio.h
 * author  sunlingge
 * version V100
 * brief   the ai board cmodel aux ai operator driver.
 ******************************************************************************
 */
#if (AI_PRODUCT_OPTION_ACCELERATOR_TYPE == AI_PRODUCT_ACCELERATOR_SISCTECH_CMODEL1)
#ifndef AI_BASE_BOARD_CMODEL1_AUX_AIO_H
#define AI_BASE_BOARD_CMODEL1_AUX_AIO_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseBoardCmodel1AioIm2colCpuAux(FLOAT_AI_T* data_im,
	INT32_T channels, INT32_T height, INT32_T width,
	INT32_T ksize, INT32_T stride, INT32_T pad, FLOAT_AI_T* data_col);
ERROR_T AiBaseBoardCmodel1AioGemmNnAux(INT32_T m, INT32_T n, INT32_T k, FLOAT_AI_T alpha,
	FLOAT_AI_T *a, INT32_T lda,
	FLOAT_AI_T *b, INT32_T ldb,
	FLOAT_AI_T *c, INT32_T ldc);
ERROR_T AiBaseBoardCmodel1AioActivateArrayCpuCustomAux(FLOAT_AI_T *x, INT32_T n, UINT32_T a);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
#endif

