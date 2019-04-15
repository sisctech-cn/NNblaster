// SiAiDeviceConfigNumberDataSwitchBar.cpp : 实现文件
//

#include "stdafx.h"
#include "SiAiGlobal.h"
#include "SiAiDeviceConfigNumberDataSwitchBar.h"

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

IMPLEMENT_DYNAMIC(SiAiDeviceConfigNumberDataSwitchBar, CDialogEx)

SiAiDeviceConfigNumberDataSwitchBar::SiAiDeviceConfigNumberDataSwitchBar(CWnd* pParent /*=NULL*/)
    : CDialogEx(SiAiDeviceConfigNumberDataSwitchBar::IDD, pParent)
{
}

SiAiDeviceConfigNumberDataSwitchBar::~SiAiDeviceConfigNumberDataSwitchBar()
{
}

BOOL_T SiAiDeviceConfigNumberDataSwitchBar::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void SiAiDeviceConfigNumberDataSwitchBar::OnPaint()
{
}


HBRUSH SiAiDeviceConfigNumberDataSwitchBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

void SiAiDeviceConfigNumberDataSwitchBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// soft info
	DDX_Control(pDX, IDC_STATIC_STATE_NUMBER_DATA, m_cstaticNumberData);
	DDX_CBString(pDX, IDC_STATIC_STATE_NUMBER_DATA, m_strNumberData);
	DDX_Control(pDX, IDC_SLIDER_NUMBER_DATA_SWITCH, m_csliderSwitch);
}


BEGIN_MESSAGE_MAP(SiAiDeviceConfigNumberDataSwitchBar, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_MESSAGE(AI_SERVER_DISPLAY_CHANGE, OnDisplayChange)
    ON_WM_CTLCOLOR()
    ON_WM_TIMER()
	ON_STN_CLICKED(IDC_STATIC_STATE_NUMBER_DATA, &SiAiDeviceConfigNumberDataSwitchBar::OnStnClickedStaticStateNumberData)
END_MESSAGE_MAP()

void SiAiDeviceConfigNumberDataSwitchBar::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialog::OnSysCommand(nID, lParam);
}

void SiAiDeviceConfigNumberDataSwitchBar::OnOK()
{
    UpdateData(true);
    UpdateData(false);
    return;
}
void SiAiDeviceConfigNumberDataSwitchBar::OnCancel()
{
    return;
}

LRESULT SiAiDeviceConfigNumberDataSwitchBar::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
    UpdateData(false);
    UpdateData(false);
    return 0;
}

void SiAiDeviceConfigNumberDataSwitchBar::OnStnClickedStaticStateNumberData()
{
	// TODO: 在此添加控件通知处理程序代码
}


