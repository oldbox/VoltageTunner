
// VoltageVerification.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVoltageVerificationApp: 
// �йش����ʵ�֣������ VoltageVerification.cpp
//

class CVoltageVerificationApp : public CWinApp
{
public:
	CVoltageVerificationApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVoltageVerificationApp theApp;