// SiAiDeviceStateNameData.cpp : 实现文件
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


// SiAiDeviceState 对话框

IMPLEMENT_DYNAMIC(SiAiDeviceStateNameData, CDialogEx)

SiAiDeviceStateNameData::SiAiDeviceStateNameData(CWnd* pParent /*=NULL*/)
    : CDialogEx(SiAiDeviceStateNameData::IDD, pParent)
{
}

SiAiDeviceStateNameData::~SiAiDeviceStateNameData()
{
}

BOOL_T SiAiDeviceStateNameData::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void SiAiDeviceStateNameData::OnPaint()
{
}


HBRUSH SiAiDeviceStateNameData::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

void SiAiDeviceStateNameData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// soft info
	DDX_Control(pDX, IDC_STATIC_STATE_NAME, m_cstaticNameData);
	DDX_CBString(pDX, IDC_STATIC_STATE_NAME, m_strNameData);
	DDX_Control(pDX, IDC_STATIC_STATE_NAME_DATA, m_cstaticNumberData);
	DDX_CBString(pDX, IDC_STATIC_STATE_NAME_DATA, m_strNumberData);
}


BEGIN_MESSAGE_MAP(SiAiDeviceStateNameData, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_MESSAGE(AI_SERVER_DISPLAY_CHANGE, OnDisplayChange)
    ON_WM_CTLCOLOR()
    ON_WM_TIMER()
	ON_STN_CLICKED(IDC_STATIC_STATE_NAME_DATA, &SiAiDeviceStateNameData::OnStnClickedStaticStateNumberData)
END_MESSAGE_MAP()

void SiAiDeviceStateNameData::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialog::OnSysCommand(nID, lParam);
}

void SiAiDeviceStateNameData::OnOK()
{
    UpdateData(true);
    UpdateData(false);
    return;
}
void SiAiDeviceStateNameData::OnCancel()
{
    return;
}

LRESULT SiAiDeviceStateNameData::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
    UpdateData(false);
    UpdateData(false);
    return 0;
}

void SiAiDeviceStateNameData::OnStnClickedStaticStateNumberData()
{
	// TODO: 在此添加控件通知处理程序代码
}


