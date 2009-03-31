#pragma once

struct TagDataForDic
{
	std::string _tagName;
	std::vector<std::string> _dataList;
};

typedef std::vector<TagDataForDic> WNDic;

class WNDictionary
{
public:
	WNDictionary();
	~WNDictionary();

	void Init( bool createDic = false );
	void Uninit();

	bool LoadFromXML( const XmlNode * resNode );
	bool SaveToXML( XmlNode * resNode );

	std::string GetTagName( const std::string & inData );

private:
	bool CreateWNDic();
	Synset * GetSynset( std::string keyword );

private:
	WNDic _dic;
	Synset * _synsetPtr;
};