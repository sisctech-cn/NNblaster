/**
 ******************************************************************************
 * file    ai_net_weights.h
 * author  sunlingge
 * version V100
 * brief   the ai net weights driver.
 ******************************************************************************
 */
#ifndef AI_NET_WEIGHTS_H
#define AI_NET_WEIGHTS_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/


/*------------------------- Macro Definition --------------------------------*/


/*------------------------- Type Definition----------------------------------*/
typedef struct ai_net_weights_struct {
	UINT8_T reserved;
} AI_NET_WEIGHTS, *AI_NET_WEIGHTS_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiNetWeightsF16Save(AI_NET_NETWORK *net_network, AI_NET_LAYER *net_layer);
ERROR_T AiNetWeightsLoad(AI_NET_NETWORK *net_network, AI_NET_LAYER *net_layer);
ERROR_T AiNetWeightsLoadConvolutional(AI_NET_LAYER *net_layer, INT32_T layer_index, FILE *file);
ERROR_T AiNetWeightsTransposeMatrix(FLOAT_AI_T *a, UINT32_T rows, UINT32_T cols);
ERROR_T AiNetWeightsFuseConvBatchnorm(AI_NET_LAYER *net_layer, INT32_T layer_index);
ERROR_T AiNetWeightsCalculateBinaryWeights(AI_NET_LAYER *net_layer, INT32_T layer_index);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

