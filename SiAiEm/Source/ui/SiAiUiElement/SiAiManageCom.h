#pragma once

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
class SiAiManageCom {
public:
	SiAiManageCom();   
	virtual ~SiAiManageCom();

public:
	AI_BASE_UART_DMA stub_phy;

public:
	AI_BASE_LNET_INTERFACE inteface;
	SiAiSerialComm phy;
	UINT32_T decode;
    AI_BASE_LNET_LINK_RX link_rx_func;
    AI_BASE_LNET_LINK_TX link_tx_func;
    AI_BASE_LNET_PHY_TX_FUNC phy_tx_func;
    AI_BASE_LNET_PHY_TX_COMPLETED_FUNC phy_tx_completed_func;

public:
	void AiManageComInit();
	void AiManageComRecv();
	void AiManageComSend(UINT8_T *buffer, UINT32_T length, UINT32_T *written);
};

#if (AI_PRODUCT_OPTION_WIN32 == 1)
ERROR_T AiBaseUartDmaTxCompleted(VOID_T phy);
ERROR_T AiBaseUartDmaTx(VOID_T phy, UINT8_T *tx_buffer, UINT32_T tx_length);
#endif

/*------------------------- End ---------------------------------------------*/

