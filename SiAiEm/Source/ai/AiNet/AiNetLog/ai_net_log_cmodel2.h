/**
 ******************************************************************************
 * file    ai_net_log_cmodel2.h
 * author  sunlingge
 * version V100
 * brief   the ai net log cmodel2 driver.
 ******************************************************************************
 */
#if (AI_PRODUCT_OPTION_ACCELERATOR_TYPE == AI_PRODUCT_ACCELERATOR_SISCTECH_CMODEL2)
#ifndef AI_NET_LOG_CMODEL2_H
#define AI_NET_LOG_CMODEL2_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/


/*------------------------- Macro Definition --------------------------------*/
#define AI_NET_LOG_CMODEL2_LAYER_DATA_STRING_LENGTH_MAX			(512)
#define AI_NET_LOG_CMODEL2_DATA_LENGTH_MAX						(16*1024*1024)

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_net_image_log_cmodel2_struct {
	INT32_T w;
	INT32_T h;
	INT32_T c;
	UINT32_T data_len;
	FLOAT_AI_T data[AI_NET_LOG_CMODEL2_DATA_LENGTH_MAX];
} AI_NET_IMAGE_LOG_CMODEL2, *AI_NET_IMAGE_LOG_CMODEL2_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiNetLogCmodel2LayerWeights(INT32_T layer_index, UINT8_T *name, UINT8_T *buffer, UINT32_T length);
ERROR_T AiNetLogCmodel2LayerInput(INT32_T layer_index, UINT8_T *name, UINT8_T *buffer, UINT32_T length);
ERROR_T AiNetLogCmodel2LayerOutput(INT32_T layer_index, UINT8_T *name, UINT8_T *buffer, UINT32_T length);
ERROR_T AiNetLogCmodel2LayerAddress(INT32_T layer_index, UINT64_T net_state_address, UINT8_T *name);


#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
#endif

