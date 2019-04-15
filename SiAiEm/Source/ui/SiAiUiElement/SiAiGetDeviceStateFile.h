#pragma once


// ci100GetSourceFile

class SiAiGetDeviceStateFile : public CFileDialog
{
    DECLARE_DYNAMIC(SiAiGetDeviceStateFile)

public:
    SiAiGetDeviceStateFile(BOOL bOpenFileDialog, // ���� FileOpen Ϊ TRUE������ FileSaveAs Ϊ FALSE
                       LPCTSTR lpszDefExt = NULL,
                       LPCTSTR lpszFileName = NULL,
                       DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                       LPCTSTR lpszFilter = NULL,
                       CWnd* pParentWnd = NULL);
    virtual ~SiAiGetDeviceStateFile();

protected:
    DECLARE_MESSAGE_MAP()
};


