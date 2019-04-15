/**
 ******************************************************************************
 * file    ai_base_board_cmodel_aio.c
 * author  sunlingge
 * version V100
 * brief   the ai base board cmodel ai operator driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"
#if defined(_OPENMP)
#include <omp.h>
#endif
#pragma warning(disable:4996)

#if (AI_PRODUCT_OPTION_ACCELERATOR_TYPE == AI_PRODUCT_ACCELERATOR_SISCTECH_CMODEL2)
/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/

/*------------------------- Function Definition -----------------------------*/
/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseBoardCmodel2AioScaleCpu(UINT32_T n, FLOAT_AI_T alpha, FLOAT_AI_T *x, UINT32_T incx)
{
    UINT32_T i;

    for (i = 0; i < n; i++) {
		x[i*incx] = AiBaseFloatMul(x[i*incx],alpha);
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
ERROR_T AiBaseBoardCmodel2AioFillCpu(UINT32_T n, FLOAT_AI_T alpha, FLOAT_AI_T *x, UINT32_T incx)
{
    UINT32_T i;
    
	for (i = 0; i < n; i++) {
		x[i*incx] = alpha;
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
FLOAT_AI_T AiBaseBoardCmodel2AioIm2colGetPixel(FLOAT_AI_T *im, INT32_T height, INT32_T width, INT32_T channels, 
	INT32_T row, INT32_T col, INT32_T channel, INT32_T pad)
{
    row -= pad;
    col -= pad;

    if ((row < 0) || (col < 0) || (row >= height) || (col >= width)) 
		return 0;
    
    return im[col + width*(row + height*channel)];
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseBoardCmodel2AioIm2colCpu(FLOAT_AI_T* data_im,
     INT32_T channels,  INT32_T height,  INT32_T width,
     INT32_T ksize,  INT32_T stride, INT32_T pad, FLOAT_AI_T* data_col) 
{
    INT32_T c,h,w;
    INT32_T height_col = (height + 2*pad - ksize) / stride + 1;
    INT32_T width_col = (width + 2*pad - ksize) / stride + 1;
    INT32_T channels_col = channels * ksize * ksize;
	INT32_T w_offset;
	INT32_T h_offset;
	INT32_T c_im;
	INT32_T im_row;
	INT32_T im_col;
	INT32_T col_index;

    for (c = 0; c < channels_col; ++c) {
        w_offset = c % ksize;
        h_offset = (c / ksize) % ksize;
        c_im = c / ksize / ksize;

        for (h = 0; h < height_col; ++h) {
            for (w = 0; w < width_col; ++w) {
                im_row = h_offset + h * stride;
                im_col = w_offset + w * stride;
                col_index = (c * height_col + h) * width_col + w;
                data_col[col_index] = AiBaseBoardCmodel2AioIm2colGetPixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }
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
ERROR_T AiBaseBoardCmodel2AioIm2colCpuCustom(FLOAT_AI_T * data_im,
    INT32_T channels, INT32_T height, INT32_T width,
    INT32_T ksize, INT32_T stride, INT32_T pad, FLOAT_AI_T* data_col)
{
#if 0
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	ERROR_T result;

	result = AiBaseBoardCmodel2AioIm2colCpuAux(data_im, channels, height, width, ksize, stride, pad, data_col);
	if (result == AI_ERROR_SUCCESS)
		return AI_ERROR_SUCCESS;
#endif
#endif
    AiBaseBoardCmodel2AioIm2colCpu(data_im, channels, height, width, ksize, stride, pad, data_col);

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseBoardCmodel2AioGemmNn(INT32_T m, INT32_T n, INT32_T k, FLOAT_AI_T alpha,
    FLOAT_AI_T *a, INT32_T lda,
    FLOAT_AI_T *b, INT32_T ldb,
    FLOAT_AI_T *c, INT32_T ldc)
{
    INT32_T i, j, f;
	register FLOAT_AI_T a_part;
#if 0
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	ERROR_T result;

	result = AiBaseBoardCmodel2AioGemmNnAux(m, n, k, alpha, a, lda, b, ldb, c, ldc);
	if (result == AI_ERROR_SUCCESS) {
		return AI_ERROR_SUCCESS;
	}
#endif
#endif

	for (i = 0; i < m; ++i) {
        for (f = 0; f < k; ++f) {
			a_part = AiBaseFloatMul(alpha, a[i*lda + f]);
            for (j = 0; j < n; ++j) {
				c[i*ldc + j] = AiBaseFloatAdd(c[i*ldc + j], AiBaseFloatMul(a_part, b[f*ldb + j]));
            }
        }
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
ERROR_T AiBaseBoardCmodel2AioGemmTn(INT32_T m, INT32_T n, INT32_T k, FLOAT_AI_T alpha,
    FLOAT_AI_T *a, INT32_T lda,
    FLOAT_AI_T *b, INT32_T ldb,
    FLOAT_AI_T *c, INT32_T ldc)
{
    INT32_T i,j,f;
	register FLOAT_AI_T a_part;

    for(i = 0; i < m; ++i){
        for(f = 0; f < k; ++f){
			a_part = AiBaseFloatMul(alpha, a[f*lda + i]);
            for(j = 0; j < n; ++j){
				c[i*ldc + j] = AiBaseFloatAdd(c[i*ldc + j], AiBaseFloatMul(a_part, b[f*ldb + j]));
            }
        }
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
ERROR_T AiBaseBoardCmodel2AioGemmNt(INT32_T m, INT32_T n, INT32_T k, FLOAT_AI_T alpha,
    FLOAT_AI_T *a, INT32_T lda,
    FLOAT_AI_T *b, INT32_T ldb,
    FLOAT_AI_T *c, INT32_T ldc)
{
    INT32_T i,j,f;
	register FLOAT_AI_T sum;

    for(i = 0; i < m; ++i){
        for(j = 0; j < n; ++j){
			sum = AiBaseFloatCvtF32Fai(0.0f);
            for(f = 0; f < k; ++f){
				sum = AiBaseFloatAdd(sum, AiBaseFloatMul(AiBaseFloatMul(alpha, a[i*lda + f]), b[j*ldb + f]));
            }
			c[i*ldc + j] = AiBaseFloatAdd(c[i*ldc + j], sum);
        }
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
ERROR_T AiBaseBoardCmodel2AioGemmTt(INT32_T m, INT32_T n, INT32_T k, FLOAT_AI_T alpha,
    FLOAT_AI_T *a, INT32_T lda,
    FLOAT_AI_T *b, INT32_T ldb,
    FLOAT_AI_T *c, INT32_T ldc)
{
    INT32_T i,j,f;
	register FLOAT_AI_T sum;

    for(i = 0; i < m; ++i){
        for(j = 0; j < n; ++j){
			sum = AiBaseFloatCvtF32Fai(0.0f);
            for(f = 0; f < k; ++f){
				sum = AiBaseFloatAdd(sum, AiBaseFloatMul(AiBaseFloatMul(alpha, a[i + f*lda]), b[f + j*ldb]));
            }
			c[i*ldc + j] = AiBaseFloatAdd(c[i*ldc + j], sum);
        }
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
ERROR_T AiBaseBoardCmodel2AioGemmCpu(INT32_T ta, INT32_T tb, INT32_T m, INT32_T n, INT32_T k, FLOAT_AI_T alpha,
        FLOAT_AI_T *a, INT32_T lda,
        FLOAT_AI_T *b, INT32_T ldb,
        FLOAT_AI_T beta,
        FLOAT_AI_T *c, INT32_T ldc)
{
    INT32_T i, j, t;

	if (!(AiBaseFloatCmpEq(beta, AiBaseFloatCvtF32Fai(1.0f)))){
        for(i = 0; i < m; ++i){
            for(j = 0; j < n; ++j){
				c[i*ldc + j] = AiBaseFloatMul(c[i*ldc + j], beta);
            }
        }
    }

	#pragma omp parallel for
    for (t = 0; t < m; ++t) {
        if (!ta && !tb)
			AiBaseBoardCmodel2AioGemmNn(1, n, k, alpha, a + t*lda, lda, b, ldb, c + t*ldc, ldc);
        else if (ta && !tb)
            AiBaseBoardCmodel2AioGemmTn(1, n, k, alpha, a + t,	   lda, b, ldb, c + t*ldc, ldc);
        else if (!ta && tb)
            AiBaseBoardCmodel2AioGemmNt(1, n, k, alpha, a + t*lda, lda, b, ldb, c + t*ldc, ldc);
        else
            AiBaseBoardCmodel2AioGemmTt(1, n, k, alpha, a + t,     lda, b, ldb, c + t*ldc, ldc);
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
ERROR_T AiBaseBoardCmodel2AioGemm(INT32_T ta, INT32_T tb, INT32_T m, INT32_T n, INT32_T k, FLOAT_AI_T alpha,
        FLOAT_AI_T *a, INT32_T lda,
        FLOAT_AI_T *b, INT32_T ldb,
        FLOAT_AI_T beta,
        FLOAT_AI_T *c, INT32_T ldc)
{
    AiBaseBoardCmodel2AioGemmCpu(ta, tb, m, n, k, alpha, a,lda, b, ldb, beta, c, ldc);
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseBoardCmodel2AioAddBias(FLOAT_AI_T *output, FLOAT_AI_T *biases, INT32_T batch, INT32_T n,
	INT32_T out_h, INT32_T out_w, INT32_T out_h_align, INT32_T out_w_align, INT32_T padding)
{
    INT32_T i, b;
	INT32_T w, h;
	UINT32_T index;

    for(b = 0; b < batch; ++b){
        for(i = 0; i < n; ++i){
            for(h = 0; h < out_h; ++h){
				for (w = 0; w < out_w; ++w) {
					index = b*n*out_h_align*out_w_align + i*out_h_align*out_w_align + h*out_w_align + w + padding;
					output[index] = AiBaseFloatAdd(output[index], biases[i]);
				}
            }
        }
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
FLOAT_AI_T AiBaseBoardCmodel2AioLinearActivate(FLOAT_AI_T x)
{
	return (FLOAT_AI_T)(x);
}
FLOAT_AI_T AiBaseBoardCmodel2AioLogisticActivate(FLOAT_AI_T x)
{
	FLOAT_AI_T r, r1;

	r = AiBaseFloatCvtF32Fai(0.0f);
	r = AiBaseFloatSub(r, x);
	r = AiBaseFloatExp(r);
	r1 = AiBaseFloatCvtF32Fai(1.0f);
	r = AiBaseFloatAdd(r1, r);
	r1 = AiBaseFloatCvtF32Fai(1.0f);
	r = AiBaseFloatDiv(r1, r);

	return (FLOAT_AI_T)r;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioLoggyActivate(FLOAT_AI_T x)
{
	FLOAT_AI_T r, r1;

	r = AiBaseFloatCvtF32Fai(0.0f);
	r = AiBaseFloatSub(r, x);
	r = AiBaseFloatExp(r);
	r1 = AiBaseFloatCvtF32Fai(1.0f);
	r = AiBaseFloatAdd(r1, r);
	r1 = AiBaseFloatCvtF32Fai(1.0f);
	r = AiBaseFloatSub(r, r1);
	r1 = AiBaseFloatCvtF32Fai(2.0f);
	r = AiBaseFloatDiv(r1, r);

	return (FLOAT_AI_T)r;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioReluActivate(FLOAT_AI_T x)
{
	FLOAT_AI_T r, r1;

	r1 = AiBaseFloatCvtF32Fai(0.0f);
	r = AiBaseFloatCvtF32Fai((FLOAT32_T)(AiBaseFloatCmpGe(x, r1)));
	r = AiBaseFloatMul(x, r);

	return (FLOAT_AI_T)r;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioEluActivate(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioSeluActivate(FLOAT_AI_T x) 
{ 
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioRelieActivate(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioRampActivate(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioLeakyActivate(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioTanhActivate(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioPlseActivate(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioStairActivate(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioHardtanActivate(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioLhtanActivate(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioLhtanGradient(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioHardtanGradient(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioLinearGradient(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioLogisticGradient(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioLoggyGradient(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioStairGradient(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioReluGradient(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioEluGradient(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioSeluGradient(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioRelieGradient(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioRampGradient(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioLeakyGradient(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioTanhGradient(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioPlseGradient(FLOAT_AI_T x)
{
	AiBaseLogErrorCritial();
	return x;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiBaseBoardCmodel2AioActivate(FLOAT_AI_T x, UINT32_T a)
{
    switch(a){
        case AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LINEAR_VALUE:
            return AiBaseBoardCmodel2AioLinearActivate(x);
        case AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LOGISTIC_VALUE:
            return AiBaseBoardCmodel2AioLogisticActivate(x);
        case AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LOGGY_VALUE:
            return AiBaseBoardCmodel2AioLoggyActivate(x);
        case AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_RELU_VALUE:
            return AiBaseBoardCmodel2AioReluActivate(x);
        case AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_ELU_VALUE:
            return AiBaseBoardCmodel2AioEluActivate(x);
        case AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_SELU_VALUE:
            return AiBaseBoardCmodel2AioSeluActivate(x);
        case AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_RELIE_VALUE:
            return AiBaseBoardCmodel2AioRelieActivate(x);
        case AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_RAMP_VALUE:
            return AiBaseBoardCmodel2AioRampActivate(x);
        case AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LEAKY_VALUE:
            return AiBaseBoardCmodel2AioLeakyActivate(x);
        case AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_TANH_VALUE:
            return AiBaseBoardCmodel2AioTanhActivate(x);
        case AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_PLSE_VALUE:
            return AiBaseBoardCmodel2AioPlseActivate(x);
        case AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_STAIR_VALUE:
            return AiBaseBoardCmodel2AioStairActivate(x);
        case AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_HARDTAN_VALUE:
            return AiBaseBoardCmodel2AioHardtanActivate(x);
        case AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LHTAN_VALUE:
            return AiBaseBoardCmodel2AioLhtanActivate(x);
    }
    return 0;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseBoardCmodel2AioActivateArrayCpuCustom(FLOAT_AI_T *x, INT32_T n, UINT32_T a)
{
    INT32_T i;
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	ERROR_T result;

	result = AiBaseBoardCmodel2AioActivateArrayCpuCustomAux(x, n, a);
	if (result == AI_ERROR_SUCCESS)
		return AI_ERROR_SUCCESS;
#endif
    if (a == AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LINEAR_VALUE)
    {
    }
    else if (a == AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LEAKY_VALUE)
    {
        for (i = 0; i < n; ++i) {
			{
				FLOAT_AI_T r, r1;

				r = AiBaseFloatCvtF32Fai(0.0f);
				r = AiBaseFloatCvtF32Fai((FLOAT32_T)(AiBaseFloatCmpGe(x[i], r)));
				if (r) {
					x[i] = x[i];
				}
				else {
					r1 = AiBaseFloatCvtF32Fai(0.1f);
					r = AiBaseFloatMul(r1, x[i]);
					x[i] = r;
				}
			}
        }
    }
    else {
        for (i = 0; i < n; ++i) {
            x[i] = AiBaseBoardCmodel2AioActivate(x[i], a);
        }
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
ERROR_T AiBaseBoardCmodel2AioCopyCpu(int n, FLOAT_AI_T *x, INT32_T incx, FLOAT_AI_T *y, INT32_T incy)
{
    INT32_T i;

    for(i = 0; i < n; ++i) {
		y[i*incy] = x[i*incx];
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
ERROR_T AiBaseBoardCmodel2AioMapCopyCpu(INT32_T batch, INT32_T w1, INT32_T h1, INT32_T c1, INT32_T input_padding, FLOAT_AI_T *add,
	INT32_T w2, INT32_T h2, INT32_T c2, INT32_T output_padding, FLOAT_AI_T *out)
{
	INT32_T stride = 1;
	INT32_T sample = 1;
	INT32_T minw;
	INT32_T minh;
	INT32_T minc;
	INT32_T i, j, k, b;
	INT32_T out_index;
	INT32_T add_index;
	INT32_T padding_input_w;
	INT32_T padding_output_w;

	memset((char *)out, 0, w2*h2*c2 * sizeof(FLOAT_AI_T));
	minw = (w1 < w2) ? w1 : w2;
	minh = (h1 < h2) ? h1 : h2;
	minc = (c1 < c2) ? c1 : c2;
	if ((input_padding == 1) && (output_padding == 1)) {
		padding_input_w = 1;
		padding_output_w = 1;
	}
	if ((input_padding == 1) && (output_padding == 0)) {
		padding_input_w = 1;
		padding_output_w = 0;
	}
	if ((input_padding == 0) && (output_padding == 1)) {
		padding_input_w = 0;
		padding_output_w = 1;
	}
	if ((input_padding == 0) && (output_padding == 0)) {
		padding_input_w = 0;
		padding_output_w = 0;
	}

	for (b = 0; b < batch; ++b) {
		for (k = 0; k < minc; ++k) {
			for (j = 0; j < minh; ++j) {
				for (i = 0; i < minw; ++i) {
					out_index = i*sample + w2*(j*sample + h2*(k + c2*b)) + padding_output_w;
					add_index = i*stride + w1*(j*stride + h1*(k + c1*b)) + padding_input_w;
					out[out_index] = add[add_index];
				}
			}
		}
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
ERROR_T AiBaseBoardCmodel2AioShortcutCopyCpu(INT32_T batch, INT32_T w1, INT32_T h1, INT32_T c1, INT32_T input_padding, FLOAT_AI_T *add,
	INT32_T w2, INT32_T h2, INT32_T c2, INT32_T output_padding, FLOAT_AI_T *out)
{
	INT32_T stride = 1;
	INT32_T sample = 1;
	INT32_T minw;
	INT32_T minh;
	INT32_T minc;
	INT32_T i, j, k, b;
	INT32_T out_index;
	INT32_T add_index;
	INT32_T padding_input_w;
	INT32_T padding_output_w;

	memset((char *)out, 0, w2*h2*c2 * sizeof(FLOAT_AI_T));
	minw = (w1 < w2) ? w1 : w2;
	minh = (h1 < h2) ? h1 : h2;
	minc = (c1 < c2) ? c1 : c2;
	if ((input_padding == 1) && (output_padding == 1)) {
		padding_input_w = 1;
		padding_output_w = 1;
	}
	if ((input_padding == 1) && (output_padding == 0)) {
		padding_input_w = 1;
		padding_output_w = 0;
	}
	if ((input_padding == 0) && (output_padding == 1)) {
		padding_input_w = 0;
		padding_output_w = 1;
	}
	if ((input_padding == 0) && (output_padding == 0)) {
		padding_input_w = 0;
		padding_output_w = 0;
	}

	for (b = 0; b < batch; ++b) {
		for (k = 0; k < minc; ++k) {
			for (j = 0; j < minh; ++j) {
				for (i = 0; i < minw; ++i) {
					out_index = i*sample + w2*(j*sample + h2*(k + c2*b)) + padding_output_w;
					add_index = i*stride + w1*(j*stride + h1*(k + c1*b)) + padding_input_w;
					out[out_index] = add[add_index];
				}
			}
		}
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
ERROR_T AiBaseBoardCmodel2AioShortcutCpu(INT32_T batch, INT32_T w1, INT32_T h1, INT32_T c1, INT32_T input_padding, FLOAT_AI_T *add,
	INT32_T w2, INT32_T h2, INT32_T c2, INT32_T output_padding, FLOAT_AI_T *out)
{
    INT32_T stride = 1;
    INT32_T sample = 1;
	INT32_T minw;
    INT32_T minh;
    INT32_T minc;
	INT32_T i,j,k,b;
	INT32_T out_index;
    INT32_T add_index;
	INT32_T padding_input_w;
	INT32_T padding_output_w;
    
	minw = (w1 < w2) ? w1 : w2;
    minh = (h1 < h2) ? h1 : h2;
    minc = (c1 < c2) ? c1 : c2;
	if ((input_padding == 1) && (output_padding == 1)) {
		padding_input_w = 1;
		padding_output_w = 1;
	}
	if ((input_padding == 1) && (output_padding == 0)) {
		padding_input_w = 1;
		padding_output_w = 0;
	}
	if ((input_padding == 0) && (output_padding == 1)) {
		padding_input_w = 0;
		padding_output_w = 1;
	}
	if ((input_padding == 0) && (output_padding == 0)) {
		padding_input_w = 0;
		padding_output_w = 0;
	}

    for(b = 0; b < batch; ++b){
        for(k = 0; k < minc; ++k){
            for(j = 0; j < minh; ++j){
                for(i = 0; i < minw; ++i){
                    out_index = i*sample + w2*(j*sample + h2*(k + c2*b)) + padding_output_w;
                    add_index = i*stride + w1*(j*stride + h1*(k + c1*b)) + padding_input_w;
					out[out_index] = AiBaseFloatAdd(out[out_index], add[add_index]);
                }
            }
        }
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
ERROR_T AiBaseBoardCmodel2AioRouteCopyCpu(INT32_T batch, INT32_T w1, INT32_T h1, INT32_T c1, INT32_T input_padding, FLOAT_AI_T *add,
	INT32_T w2, INT32_T h2, INT32_T c2, INT32_T output_padding, FLOAT_AI_T *out)
{
	INT32_T stride = 1;
	INT32_T sample = 1;
	INT32_T minw;
	INT32_T minh;
	INT32_T minc;
	INT32_T i, j, k, b;
	INT32_T out_index;
	INT32_T add_index;
	INT32_T padding_input_w;
	INT32_T padding_output_w;

	memset((char *)out, 0, w2*h2*c2 * sizeof(FLOAT_AI_T));
	minw = (w1 < w2) ? w1 : w2;
	minh = (h1 < h2) ? h1 : h2;
	minc = (c1 < c2) ? c1 : c2;
	if ((input_padding == 1) && (output_padding == 1)) {
		padding_input_w = 1;
		padding_output_w = 1;
	}
	if ((input_padding == 1) && (output_padding == 0)) {
		padding_input_w = 1;
		padding_output_w = 0;
	}
	if ((input_padding == 0) && (output_padding == 1)) {
		padding_input_w = 0;
		padding_output_w = 1;
	}
	if ((input_padding == 0) && (output_padding == 0)) {
		padding_input_w = 0;
		padding_output_w = 0;
	}

	for (b = 0; b < batch; ++b) {
		for (k = 0; k < minc; ++k) {
			for (j = 0; j < minh; ++j) {
				for (i = 0; i < minw; ++i) {
					out_index = i*sample + w2*(j*sample + h2*(k + c2*b)) + padding_output_w;
					add_index = i*stride + w1*(j*stride + h1*(k + c1*b)) + padding_input_w;
					out[out_index] = add[add_index];
				}
			}
		}
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
ERROR_T AiBaseBoardCmodel2AioUpsampleCpu(FLOAT_AI_T *in, INT32_T w, INT32_T h, INT32_T c, 
	INT32_T batch, INT32_T stride, INT32_T forward, FLOAT_AI_T scale, FLOAT_AI_T *out)
{
    INT32_T i, j, k, b;
	INT32_T in_index;
	INT32_T out_index;

    for (b = 0; b < batch; ++b) {
        for (k = 0; k < c; ++k) {
            for (j = 0; j < h*stride; ++j) {
                for (i = 0; i < w*stride; ++i) {
                    in_index = b*w*h*c + k*w*h + (j / stride)*w + i / stride;
                    out_index = b*w*h*c*stride*stride + k*w*h*stride*stride + j*w*stride + i;
					if (forward) {
						out[out_index] = AiBaseFloatMul(scale, in[in_index]);
					}
					else {
						in[in_index] = AiBaseFloatAdd(in[in_index], AiBaseFloatMul(scale, out[out_index]));
					}
                }
            }
        }
    }

	return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
#endif
