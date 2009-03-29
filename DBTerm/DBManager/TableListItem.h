#pragma once
#include "Resource.h"

class TableListItem : public CDialog
{
	DECLARE_DYNAMIC(TableListItem)

public:
	TableListItem(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~TableListItem();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TABLE_LIST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();	

	DECLARE_MESSAGE_MAP()
	afx_msg void OnEnChangeEditFiledName();
	afx_msg void OnBnClickedCheckAutoIncr();
	afx_msg void OnCbnSelchangeComboFieldType();
	afx_msg void OnBnClickedCheckPk();
	afx_msg void OnBnClickedCheckNotNull();

public:
	void Init( CDialog * parent );
	void Uninit();
	void Refresh();

	std::string GetFieldName();
	void SetFieldName( std::string name );
	std::string GetFieldType();
	void SetFieldType( std::string type );
	std::string GetFieldLenth();
	void SetFieldLenth( std::string length );
	bool IsNotNull();
	void SetNotNull( bool value );
	bool IsPK();
	void SetPK( bool value );
	bool IsAutoIncr();
	void SetAutoIncr( bool value );

public:
	CDialog * _parent;
	CEdit _fieldName;
	CComboBox _fieldType;
	CEdit _fieldLength;
	CButton _notNull;
	CButton _primaryKey;
	CButton _autoIncr;
};
