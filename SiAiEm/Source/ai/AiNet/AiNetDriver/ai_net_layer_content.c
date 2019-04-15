/**
 ******************************************************************************
 * file    ai_net_layer_buffer.c
 * author  sunlingge
 * version V100
 * brief   the ai net layer buffer driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_net_global.h"
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
ERROR_T AiNetLayerBufferInit(AI_NET_LAYER_BUFFER *layer_buffer, UINT8_T *buffer, UINT32_T length)
{
	UINT32_T layer_index;
	UINT32_T offset_aligned_16_byte;

	for (layer_index = 0; layer_index < AI_NET_LAYER_MAX; layer_index++) {
		layer_buffer->stack[layer_index].layer_length = 0;
		layer_buffer->stack[layer_index].layer_offset = 0;
	}

	offset_aligned_16_byte = (UINT32_T)((((((UINT64_T)(buffer)) + 15) / 16) * 16) - ((UINT64_T)(buffer)));

	layer_buffer->offset_aligned_16_btye = offset_aligned_16_byte;
	layer_buffer->offset = offset_aligned_16_byte;
	layer_buffer->length = length - offset_aligned_16_byte;
	layer_buffer->buffer = buffer;

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerBufferInitOffset(AI_NET_LAYER_BUFFER *layer_buffer)
{
	layer_buffer->offset = layer_buffer->offset_aligned_16_btye;
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerBufferSetLayerOffset(AI_NET_LAYER_BUFFER *layer_buffer, UINT32_T layer_index)
{
	if (layer_index == 0) {
		layer_buffer->stack[layer_index].layer_offset = layer_buffer->offset;
	} else {
		layer_buffer->stack[layer_index].layer_offset = layer_buffer->offset + layer_buffer->stack[layer_index-1].layer_offset;
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
ERROR_T AiNetLayerBufferLayerOffsetStack(AI_NET_LAYER_BUFFER *layer_buffer, UINT32_T layer_index_start, UINT32_T layer_index_end)
{
	UINT32_T layer_index;
	UINT32_T layer_offset_base;
	AI_NET_LAYER_BUFFER_STACK *stack;

	stack = &(layer_buffer->stack[layer_index_start - 1]);
	layer_offset_base = stack->layer_offset + stack->layer_length;
	for (layer_index = layer_index_start; layer_index <= layer_index_end; layer_index++) {
		stack = &(layer_buffer->stack[layer_index]);
		stack->layer_offset = layer_offset_base + stack->layer_offset;
		if ((stack->layer_offset + stack->layer_length) > layer_buffer->length) {
			AiBaseLogErrorCritial();
			return AI_ERROR_GENERAL;
		}
	}
	for (layer_index = layer_index_end+1; layer_index <= (AI_NET_LAYER_MAX-1); layer_index++) {
		stack = &(layer_buffer->stack[layer_index]);
		if (stack->layer_offset == 0)
			continue;
		stack->layer_offset = layer_offset_base + stack->layer_offset;
		if ((stack->layer_offset + stack->layer_length) > layer_buffer->length) {
			AiBaseLogErrorCritial();
			return AI_ERROR_GENERAL;
		}
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
ERROR_T AiNetLayerContentInit(AI_NET_LAYER_CONTENT *layer_content, UINT32_T length, AI_NET_LAYER_BUFFER *layer_buffer, UINT32_T layer_index)
{
	UINT32_T offset;
	UINT32_T length_aligned_16_byte;

	length_aligned_16_byte = ((length + 15) / 16) * 16;

	offset = layer_buffer->offset;
	memset((char *)layer_content, 0, sizeof(AI_NET_LAYER_CONTENT));
	if (offset + length_aligned_16_byte > layer_buffer->length) {
		layer_buffer->offset = offset + length_aligned_16_byte;
		AiBaseLogErrorCritial();
		return AI_ERROR_GENERAL;
	}
	layer_content->length = length;
	layer_content->offset = offset;
	layer_buffer->offset = offset + length_aligned_16_byte;
	layer_content->layer_buffer = layer_buffer;
	layer_content->layer_index = layer_index;

	layer_buffer->stack[layer_index].layer_length = layer_buffer->offset;

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
UINT8_T *AiNetLayerContentGetAddress(AI_NET_LAYER_CONTENT *layer_content)
{
	UINT8_T *base_address;
	UINT32_T offset;
	INT32_T layer_index;

	if (layer_content->layer_buffer == NULL) {
		return 0;
	}
	base_address = layer_content->layer_buffer->buffer;
	offset = layer_content->offset;
	layer_index = layer_content->layer_index;
	offset += layer_content->layer_buffer->stack[layer_index].layer_offset;
	base_address = base_address + offset;
	
	return base_address;
}


/*------------------------- End ---------------------------------------------*/
