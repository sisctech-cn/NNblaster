/**
 ******************************************************************************
 * file    ai_net_weights_f16.h
 * author  sunlingge
 * version V100
 * brief   the ai net weights f16 driver.
 ******************************************************************************
 */
#ifndef AI_NET_WEIGHTS_F16_H
#define AI_NET_WEIGHTS_F16_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/


/*------------------------- Macro Definition --------------------------------*/


/*------------------------- Type Definition----------------------------------*/
typedef struct ai_net_weights_f16_struct {
	UINT8_T reserved;
} AI_NET_WEIGHTS_F16, *AI_NET_WEIGHTS_F16_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiNetWeightsF16Load(AI_NET_NETWORK *net_network, AI_NET_LAYER *net_layer);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

