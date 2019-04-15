/**
 ******************************************************************************
 * file    ai_base_image_resizep.h
 * author  sunlingge
 * version V100
 * brief   the ai base image resize driver.
 ******************************************************************************
 */
#ifndef AI_BASE_IMAGE_RESIZE_H
#define AI_BASE_IMAGE_RESIZE_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/


/*------------------------- Macro Definition --------------------------------*/
#if (AI_PRODUCT_OPTION_NET_IMAGE == AI_NET_IMAGE_416_416)
#define AI_BASE_IMAGE_NET_WIDTH									(416)
#define AI_BASE_IMAGE_NET_HEIGHT								(416)
#define AI_BASE_IMAGE_NET_CHANNEL								(3)
#define AI_BASE_IMAGE_NET_IMAGE_DATA_LEN_MAX					(416*416*3)
#define AI_BASE_IMAGE_NET_CONTENT_LENGTH_MAX					(416*416*3*sizeof(FLOAT_AI_T))
#endif
#if (AI_PRODUCT_OPTION_NET_IMAGE == AI_NET_IMAGE_608_608)
#define AI_BASE_IMAGE_NET_WIDTH									(608)
#define AI_BASE_IMAGE_NET_HEIGHT								(608)
#define AI_BASE_IMAGE_NET_CHANNEL								(3)
#define AI_BASE_IMAGE_NET_IMAGE_DATA_LEN_MAX					(608*608*3)
#define AI_BASE_IMAGE_NET_CONTENT_LENGTH_MAX					(608*608*3*sizeof(FLOAT_AI_T))
#endif

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_base_image_net_struct {
    INT32_T w;
    INT32_T h;
    INT32_T c;
    UINT32_T data_len;
	FLOAT_AI_T data[AI_BASE_IMAGE_NET_IMAGE_DATA_LEN_MAX];
} AI_BASE_IMAGE_NET, *AI_BASE_IMAGE_NET_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseImageNetMakeImage(INT32_T w, INT32_T h, INT32_T c, AI_BASE_IMAGE *image, UINT32_T content_len);
ERROR_T AiBaseImageNet(AI_BASE_IMAGE_NET *dest, AI_BASE_IMAGE_1080P *source);
ERROR_T AiBaseImageNetSave(AI_BASE_IMAGE_NET *image_net);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

