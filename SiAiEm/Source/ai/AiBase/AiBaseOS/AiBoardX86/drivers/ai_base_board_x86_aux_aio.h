/**
 ******************************************************************************
 * file    ai_board_x86_aux_aio.h
 * author  sunlingge
 * version V100
 * brief   the ai board x86 aux ai operator driver.
 ******************************************************************************
 */
#ifndef AI_BASE_BOARD_X86_AUX_AIO_H
#define AI_BASE_BOARD_X86_AUX_AIO_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseBoardX86AioIm2colCpuAux(FLOAT_AI_T* data_im,
	INT32_T channels, INT32_T height, INT32_T width,
	INT32_T ksize, INT32_T stride, INT32_T pad, FLOAT_AI_T* data_col);
ERROR_T AiBaseBoardX86AioGemmNnAux(INT32_T m, INT32_T n, INT32_T k, FLOAT_AI_T alpha,
	FLOAT_AI_T *a, INT32_T lda,
	FLOAT_AI_T *b, INT32_T ldb,
	FLOAT_AI_T *c, INT32_T ldc);
ERROR_T AiBaseBoardX86AioActivateArrayCpuCustomAux(FLOAT_AI_T *x, INT32_T n, UINT32_T a);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
