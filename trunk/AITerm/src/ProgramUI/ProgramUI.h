// ProgramUI.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CProgramUIApp:
// �� Ŭ������ ������ ���ؼ��� ProgramUI.cpp�� �����Ͻʽÿ�.
//

class CProgramUIApp : public CWinApp
{
public:
	CProgramUIApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CProgramUIApp theApp;