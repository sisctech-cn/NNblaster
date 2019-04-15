// SiAiDeviceConfigNumberData.cpp : 实现文件
//

#include "stdafx.h"
#include "SiAiGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// Global Variable Declaration

////////////////////////////////////////////////////////////////////////////
// Extern Variable Declaration
extern SiAiDlg *g_SiAiDlg;


// SiAiDeviceConfig 对话框

IMPLEMENT_DYNAMIC(SiAiDeviceConfigNumberDataProcessBar, CDialogEx)

SiAiDeviceConfigNumberDataProcessBar::SiAiDeviceConfigNumberDataProcessBar(CWnd* pParent /*=NULL*/)
    : CDialogEx(SiAiDeviceConfigNumberDataProcessBar::IDD, pParent)
{
}

SiAiDeviceConfigNumberDataProcessBar::~SiAiDeviceConfigNumberDataProcessBar()
{
}

BOOL_T SiAiDeviceConfigNumberDataProcessBar::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void SiAiDeviceConfigNumberDataProcessBar::OnPaint()
{
}


HBRUSH SiAiDeviceConfigNumberDataProcessBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

void SiAiDeviceConfigNumberDataProcessBar::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
	// soft info
    DDX_Control(pDX, IDC_STATIC_STATE_NUMBER_DATA, m_cstaticNumberData);
    DDX_CBString(pDX, IDC_STATIC_STATE_NUMBER_DATA, m_strNumberData);
}


BEGIN_MESSAGE_MAP(SiAiDeviceConfigNumberDataProcessBar, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_MESSAGE(AI_SERVER_DISPLAY_CHANGE, OnDisplayChange)
    ON_WM_CTLCOLOR()
    ON_WM_TIMER()
	ON_STN_CLICKED(IDC_STATIC_STATE_NUMBER_DATA, &SiAiDeviceConfigNumberDataProcessBar::OnStnClickedStaticStateNumberData)
END_MESSAGE_MAP()

void SiAiDeviceConfigNumberDataProcessBar::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialog::OnSysCommand(nID, lParam);
}

void SiAiDeviceConfigNumberDataProcessBar::OnOK()
{
    UpdateData(true);
    UpdateData(false);
    return;
}
void SiAiDeviceConfigNumberDataProcessBar::OnCancel()
{
    return;
}

LRESULT SiAiDeviceConfigNumberDataProcessBar::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
    UpdateData(false);
    UpdateData(false);
    return 0;
}

void SiAiDeviceConfigNumberDataProcessBar::OnStnClickedStaticStateNumberData()
{
	// TODO: 在此添加控件通知处理程序代码
}
