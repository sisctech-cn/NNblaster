// SiAiDeviceConfigNameDataSwitchBar.cpp : ʵ���ļ�
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


// SiAiDeviceConfig �Ի���

IMPLEMENT_DYNAMIC(SiAiDeviceConfigNameDataSwitchBar, CDialogEx)

SiAiDeviceConfigNameDataSwitchBar::SiAiDeviceConfigNameDataSwitchBar(CWnd* pParent /*=NULL*/)
    : CDialogEx(SiAiDeviceConfigNameDataSwitchBar::IDD, pParent)
{
}

SiAiDeviceConfigNameDataSwitchBar::~SiAiDeviceConfigNameDataSwitchBar()
{
}

BOOL_T SiAiDeviceConfigNameDataSwitchBar::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void SiAiDeviceConfigNameDataSwitchBar::OnPaint()
{
}


HBRUSH SiAiDeviceConfigNameDataSwitchBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
    return hbr;
}

void SiAiDeviceConfigNameDataSwitchBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// soft info
	DDX_Control(pDX, IDC_STATIC_STATE_NAME_DATA, m_cstaticNameData);
	DDX_CBString(pDX, IDC_STATIC_STATE_NAME_DATA, m_strNameData);
	DDX_Control(pDX, IDC_STATIC_STATE_NUMBER_DATA, m_cstaticNumberData);
	DDX_CBString(pDX, IDC_STATIC_STATE_NUMBER_DATA, m_strNumberData);
	DDX_Control(pDX, IDC_SLIDER_NUMBER_DATA_SWITCH, m_csliderSwitch);
}


BEGIN_MESSAGE_MAP(SiAiDeviceConfigNameDataSwitchBar, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_MESSAGE(AI_SERVER_DISPLAY_CHANGE, OnDisplayChange)
    ON_WM_CTLCOLOR()
    ON_WM_TIMER()
	ON_STN_CLICKED(IDC_STATIC_STATE_NUMBER_DATA, &SiAiDeviceConfigNameDataSwitchBar::OnStnClickedStaticStateNumberData)
END_MESSAGE_MAP()

void SiAiDeviceConfigNameDataSwitchBar::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialog::OnSysCommand(nID, lParam);
}

void SiAiDeviceConfigNameDataSwitchBar::OnOK()
{
    UpdateData(true);
    UpdateData(false);
    return;
}
void SiAiDeviceConfigNameDataSwitchBar::OnCancel()
{
    return;
}

LRESULT SiAiDeviceConfigNameDataSwitchBar::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
    UpdateData(false);
    UpdateData(false);
    return 0;
}

void SiAiDeviceConfigNameDataSwitchBar::OnStnClickedStaticStateNumberData()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


