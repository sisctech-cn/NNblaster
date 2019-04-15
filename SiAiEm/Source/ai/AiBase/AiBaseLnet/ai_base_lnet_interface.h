/**
 ******************************************************************************
 * file    ai_base_tnet_interface.h
 * author  sunlingge
 * version V100
 * brief   the ai base lnet interface driver.
 ******************************************************************************
 */
#ifndef AI_BASE_LNET_INTERFACE_H
#define AI_BASE_LNET_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_base_lnet_interface_struct {
    UINT8_T sequence_send;
	UINT8_T sequence_send_last;
	UINT8_T sequence_recv_last;
	UINT8_T retcode_recv_last;
    AI_BASE_LNET_LINK link;
    AI_BASE_LNET_NET net;
} AI_BASE_LNET_INTERFACE, *AI_BASE_LNET_INTERFACE_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseLnetInterfaceInit(AI_BASE_LNET_INTERFACE_PTR inteface,
        UINT32_T phy,
        AI_BASE_LNET_ADDR_PTR addr,
        UINT32_T decode,
        AI_BASE_LNET_LINK_RX link_rx_func,
        AI_BASE_LNET_LINK_TX link_tx_func,
        AI_BASE_LNET_PHY_TX_FUNC phy_tx_func,
        AI_BASE_LNET_PHY_TX_COMPLETED_FUNC phy_tx_completed_func);
ERROR_T AiBaseLnetInterfaceRun(AI_BASE_LNET_INTERFACE_PTR inteface);
ERROR_T AiBaseLnetInterfaceRequestRead(AI_BASE_LNET_INTERFACE_PTR inteface, UINT8_T source_addr, UINT8_T dest_addr, UINT32_T field_num, AI_BASE_LNET_PACKET_FIELD_READ_PTR field_read);
ERROR_T AiBaseLnetInterfaceRequestWrite(AI_BASE_LNET_INTERFACE_PTR inteface, UINT8_T source_addr, UINT8_T dest_addr, UINT32_T field_num, AI_BASE_LNET_PACKET_FIELD_WRITE_PTR field_write);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
