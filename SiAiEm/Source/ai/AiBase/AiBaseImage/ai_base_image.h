/**
 ******************************************************************************
 * file    ai_base_image.h
 * author  sunlingge
 * version V100
 * brief   the ai base image driver.
 ******************************************************************************
 */
#ifndef AI_BASE_IMAGE_H
#define AI_BASE_IMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/


/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_IMAGE_DATA_LEN_MAX                (1)
#define AI_BASE_IMAGE_DATA_COLOR_LENGTH_MAX		  ((1920*1080*3))

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_base_image_struct {
    INT32_T w;
    INT32_T h;
    INT32_T c;
    UINT32_T data_len;
	FLOAT_AI_T data[AI_BASE_IMAGE_DATA_LEN_MAX];
} AI_BASE_IMAGE, *AI_BASE_IMAGE_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseImageMakeEmptyImage(INT32_T w, INT32_T h, INT32_T c, AI_BASE_IMAGE *image);
FLOAT_AI_T AiBaseImageGetPixel(FLOAT_AI_T *content, UINT32_T width, UINT32_T height, UINT32_T channel, UINT32_T x, UINT32_T y, UINT32_T c);
ERROR_T AiBaseImageSetPixel(FLOAT_AI_T *content, UINT32_T width, UINT32_T height, UINT32_T channel, UINT32_T x, UINT32_T y, UINT32_T c, FLOAT_AI_T val);
ERROR_T AiBaseImageAddPixel(FLOAT_AI_T *content, UINT32_T width, UINT32_T height, UINT32_T channel, UINT32_T x, UINT32_T y, UINT32_T c, FLOAT_AI_T val);
ERROR_T AiBaseImageMakeImage(INT32_T w, INT32_T h, INT32_T c, AI_BASE_IMAGE *image, UINT32_T content_len);
ERROR_T AiBaseImageLoadImageColor(UINT8_T *filename, INT32_T w, INT32_T h, AI_BASE_IMAGE *image, UINT8_T *content, UINT32_T *content_len);
ERROR_T AiBaseImageSaveColor(AI_BASE_IMAGE *image, UINT8_T *name, UINT8_T *content, UINT32_T *content_len);
ERROR_T AiBaseImageLoadStb(UINT8_T *filename, INT32_T channels, AI_BASE_IMAGE *image);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

