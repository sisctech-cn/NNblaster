/**
 ******************************************************************************
 * file    ai_net_layer_upsample.h
 * author  sunlingge
 * version V100
 * brief   the ai net layer upsample driver.
 ******************************************************************************
 */
#ifndef AI_NET_LAYER_UPSAMPLE_H
#define AI_NET_LAYER_UPSAMPLE_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiNetLayerUpsampleDefault(AI_NET_CONFIG *net_config, AI_NET_NETWORK *net_network, UINT32_T section_index);
ERROR_T AiNetLayerUpsampleSetLayer(AI_NET_LAYER *net_layer, AI_NET_CONFIG *net_config, UINT32_T section_index, AI_NET_NETWORK *net_network);
ERROR_T AiNetLayerUpsampleForward(AI_NET_STATE *net_state, UINT32_T layer_index);
ERROR_T AiNetLayerUpsampleBackward(AI_NET_STATE *net_state, UINT32_T layer_index);
ERROR_T AiNetLayerUpsampleSetLayerCmodelCommandInit(AI_NET_LAYER *net_layer, INT32_T layer_index);
ERROR_T AiNetLayerUpsampleSetLayerContent(AI_NET_LAYER *net_layer, AI_NET_CONFIG *net_config, UINT32_T section_index, AI_NET_NETWORK *net_network);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

