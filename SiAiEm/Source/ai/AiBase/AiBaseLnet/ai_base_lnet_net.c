/**
 ******************************************************************************
 * file    ai_base_tnet_net.c
 * author  sunlingge
 * version V100
 * brief   the ai base tnet net driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/

/*------------------------- Function Definition -----------------------------*/
/**
 * brief    Ai base lnet link init.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetNetInit(AI_BASE_LNET_NET_PTR net) {
    net->brigde = (AI_BASE_LNET_INTERFACE_ADDR)NULL;

    return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
