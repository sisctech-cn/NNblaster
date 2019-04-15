// SiAiAddLocalFiles.cpp : implementation file
//

#include "stdafx.h"
#include "SiAiAddLocalFiles.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SiAiAddLocalFiles

IMPLEMENT_DYNAMIC(SiAiAddLocalFiles, CFileDialog)

SiAiAddLocalFiles::SiAiAddLocalFiles(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
}


BEGIN_MESSAGE_MAP(SiAiAddLocalFiles, CFileDialog)
	//{{AFX_MSG_MAP(SiAiAddLocalFiles)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

