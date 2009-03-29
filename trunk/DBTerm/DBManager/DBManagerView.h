#pragma once

class CDBManagerView : public CView
{
public:
	CDBManagerView();
	virtual ~CDBManagerView();

	DECLARE_DYNCREATE(CDBManagerView)

public:
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnDraw(CDC* pDC);

	void Refresh() { Invalidate(); }

protected:
	DECLARE_MESSAGE_MAP()

private:
};

