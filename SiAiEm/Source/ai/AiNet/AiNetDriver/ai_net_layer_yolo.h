/**
 ******************************************************************************
 * file    ai_net_layer_yolo.h
 * author  sunlingge
 * version V100
 * brief   the ai net layer yolo driver.
 ******************************************************************************
 */
#ifndef AI_NET_LAYER_YOLO_H
#define AI_NET_LAYER_YOLO_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiNetLayerYoloDefault(AI_NET_CONFIG *net_config, AI_NET_NETWORK *net_network, UINT32_T section_index);
ERROR_T AiNetLayerYoloSetLayer(AI_NET_LAYER *net_layer, AI_NET_CONFIG *net_config, UINT32_T section_index, AI_NET_NETWORK *net_network);
ERROR_T AiNetLayerYoloForward(AI_NET_STATE *net_state, UINT32_T layer_index);
ERROR_T AiNetLayerYoloBackward(AI_NET_STATE *net_state, UINT32_T layer_index);
INT32_T AiNetLayerYoloEntryIndex(UINT32_T layer_index, INT32_T batch, INT32_T location, INT32_T entry);
UINT32_T AiNetLayerYoloNumDetections(UINT32_T layer_index, FLOAT_AI_T thresh);
ERROR_T AiNetLayerYoloSetLayerCmodelCommandInit(AI_NET_LAYER *net_layer, INT32_T layer_index);
ERROR_T AiNetLayerYoloSetLayerContent(AI_NET_LAYER *net_layer, AI_NET_CONFIG *net_config, UINT32_T section_index, AI_NET_NETWORK *net_network);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

