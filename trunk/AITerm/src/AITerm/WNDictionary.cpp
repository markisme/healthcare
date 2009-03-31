#include "stdafx.h"
#include "WNDictionary.h"

WNDictionary::WNDictionary()
{
}

WNDictionary::~WNDictionary()
{
}

void WNDictionary::Init( bool createDic )
{
	wninit();

	_synsetPtr=(Synset *)malloc(sizeof(Synset));
	memset( _synsetPtr, 0, sizeof(_synsetPtr));

	// 사전 초기화
	_dic.clear();

	if( createDic == true )
	{
		// 사전 생성
		CreateWNDic();

		// 사전 저장
		XmlDocument xmlDoc;
		XmlNode * resNode = xmlDoc.AddNode( "resource" );
		SaveToXML( resNode );

		std::string path = "./resource/WNDic.xml";
		xmlDoc.SaveFile( path.c_str() );
	}
	else
	{
		// 사전 로드
		XmlDocument xmlDoc;
		std::string path = "./resource/WNDic.xml";
		xmlDoc.LoadFile( path.c_str() );

		const XmlNode * resNode = xmlDoc.GetNode( "resource" );
		LoadFromXML( resNode );
	}
}

void WNDictionary::Uninit()
{
	// 사전 초기화
	_dic.clear();
}

bool WNDictionary::CreateWNDic()
{
	XmlDocument xmlDoc;
	std::string path = "./resource/WNDicConfig.xml";
	xmlDoc.LoadFile( path.c_str() );

	const XmlNode * resNode = xmlDoc.GetNode( "resource" );

	int nodeCount = resNode->GetNodeCount( "tag" );
	for( int num = 0; num < nodeCount; num++ )
	{
		const XmlNode * tagNode = resNode->GetNode( "tag", num );
		std::string tagName = tagNode->GetAttribute( "name" );

		TagDataForDic tagDataList;
		tagDataList._tagName = tagName;

		int nodeCount = tagNode->GetNodeCount( "keyword" );
		for( int cnt = 0; cnt < nodeCount; cnt++ )
		{
			const XmlNode * keywordNode = tagNode->GetNode( "keyword", cnt );
			std::string keyword = keywordNode->GetAttribute( "name" );
			std::string sense = keywordNode->GetAttribute( "sense" );

			if( ( _synsetPtr = GetSynset( keyword ) ) == NULL )
			{
				perror("serach failure\n");
				continue;
			}

			// 검색어 차제의 의미 (NOUN으로써)
			for( int i=0; i<_synsetPtr->wcount; i++)			// synset에 포함된 동의어 개수만큼
			{
				std::string word = (*(_synsetPtr->words)++);
				tagDataList._dataList.push_back( word );

				//printf("%s ", (*(_synsetPtr->words)++)  );		// synset에 포함된 동의어들 출력
			}
			printf("-- %s\n", _synsetPtr->defn);				// 그 단어의 정의 출력

			if( ( _synsetPtr= _synsetPtr->ptrlist ) == NULL ) 
			{
				continue;
			}

			printf("       => ");
			for( int i=0; i<_synsetPtr->wcount; i++)			// synset에 포함된 동의어 개수만큼
			{
				std::string word = (*(_synsetPtr->words)++);
				tagDataList._dataList.push_back( word );
			}
		}

		_dic.push_back( tagDataList );
	}

	return true;
}

bool WNDictionary::LoadFromXML( const XmlNode * resNode )
{
	return true;
}

bool WNDictionary::SaveToXML( XmlNode * resNode )
{
	int size = _dic.size();
	for( int num = 0; num < size; num++ )
	{
		TagDataForDic tagDataList = _dic[num];
		std::string tagName = tagDataList._tagName;

		XmlNode * tagNode = resNode->AddNode( "tag" );
		tagNode->SetAttribute( "name", tagName.c_str() );

		int dataSize = tagDataList._dataList.size();
		for( int cnt = 0; cnt < dataSize; cnt++ )
		{
			std::string word = tagDataList._dataList[ cnt ];

			XmlNode * keywordNode = tagNode->AddNode( "keyword" );
			keywordNode->SetAttribute( "name", word.c_str() );
		}
	}
	return true;
}

std::string WNDictionary::GetTagName( const std::string & inData )
{
	int size = _dic.size();
	for( int num = 0; num < size; num++ )
	{
		std::string tag = _dic[ num ]._tagName;

		int dataSize = _dic[ num ]._dataList.size();
		for( int cnt = 0; cnt < dataSize; cnt++ )
		{
			std::string word = _dic[ num ]._dataList[ cnt ];
			if( inData == word )
			{
				return tag;
			}
		}
	}
	return "";
}

Synset * WNDictionary::GetSynset( std::string keyword )
{
	Synset * synsetPtr = NULL;

	if( (synsetPtr= findtheinfo_ds( (char*)keyword.c_str(), NOUN, HYPERPTR, ALLSENSES )) != NULL )
	{
		return synsetPtr;
	}

	if( (synsetPtr= findtheinfo_ds( (char*)keyword.c_str(), VERB, HYPERPTR, ALLSENSES )) != NULL )
	{
		return synsetPtr;
	}

	if( (synsetPtr= findtheinfo_ds( (char*)keyword.c_str(), ADJ, HYPERPTR, ALLSENSES )) != NULL )
	{
		return synsetPtr;
	}

	if( (synsetPtr= findtheinfo_ds( (char*)keyword.c_str(), ADV, HYPERPTR, ALLSENSES )) != NULL )
	{
		return synsetPtr;
	}

	return synsetPtr;
}