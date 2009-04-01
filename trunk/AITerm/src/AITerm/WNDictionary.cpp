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

			// 동의어 추가
			for( int i=0; i<_synsetPtr->wcount; i++)			
			{
				std::string word = (*(_synsetPtr->words)++);
				tagDataList._dataList.push_back( word );
			}

			// 상의어 추가
			while( ( _synsetPtr= _synsetPtr->ptrlist ) != NULL )
			{
				for( int i=0; i<_synsetPtr->wcount; i++)
				{
					std::string word = (*(_synsetPtr->words)++);
					tagDataList._dataList.push_back( word );
				}
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
			if( IsSameWord( inData , word ) )
			{
				return tag;
			}
		}
	}
	return "";
}

bool WNDictionary::IsSameWord( std::string lWord, std::string rWord )
{
	// 전체 비교
	if( lWord == rWord )
	{
		return true;
	}

	//
	int length = lWord.size();
	if( length < 3 )
	{
		return false;
	}

	// 복수 비교 (s)
	if( lWord[length-1] == 's' )
	{
		lWord.erase(length-1);
		
		if( lWord == rWord )
		{
			return true;
		}
	}

	// 복수 비교 (es)
	if( lWord[length-1] == 's' && lWord[length-2] == 'e' )
	{
		lWord.erase(length-1);
		lWord.erase(length-2);
		
		if( lWord == rWord )
		{
			return true;
		}
	}

	return false;
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