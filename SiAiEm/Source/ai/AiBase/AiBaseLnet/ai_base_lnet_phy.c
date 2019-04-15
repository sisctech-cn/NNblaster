/**
 ******************************************************************************
 * file    ai_base_lnet_phy.c
 * author  sunlingge
 * version V100
 * brief   the ai base lnet phy driver.
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
 * brief    Ai base lnet phy init.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPhyInit(AI_BASE_LNET_PHY_PTR lnet_phy,
        UINT32_T phy,
        AI_BASE_LNET_ADDR_PTR addr,
        AI_BASE_LNET_PHY_TX_FUNC tx_func,
        AI_BASE_LNET_PHY_TX_COMPLETED_FUNC tx_completed_func) {
    memset((char *)lnet_phy, 0, sizeof(AI_BASE_LNET_PHY));
    lnet_phy->phy = phy;
    lnet_phy->addr.io_addr = addr->io_addr;
    lnet_phy->tx_state = AI_BASE_LNET_LINK_TX_IDLE;
    lnet_phy->tx_length = 0;
    lnet_phy->tx_func = tx_func;
    lnet_phy->tx_completed_func = tx_completed_func;

    return AI_ERROR_SUCCESS;
}


/*------------------------- End ---------------------------------------------*/
