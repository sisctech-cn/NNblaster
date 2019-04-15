// SiAiGetDeviceStateFile.cpp
//

#include "stdafx.h"
#include "SiAiGetDeviceStateFile.h"


IMPLEMENT_DYNAMIC(SiAiGetDeviceStateFile, CFileDialog)

SiAiGetDeviceStateFile::SiAiGetDeviceStateFile(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
                                       DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
    CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{

}

SiAiGetDeviceStateFile::~SiAiGetDeviceStateFile()
{
}


BEGIN_MESSAGE_MAP(SiAiGetDeviceStateFile, CFileDialog)
END_MESSAGE_MAP()



// ci100GetSourceFile 消息处理程序


