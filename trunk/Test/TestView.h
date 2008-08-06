#pragma once

class CTestView : public CView
{
public:
	CTestView();
	virtual ~CTestView();

	DECLARE_DYNCREATE(CTestView)

public:
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnDraw(CDC* pDC);

protected:
	DECLARE_MESSAGE_MAP()
};
