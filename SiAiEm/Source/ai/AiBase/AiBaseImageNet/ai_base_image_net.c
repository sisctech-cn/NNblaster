/**
 ******************************************************************************
 * file    ai_base_image_net.c
 * author  sunlingge
 * version V100
 * brief   the ai base image net driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"
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
ERROR_T AiBaseImageNetMakeImage(INT32_T w, INT32_T h, INT32_T c, AI_BASE_IMAGE *image, UINT32_T content_len)
{
	AiBaseImageMakeEmptyImage(w, h, c, image);
	if ((h*w*c * sizeof(FLOAT_AI_T)) > content_len) {
		return AI_ERROR_GENERAL;
	}
	memset((char *)image->data, 0, content_len);
	return AI_ERROR_SUCCESS;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageNetSave(AI_BASE_IMAGE_NET *image_net)
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
ERROR_T AiBaseImageNet(AI_BASE_IMAGE_NET *dest, AI_BASE_IMAGE_1080P *source)
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


	AiBaseImageMakeImage(AI_BASE_IMAGE_NET_WIDTH, AI_BASE_IMAGE_NET_HEIGHT, AI_BASE_IMAGE_NET_CHANNEL, (AI_BASE_IMAGE *)dest, AI_BASE_IMAGE_NET_CONTENT_LENGTH_MAX);

	content = (FLOAT_AI_T *)g_ai_base_image_1080p_content;
	part_w = AI_BASE_IMAGE_NET_WIDTH;
	part_h = source->h;
	part_c = source->c;
	w_scale = (FLOAT_AI_T)(source->w - 1) / (AI_BASE_IMAGE_NET_WIDTH - 1);
	h_scale = (FLOAT_AI_T)(source->h - 1) / (AI_BASE_IMAGE_NET_HEIGHT - 1);
	for (k = 0; k < source->c; k++) {
		for (r = 0; r < source->h; ++r) {
			for (c = 0; c < AI_BASE_IMAGE_NET_WIDTH; ++c) {
				val = 0;
				if ((c == AI_BASE_IMAGE_NET_WIDTH - 1) || (source->w == 1)) {
					val = AiBaseImageGetPixel(source->data, source->w, source->h, source->c, source->w - 1, r, k);
				}
				else {
					sx = c*w_scale;
					ix = (INT32_T)sx;
					dx = sx - ix;
					val = (1 - dx) *  AiBaseImageGetPixel(source->data, source->w, source->h, source->c, ix, r, k) + 
						  dx * AiBaseImageGetPixel(source->data, source->w, source->h, source->c, ix + 1, r, k);
				}
				AiBaseImageSetPixel(content, part_w, part_h, part_c, c, r, k, val);
			}
		}
	}
	for (k = 0; k < source->c; ++k) {
		for (r = 0; r < AI_BASE_IMAGE_NET_HEIGHT; ++r) {
			sy = r*h_scale;
			iy = (INT32_T)sy;
			dy = sy - iy;
			for (c = 0; c < AI_BASE_IMAGE_NET_WIDTH; ++c) {
				val = (1 - dy) * AiBaseImageGetPixel(content, part_w, part_h, part_c, c, iy, k);
				AiBaseImageSetPixel(dest->data, dest->w, dest->h, dest->c, c, r, k, val);
			}
			if ((r == AI_BASE_IMAGE_NET_HEIGHT - 1) || (source->h == 1)) 
				continue;
			for (c = 0; c < AI_BASE_IMAGE_NET_WIDTH; ++c) {
				val = dy * AiBaseImageGetPixel(content, part_w, part_h, part_c, c, iy + 1, k);
				AiBaseImageAddPixel(dest->data, dest->w, dest->h, dest->c, c, r, k, val);
			}
		}
	}
	
	dest->data_len = (dest->w * dest->h * dest->c);

#if 0
	AiBaseImageNetSave(dest);
#endif
#if 1
	AiBaseLogLayerData(0, "NetPng", (char *)(dest->data), (dest->data_len * sizeof(FLOAT_AI_T)));
#endif
	return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
