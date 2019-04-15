/**
 ******************************************************************************
 * file    ai_net_log_cmodel1.h
 * author  sunlingge
 * version V100
 * brief   the ai net log cmodel1 driver.
 ******************************************************************************
 */
#if (AI_PRODUCT_OPTION_ACCELERATOR_TYPE == AI_PRODUCT_ACCELERATOR_SISCTECH_CMODEL1)
#ifndef AI_NET_LOG_CMODEL1_H
#define AI_NET_LOG_CMODEL1_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/


/*------------------------- Macro Definition --------------------------------*/
#define AI_NET_LOG_CMODEL1_LAYER_DATA_STRING_LENGTH_MAX			(512)
#define AI_NET_LOG_CMODEL1_DATA_LENGTH_MAX						(16*1024*1024)

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_net_image_log_cmodel1_struct {
	INT32_T w;
	INT32_T h;
	INT32_T c;
	UINT32_T data_len;
	FLOAT_AI_T data[AI_NET_LOG_CMODEL1_DATA_LENGTH_MAX];
} AI_NET_IMAGE_LOG_CMODEL1, *AI_NET_IMAGE_LOG_CMODEL1_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiNetLogCmodel1LayerWeights(INT32_T layer_index, UINT8_T *name, UINT8_T *buffer, UINT32_T length);
ERROR_T AiNetLogCmodel1LayerInput(INT32_T layer_index, UINT8_T *name, UINT8_T *buffer, UINT32_T length);
ERROR_T AiNetLogCmodel1LayerOutput(INT32_T layer_index, UINT8_T *name, UINT8_T *buffer, UINT32_T length);
ERROR_T AiNetLogCmodel1LayerAddress(INT32_T layer_index, UINT64_T net_state_address, UINT8_T *name);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
#endif

