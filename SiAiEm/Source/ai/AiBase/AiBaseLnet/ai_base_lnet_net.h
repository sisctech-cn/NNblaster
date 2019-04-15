/**
 ******************************************************************************
 * file    ai_base_lnet_net.h
 * author  sunlingge
 * version V100
 * brief   the ai base lnet net driver.
 ******************************************************************************
 */
#ifndef AI_BASE_LNET_NET_H
#define AI_BASE_LNET_NET_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/
typedef VOID_T AI_BASE_LNET_INTERFACE_ADDR;

typedef struct ai_base_lnet_net_struct {
    /* the bridge interface address */
    AI_BASE_LNET_INTERFACE_ADDR brigde;
} AI_BASE_LNET_NET, *AI_BASE_LNET_NET_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseLnetNetInit(AI_BASE_LNET_NET_PTR net);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
