#pragma once


// SiAiHelp �Ի���

class SiAiHelp : public CDialogEx
{
	DECLARE_DYNAMIC(SiAiHelp)

public:
	SiAiHelp(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SiAiHelp();

// �Ի�������
	enum { IDD = IDD_AI_SERVER_HELP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
