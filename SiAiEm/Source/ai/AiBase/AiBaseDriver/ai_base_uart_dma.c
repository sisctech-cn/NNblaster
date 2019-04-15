/**
 ******************************************************************************
 * file    ai_base_uart_dma.c
 * author  sunlingge
 * version V100
 * brief   the ai base uart dma driver.
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
 * brief  	ai uart dma init.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseUartDmaInit(AI_BASE_UART_DMA_PTR ai_base_uart) {
    return AI_ERROR_SUCCESS;
}

/**
 * brief  	ai uart dma clock init.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseUartDmaRccInit(AI_BASE_UART_DMA_PTR ai_base_uart) {
    return AI_ERROR_SUCCESS;
}

/**
 * brief  	ai uart dma gpio init.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseUartDmaGpioInit(AI_BASE_UART_DMA_PTR ai_base_uart) {
    return AI_ERROR_SUCCESS;
}

/**
 * brief  	ai uart dma nvic init.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseUartDmaNvicInit(AI_BASE_UART_DMA_PTR ai_base_uart) {
    return AI_ERROR_SUCCESS;
}

/**
 * brief  	ai uart dma dma init.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseUartDmaDmaInit(AI_BASE_UART_DMA_PTR ai_base_uart) {
    return AI_ERROR_SUCCESS;
}

/**
 * brief  	ai uart dma para init.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseUartDmaParaInit(AI_BASE_UART_DMA_PTR ai_base_uart) {
    return AI_ERROR_SUCCESS;
}

/**
 * brief  	ai uart dma int init.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment	V100
 */
ERROR_T AiBaseUartDmaIntInit(AI_BASE_UART_DMA_PTR ai_base_uart) {
    return AI_ERROR_SUCCESS;
}

/**
 * brief    ai uart dma rx.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseUartDmaRxIrq(AI_BASE_UART_DMA_PTR ai_base_uart, AI_BASE_LNET_LINK_PTR link) {
    return AI_ERROR_SUCCESS;
}

/**
 * brief    ai uart dma tx.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseUartDmaTxIrq(AI_BASE_UART_DMA_PTR ai_base_uart, AI_BASE_LNET_LINK_PTR link) {
    return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
