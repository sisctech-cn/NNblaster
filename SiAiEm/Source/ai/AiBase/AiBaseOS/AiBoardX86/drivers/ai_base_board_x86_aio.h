/**
 ******************************************************************************
 * file    ai_board_x86_aio.h
 * author  sunlingge
 * version V100
 * brief   the ai board x86 ai operator driver.
 ******************************************************************************
 */
#ifndef AI_BASE_BOARD_X86_AIO_H
#define AI_BASE_BOARD_X86_AIO_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LOGISTIC_VALUE					(0)
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_RELU_VALUE						(1)
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_RELIE_VALUE						(2)
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LINEAR_VALUE						(3)
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_RAMP_VALUE						(4)
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_TANH_VALUE						(5)
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_PLSE_VALUE						(6)
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LEAKY_VALUE						(7)
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_ELU_VALUE							(8)
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LOGGY_VALUE						(9)
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_STAIR_VALUE						(10)
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_HARDTAN_VALUE						(11)
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LHTAN_VALUE						(12)
#define AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_SELU_VALUE						(13)


/*------------------------- Macro Definition --------------------------------*/
typedef struct ai_base_board_x86_aio_struct {
	UINT8_T reserved;
} AI_BASE_BOARD_X86_AIO, *AI_BASE_BOARD_X86_AIO_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseBoardX86AioScaleCpu(UINT32_T n, FLOAT_AI_T alpha, FLOAT_AI_T *x, UINT32_T incx);
ERROR_T AiBaseBoardX86AioFillCpu(UINT32_T n, FLOAT_AI_T alpha, FLOAT_AI_T *x, UINT32_T incx);
FLOAT_AI_T AiBaseBoardX86AioIm2colGetPixel(FLOAT_AI_T *im, INT32_T height, INT32_T width, INT32_T channels, 
	INT32_T row, INT32_T col, INT32_T channel, INT32_T pad);
ERROR_T AiBaseBoardX86AioIm2colCpu(FLOAT_AI_T* data_im,
     INT32_T channels,  INT32_T height,  INT32_T width,
     INT32_T ksize,  INT32_T stride, INT32_T pad, FLOAT_AI_T* data_col);
ERROR_T AiBaseBoardX86AioIm2colCpuCustom(FLOAT_AI_T * data_im,
    INT32_T channels, INT32_T height, INT32_T width,
    INT32_T ksize, INT32_T stride, INT32_T pad, FLOAT_AI_T* data_col);
ERROR_T AiBaseBoardX86AioGemm(INT32_T ta, INT32_T tb, INT32_T m, INT32_T n, INT32_T k, FLOAT_AI_T alpha,
        FLOAT_AI_T *a, INT32_T lda,
        FLOAT_AI_T *b, INT32_T ldb,
        FLOAT_AI_T beta,
        FLOAT_AI_T *c, INT32_T ldc);
ERROR_T AiBaseBoardX86AioAddBias(FLOAT_AI_T *output, FLOAT_AI_T *biases, INT32_T batch, INT32_T n, INT32_T size);
FLOAT_AI_T AiBaseBoardX86AioActivate(FLOAT_AI_T x, UINT32_T a);
ERROR_T AiBaseBoardX86AioActivateArrayCpuCustom(FLOAT_AI_T *x, INT32_T n, UINT32_T a);
ERROR_T AiBaseBoardX86AioCopyCpu(int n, FLOAT_AI_T *x, INT32_T incx, FLOAT_AI_T *y, INT32_T incy);
ERROR_T AiBaseBoardX86AioShortcutCpu(INT32_T batch, INT32_T w1, INT32_T h1, INT32_T c1, FLOAT_AI_T *add, 
	INT32_T w2, INT32_T h2, INT32_T c2, FLOAT_AI_T *out);
ERROR_T AiBaseBoardX86AioUpsampleCpu(FLOAT_AI_T *in, INT32_T w, INT32_T h, INT32_T c, 
	INT32_T batch, INT32_T stride, INT32_T forward, FLOAT_AI_T scale, FLOAT_AI_T *out);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

