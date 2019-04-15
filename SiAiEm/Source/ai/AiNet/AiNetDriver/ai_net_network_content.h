/**
 ******************************************************************************
 * file    ai_net_network_buffer_ro.h
 * author  sunlingge
 * version V100
 * brief   the ai net network buffer ro driver.
 ******************************************************************************
 */
#ifndef AI_NET_NETWORK_BUFFER_H
#define AI_NET_NETWORK_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_NET_NETWORK_CONTENT_RO				(0)
#define AI_NET_NETWORK_CONTENT_RW				(1)	
#if (AI_PRODUCT_OPTION_NET_IMAGE == AI_NET_IMAGE_416_416) 
#define AI_NET_NETWORK_BUFFER_LENGTH_MAX		(48*1024*1024)
#endif
#if (AI_PRODUCT_OPTION_NET_IMAGE == AI_NET_IMAGE_608_608)
#define AI_NET_NETWORK_BUFFER_LENGTH_MAX		(102*1024*1024)
#endif
/*------------------------- Macro Definition --------------------------------*/
typedef struct ai_net_networl_memory_struct {
	UINT8_T memory[AI_NET_NETWORK_BUFFER_LENGTH_MAX];
} AI_NET_NETWORK_MEMORY, *AI_NET_NETWORK_MEMORY_PTR;

typedef struct ai_net_network_buffer_struct {
	UINT32_T offset;
	UINT32_T length;
	UINT8_T buffer[AI_NET_NETWORK_BUFFER_LENGTH_MAX];
} AI_NET_NETWORK_BUFFER, *AI_NET_NETWORK_BUFFER_PTR;

typedef struct ai_net_network_content_struct {
	UINT32_T offset;
	UINT32_T length;
	AI_NET_NETWORK_BUFFER *network_buffer;
} AI_NET_NETWORK_CONTENT, *AI_NET_NETWORK_CONTENT_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiNetNetworkBufferInit(AI_NET_NETWORK_BUFFER *network_buffer);
ERROR_T AiNetNetworkContentInit(AI_NET_NETWORK_CONTENT *network_content, UINT32_T length, AI_NET_NETWORK_BUFFER *network_buffer);
UINT8_T *AiNetNetworkContentGetAddress(AI_NET_NETWORK_CONTENT *network_content);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

