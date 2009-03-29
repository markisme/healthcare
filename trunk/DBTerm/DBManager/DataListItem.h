#pragma once
#include "Resource.h"

class DataListItem : public CDialog
{
	DECLARE_DYNAMIC(DataListItem)

public:
	DataListItem(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DataListItem();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DATA_LIST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	void SetColName( std::string colName );
	std::string GetColName();
	void SetFiledText( std::string text );
	std::string GetFieldText();
	void SetFiledType( std::string text )	{ _dataType.SetWindowText( text.c_str() ); }
	void SetFiledNull( std::string text )	{ _dataNull.SetWindowText( ("Null?(" + text + ")").c_str() ); }
	void SetFiledPK( std::string text )		{ _dataPK.SetWindowText( text.c_str() ); }

public:
	CStatic _colName;
	CEdit _dataValue;
	CStatic _dataType;
	CStatic _dataNull;
	CStatic _dataPK;
};
