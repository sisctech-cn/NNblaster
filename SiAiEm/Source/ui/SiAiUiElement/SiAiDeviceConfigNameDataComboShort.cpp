// SiAiDeviceConfigNameDataComboShort.cpp : 实现文件
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
extern SiAiDeviceSelectService *g_SiAiDeviceSelectService;


// SiAiDeviceConfig 对话框

IMPLEMENT_DYNAMIC(SiAiDeviceConfigNameDataComboShort, CDialogEx)

SiAiDeviceConfigNameDataComboShort::SiAiDeviceConfigNameDataComboShort(CWnd* pParent /*=NULL*/)
    : CDialogEx(SiAiDeviceConfigNameDataComboShort::IDD, pParent)
{
	timer_enable = 0;
}

SiAiDeviceConfigNameDataComboShort::~SiAiDeviceConfigNameDataComboShort()
{
}

BOOL_T SiAiDeviceConfigNameDataComboShort::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
	SetTimer(AI_SERVER_CONFIG_NAME_DATA_COMBO_TIMER, AI_SERVER_CONFIG_NAME_DATA_COMBO_TIMER_PERIOD, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void SiAiDeviceConfigNameDataComboShort::OnPaint()
{
}

void SiAiDeviceConfigNameDataComboShort::ComboDataSet(SiAiDeviceConfigComboDataPtr combo_data, UINT16_T sdk_data_index, UINT64_T sdk_data_sub_index)
{
	int index;

	m_sdk_data_index = sdk_data_index;
	m_sdk_data_sub_index = sdk_data_sub_index;
    for(index = 0; index < AI_SERVER_CONFIG_DATA_COMBO_MAX; index++) {
		if (combo_data->valid[index] != 0) {
			m_combo_data.valid[index] = 1;
			m_combo_data.value[index] = combo_data->value[index];
			m_combo_data.strValue[index].Format(_T("%s"), combo_data->strValue[index]);
		} else {
			m_combo_data.valid[index] = 0;
		}
    }

	m_ccomboNumberData.ResetContent();
    for(index = 0; index < AI_SERVER_CONFIG_DATA_COMBO_MAX; index++) {
		if (m_combo_data.valid[index] != 0) {
			m_ccomboNumberData.AddString((LPCTSTR)(m_combo_data.strValue[index]));
		}
    }
	for(index = 0; index < AI_SERVER_CONFIG_DATA_COMBO_MAX; index++) {
		if (m_combo_data.valid[index] != 0) {
			m_ccomboNumberData.SetCurSel(m_ccomboNumberData.FindString(0, m_combo_data.strValue[index]));
			break;
		}
    }

	return;
}

void SiAiDeviceConfigNameDataComboShort::ComboDataCurSet(UINT64_T value) 
{
	int index;

	for(index = 0; index < AI_SERVER_CONFIG_DATA_COMBO_MAX; index++) {
		if (m_combo_data.valid[index] != 0) {
			if (m_combo_data.value[index] == value) {
				m_ccomboNumberData.SetCurSel(m_ccomboNumberData.FindString(0, m_combo_data.strValue[index]));
				break;
			}
		}
    }

	return;
}

UINT64_T SiAiDeviceConfigNameDataComboShort::ComboDataCurGet(void) 
{
	int index;
	CString strValue;

	m_ccomboNumberData.GetLBText(m_ccomboNumberData.GetCurSel(), strValue);
	for(index = 0; index < AI_SERVER_CONFIG_DATA_COMBO_MAX; index++) {
		if (m_combo_data.valid[index] != 0) {
			if (m_combo_data.strValue[index].Compare(strValue) == 0) {
				return m_combo_data.value[index];
			}
		}
    }

	return 0;
}

CString SiAiDeviceConfigNameDataComboShort::ComboDataStringGet(UINT64_T value)
{
	int index;
	CString temp;

	for(index = 0; index < AI_SERVER_CONFIG_DATA_COMBO_MAX; index++) {
		if (m_combo_data.valid[index] != 0) {
			if (m_combo_data.value[index] == value) {
				return m_combo_data.strValue[index];
			}
		}
    }

	temp.Format(_T(""));
	return temp; 
}


HBRUSH SiAiDeviceConfigNameDataComboShort::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

void SiAiDeviceConfigNameDataComboShort::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// soft info
	DDX_Control(pDX, IDC_STATIC_CONFIG_NAME, m_cstaticNameData);
	DDX_CBString(pDX, IDC_STATIC_CONFIG_NAME, m_strNameData);
    DDX_Control(pDX, IDC_COMBO_CONFIG_NAME_DATA, m_ccomboNumberData);
    DDX_CBString(pDX, IDC_COMBO_CONFIG_NAME_DATA, m_strNumberData);
}


BEGIN_MESSAGE_MAP(SiAiDeviceConfigNameDataComboShort, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_MESSAGE(AI_SERVER_DISPLAY_CHANGE, OnDisplayChange)
    ON_WM_CTLCOLOR()
    ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CONFIG_SET, &SiAiDeviceConfigNameDataComboShort::OnBnClickedButtonConfigSet)
	ON_CBN_SELCHANGE(IDC_COMBO_CONFIG_NAME_DATA, &SiAiDeviceConfigNameDataComboShort::OnCbnSelchangeComboNumberData)
END_MESSAGE_MAP()

void SiAiDeviceConfigNameDataComboShort::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialog::OnSysCommand(nID, lParam);
}

void SiAiDeviceConfigNameDataComboShort::OnOK()
{
    UpdateData(true);
    UpdateData(false);
    return;
}
void SiAiDeviceConfigNameDataComboShort::OnCancel()
{
    return;
}

LRESULT SiAiDeviceConfigNameDataComboShort::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
    UpdateData(false);
    UpdateData(false);
    return 0;
}

void SiAiDeviceConfigNameDataComboShort::OnCbnSelchangeComboNumberData()
{
	// TODO: 在此添加控件通知处理程序代码
}

void SiAiDeviceConfigNameDataComboShort::OnBnClickedButtonConfigSet(void)
{
	timer_enable = 1;
	return;
}

void SiAiDeviceConfigNameDataComboShort::AiServerDataSet(LPVOID pParam)
{
	return;
}

UINT SiAiDeviceConfigNameDataComboShort::ServiceProcess(LPVOID pParam)
{
	AiServerDataSet(pParam);
	return 0;
}

void SiAiDeviceConfigNameDataComboShort::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
	if (timer_enable == 1) {
		m_Thread = AfxBeginThread(ServiceProcess, this);
		timer_enable = 0;
	}

    CDialogEx::OnTimer(nIDEvent);
}

