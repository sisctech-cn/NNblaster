/**
 ******************************************************************************
 * file    ai_net_state.h
 * author  sunlingge
 * version V100
 * brief   the ai net state driver.
 ******************************************************************************
 */
#ifndef AI_NET_STATE_H
#define AI_NET_STATE_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
typedef struct ai_net_state_struct {
	FLOAT_AI_T *truth;
    FLOAT_AI_T *input;
	FLOAT_AI_T *output;
    FLOAT_AI_T *delta;
    FLOAT_AI_T *workspace;
    INT32_T train;
	INT32_T index; 
} AI_NET_STATE, *AI_NET_STATE_PTR;


/*------------------------- Function Definition -----------------------------*/

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

