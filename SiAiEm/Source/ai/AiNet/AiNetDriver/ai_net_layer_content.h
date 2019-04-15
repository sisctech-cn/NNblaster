/**
 ******************************************************************************
 * file    ai_net_layer_buffer_ro.h
 * author  sunlingge
 * version V100
 * brief   the ai net layer buffer ro driver.
 ******************************************************************************
 */
#ifndef AI_NET_LAYER_BUFFER_H
#define AI_NET_LAYER_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_NET_LAYER_CONTENT_RO				(0)
#define AI_NET_LAYER_CONTENT_RW				(1)	
#define AI_NET_LAYER_MAX					(AI_NET_CONFIG_SECTION_MAX-1)

/*------------------------- Macro Definition --------------------------------*/
typedef struct ai_net_layer_buffer_stack_struct {
	UINT32_T layer_length;
	UINT32_T layer_offset;
} AI_NET_LAYER_BUFFER_STACK, *AI_NET_LAYER_BUFFER_STACK_PTR;

typedef struct ai_net_layer_buffer_struct {
	AI_NET_LAYER_BUFFER_STACK stack[AI_NET_LAYER_MAX];
	UINT32_T offset_aligned_16_btye;
	UINT32_T offset;
	UINT32_T length;
	UINT8_T *buffer;
} AI_NET_LAYER_BUFFER, *AI_NET_LAYER_BUFFER_PTR;

typedef struct ai_net_layer_content_struct {
	UINT32_T offset;
	UINT32_T length;
	INT32_T layer_index;
	AI_NET_LAYER_BUFFER *layer_buffer;
} AI_NET_LAYER_CONTENT, *AI_NET_LEYER_CONTENT_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiNetLayerBufferInit(AI_NET_LAYER_BUFFER *layer_buffer, UINT8_T *buffer, UINT32_T length);
ERROR_T AiNetLayerBufferInitOffset(AI_NET_LAYER_BUFFER *layer_buffer);
ERROR_T AiNetLayerBufferSetLayerOffset(AI_NET_LAYER_BUFFER *layer_buffer, UINT32_T layer_index);
ERROR_T AiNetLayerBufferLayerOffsetStack(AI_NET_LAYER_BUFFER *layer_buffer, UINT32_T layer_index_start, UINT32_T layer_index_end);
ERROR_T AiNetLayerContentInit(AI_NET_LAYER_CONTENT *layer_content, UINT32_T length, AI_NET_LAYER_BUFFER *layer_buffer, UINT32_T layer_index);
UINT8_T *AiNetLayerContentGetAddress(AI_NET_LAYER_CONTENT *layer_content);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

