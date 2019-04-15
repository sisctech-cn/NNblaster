/**
 ******************************************************************************
 * file    ai_net_image.h
 * author  sunlingge
 * version V100
 * brief   the ai net image driver.
 ******************************************************************************
 */
#ifndef AI_NET_IMAGE_H
#define AI_NET_IMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/
#define AI_NET_IMAGE_WIDTH_PADDING						(0)
#define AI_NET_IMAGE_HEIGHT_PADDING						(0)
#define AI_NET_IMAGE_WIDTH_PADDING_ALIGNED_MBLOCK		(0)
#define AI_NET_IMAGE_HEIGHT_PADDING_ALIGNED_MBLOCK		(0)

/*------------------------- Macro Definition --------------------------------*/
#if (AI_PRODUCT_OPTION_NET_IMAGE == AI_NET_IMAGE_416_416)
#define AI_NET_IMAGE_WIDTH									(416+AI_NET_IMAGE_WIDTH_PADDING*2+AI_NET_IMAGE_WIDTH_PADDING_ALIGNED_MBLOCK)
#define AI_NET_IMAGE_HEIGHT									(416+AI_NET_IMAGE_HEIGHT_PADDING*2+AI_NET_IMAGE_HEIGHT_PADDING_ALIGNED_MBLOCK)
#define AI_NET_IMAGE_CHANNEL								(3)
#define AI_NET_IMAGE_IMAGE_DATA_LEN_MAX						(AI_NET_IMAGE_WIDTH*AI_NET_IMAGE_HEIGHT*AI_NET_IMAGE_CHANNEL)
#define AI_NET_IMAGE_CONTENT_LENGTH_MAX						(AI_NET_IMAGE_WIDTH*AI_NET_IMAGE_HEIGHT*AI_NET_IMAGE_CHANNEL*sizeof(FLOAT_AI_T))
#endif
#if (AI_PRODUCT_OPTION_NET_IMAGE == AI_NET_IMAGE_608_608)
#define AI_NET_IMAGE_WIDTH									(608+AI_NET_IMAGE_WIDTH_PADDING*2+AI_NET_IMAGE_WIDTH_PADDING_ALIGNED_MBLOCK)
#define AI_NET_IMAGE_HEIGHT									(608+AI_NET_IMAGE_HEIGHT_PADDING*2+AI_NET_IMAGE_HEIGHT_PADDING_ALIGNED_MBLOCK)
#define AI_NET_IMAGE_CHANNEL								(3)
#define AI_NET_IMAGE_IMAGE_DATA_LEN_MAX						(AI_NET_IMAGE_WIDTH*AI_NET_IMAGE_HEIGHT*AI_NET_IMAGE_CHANNEL)
#define AI_NET_IMAGE_CONTENT_LENGTH_MAX						(AI_NET_IMAGE_WIDTH*AI_NET_IMAGE_HEIGHT*AI_NET_IMAGE_CHANNEL*sizeof(FLOAT_AI_T))
#endif

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_net_image_struct {
    INT32_T w;
    INT32_T h;
    INT32_T c;
    UINT32_T data_len;
	FLOAT_AI_T data[AI_NET_IMAGE_IMAGE_DATA_LEN_MAX];
} AI_NET_IMAGE, *AI_NET_IMAGE_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiNetImage(AI_NET_IMAGE *dest, AI_BASE_IMAGE_1080P *source);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

