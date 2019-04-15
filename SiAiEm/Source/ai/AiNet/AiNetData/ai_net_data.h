/**
 ******************************************************************************
 * file    ai_net_data.h
 * author  sunlingge
 * version V100
 * brief   the ai net data.
 ******************************************************************************
 */
#ifndef AI_NET_DATA_H
#define AI_NET_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiNetDataInitPrevious(void);
ERROR_T AiNetDataInitPost(void);
ERROR_T AiNetDataMemoryAddressCheck(UINT64_T address);
AI_NET_CLASSES *AiNetDataGetAiNetClasses(void);
AI_NET_CONFIG *AiNetDataGetAiNetConfig(void);
AI_NET_NETWORK *AiNetDataGetAiNetNetwork(void);
AI_NET_LAYER *AiNetDataGetAiNetLayer(void);


#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

