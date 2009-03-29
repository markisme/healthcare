#pragma once

class DicCreator
{
public:
	DicCreator();
	~DicCreator();

	void Init();
	void Uninit();

private:
	int GetColNum( std::string name, const ColList & colList );
	void GetColData( int colNum, const DataList & dataList, std::vector<std::string> & outList );

private:
};