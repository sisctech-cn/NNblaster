#pragma once


// SiAiHelp 对话框

class SiAiHelp : public CDialogEx
{
	DECLARE_DYNAMIC(SiAiHelp)

public:
	SiAiHelp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SiAiHelp();

// 对话框数据
	enum { IDD = IDD_AI_SERVER_HELP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
