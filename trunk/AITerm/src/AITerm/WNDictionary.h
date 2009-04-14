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

	void Init();
	void Uninit();

	bool LoadFromXML( const XmlNode * resNode );
	bool SaveToXML( XmlNode * resNode );

	int GetTagName( const std::vector<std::string> & inDataList, std::string & outTagName, std::string & outWords );

private:
	bool CreateWNDic( const XmlNode * resNode );
	int IsSameWords( const std::vector<std::string> & lWordList, std::string rWord, std::string & outWords );
	bool IsNumber( const std::string & inData );
	Synset * GetSynset( std::string keyword );

private:
	WNDic _dic;
	Synset * _synsetPtr;
};