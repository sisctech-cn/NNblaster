/**
 ******************************************************************************
 * file    ai_net_image.c
 * author  sunlingge
 * version V100
 * brief   the ai net image driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_net_global.h"
#pragma warning(disable:4996)

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
ERROR_T AiNetImageF16Load(AI_NET_IMAGE *image_net)
{
#if (AI_PRODUCT_OPTION_FP16_TEST_PRELOAD == 1)
	UINT8_T file_path[256];
	FILE *f;
	UINT32_T length;
	UINT32_T read_length;

	sprintf((char *)file_path, AI_PRODUCT_FILE_PATH""AI_PRODUCT_YOLOV3_TEST_DTA_PATH""AI_PRODUCT_YOLOV3_NET_PNG_FILE_FP16);
	f = fopen(file_path, "rb");
	if (!f) {
		return AI_ERROR_GENERAL;
	}

	length = (AI_NET_IMAGE_WIDTH * AI_NET_IMAGE_HEIGHT * AI_NET_IMAGE_CHANNEL);
	image_net->w = AI_NET_IMAGE_WIDTH;
	image_net->h = AI_NET_IMAGE_HEIGHT;
	image_net->c = AI_NET_IMAGE_CHANNEL;
	image_net->data_len = length;

	read_length = (UINT32_T)fread((char *)image_net->data, 1, sizeof(FLOAT_AI_T)*length, f);
	if (read_length != sizeof(FLOAT_AI_T)*length) {
		fclose(f);
		return AI_ERROR_GENERAL;
	}

	fclose(f);
	return AI_ERROR_SUCCESS;
#else
	return AI_ERROR_GENERAL;
#endif
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiNetImageSave(AI_NET_IMAGE *image_net)
{
    UINT8_T file_path[256];
	ERROR_T result;

    sprintf((char *)file_path, AI_PRODUCT_LOG_IDENT_PATH"/net.png");
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
ERROR_T AiNetImage(AI_NET_IMAGE *dest, AI_BASE_IMAGE_1080P *source)
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
	ERROR_T result;

	result = AiNetImageF16Load(dest);
	if (result == AI_ERROR_SUCCESS)
		return AI_ERROR_SUCCESS;

	AiBaseImageMakeImage(AI_NET_IMAGE_WIDTH, AI_NET_IMAGE_HEIGHT, AI_NET_IMAGE_CHANNEL, (AI_BASE_IMAGE *)dest, AI_NET_IMAGE_CONTENT_LENGTH_MAX);

	content = (FLOAT_AI_T *)g_ai_base_image_1080p_content;
	part_w = AI_NET_IMAGE_WIDTH;
	part_h = source->h;
	part_c = source->c;
	w_scale = AiBaseFloatDiv(AiBaseFloatCvtF32Fai((FLOAT32_T)(source->w - 1)), AiBaseFloatCvtF32Fai((FLOAT32_T)(AI_NET_IMAGE_WIDTH - 1)));
	h_scale = AiBaseFloatDiv(AiBaseFloatCvtF32Fai((FLOAT32_T)(source->h - 1)), AiBaseFloatCvtF32Fai((FLOAT32_T)(AI_NET_IMAGE_HEIGHT - 1)));
	for (k = 0; k < source->c; k++) {
		for (r = 0; r < source->h; ++r) {
			for (c = 0; c < AI_NET_IMAGE_WIDTH; ++c) {
				val = 0;
				if ((c == AI_NET_IMAGE_WIDTH - 1) || (source->w == 1)) {
					val = (AiBaseImageGetPixel(source->data, source->w, source->h, source->c, source->w - 1, r, k));
				}
				else {
					sx = AiBaseFloatMul(AiBaseFloatCvtF32Fai((FLOAT32_T)(c)), w_scale);
					ix = AiBaseFloatCvtFaiI32(sx);
					dx = AiBaseFloatSub(sx, AiBaseFloatCvtI32Fai(ix));
					val = AiBaseFloatAdd(AiBaseFloatMul(AiBaseFloatSub(AiBaseFloatCvtF32Fai(1.0f), dx), 
						(AiBaseImageGetPixel(source->data, source->w, source->h, source->c, ix, r, k))),
						AiBaseFloatMul(dx, (AiBaseImageGetPixel(source->data, source->w, source->h, source->c, ix + 1, r, k))));
				}
				AiBaseImageSetPixel(content, part_w, part_h, part_c, c, r, k, (val));
			}
		}
	}
	for (k = 0; k < source->c; ++k) {
		for (r = 0; r < AI_NET_IMAGE_HEIGHT; ++r) {
			sy = AiBaseFloatMul(AiBaseFloatCvtF32Fai((FLOAT32_T)(r)), h_scale);
			iy = AiBaseFloatCvtFaiI32(sy);
			dy = AiBaseFloatSub(sy, AiBaseFloatCvtI32Fai(iy));
			for (c = 0; c < AI_NET_IMAGE_WIDTH; ++c) {
				val = AiBaseFloatMul(AiBaseFloatSub(AiBaseFloatCvtF32Fai(1.0f), dy), 
					(AiBaseImageGetPixel(content, part_w, part_h, part_c, c, iy, k)));
				AiBaseImageSetPixel(dest->data, dest->w, dest->h, dest->c, c, r, k, (val));
			}
			if ((r == AI_NET_IMAGE_HEIGHT - 1) || (source->h == 1)) 
				continue;
			for (c = 0; c < AI_NET_IMAGE_WIDTH; ++c) {
				val = AiBaseFloatMul(dy, (AiBaseImageGetPixel(content, part_w, part_h, part_c, c, iy + 1, k)));
				AiBaseImageAddPixel(dest->data, dest->w, dest->h, dest->c, c, r, k, (val));
			}
		}
	}

	dest->data_len = (dest->w * dest->h * dest->c);

#if 1
	AiBaseImageNetSave((AI_BASE_IMAGE_NET *)dest);
#endif
#if 1
	AiBaseLogLayerData(0, "NetPng", (char *)(dest->data), (dest->data_len * sizeof(FLOAT_AI_T)));
#endif
	return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/

/*------------------------- End ---------------------------------------------*/
