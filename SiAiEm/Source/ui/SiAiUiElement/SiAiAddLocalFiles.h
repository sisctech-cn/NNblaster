#if !defined(AFX_SELECTFILE_H_INCLUDED_)
#define AFX_SELECTFILE_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectFile.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SiAiAddLocalFiles dialog

class SiAiAddLocalFiles : public CFileDialog
{
	DECLARE_DYNAMIC(SiAiAddLocalFiles)

public:
	SiAiAddLocalFiles(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

protected:
	//{{AFX_MSG(SiAiAddLocalFiles)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTFILE_H__7C5638A1_8269_11D5_8DAF_0050FCFF77C5__INCLUDED_)
