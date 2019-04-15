/**
 ******************************************************************************
 * file    ai_net_network.h
 * author  sunlingge
 * version V100
 * brief   the ai net network driver.
 ******************************************************************************
 */
#ifndef AI_NET_NETWORK_H
#define AI_NET_NETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_net_network_data_struct {
    UINT32_T batch;
    UINT32_T inputs;
    UINT32_T h;
    UINT32_T w;
    UINT32_T c;
	INT32_T layer_index;
    UINT32_T time_steps;
	UINT32_T outputs;
	INT32_T seen;
	AI_NET_LAYER_CONTENT output;
	AI_NET_NETWORK_CONTENT workspace;
	AI_NET_NETWORK_BUFFER network_rw;
	AI_NET_CONFIG_SECTION *network_config;
} AI_NET_NETWORK_DATA, *AI_NET_NETWORK_DATA_PTR;

typedef struct ai_net_network_struct {
	AI_NET_NETWORK_DATA network_data;
} AI_NET_NETWORK, *AI_NET_NETWORK_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiNetNetworkInit(AI_NET_NETWORK *net_network, AI_NET_CONFIG *net_config);
ERROR_T AiNetNetworkConfigDefault(AI_NET_CONFIG *net_config, UINT32_T section_index);
ERROR_T AiNetNetworkSetNetwork(AI_NET_NETWORK *net_network, AI_NET_CONFIG *net_config, UINT32_T section_index);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

