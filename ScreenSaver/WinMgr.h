// WinMgr.h : main header file for the WINMGR application
//

#if !defined(AFX_WINMGR_H__BEAD4E84_F81B_11D2_95F4_D164A61AE6D6__INCLUDED_)
#define AFX_WINMGR_H__BEAD4E84_F81B_11D2_95F4_D164A61AE6D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWinMgrApp:
// See WinMgr.cpp for the implementation of this class
//

class CWinMgrApp : public CWinApp
{
public:
	CWinMgrApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinMgrApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWinMgrApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINMGR_H__BEAD4E84_F81B_11D2_95F4_D164A61AE6D6__INCLUDED_)
