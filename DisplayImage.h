
// DisplayImage.h : DisplayImage Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CDisplayImageApp:
// �йش����ʵ�֣������ DisplayImage.cpp
//

class CDisplayImageApp : public CWinApp
{
public:
	CDisplayImageApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	BOOL  m_bHiColorIcons;

	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	DECLARE_MESSAGE_MAP()
};

extern CDisplayImageApp theApp;
