/**
 ******************************************************************************
 * file    ai_net_data_ro.h
 * author  sunlingge
 * version V100
 * brief   the ai net read only data.
 ******************************************************************************
 */
#ifndef AI_NET_DATA_RO_H
#define AI_NET_DATA_RO_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_net_data_ro_struct {
	UINT8_T reserved;
} AI_NET_DATA_RO, *AI_NET_DATA_RO_PTR;

#pragma pack(push, 16)
typedef struct ai_net_data_config_struct {
	AI_NET_CLASSES net_classes;
	AI_NET_CONFIG net_config;
	AI_NET_NETWORK net_network;
	AI_NET_LAYER net_layer;
} AI_NET_DATA_CONFIG, *AI_NET_DATA_CONFIG_PTR;
#pragma pack(pop)

/*------------------------- Function Definition -----------------------------*/

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

