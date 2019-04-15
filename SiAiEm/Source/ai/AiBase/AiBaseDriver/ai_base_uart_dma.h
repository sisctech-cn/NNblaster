/**
 ******************************************************************************
 * file    ai_base_uart_dma.h
 * author  sunlingge
 * version V100
 * brief   the ai base uart dma driver.
 ******************************************************************************
 */
#ifndef AI_BASE_UART_DMA_H
#define AI_BASE_UART_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_base_uart_dma_struct {
//  USART_TypeDef *uart;
//  UINT32_T rcc;
//  AI_BASE_GPIO send;
//  AI_BASE_GPIO recv;
//  USART_InitPara para;
//  AI_BASE_INT irq;
//  AI_BASE_DMA rx_dma;
//  AI_BASE_DMA tx_dma;
    UINT32_T tx_dma_int_state;
    UINT32_T tx_dma_int;
    UINT32_T decode;
    AI_BASE_LNET_LINK_RX link_rx_func;
    AI_BASE_LNET_LINK_TX link_tx_func;
    AI_BASE_LNET_PHY_TX_FUNC phy_tx_func;
    AI_BASE_LNET_PHY_TX_COMPLETED_FUNC phy_tx_completed_func;
} AI_BASE_UART_DMA, *AI_BASE_UART_DMA_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseUartDmaInit(AI_BASE_UART_DMA_PTR ai_base_uart);
ERROR_T AiBaseUartDmaRccInit(AI_BASE_UART_DMA_PTR ai_base_uart);
ERROR_T AiBaseUartDmaGpioInit(AI_BASE_UART_DMA_PTR ai_base_uart);
ERROR_T AiBaseUartDmaNvicInit(AI_BASE_UART_DMA_PTR ai_base_uart);
ERROR_T AiBaseUartDmaDmaInit(AI_BASE_UART_DMA_PTR ai_base_uart);
ERROR_T AiBaseUartDmaParaInit(AI_BASE_UART_DMA_PTR ai_base_uart);
ERROR_T AiBaseUartDmaIntInit(AI_BASE_UART_DMA_PTR ai_base_uart);
ERROR_T AiBaseUartDmaRxIrq(AI_BASE_UART_DMA_PTR ai_base_uart, AI_BASE_LNET_LINK_PTR link);
ERROR_T AiBaseUartDmaTxIrq(AI_BASE_UART_DMA_PTR ai_base_uart, AI_BASE_LNET_LINK_PTR link);
ERROR_T AiBaseUartDmaTxCompleted(UINT32_T phy);
ERROR_T AiBaseUartDmaTx(UINT32_T phy, UINT32_T tx_buffer, UINT32_T tx_length);


#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
