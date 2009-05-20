// CSMS.h  : Microsoft Visual C++로 만든 ActiveX 컨트롤 래퍼 클래스의 선언입니다.

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSMS

class CSMS : public CWnd
{
protected:
	DECLARE_DYNCREATE(CSMS)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xCB18D7AB, 0x83A8, 0x470C, { 0xB9, 0xB3, 0x21, 0xE3, 0x1, 0x48, 0xDB, 0x61 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 특성입니다.
public:

// 작업입니다.
public:

	void Add(LPCTSTR called_number, LPCTSTR calling_number, LPCTSTR message, LPCTSTR called_refname, LPCTSTR reservdate)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, called_number, calling_number, message, called_refname, reservdate);
	}
	void Close()
	{
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL Connect()
	{
		BOOL result;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	long Credits()
	{
		long result;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString MD5(LPCTSTR src)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, src);
		return result;
	}
	unsigned long Send()
	{
		unsigned long result;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void SetUser(LPCTSTR userid, LPCTSTR passwd)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, userid, passwd);
	}
	void Empty()
	{
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SetSvcPort(unsigned short port)
	{
		static BYTE parms[] = VTS_UI2 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, port);
	}
	void SetSvcHost(LPCTSTR host)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms, host);
	}
	CString LastErrorStr()
	{
		CString result;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}


};
