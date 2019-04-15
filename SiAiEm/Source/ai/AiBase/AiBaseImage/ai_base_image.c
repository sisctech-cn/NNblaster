/**
 ******************************************************************************
 * file    ai_base_image.c
 * author  sunlingge
 * version V100
 * brief   the ai base image driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"
#pragma warning(disable:4996)

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
FLOAT_AI_T AiBaseImageGetPixel(FLOAT_AI_T *content, UINT32_T width, UINT32_T height, UINT32_T channel, UINT32_T x, UINT32_T y, UINT32_T c)
{
    if (!((x < width) && (y < height) && (c < channel))) {
		return 0;
	}
    return content[c*height*width + y*width + x];
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseImageSetPixel(FLOAT_AI_T *content, UINT32_T width, UINT32_T height, UINT32_T channel, UINT32_T x, UINT32_T y, UINT32_T c, FLOAT_AI_T val)
{
    if ((x >= width) || (y >= height) || (c >= channel)) 
		return AI_ERROR_GENERAL;
    content[c*height*width + y*width + x] = val;
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseImageAddPixel(FLOAT_AI_T *content, UINT32_T width, UINT32_T height, UINT32_T channel, UINT32_T x, UINT32_T y, UINT32_T c, FLOAT_AI_T val)
{
    if (!((x < width) && (y < height) && (c < channel)))
		return AI_ERROR_GENERAL;
	content[c*height*width + y*width + x] = AiBaseFloatAdd((content[c*height*width + y*width + x]), (val));
	return AI_ERROR_SUCCESS;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageMakeEmptyImage(INT32_T w, INT32_T h, INT32_T c, AI_BASE_IMAGE *image)
{
    image->h = h;
    image->w = w;
    image->c = c;
    return AI_ERROR_SUCCESS;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageMakeImage(INT32_T w, INT32_T h, INT32_T c, AI_BASE_IMAGE *image, UINT32_T content_len)
{
    AiBaseImageMakeEmptyImage(w,h,c, image);
    if ((h*w*c*sizeof(FLOAT_AI_T)) > content_len) {
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
ERROR_T AiBaseImageLoadImageStb(UINT8_T *filename, INT32_T channels, AI_BASE_IMAGE *image, UINT8_T *content, UINT32_T *content_len)
{
    INT32_T w, h, c;
    ERROR_T result;
    INT32_T i,j,k;
    INT32_T dst_index;
    INT32_T src_index;
	UINT32_T content_len_backup;

	content_len_backup = *content_len;
	result =  AiBaseImageStbLoad(filename, &w, &h, &c, channels, (UINT8_T *)(content), (content_len));
    if (result != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }

    c = channels;
    if (AiBaseImageMakeImage(w, h, c, image, content_len_backup) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }
    for(k = 0; k < c; ++k){
        for(j = 0; j < h; ++j){
            for(i = 0; i < w; ++i){
                dst_index = i + w*j + w*h*k;
                src_index = k + c*i + c*w*j;
				image->data[dst_index] = ((FLOAT_AI_T)AiBaseFloatCvtF32Fai(((FLOAT32_T)(content[src_index])) / 255.0f));
            }
        }
    }
	image->data_len = *content_len;
    return AI_ERROR_SUCCESS;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageLoadImage(UINT8_T *filename, INT32_T w, INT32_T h, INT32_T c, AI_BASE_IMAGE *image, UINT8_T *content, UINT32_T *content_len)
{
    ERROR_T result;

    result = AiBaseImageLoadImageStb(filename, c, image, content, content_len);    
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
ERROR_T AiBaseImageLoadImageColor(UINT8_T *filename, INT32_T w, INT32_T h, AI_BASE_IMAGE *image, UINT8_T *content, UINT32_T *content_len)
{
	ERROR_T result;

    result = AiBaseImageLoadImage(filename, w, h, 3, image, content, content_len);
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
ERROR_T AiBaseImageSaveColor(AI_BASE_IMAGE *image, UINT8_T *name, UINT8_T *content, UINT32_T *content_len)
{
	UINT8_T data[AI_BASE_IMAGE_DATA_COLOR_LENGTH_MAX];
    int i,k;
	ERROR_T result;

	for(k = 0; k < image->c; ++k){
        for(i = 0; i < ((image->w)*(image->h)); ++i){
			data[i*(image->c) + k] = (UINT8_T)(255 * AiBaseFloatCvtFaiF32(image->data[i + (k*((image->w)*(image->h)))]));
        }
    }

	result = AiBaseImageStbWritePng(name, image->w, image->h, image->c, data, image->w*image->c, content, content_len);
	return result;
}



/*------------------------- End ---------------------------------------------*/
