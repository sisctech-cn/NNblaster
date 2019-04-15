/**
 ******************************************************************************
 * file    ai_base_image_1080p.h
 * author  sunlingge
 * version V100
 * brief   the ai base image 1080p driver.
 ******************************************************************************
 */
#ifndef AI_BASE_IMAGE_1080P_H
#define AI_BASE_IMAGE_1080P_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/


/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_IMAGE_1080P_IMAGE_DATA_LEN_MAX					(1920*1080*3)
#define AI_BASE_IMAGE_1080P_CONTENT_LENGTH_MAX					(1920*1080*3*sizeof(FLOAT_AI_T))

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_base_image_1080p_struct {
    INT32_T w;
    INT32_T h;
    INT32_T c;
    UINT32_T data_len;
	FLOAT_AI_T data[AI_BASE_IMAGE_1080P_IMAGE_DATA_LEN_MAX];
} AI_BASE_IMAGE_1080P, *AI_BASE_IMAGE_1080P_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseImageF16Load1080pPng(AI_BASE_IMAGE_1080P *image_1080p);
ERROR_T AiBaseImageLoad1080pPng(AI_BASE_IMAGE_1080P *image_1080p);
ERROR_T AiBaseImageSave1080pPng(AI_BASE_IMAGE_1080P *image_1080p);
ERROR_T AiBaseImageLoad1080p(void);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

