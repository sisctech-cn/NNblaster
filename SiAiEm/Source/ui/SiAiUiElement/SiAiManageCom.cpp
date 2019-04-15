// SiAiAlgorithm.cpp : 实现文件
//

#include "stdafx.h"
#include "SiAiGlobal.h"

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/
extern SiAiDlg *g_SiAiDlg;

/*------------------------- Variable Definition------------------------------*/

/*------------------------- Function Definition -----------------------------*/
// Function
SiAiManageCom::SiAiManageCom()
{
    AiManageComInit();
    return;
}

SiAiManageCom::~SiAiManageCom()
{
}

/**
 * brief    Ai manage rs232 read only init.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
void SiAiManageCom::AiManageComInit()
{
    return;
}

/**
 * brief    Ai manage rs232 interrupt recv handler.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
void SiAiManageCom::AiManageComRecv(void)
{
    DWORD OldCount,NewCount;
    DWORD timeout = 0;
    unsigned long BufferLength = SERIAL_PACKET_LENGTH_MAX;
    unsigned long ReadBufferLength = 0;
    unsigned char Buffer[SERIAL_PACKET_LENGTH_MAX];

    OldCount=GetTickCount();
    do {
        ReadBufferLength = (unsigned long)(g_SiAiDlg->m_SiAiManageCom.phy.ReadSerialPort((BYTE *)Buffer, (DWORD)BufferLength, timeout));
        if (ReadBufferLength != 0) {
#if (AI_SERVER_TICKET_TEST == 1)
			AiBaseTickTestAdd(_T("AiManageComRecv"));
#endif
            g_SiAiDlg->m_SiAiManageCom.inteface.link.rx_func((VOID_T)&(g_SiAiDlg->m_SiAiManageCom.inteface.link), Buffer, ReadBufferLength);
            OldCount=GetTickCount();
        } else {
        }
        NewCount = GetTickCount();
    } while ((OldCount + timeout) > NewCount);
    return;
}

/**
 * brief    Ai manage rs232 interrupt send handler.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
void SiAiManageCom::AiManageComSend(UINT8_T *buffer, UINT32_T length, UINT32_T *written)
{

    DWORD t_written = 0;

    /* this is for test */
#if (DEVICE_STATE_LOCAL_TEST == 1)
    Sleep(1);
    t_written = length;
    g_SiAiDlg->m_SiAiManageCom.inteface->sequence_recv_last = g_SiAiDlg->m_SiAiManageCom.inteface->sequence_send_last;
#else
    g_SiAiDlg->m_SiAiManageCom.phy.WriteSerialPort(buffer, length, 1000, &t_written);
#endif
    *written = (UINT32_T)t_written;

    return;
}

#if (AI_PRODUCT_OPTION_WIN32 == 1)
/**
 * brief    Ai uart dma tx.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseUartDmaTxCompleted(VOID_T phy)
{
    AI_BASE_LNET_PHY_PTR phy_ptr = (AI_BASE_LNET_PHY_PTR)phy;
#if (AI_PRODUCT_OPTION_LOG_COMM == 1)
	DWORD oldCount, newCount;
	CString temp;

	oldCount=GetTickCount();
#endif
    phy_ptr->tx_length = 0;

#if (AI_PRODUCT_OPTION_LOG_COMM == 1)
	newCount=GetTickCount();

	temp.Format(_T("AiBaseUartDmaTxCompleted dateTime=[%s] "), g_SiAiDlg->m_SiAiLogFile.GetCurrentDateTime());
	temp.AppendFormat(_T("start=%u "), oldCount);
	temp.AppendFormat(_T("end=%u "), newCount);
	temp.AppendFormat(_T("period=%u\n"), (newCount-oldCount));
	g_SiAiDlg->m_SiAiLogFile.WriteLogFile(temp);
#endif
    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai uart dma tx.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseUartDmaTx(VOID_T phy, UINT8_T *tx_buffer, UINT32_T tx_length)
{
    UINT32_T written = 0;
    AI_BASE_LNET_PHY_PTR phy_ptr = (AI_BASE_LNET_PHY_PTR)phy;
#if (AI_PRODUCT_OPTION_LOG_COMM == 1)
	DWORD oldCount, newCount;
	CString temp;
#endif

#if (AI_PRODUCT_OPTION_LOG_COMM == 1)
	oldCount=GetTickCount();
#endif
    g_SiAiDlg->m_SiAiManageCom.AiManageComSend((UINT8_T *)tx_buffer, tx_length, &written);
    phy_ptr->tx_length = written;
#if (AI_PRODUCT_OPTION_LOG_COMM == 1)
	newCount=GetTickCount();

	temp.Format(_T("AiBaseUartDmaTx dateTime=[%s] "), g_SiAiDlg->m_SiAiLogFile.GetCurrentDateTime());
	temp.AppendFormat(_T("start=%u "), oldCount);
	temp.AppendFormat(_T("end=%u "), newCount);
	temp.AppendFormat(_T("period=%u\n"), (newCount-oldCount));
	g_SiAiDlg->m_SiAiLogFile.WriteLogFile(temp);
#endif
    return AI_ERROR_SUCCESS;
}

#endif

/*------------------------- End ---------------------------------------------*/
