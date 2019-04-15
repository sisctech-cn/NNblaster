// SiAiFp16Test.cpp : 实现文件
//

#include "stdafx.h"
#include "SiAiGlobal.h"
#include "SiAiConfigIdent.h"
#include "SiAiAddLocalFiles.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// extern c function declaration
#ifdef __cplusplus
extern "C" {
#endif
INT32_T AiBaseFloat16TestMain(void);
INT32_T AiBaseFloat32TestMain(void);
INT32_T AiBaseFloat64TestMain(void);
int tna_main(void);
#ifdef __cplusplus
}
#endif

////////////////////////////////////////////////////////////////////////////
// Global Variable Declaration
SiAiConfigIdent *g_SiAiConfigIdent = NULL;

////////////////////////////////////////////////////////////////////////////
// Extern Variable Declaration
extern SiAiDlg *g_SiAiDlg;

// AIServerDeviceConfig 对话框

IMPLEMENT_DYNAMIC(SiAiConfigIdent, CDialogEx)

SiAiConfigIdent::SiAiConfigIdent(CWnd* pParent /*=NULL*/)
    : CDialogEx(SiAiConfigIdent::IDD, pParent)
{
	timer_enable = 0;
}

SiAiConfigIdent::~SiAiConfigIdent()
{
}

BOOL_T SiAiConfigIdent::OnInitDialog()
{
	CRect clientRC;
    CDialog::OnInitDialog();

    g_SiAiConfigIdent = this;
	SetTimer(AI_SERVER_IDENT_TIMER, AI_SERVER_IDENT_TIMER_PERIOD, NULL);

	m_IdentThreadFlag = IDENT_THREAD_STOP;
	m_IdentThread = AfxBeginThread(IdentService, this);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void SiAiConfigIdent::OnPaint()
{
	return;
}


HBRUSH SiAiConfigIdent::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

void SiAiConfigIdent::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SiAiConfigIdent, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_MESSAGE(AI_SERVER_DISPLAY_CHANGE, OnDisplayChange)
    ON_WM_CTLCOLOR()
    ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_IDENT, &SiAiConfigIdent::OnBnClickedButtonIdent)
	ON_BN_CLICKED(IDC_BUTTON_IDENT_FILE_BROWSER, &SiAiConfigIdent::OnBnClickedButtonIdentFileBrowser)
END_MESSAGE_MAP()

void SiAiConfigIdent::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialog::OnSysCommand(nID, lParam);
}

void SiAiConfigIdent::OnOK()
{
    UpdateData(true);
    UpdateData(false);
    return;
}
void SiAiConfigIdent::OnCancel()
{
    return;
}

LRESULT SiAiConfigIdent::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
    UpdateData(false);
    UpdateData(false);
    return 0;
}

void SiAiConfigIdent::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
	if (timer_enable == 1) {
		// display image
		DisplayIdentImage();
		timer_enable = 0;
	}

    CDialogEx::OnTimer(nIDEvent);
}

void SiAiConfigIdent::OnBnClickedButtonIdent()
{
	m_IdentThreadFlag = IDENT_THREAD_START;
	return;
}

void SiAiConfigIdent::OnBnClickedButtonIdentFileBrowser()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_strIdentFile;

	SiAiAddLocalFiles dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, _T("Bin File (*.png)|*.png||"), NULL);
	if (dlg.DoModal() == IDCANCEL)
		return;

	m_strIdentFilePath = dlg.GetPathName();
	GetDlgItem(IDC_EDIT_IDENT_FILE_PATH)->SetWindowText(m_strIdentFilePath);

	// display image
	DisplayIdentImage();

	return;
}

void SiAiConfigIdent::DisplayIdentImage(void)
{
	// read image
	CImage image;
	if (image.Load(m_strIdentFilePath) != 0) {
		return;
	}

	// get rect
	CRect imageRect;
	GetDlgItem(IDC_STATIC_SOURCE_IMAGE)->GetClientRect(&imageRect);
	imageRect.DeflateRect(6, 40, -1, -36);

	// write image
	CDC *picDC = GetDC();
	image.Draw(picDC->m_hDC, imageRect);
	ReleaseDC(picDC);

	return;
}

UINT SiAiConfigIdent::IdentService(LPVOID pParam)
{
	SiAiConfigIdent *identDlg = (SiAiConfigIdent *)g_SiAiConfigIdent;

	while (identDlg->m_IdentThreadFlag != IDENT_THREAD_EXIT_START) {
		if (identDlg->m_IdentThreadFlag == IDENT_THREAD_STOP) {
			Sleep(1);
			continue;
		}
		AiMain();
		identDlg->m_strIdentFilePath.Format(_T("%s"), _T(AI_PRODUCT_LOG_IDENT_PATH"/1080p.png"));
		identDlg->DisplayIdentImage();
		identDlg->m_IdentThreadFlag = IDENT_THREAD_STOP;
	}
	identDlg->m_IdentThreadFlag = IDENT_THREAD_EXIT_FINISH;
	return 0;
}

void SiAiConfigIdent::onBnClickedButtonTest()
{
	// TODO: 在此添加控件通知处理程序代码
#if 0
#if (AI_BASE_FLOAT_FP16 == 1)
	AiBaseFloat16TestMain();
#endif
#if (AI_BASE_FLOAT_FP32 == 1)
	AiBaseFloat32TestMain();
#endif
#if (AI_BASE_FLOAT_FP64 == 1)
	AiBaseFloat64TestMain();
#endif
#endif
#if 0
	AiBaseImageLoadAlphabet();
#endif
#if 0
	AiBaseImageLoad1080p();
#endif
#if 0
	AiNetClassesLoad(AiNetDataGetAiNetClasses());
#endif
#if 0
	AiNetConfigLoad(AiNetDataGetAiNetConfig());
#endif
#if 0
	AiBaseCmodelMain(AI_BASE_CMODEL_RUN_INIT_POR, 0);
	AiBaseCmodelMain(AI_BASE_CMODEL_RUN_INIT_LAYER, 0);
#endif
#if 0
	AiMain();
#endif
#if 0
	tna_main();
#endif
	return;
}