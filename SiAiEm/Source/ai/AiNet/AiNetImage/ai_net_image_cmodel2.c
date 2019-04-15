/**
 ******************************************************************************
 * file    ai_net_image_cmodel.c
 * author  sunlingge
 * version V100
 * brief   the ai net image cmodel driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_net_global.h"
#pragma warning(disable:4996)

#if (AI_PRODUCT_OPTION_ACCELERATOR_TYPE == AI_PRODUCT_ACCELERATOR_SISCTECH_CMODEL2)
/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/
extern INT32_T g_ai_base_image_1080p_content_len;
extern UINT8_T g_ai_base_image_1080p_content[AI_BASE_IMAGE_1080P_CONTENT_LENGTH_MAX];

/*------------------------- Function Definition -----------------------------*/
/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiNetImageCmodel2Save(AI_NET_IMAGE_CMODEL2 *image_net)
{
    UINT8_T file_path[256];
	ERROR_T result;

    sprintf((char *)file_path, AI_PRODUCT_LOG_IDENT_PATH"/netCmodel2.png");
	g_ai_base_image_1080p_content_len = AI_BASE_IMAGE_1080P_CONTENT_LENGTH_MAX;
	result = AiBaseImageSaveColor((AI_BASE_IMAGE *)image_net, file_path, g_ai_base_image_1080p_content, &g_ai_base_image_1080p_content_len);
	if (result != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
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
ERROR_T AiNetImageCmodel2(AI_NET_IMAGE_CMODEL2 *dest, AI_BASE_IMAGE_1080P *source)
{
	INT32_T part_w, part_h, part_c;
	INT32_T r, c, k;
	FLOAT_AI_T w_scale, h_scale;
	FLOAT_AI_T val;
	FLOAT_AI_T sx;
	INT32_T ix;
	FLOAT_AI_T dx;
	FLOAT_AI_T sy;
	INT32_T iy;
	FLOAT_AI_T dy;
	FLOAT_AI_T *content;

	AiBaseImageMakeImage(AI_NET_IMAGE_CMODEL2_WIDTH, AI_NET_IMAGE_CMODEL2_HEIGHT, AI_NET_IMAGE_CMODEL2_CHANNEL, (AI_BASE_IMAGE *)dest, AI_NET_IMAGE_CMODEL2_CONTENT_LENGTH_MAX);

	content = (FLOAT_AI_T *)g_ai_base_image_1080p_content;
	part_w = AI_NET_IMAGE_CMODEL2_WIDTH - AI_NET_IMAGE_CMODEL2_WIDTH_PADDING*2 - AI_NET_IMAGE_CMODEL2_WIDTH_PADDING_ALIGNED_MBLOCK;
	part_h = source->h;
	part_c = source->c;
	w_scale = (FLOAT_AI_T)AiBaseFloatDiv(AiBaseFloatCvtF32Fai((FLOAT32_T)(source->w - 1)),
				AiBaseFloatCvtF32Fai((FLOAT32_T)(AI_NET_IMAGE_CMODEL2_WIDTH - AI_NET_IMAGE_CMODEL2_WIDTH_PADDING*2 - AI_NET_IMAGE_CMODEL2_WIDTH_PADDING_ALIGNED_MBLOCK - 1)));
	h_scale = (FLOAT_AI_T)AiBaseFloatDiv(AiBaseFloatCvtF32Fai((FLOAT32_T)(source->h - 1)), 
				AiBaseFloatCvtF32Fai((FLOAT32_T)(AI_NET_IMAGE_CMODEL2_HEIGHT - 1)));
	for (k = 0; k < source->c; k++) {
		for (r = 0; r < source->h; ++r) {
			for (c = 0; c < AI_NET_IMAGE_CMODEL2_WIDTH - AI_NET_IMAGE_CMODEL2_WIDTH_PADDING * 2 - AI_NET_IMAGE_CMODEL2_WIDTH_PADDING_ALIGNED_MBLOCK; ++c) {
				val = 0;
				if ((c == AI_NET_IMAGE_CMODEL2_WIDTH - AI_NET_IMAGE_CMODEL2_WIDTH_PADDING * 2 - AI_NET_IMAGE_CMODEL2_WIDTH_PADDING_ALIGNED_MBLOCK - 1) || (source->w == 1)) {
					val = AiBaseImageGetPixel(source->data, source->w, source->h, source->c, source->w - 1, r, k);
				}
				else {
					sx = AiBaseFloatMul(AiBaseFloatCvtI32Fai((c)), w_scale);
					ix = (INT32_T)AiBaseFloatCvtFaiF32(sx);
					dx = AiBaseFloatSub(sx, AiBaseFloatCvtI32Fai(ix));
					val = AiBaseFloatAdd(
							AiBaseFloatMul(AiBaseFloatSub(AiBaseFloatCvtF32Fai((FLOAT32_T)(1.0f)), dx),  AiBaseImageGetPixel(source->data, source->w, source->h, source->c, ix, r, k)),
							AiBaseFloatMul(dx, AiBaseImageGetPixel(source->data, source->w, source->h, source->c, ix + 1, r, k)));
				}
				AiBaseImageSetPixel(content, part_w, part_h, part_c, c, r, k, val);
			}
		}
	}
	for (k = 0; k < source->c; ++k) {
		for (r = 0; r < AI_NET_IMAGE_CMODEL2_HEIGHT; ++r) {
			sy = AiBaseFloatMul(AiBaseFloatCvtI32Fai((r)), h_scale);
			iy = (INT32_T)AiBaseFloatCvtFaiF32(sy);
			dy = AiBaseFloatSub(sy, AiBaseFloatCvtI32Fai(iy));
			for (c = 0; c < AI_NET_IMAGE_CMODEL2_WIDTH - AI_NET_IMAGE_CMODEL2_WIDTH_PADDING * 2 - AI_NET_IMAGE_CMODEL2_WIDTH_PADDING_ALIGNED_MBLOCK; ++c) {
				val = AiBaseFloatMul(AiBaseFloatSub(AiBaseFloatCvtI32Fai((1)), dy), AiBaseImageGetPixel(content, part_w, part_h, part_c, c, iy, k));
				AiBaseImageSetPixel(dest->data, dest->w, dest->h, dest->c, c+ AI_NET_IMAGE_CMODEL2_WIDTH_PADDING, r, k, val);
			}
			if ((r == AI_NET_IMAGE_CMODEL2_HEIGHT - 1) || (source->h == 1))
				continue;
			for (c = 0; c < AI_NET_IMAGE_CMODEL2_WIDTH - AI_NET_IMAGE_CMODEL2_WIDTH_PADDING * 2 - AI_NET_IMAGE_CMODEL2_WIDTH_PADDING_ALIGNED_MBLOCK; ++c) {
				val = AiBaseFloatMul(dy, AiBaseImageGetPixel(content, part_w, part_h, part_c, c, iy + 1, k));
				AiBaseImageAddPixel(dest->data, dest->w, dest->h, dest->c, c + AI_NET_IMAGE_CMODEL2_WIDTH_PADDING, r, k, val);
			}
		}
	}

	/* padding */
	val = AiBaseFloatCvtF32Fai(0.0f);
	for (r = 0; r < AI_NET_IMAGE_CMODEL2_HEIGHT; r++) {
		for (k = 0; k < dest->c; ++k) {
			AiBaseImageAddPixel(dest->data, dest->w, dest->h, dest->c, 0, r, k, val);
			AiBaseImageAddPixel(dest->data, dest->w, dest->h, dest->c, dest->w-1, r, k, val);
			for (c = 0; c < AI_NET_IMAGE_CMODEL2_WIDTH_PADDING_ALIGNED_MBLOCK; ++c) {
				AiBaseImageAddPixel(dest->data, dest->w, dest->h, dest->c,
					AI_NET_IMAGE_CMODEL2_WIDTH - AI_NET_IMAGE_CMODEL2_WIDTH_PADDING * 2 - AI_NET_IMAGE_CMODEL2_WIDTH_PADDING_ALIGNED_MBLOCK + c, r, k, val);
			}
		}
	}
	
	dest->data_len = (dest->w * dest->h * dest->c);

#if 0
	AiBaseImageNetSave(dest);
#endif
#if 1
	AiBaseLogLayerData(0, "NetPngCmodel2", (char *)(dest->data), (dest->data_len * sizeof(FLOAT_AI_T)));
#endif
	return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
#endif
/*------------------------- End ---------------------------------------------*/
