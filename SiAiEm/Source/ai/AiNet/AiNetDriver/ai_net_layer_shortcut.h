/**
 ******************************************************************************
 * file    ai_net_layer_shortcut.h
 * author  sunlingge
 * version V100
 * brief   the ai net layer shortcut driver.
 ******************************************************************************
 */
#ifndef AI_NET_LAYER_SHORTCUT_H
#define AI_NET_LAYER_SHORTCUT_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiNetLayerShortcutDefault(AI_NET_CONFIG *net_config, AI_NET_NETWORK *net_network, UINT32_T section_index);
ERROR_T AiNetLayerShortcutSetLayer(AI_NET_LAYER *net_layer, AI_NET_CONFIG *net_config, UINT32_T section_index, AI_NET_NETWORK *net_network);
ERROR_T AiNetLayerShortcutForward(AI_NET_STATE *net_state, UINT32_T layer_index);
ERROR_T AiNetLayerShortcutBackward(AI_NET_STATE *net_state, UINT32_T layer_index);
ERROR_T AiNetLayerShortcutSetLayerCmodelCommandInit(AI_NET_LAYER *net_layer, INT32_T layer_index);
ERROR_T AiNetLayerShortcutSetLayerContent(AI_NET_LAYER *net_layer, AI_NET_CONFIG *net_config, UINT32_T section_index, AI_NET_NETWORK *net_network);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

