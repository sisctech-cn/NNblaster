/**
 ******************************************************************************
 * file    ai_base_lnet_phy.h
 * author  sunlingge
 * version V100
 * brief   the ai base lnet phy driver.
 ******************************************************************************
 */
#ifndef AI_BASE_LNET_PHY_H
#define AI_BASE_LNET_PHY_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
typedef ERROR_T (*AI_BASE_LNET_PHY_TX_COMPLETED_FUNC)(VOID_T);
typedef ERROR_T (*AI_BASE_LNET_PHY_TX_FUNC)(VOID_T, __IO UINT8_T *, UINT32_T);
typedef ERROR_T (*AI_BASE_LNET_LINK_RX)(VOID_T, UINT8_T*, UINT32_T);
typedef ERROR_T (*AI_BASE_LNET_LINK_TX)(VOID_T link);

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_base_lnet_phy_struct {
	VOID_T phy;
    AI_BASE_LNET_ADDR addr;
    UINT8_T tx_state;
    UINT32_T tx_length;
    AI_BASE_LNET_PHY_TX_FUNC tx_func;
    AI_BASE_LNET_PHY_TX_COMPLETED_FUNC tx_completed_func;
} AI_BASE_LNET_PHY, *AI_BASE_LNET_PHY_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseLnetPhyInit(AI_BASE_LNET_PHY_PTR lnet_phy,
        UINT32_T phy,
        AI_BASE_LNET_ADDR_PTR addr,
        AI_BASE_LNET_PHY_TX_FUNC tx_func,
        AI_BASE_LNET_PHY_TX_COMPLETED_FUNC tx_completed_func);


#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
