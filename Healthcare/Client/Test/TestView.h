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

	void Refresh() { Invalidate(); }
	void SetUserNo( int userNo ) { _curUserNo = userNo; }

protected:
	DECLARE_MESSAGE_MAP()

private:
	int _curUserNo;
};

