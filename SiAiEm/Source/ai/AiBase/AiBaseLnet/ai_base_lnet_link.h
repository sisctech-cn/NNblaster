/**
 ******************************************************************************
 * file    ai_base_lnet_link.h
 * author  sunlingge
 * version V100
 * brief   the ai base lnet link driver.
 ******************************************************************************
 */
#ifndef AI_BASE_LNET_LINK_H
#define AI_BASE_LNET_LINK_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
typedef ERROR_T (*AI_BASE_LNET_LINK_RX)(VOID_T, UINT8_T*, UINT32_T);
typedef ERROR_T (*AI_BASE_LNET_LINK_TX)(VOID_T link);

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_base_lnet_link_struct {
    __IO UINT8_T rx_buffer[AI_BASE_LNET_LINK_BUFFER_LENGTH];
    __IO UINT16_T rx_start;
    __IO UINT16_T rx_end;
    __IO UINT8_T tx_buffer[AI_BASE_LNET_LINK_BUFFER_LENGTH];
    __IO UINT16_T tx_start;
    __IO UINT16_T tx_end;
    UINT32_T decode;
    AI_BASE_LNET_LINK_RX rx_func;
    AI_BASE_LNET_LINK_TX tx_func;
    AI_BASE_LNET_PHY phy;
	UINT64_T rx_tick;
} AI_BASE_LNET_LINK, *AI_BASE_LNET_LINK_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseLnetLinkInit(AI_BASE_LNET_LINK_PTR link, UINT32_T phy,
        AI_BASE_LNET_ADDR_PTR addr,
        UINT32_T decode,
        AI_BASE_LNET_LINK_RX link_rx_func,
        AI_BASE_LNET_LINK_TX link_tx_func,
        AI_BASE_LNET_PHY_TX_FUNC phy_tx_func,
        AI_BASE_LNET_PHY_TX_COMPLETED_FUNC phy_tx_completed_func);
ERROR_T AiBaseLnetLinkDmaRx(VOID_T link, UINT8_T *buffer, UINT32_T data);
ERROR_T AiBaseLnetLinkRx(VOID_T link, UINT8_T *buffer, UINT32_T len);
ERROR_T AiBaseLnetLinkCanRx(VOID_T link, UINT8_T *buffer, UINT32_T len);
ERROR_T AiBaseLnetLinkDmaTx(VOID_T link);
ERROR_T AiBaseLnetLinkTx(VOID_T link);
ERROR_T AiBaseLnetLinkCanTx(VOID_T link);
ERROR_T AiBaseLnetRxStartSet(AI_BASE_LNET_LINK_PTR link, UINT32_T length);
ERROR_T AiBaseLentRxCheck(AI_BASE_LNET_LINK_PTR link);
ERROR_T AiBaseLnetRxDataGet(AI_BASE_LNET_LINK_PTR link, UINT32_T check_end, UINT32_T start, UINT32_T length, UINT32_T *value);
ERROR_T AiBaseLnetRxBufferGet(AI_BASE_LNET_LINK_PTR link, UINT32_T check_end, UINT32_T start, UINT32_T length, UINT8_T *buffer);
ERROR_T AiBaseLnetTxBufferSet(AI_BASE_LNET_LINK_PTR link, UINT8_T *buffer, UINT32_T length);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
