// SiAiStateFileContent.cpp : 实现文件
//

#include "stdafx.h"
#include "SiAiGlobal.h"
#include "SiAiAddLocalFiles.h"

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

IMPLEMENT_DYNAMIC(SiAiStateFileF32Content, CDialogEx)

SiAiStateFileF32Content::SiAiStateFileF32Content(CWnd* pParent /*=NULL*/)
    : CDialogEx(SiAiStateFileF32Content::IDD, pParent)
{
	timer_enable = 0;
	m_page_number = 0;
	m_strF32FilePath = "";
	m_strF32TxtFilePath.Format("%sFileContentF32.txt", AI_PRODUCT_LOG_IDENT_PATH"/");
}

SiAiStateFileF32Content::~SiAiStateFileF32Content()
{
}

BOOL_T SiAiStateFileF32Content::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
	SetTimer(AI_SERVER_STATE_FILE_CONTENT_TIMER, AI_SERVER_STATE_FILE_CONTENT_TIMER_PERIOD, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void SiAiStateFileF32Content::OnPaint()
{
}

HBRUSH SiAiStateFileF32Content::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

void SiAiStateFileF32Content::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// soft info
	DDX_Control(pDX, IDC_EDIT_FILE_PAGE_CONTENT, m_ceditFilePageData);
	DDX_Text(pDX, IDC_EDIT_FILE_PAGE_CONTENT, m_strFilePageData);
}


BEGIN_MESSAGE_MAP(SiAiStateFileF32Content, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_MESSAGE(AI_SERVER_DISPLAY_CHANGE, OnDisplayChange)
    ON_WM_CTLCOLOR()
    ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &SiAiStateFileF32Content::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_PAGE_FIRST, &SiAiStateFileF32Content::OnBnClickedButtonPageFirst)
	ON_BN_CLICKED(IDC_BUTTON_PAGE_NEXT, &SiAiStateFileF32Content::OnBnClickedButtonPageNext)
	ON_BN_CLICKED(IDC_BUTTON_PAGE_BEFORE, &SiAiStateFileF32Content::OnBnClickedButtonPageBefore)
	ON_BN_CLICKED(IDC_BUTTON_PAGE_END, &SiAiStateFileF32Content::OnBnClickedButtonPageEnd)
	ON_BN_CLICKED(IDC_BUTTON_F32_FILE_BROWSER, &SiAiStateFileF32Content::OnBnClickedButtonF32FileBrowser)
	ON_BN_CLICKED(IDC_BUTTON_ACCURACY, &SiAiStateFileF32Content::OnBnClickedButtonAccuracy)
END_MESSAGE_MAP()

void SiAiStateFileF32Content::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialog::OnSysCommand(nID, lParam);
}

void SiAiStateFileF32Content::OnOK()
{
    UpdateData(true);
    UpdateData(false);
    return;
}
void SiAiStateFileF32Content::OnCancel()
{
    return;
}

LRESULT SiAiStateFileF32Content::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
    UpdateData(false);
    UpdateData(false);
    return 0;
}

void SiAiStateFileF32Content::AiServerDataSet(LPVOID pParam)
{
	return;
}

UINT SiAiStateFileF32Content::ServiceProcess(LPVOID pParam)
{
	SiAiStateFileF32Content *nameDataDlg = (SiAiStateFileF32Content *)pParam;

	AiServerDataSet(pParam);
	return 0;
}

void SiAiStateFileF32Content::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
	if (timer_enable == 1) {
		m_Thread = AfxBeginThread(ServiceProcess, this);
		timer_enable = 0;
	}

    CDialogEx::OnTimer(nIDEvent);
}

