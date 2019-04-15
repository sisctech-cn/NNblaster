/**
 ******************************************************************************
 * file    ai_net_layer_convolutional.h
 * author  sunlingge
 * version V100
 * brief   the ai net layer convolutional driver.
 ******************************************************************************
 */
#ifndef AI_NET_LAYER_CONVOLUTIONAL_H
#define AI_NET_LAYER_CONVOLUTIONAL_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiNetLayerConvolutionalDefault(AI_NET_CONFIG *net_config, AI_NET_NETWORK *net_network, UINT32_T section_index);
ERROR_T AiNetLayerConvolutionalSetLayer(AI_NET_LAYER *net_layer, AI_NET_CONFIG *net_config, UINT32_T section_index, AI_NET_NETWORK *net_network);
ERROR_T AiNetLayerConvolutionalForward(AI_NET_STATE *net_state, UINT32_T layer_index);
ERROR_T AiNetLayerConvolutionalBackward(AI_NET_STATE *net_state, UINT32_T layer_index);
ERROR_T AiNetLayerConvolutionalUpdate(AI_NET_STATE *net_state, UINT32_T layer_index);
ERROR_T AiNetLayerConvolutionalSetLayerCmodelCommandInit(AI_NET_LAYER *net_layer, INT32_T layer_index);
ERROR_T AiNetLayerConvolutionalSetLayerContent(AI_NET_LAYER *net_layer, AI_NET_CONFIG *net_config, UINT32_T section_index, AI_NET_NETWORK *net_network);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

