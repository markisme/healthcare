#include "stdafx.h"
#include "WNDictionary.h"

WNDictionary::WNDictionary()
{
}

WNDictionary::~WNDictionary()
{
}

void WNDictionary::Init()
{
	// 워드넷 초기화
	wninit();

	_synsetPtr=(Synset *)malloc(sizeof(Synset));
	memset( _synsetPtr, 0, sizeof(_synsetPtr));

	// 사전 초기화
	_dic.clear();

	// 사전 생성 옵션 로드
	XmlDocument xmlDoc;
	std::string path = "./resource/WNDicConfig.xml";
	xmlDoc.LoadFile( path.c_str() );

	const XmlNode * resNode = xmlDoc.GetNode( "resource" );
	std::string create = resNode->GetAttribute( "create" );

	if( create == "true" )
	{
		// 사전 생성
		CreateWNDic( resNode );

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

bool WNDictionary::CreateWNDic( const XmlNode * resNode )
{
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

			// sense 0 인경우 워드넷 참조 안함
			if( sense == "0" )
			{
				tagDataList._dataList.push_back( keyword );
				continue;
			}

			// 0 이 아닌 경우엔 워드넷 참조
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

			// 상위어 추가
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
	int nodeCount = resNode->GetNodeCount( "tag" );
	for( int num = 0; num < nodeCount; num++ )
	{
		const XmlNode * tagNode = resNode->GetNode( "tag", num );
		std::string tagName = tagNode->GetAttribute( "name" );

		TagDataForDic tagDataList;
		tagDataList._tagName = tagName;
		
		int keyCount = tagNode->GetNodeCount( "keyword" );
		for( int cnt = 0; cnt < keyCount; cnt++ )
		{
			const XmlNode * keyNode = tagNode->GetNode( "keyword", cnt );
			std::string keyword = keyNode->GetAttribute( "name" );

			tagDataList._dataList.push_back( keyword );
		}

		_dic.push_back( tagDataList );
	}

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

int WNDictionary::GetTagName( const std::vector<std::string> & inDataList, std::string & outTagName, std::string & outWords )
{
	// 날짜 / 시간

	// 숫자
	if( IsNumber( (std::string)inDataList[0] ) == true )
	{
		outTagName = "number";
		outWords = inDataList[0];
		return 0;
	}

	// 단어인 경우
	int size = _dic.size();
	for( int num = 0; num < size; num++ )
	{
		std::string tag = _dic[ num ]._tagName;

		int dataSize = _dic[ num ]._dataList.size();
		for( int cnt = 0; cnt < dataSize; cnt++ )
		{
			std::string data = _dic[ num ]._dataList[ cnt ];
			std::string word = ToLowerCase( (char*)data.c_str() );

			int count = IsSameWords( inDataList, word, outWords );
			if( count >= 0 )
			{
				outTagName = tag;
				return count;
			}
		}
	}
	return 0;
}

int WNDictionary::IsSameWords( const std::vector<std::string> & lWordList, std::string rWord, std::string & outWords )
{
	// 연속된 단어 비교
	std::string words;

	int count = lWordList.size();
	for( int num = 0; num < count; num++ )
	{
		std::string word = lWordList[ num ];
		if( words.length() > 0 )
		{
			char prefix = words[words.length()-1];
			if( word == "-" || prefix == '-')
			{
				words += word;
			}
			else
			{
				words += " ";
				words += word;
			}
		}
		else
		{
			words = word;
		}
		
		if( words == rWord )
		{
			outWords = words;
			return num;
		}
	}

	std::string lWord = lWordList[ 0 ];

	//
	int length = lWord.size();
	if( length < 3 )
	{
		return -1;
	}

	// 복수 비교 (s)
	if( lWord[length-1] == 's' )
	{
		lWord.erase(length-1);
		
		if( lWord == rWord )
		{
			outWords = lWord;
			return 0;
		}
	}

	// 복수 비교 (es)
	if( lWord[length-1] == 's' && lWord[length-2] == 'e' )
	{
		lWord.erase(length-1);
		lWord.erase(length-2);
		
		if( lWord == rWord )
		{
			outWords = lWord;
			return 0;
		}
	}

	return -1;
}

bool WNDictionary::IsNumber( const std::string & inData )
{
	int num = atoi( inData.c_str() );
	if( num > 0 || inData == "0" )
	{
		return true;
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