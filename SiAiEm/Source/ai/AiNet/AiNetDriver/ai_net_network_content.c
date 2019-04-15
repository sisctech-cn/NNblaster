/**
 ******************************************************************************
 * file    ai_net_network_buffer.c
 * author  sunlingge
 * version V100
 * brief   the ai net network buffer driver.
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
ERROR_T AiNetNetworkBufferInit(AI_NET_NETWORK_BUFFER *network_buffer)
{
	network_buffer->offset = 0;
	network_buffer->length = AI_NET_NETWORK_BUFFER_LENGTH_MAX;
//	memset((char *)(network_buffer->buffer), 0, network_buffer->length);

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetNetworkContentInit(AI_NET_NETWORK_CONTENT *network_content, UINT32_T length, AI_NET_NETWORK_BUFFER *network_buffer)
{
	memset((char *)network_content, 0, sizeof(AI_NET_NETWORK_CONTENT));
	if (network_buffer->offset + length > network_buffer->length) {
		network_buffer->offset = network_buffer->offset + length;
		return AI_ERROR_GENERAL;
	}
	network_content->length = length;
	network_content->offset = network_buffer->offset;
	network_buffer->offset = (network_buffer->offset) + length;
	network_content->network_buffer = network_buffer;

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
UINT8_T *AiNetNetworkContentGetAddress(AI_NET_NETWORK_CONTENT *network_content)
{
	UINT8_T *base_address;
	UINT32_T offset;

	base_address = network_content->network_buffer->buffer;
	offset = network_content->offset;
	base_address = base_address + offset;
	
	return base_address;
}

/*------------------------- End ---------------------------------------------*/
