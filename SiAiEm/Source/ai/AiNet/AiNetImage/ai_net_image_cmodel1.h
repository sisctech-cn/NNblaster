/**
 ******************************************************************************
 * file    ai_net_image_cmodel1.h
 * author  sunlingge
 * version V100
 * brief   the ai net image cmodel1 driver.
 ******************************************************************************
 */
#if (AI_PRODUCT_OPTION_ACCELERATOR_TYPE == AI_PRODUCT_ACCELERATOR_SISCTECH_CMODEL1)
#ifndef AI_NET_IMAGE_CMODEL1_H
#define AI_NET_IMAGE_CMODEL1_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/
#define AI_NET_IMAGE_CMODEL1_WIDTH_PADDING						(1)
#define AI_NET_IMAGE_CMODEL1_HEIGHT_PADDING						(0)
#define AI_NET_IMAGE_CMODEL1_WIDTH_PADDING_ALIGNED_MBLOCK		(6)
#define AI_NET_IMAGE_CMODEL1_HEIGHT_PADDING_ALIGNED_MBLOCK		(0)

/*------------------------- Macro Definition --------------------------------*/
#if (AI_PRODUCT_OPTION_NET_IMAGE == AI_NET_IMAGE_416_416)
#define AI_NET_IMAGE_CMODEL1_WIDTH								(416+AI_NET_IMAGE_CMODEL1_WIDTH_PADDING*2+AI_NET_IMAGE_CMODEL1_WIDTH_PADDING_ALIGNED_MBLOCK)
#define AI_NET_IMAGE_CMODEL1_HEIGHT								(416)
#define AI_NET_IMAGE_CMODEL1_CHANNEL								(3)
#define AI_NET_IMAGE_CMODEL1_IMAGE_DATA_LEN_MAX					(AI_NET_IMAGE_CMODEL1_WIDTH*AI_NET_IMAGE_CMODEL1_HEIGHT*AI_NET_IMAGE_CMODEL1_CHANNEL)
#define AI_NET_IMAGE_CMODEL1_CONTENT_LENGTH_MAX					(AI_NET_IMAGE_CMODEL1_WIDTH*AI_NET_IMAGE_CMODEL1_HEIGHT*AI_NET_IMAGE_CMODEL1_CHANNEL*sizeof(FLOAT_AI_T))
#endif
#if (AI_PRODUCT_OPTION_NET_IMAGE == AI_NET_IMAGE_608_608)
#define AI_NET_IMAGE_CMODEL1_WIDTH								(608+AI_NET_IMAGE_CMODEL1_WIDTH_PADDING*2+AI_NET_IMAGE_CMODEL1_WIDTH_PADDING_ALIGNED_MBLOCK)
#define AI_NET_IMAGE_CMODEL1_HEIGHT								(608)
#define AI_NET_IMAGE_CMODEL1_CHANNEL							(3)
#define AI_NET_IMAGE_CMODEL1_IMAGE_DATA_LEN_MAX					(AI_NET_IMAGE_CMODEL1_WIDTH*AI_NET_IMAGE_CMODEL1_HEIGHT*AI_NET_IMAGE_CMODEL1_CHANNEL)
#define AI_NET_IMAGE_CMODEL1_CONTENT_LENGTH_MAX					(AI_NET_IMAGE_CMODEL1_WIDTH*AI_NET_IMAGE_CMODEL1_HEIGHT*AI_NET_IMAGE_CMODEL1_CHANNEL*sizeof(FLOAT_AI_T))
#endif

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_net_image_cmodel1_struct {
    INT32_T w;
    INT32_T h;
    INT32_T c;
    UINT32_T data_len;
	FLOAT_AI_T data[AI_NET_IMAGE_CMODEL1_IMAGE_DATA_LEN_MAX];
} AI_NET_IMAGE_CMODEL1, *AI_NET_IMAGE_CMODEL1_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiNetImageCmodel1(AI_NET_IMAGE_CMODEL1 *dest, AI_BASE_IMAGE_1080P *source);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
#endif

