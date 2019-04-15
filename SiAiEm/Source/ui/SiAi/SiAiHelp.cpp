// SiAiHelp.cpp : 实现文件
//

#include "stdafx.h"
#include "SiAi.h"
#include "SiAiHelp.h"
#include "afxdialogex.h"


// SiAiHelp 对话框

IMPLEMENT_DYNAMIC(SiAiHelp, CDialogEx)

SiAiHelp::SiAiHelp(CWnd* pParent /*=NULL*/)
	: CDialogEx(SiAiHelp::IDD, pParent)
{

}

SiAiHelp::~SiAiHelp()
{
}

void SiAiHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SiAiHelp, CDialogEx)
END_MESSAGE_MAP()


// SiAiHelp 消息处理程序