void SiAiStateFileF32Content::DisplayFileContent(UINT32_T page_number)
{
	UINT8_T file_path[256];
	FILE *f;
	UINT32_T file_length;
	UINT32_T read_length;
	UINT32_T file_pos;
	UINT32_T index;
	UINT32_T content_start_index;
	UINT32_T content_end_index;

	sprintf((char *)file_path, "%s", m_strF32TxtFilePath.GetBuffer());
	f = fopen((char *)file_path, "rb");
	if (!f) {
		return;
	}
	fseek(f, 0, SEEK_END);
	file_length = ftell(f);
	fclose(f);

	f = fopen((char *)file_path, "rb");
	if (!f) {
		return;
	}
	m_page_number = page_number;
	if (file_length >= ((m_page_number+1)*AI_SERVER_FILE_CONTENT_PAGE_LENGTH)) {
		if (file_length >= ((m_page_number)*AI_SERVER_FILE_CONTENT_PAGE_LENGTH + AI_SERVER_FILE_CONTENT_BUFFER_LENGTH)) {
			file_pos = m_page_number*AI_SERVER_FILE_CONTENT_PAGE_LENGTH;
			read_length = AI_SERVER_FILE_CONTENT_BUFFER_LENGTH;
		} else {
			file_pos = m_page_number*AI_SERVER_FILE_CONTENT_PAGE_LENGTH;
			read_length = (file_length - file_pos) % AI_SERVER_FILE_CONTENT_BUFFER_LENGTH;
		}
	}
	else {
		if (file_length < ((m_page_number)*AI_SERVER_FILE_CONTENT_PAGE_LENGTH)) {
			file_pos = 0;
			read_length = 0;
		}
		else {
			file_pos = m_page_number*AI_SERVER_FILE_CONTENT_PAGE_LENGTH;
			read_length = (file_length - file_pos) %AI_SERVER_FILE_CONTENT_PAGE_LENGTH;
		}
	}
	fseek(f, file_pos, SEEK_SET);
	fread(m_page_buffer, 1, read_length, f);
	fclose(f);

	content_start_index = 0;
	content_end_index = 0;
	if (read_length <= AI_SERVER_FILE_CONTENT_PAGE_LENGTH) {
		if (m_page_number == 0) {
			content_start_index = 0;
		}
		else {
			for (index = 0; index < (AI_SERVER_FILE_CONTENT_PAGE_LENGTH - 1); index++) {
				if (m_page_buffer[index] == '\n') {
					content_start_index = index + 1;
					break;
				}
			}
		}
		content_end_index = read_length;
		m_page_buffer[content_end_index+1] = '\0';
	}
	else {
		if (m_page_number == 0) {
			content_start_index = 0;
		}
		else {
			for (index = 0; index < (AI_SERVER_FILE_CONTENT_PAGE_LENGTH - 1); index++) {
				if (m_page_buffer[index] == '\n') {
					content_start_index = index + 1;
					break;
				}
			}
		}
		for (index = AI_SERVER_FILE_CONTENT_PAGE_LENGTH; index < (read_length - 1); index++) {
			if (m_page_buffer[index] == '\n') {
				content_end_index = index;
				m_page_buffer[content_end_index+1] = '\0';
				break;
			}
		}
	}
	if (content_start_index >= content_end_index) {
		m_strFilePageData.Format(_T("%s"), "");
		m_ceditFilePageData.SetWindowText(m_strFilePageData);
	}
	else {
		m_strFilePageData.Format(_T("%s"), m_page_buffer+content_start_index);
		m_ceditFilePageData.SetWindowText(m_strFilePageData);
	}

	return;
}

void SiAiStateFileF32Content::OnBnClickedButtonRefresh()
{
	DisplayFileContent(m_page_number);
	return;
}

void SiAiStateFileF32Content::OnBnClickedButtonPageFirst()
{
	DisplayFileContent(0);
	return;
}

void SiAiStateFileF32Content::OnBnClickedButtonPageNext()
{
	DisplayFileContent(m_page_number+1);
	return;
}

void SiAiStateFileF32Content::OnBnClickedButtonPageBefore()
{

	DisplayFileContent(m_page_number - 1);
	return;
}

void SiAiStateFileF32Content::ConvertFile()
{
	AiBaseLogConvertTextF32((UINT8_T *)(m_strF32FilePath.GetBuffer()));
}

void SiAiStateFileF32Content::OnBnClickedButtonF32FileBrowser()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_strIdentFile;

	SiAiAddLocalFiles dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, _T("Bin File (*.*)|*.*||"), NULL);
	if (dlg.DoModal() == IDCANCEL)
		return;

	m_strF32FilePath = dlg.GetPathName();
	GetDlgItem(IDC_EDIT_F32_FILE_PATH)->SetWindowText(m_strF32FilePath);

	// display image
	ConvertFile();

	return;
}

void SiAiStateFileF32Content::OnBnClickedButtonPageEnd()
{
	UINT8_T file_path[256];
	FILE *f;
	UINT32_T file_length;

	sprintf((char *)file_path, "%s", m_strF32TxtFilePath.GetBuffer());
	f = fopen((char *)file_path, "rb");
	if (!f) {
		return;
	}
	fseek(f, 0, SEEK_END);
	file_length = ftell(f);
	fclose(f);

	f = fopen((char *)file_path, "rb");
	if (!f) {
		return;
	}
	m_page_number = (file_length) / AI_SERVER_FILE_CONTENT_PAGE_LENGTH;
	DisplayFileContent(m_page_number);
	return;
}



void SiAiStateFileF32Content::OnBnClickedButtonAccuracy()
{
	// TODO: 在此添加控件通知处理程序代码
	AiBaseLogAccuracyTextF32();
}
