/**
 ******************************************************************************
 * file    ai_base_board_x86_aio.c
 * author  sunlingge
 * version V100
 * brief   the ai base board x86 ai operator driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"
#include <float.h>
#if defined(_OPENMP)
#include <omp.h>
#endif
#if (defined(__AVX__) && defined(__x86_64__)) || defined(_WIN64)
#ifdef _WIN64
#include <intrin.h>
#include <ammintrin.h>
#include <immintrin.h>
#include <smmintrin.h>
#endif
#endif
#pragma warning(disable:4996)
#pragma warning(disable:4752)

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
ERROR_T AiBaseBoardX86AioIm2colCpuAux(FLOAT_AI_T* data_im,
     INT32_T channels,  INT32_T height,  INT32_T width,
     INT32_T ksize,  INT32_T stride, INT32_T pad, FLOAT_AI_T* data_col) 
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
    INT32_T c;
    INT32_T height_col = (height + 2 * pad - ksize) / stride + 1;
    INT32_T width_col = (width + 2 * pad - ksize) / stride + 1;
    INT32_T channels_col = channels * ksize * ksize;

    if (!((height_col == height) && (width_col == width) && (stride == 1) && (pad == 1) && (AiBaseBoardX86IsFmaAvx2() != 0)))
		return AI_ERROR_GENERAL;

	#pragma omp parallel for 
    for (c = 0; c < channels_col; ++c) {
		INT32_T h, w;
		INT32_T w_offset = c % ksize;
        INT32_T h_offset = (c / ksize) % ksize;
        INT32_T c_im = c / ksize / ksize;
        for (h = pad; h < (height_col-pad); ++h) {
            for (w = pad; w < (width_col-pad-8); w += 8) {
				INT32_T im_row = h_offset + h - pad;
				INT32_T im_col = w_offset + w - pad;
				INT32_T col_index = (c * height_col + h) * width_col + w;
                __m256 src256 = _mm256_loadu_ps((float *)(&data_im[im_col + width*(im_row + height*c_im)]));
                _mm256_storeu_ps(&data_col[col_index], src256);
            }

            for (; w < width_col - pad; ++w) {
				INT32_T im_row = h_offset + h - pad;
				INT32_T im_col = w_offset + w - pad;
				INT32_T col_index = (c * height_col + h) * width_col + w;

                data_col[col_index] = data_im[im_col + width*(im_row + height*c_im)];
            }
        }

        {
            w = 0;
            for (h = 0; h < height_col; ++h) {
				INT32_T im_row = h_offset + h;
				INT32_T im_col = w_offset + w;
				INT32_T col_index = (c * height_col + h) * width_col + w;

                data_col[col_index] = AiBaseBoardX86AioIm2colGetPixel(data_im, height, width, channels,
                    im_row, im_col, c_im, pad);
            }
        }

        {
            w = width_col-1;
            for (h = 0; h < height_col; ++h) {
				INT32_T im_row = h_offset + h;
				INT32_T im_col = w_offset + w;
				INT32_T col_index = (c * height_col + h) * width_col + w;

                data_col[col_index] = AiBaseBoardX86AioIm2colGetPixel(data_im, height, width, channels,
                    im_row, im_col, c_im, pad);
            }
        }

        {
            h = 0;
            for (w = 0; w < width_col; ++w) {
				INT32_T im_row = h_offset + h;
				INT32_T im_col = w_offset + w;
				INT32_T col_index = (c * height_col + h) * width_col + w;

                data_col[col_index] = AiBaseBoardX86AioIm2colGetPixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }

        {
            h = height_col-1;
            for (w = 0; w < width_col; ++w) {
				INT32_T im_row = h_offset + h;
				INT32_T im_col = w_offset + w;
				INT32_T col_index = (c * height_col + h) * width_col + w;

                data_col[col_index] = AiBaseBoardX86AioIm2colGetPixel(data_im, height, width, channels,
                    im_row, im_col, c_im, pad);
            }
        }
    }
#endif
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseBoardX86AioGemmNnAux(INT32_T m, INT32_T n, INT32_T k, FLOAT_AI_T alpha,
	FLOAT_AI_T *a, INT32_T lda,
	FLOAT_AI_T *b, INT32_T ldb,
	FLOAT_AI_T *c, INT32_T ldc)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	INT32_T i, j, t;

	if (AiBaseBoardX86IsAvx() != 1)
		return AI_ERROR_GENERAL;

	for (i = 0; i < m; ++i) {
		for (t = 0; t < k; ++t) {
			FLOAT_AI_T a_part = alpha*a[i*lda + t];
			__m256 a256, b256, c256, result256;
			a256 = _mm256_set1_ps(a_part);
			for (j = 0; j < n - 8; j += 8) {
				b256 = _mm256_loadu_ps(&b[t*ldb + j]);
				c256 = _mm256_loadu_ps(&c[i*ldc + j]);
				result256 = _mm256_mul_ps(a256, b256);
				result256 = _mm256_add_ps(result256, c256);
				_mm256_storeu_ps(&c[i*ldc + j], result256);
			}

			INT32_T prev_end = (n % 8 == 0) ? (n - 8) : (n / 8) * 8;
			for (j = prev_end; j < n; ++j)
				c[i*ldc + j] += a_part*b[t*ldb + j];
		}
	}
#endif	

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseBoardX86AioActivateArrayCpuCustomAux(FLOAT_AI_T *x, INT32_T n, UINT32_T a)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	INT32_T i;

	if (AiBaseBoardX86IsFmaAvx2() == 0)
		return  AI_ERROR_GENERAL;

	if (a == AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LINEAR_VALUE)
	{
	}
	else if (a == AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LEAKY_VALUE)
	{
		{
			__m256i all256_sing1 = _mm256_set_epi32(0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000);
			__m256 all256_01 = _mm256_set1_ps(0.1F);

			for (i = 0; i < n - 8; i += 8) {
				__m256 src256 = _mm256_loadu_ps(&x[i]);
				__m256 mult256 = _mm256_mul_ps((src256), all256_01); // mult * 0.1

				__m256i sign256 = _mm256_and_si256(_mm256_castps_si256(src256), all256_sing1); // check sign in 8 x 32-bit floats

				__m256 result256 = _mm256_blendv_ps(src256, mult256, _mm256_castsi256_ps(sign256)); // (sign>0) ? src : mult;
				_mm256_storeu_ps(&x[i], result256);
			}
		}

		for (; i < n; ++i) {
			x[i] = (x[i]>0) ? x[i] : (FLOAT_AI_T)(.1*x[i]);
		}
	}
	else {
		for (i = 0; i < n; ++i) {
			x[i] = AiBaseBoardX86AioActivate(x[i], a);
		}
	}
#endif

	return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
