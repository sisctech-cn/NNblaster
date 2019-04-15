// SiAiDeviceConfigNameDataTab.cpp : 实现文件
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


SiAiDeviceConfigNameDataTab::SiAiDeviceConfigNameDataTab()
{
}

SiAiDeviceConfigNameDataTab::~SiAiDeviceConfigNameDataTab()
{
}

void SiAiDeviceConfigNameDataTab::ConfigNameDataTabInit(CDialogEx *dialogEx, CString strTitle, int left, int top, int right, int bottom)
{
#if 0
    CRect clientRC;
	dialogEx->GetClientRect(clientRC);
	clientRC.DeflateRect(left, top, right, bottom);
    m_tab_ctrl.Create(TCS_TABS|WS_CHILD|WS_VISIBLE,clientRC,dialogEx,IDC_TABCTRL);
    m_tabCtrl.InsertItem(0,strTitle);
    m_page1.Create(IDD_DIALOG1,GetDlgItem(IDC_TABCTRL));
    m_page2.Create(IDD_DIALOG2,GetDlgItem(IDC_TABCTRL));
    //显示第一个页面
    m_carList.ShowWindow(SW_SHOW);
    //设置第一个页面显示
    m_tabCtrl.SetCurSel(0);

	m_dialog_name_data.Create(IDD_AI_SERVER_DEVICE_CONFIG_NAME_DATA_DIALOG, dialogEx);
	m_dialog_name_data.m_strNameData.Format(_T(AI_SERVER_UI_INFO_DEVICE_PN));
	m_dialog_name_data.m_cstaticNameData.SetWindowText(m_dialog_name_data.m_strNameData);
	this->GetClientRect(clientRC);
	m_dialog_name_data.MoveWindow(clientRC);
	m_dialog_name_data.ShowWindow(SW_HIDE);
#endif
	return;
}
