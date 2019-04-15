/**
 ******************************************************************************
 * file    ai_ident_predict.h
 * author  sunlingge
 * version V100
 * brief   the ai ident predict driver.
 ******************************************************************************
 */
#ifndef AI_IDENT_PREDICT_H
#define AI_IDENT_PREDICT_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
typedef struct ai_ident_predict_struct {
	UINT8_T reserved;
} AI_IDENT_PREDICT, *AI_IDENT_PREDICT_PTR;


/*------------------------- Function Definition -----------------------------*/
ERROR_T AiIdentPredict(AI_NET_STATE *net_state, AI_NET_NETWORK *net_network, AI_NET_LAYER *net_layer, FLOAT_AI_T *input);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

