/**
 ******************************************************************************
 * file    ai_base_lnet_link.c
 * author  sunlingge
 * version V100
 * brief   the ai base tnet link driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/
#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
HANDLE g_AIServerBaseLnetLinkSemaphore;
#endif

/*------------------------- Function Definition -----------------------------*/
/**
 * brief    Ai base lnet link init.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetLinkInit(AI_BASE_LNET_LINK_PTR link, UINT32_T phy,
                            AI_BASE_LNET_ADDR_PTR addr,
                            UINT32_T decode,
                            AI_BASE_LNET_LINK_RX link_rx_func,
                            AI_BASE_LNET_LINK_TX link_tx_func,
                            AI_BASE_LNET_PHY_TX_FUNC phy_tx_func,
                            AI_BASE_LNET_PHY_TX_COMPLETED_FUNC phy_tx_completed_func)
{

#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
    g_AIServerBaseLnetLinkSemaphore = CreateSemaphore(NULL, 1, 1, "AIServerBaseLnetLinkSem");
#endif
#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
    WaitForSingleObject(g_AIServerBaseLnetLinkSemaphore, INFINITE);
#endif
    memset((char *)link, 0, sizeof(AI_BASE_LNET_LINK));
    link->decode = decode;
    link->rx_func = link_rx_func;
    link->tx_func = link_tx_func;
    AiBaseLnetPhyInit(&(link->phy), phy, addr, phy_tx_func, phy_tx_completed_func);
#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
    ReleaseSemaphore(g_AIServerBaseLnetLinkSemaphore, 1, NULL);
#endif
    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet link Dma Receive data.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetLinkDmaRx(VOID_T link, UINT8_T *buffer, UINT32_T len)
{
    UINT32_T data;
    AI_BASE_LNET_LINK_PTR link_ptr = (AI_BASE_LNET_LINK_PTR)link;

#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
    WaitForSingleObject(g_AIServerBaseLnetLinkSemaphore, INFINITE);
#endif
    data = (AI_BASE_LNET_LINK_BUFFER_LENGTH - len);
    link_ptr->rx_end = (data & AI_BASE_LNET_LINK_BUFFER_MASK);
#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
    ReleaseSemaphore(g_AIServerBaseLnetLinkSemaphore, 1, NULL);
#endif

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet link Dma Receive data.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetLinkRx(VOID_T link, UINT8_T *buffer, UINT32_T len)
{
    AI_BASE_LNET_LINK_PTR link_ptr = (AI_BASE_LNET_LINK_PTR)link;
    UINT32_T index;

#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
    WaitForSingleObject(g_AIServerBaseLnetLinkSemaphore, INFINITE);
#endif
	link_ptr->rx_tick = AiBaseTickGet();
    for(index = 0; index < len; index++) {
        if (((link_ptr->rx_end + 1) & AI_BASE_LNET_LINK_BUFFER_MASK) == link_ptr->rx_start) {
#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
            ReleaseSemaphore(g_AIServerBaseLnetLinkSemaphore, 1, NULL);
#endif
            return AI_ERROR_GENERAL;
        }
        link_ptr->rx_buffer[link_ptr->rx_end & AI_BASE_LNET_LINK_BUFFER_MASK] = buffer[index];
        link_ptr->rx_end = (link_ptr->rx_end + 1) & AI_BASE_LNET_LINK_BUFFER_MASK;
    }

#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
    ReleaseSemaphore(g_AIServerBaseLnetLinkSemaphore, 1, NULL);
#endif
    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet link Dma Receive data.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetLinkCanRx(VOID_T link, UINT8_T *buffer, UINT32_T len)
{
    return AiBaseLnetLinkRx(link, buffer, len);
}

/**
 * brief    Ai base lnet link  Send data.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetLinkTx(VOID_T link)
{
    UINT32_T tx_length;
    AI_BASE_LNET_LINK_PTR link_ptr = (AI_BASE_LNET_LINK_PTR)link;

#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
    WaitForSingleObject(g_AIServerBaseLnetLinkSemaphore, INFINITE);
#endif
    link_ptr->tx_start = ((link_ptr->tx_start + link_ptr->phy.tx_length) & AI_BASE_LNET_LINK_BUFFER_MASK);
    if (link_ptr->tx_start == link_ptr->tx_end) {
        link_ptr->phy.tx_completed_func((UINT32_T)(link_ptr->phy.phy));
        link_ptr->phy.tx_length = 0;
        link_ptr->phy.tx_state = AI_BASE_LNET_LINK_TX_IDLE;
#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
        ReleaseSemaphore(g_AIServerBaseLnetLinkSemaphore, 1, NULL);
#endif
        return AI_ERROR_SUCCESS;
    }

    link_ptr->phy.tx_state = AI_BASE_LNET_LINK_TX_BUSY;
    if (link_ptr->tx_end >= link_ptr->tx_start) {
        tx_length = (link_ptr->tx_end - link_ptr->tx_start);
    } else {
        tx_length = (AI_BASE_LNET_LINK_BUFFER_LENGTH - link_ptr->tx_start);
    }
    link_ptr->phy.tx_length = tx_length;
    link_ptr->phy.tx_func((VOID_T)(link_ptr->phy.phy), (__IO UINT8_T *)(link_ptr->tx_buffer+link_ptr->tx_start), tx_length);
#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
    ReleaseSemaphore(g_AIServerBaseLnetLinkSemaphore, 1, NULL);
#endif

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet link Dma Send data.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetLinkDmaTx(VOID_T link)
{
    UINT32_T tx_length;
    AI_BASE_LNET_LINK_PTR link_ptr = (AI_BASE_LNET_LINK_PTR)link;

#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
    WaitForSingleObject(g_AIServerBaseLnetLinkSemaphore, INFINITE);
#endif
    AiBaseIntDisable();
    link_ptr->tx_start = ((link_ptr->tx_start + link_ptr->phy.tx_length) & AI_BASE_LNET_LINK_BUFFER_MASK);
    if (link_ptr->tx_start == link_ptr->tx_end) {
        link_ptr->phy.tx_length = 0;
        link_ptr->phy.tx_state = AI_BASE_LNET_LINK_TX_IDLE;
#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
        ReleaseSemaphore(g_AIServerBaseLnetLinkSemaphore, 1, NULL);
#endif
        AiBaseIntEnable();
        link_ptr->phy.tx_completed_func((UINT32_T)(link_ptr->phy.phy));
        return AI_ERROR_SUCCESS;
    }

    link_ptr->phy.tx_state = AI_BASE_LNET_LINK_TX_BUSY;
    if (link_ptr->tx_end >= link_ptr->tx_start) {
        tx_length = (link_ptr->tx_end - link_ptr->tx_start);
    } else {
        tx_length = (AI_BASE_LNET_LINK_BUFFER_LENGTH - link_ptr->tx_start);
    }
    link_ptr->phy.tx_length = tx_length;
    link_ptr->phy.tx_func((VOID_T)(link_ptr->phy.phy), (link_ptr->tx_buffer+link_ptr->tx_start), tx_length);
#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
    ReleaseSemaphore(g_AIServerBaseLnetLinkSemaphore, 1, NULL);
#endif
    AiBaseIntEnable();

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet link rx check.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLentRxCheck(AI_BASE_LNET_LINK_PTR link)
{
    if (link->rx_start == link->rx_end)
        return AI_ERROR_GENERAL;
    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet link rx start set.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetRxStartSet(AI_BASE_LNET_LINK_PTR link, UINT32_T length) {
    UINT32_T index;

#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
    WaitForSingleObject(g_AIServerBaseLnetLinkSemaphore, INFINITE);
#endif
    if (link->decode == AI_BASE_LNET_ESCAPE_HLUART) {
        for(index = 0; index < length*2; index++) {
            if (link->rx_start != link->rx_end) {
                link->rx_start = ((link->rx_start + 1) & AI_BASE_LNET_LINK_BUFFER_MASK);
            }
        }
    }
    if (link->decode == AI_BASE_LNET_ESCAPE_NONE) {
        for(index = 0; index < length; index++) {
            if (link->rx_start != link->rx_end) {
                link->rx_start = ((link->rx_start + 1) & AI_BASE_LNET_LINK_BUFFER_MASK);
            }
        }
    }
#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
    ReleaseSemaphore(g_AIServerBaseLnetLinkSemaphore, 1, NULL);
#endif

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet link rx buffer get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetRxBufferGet(AI_BASE_LNET_LINK_PTR link, UINT32_T check_end, UINT32_T start, UINT32_T length, UINT8_T *buffer)
{
    UINT32_T index;
    UINT32_T rx_buffer_index;

#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
    WaitForSingleObject(g_AIServerBaseLnetLinkSemaphore, INFINITE);
#endif
    rx_buffer_index = start;
    for(index = 0; index < length; index ++) {
        if (rx_buffer_index == check_end) {
#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
            ReleaseSemaphore(g_AIServerBaseLnetLinkSemaphore, 1, NULL);
#endif
            return AI_ERROR_GENERAL;
        }
        buffer[index] = link->rx_buffer[rx_buffer_index];
        rx_buffer_index = (rx_buffer_index + 1) & AI_BASE_LNET_LINK_BUFFER_MASK;
    }
#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
    ReleaseSemaphore(g_AIServerBaseLnetLinkSemaphore, 1, NULL);
#endif

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet link tx buffer set.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetTxBufferSet(AI_BASE_LNET_LINK_PTR link, UINT8_T *buffer, UINT32_T length)
{
    UINT32_T index;

#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
    WaitForSingleObject(g_AIServerBaseLnetLinkSemaphore, INFINITE);
#endif
    index = 0;
    while(index < length) {
        while (((link->tx_end + 1) & AI_BASE_LNET_LINK_BUFFER_MASK) == link->tx_start) {
            continue;
        }

        AiBaseIntDisable();
        while (((link->tx_end + 1) & AI_BASE_LNET_LINK_BUFFER_MASK) != link->tx_start) {
            link->tx_buffer[link->tx_end & AI_BASE_LNET_LINK_BUFFER_MASK] = buffer[index];
            index++;
            link->tx_end = (link->tx_end + 1) & AI_BASE_LNET_LINK_BUFFER_MASK;
            if (index >= length)
                break;
        }
/* we now send by send service timer */
#if (AI_PRODUCT_OPTION_WIN32 == 1)
#else
        if ((link->tx_start != link->tx_end) && (link->phy.tx_state == AI_BASE_LNET_LINK_TX_IDLE)) {
            link->tx_func((UINT32_T)link);
        }
#endif
        AiBaseIntEnable();
    }
#ifdef AI_SERVER_BASE_LNET_LINK_LOCK
    ReleaseSemaphore(g_AIServerBaseLnetLinkSemaphore, 1, NULL);
#endif

    return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
