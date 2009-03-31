#include "stdafx.h"
#include "TestCase.h"

TestCase TestCase::_instance;

TestCase::TestCase()
{
}

TestCase::~TestCase()
{
}

void TestCase::Init()
{
}

void TestCase::Uninit()
{
}

void TestCase::WordnetTest()
{
	Synset synset, *synsetPtr;

	//--------------------------------------------------
	// 문자열로 정리된(fomatted) 데이터를 넘겨 받을 때
	//--------------------------------------------------
	char *wordnetData;
	wordnetData= findtheinfo("edible", NOUN, HYPERPTR, ALLSENSES);
	printf("%s\n", wordnetData);

	//--------------------------------------------------
	// 각각의 정보에 따로따로 접근할 때
	//--------------------------------------------------
	synsetPtr=(Synset *)malloc(sizeof(Synset));
	memset( synsetPtr, 0, sizeof(synsetPtr));

	if( (synsetPtr= findtheinfo_ds( "man", NOUN, HYPERPTR, ALLSENSES )) == NULL )
	{
		perror("serach failure\n");
		return;
	}

	// 검색어 차제의 의미 (NOUN으로써)
	for( int i=0; i<synsetPtr->wcount; i++)				// synset에 포함된 동의어 개수만큼
	{
		printf("%s ", (*(synsetPtr->words)++)  );	// synset에 포함된 동의어들 출력
	}
	printf("-- %s\n", synsetPtr->defn);				// 그 단어의 정의 출력

	synsetPtr= synsetPtr->ptrlist;					// 검색된 상위 개념의 단어로 이동하여

	printf("       => ");
	for( int i=0; i<synsetPtr->wcount; i++)				// synset에 포함된 동의어 개수만큼
	{
		printf("%s ", (*(synsetPtr->words)++)  );	// synset에 포함된 동의어들 출력
	}
	printf("-- %s\n", synsetPtr->defn);				// 그 단어의 정의 출력
}

void TestCase::XMLLoadSaveTest()
{
	// XML 저장
	{
		XmlDocument xmlDoc;
		XmlNode * resNode = xmlDoc.AddNode( "resource" );	
		resNode->SetAttribute( "type", "type_test" );
		resNode->SetAttribute( "name", "name_test" );

		XmlNode * testNode = resNode->AddNode( "testNode" );
		testNode->SetAttribute( "type", "type_attTest" );

		std::string path = "test.xml";
		xmlDoc.SaveFile( path.c_str() );
	}

	// XML 로드
	{
		XmlDocument xmlDoc;
		std::string path = "test.xml";
		xmlDoc.LoadFile( path.c_str() );

		const XmlNode * resNode = xmlDoc.GetNode( "resource" );
		std::string type = resNode->GetAttribute( "type" );
		std::string name = resNode->GetAttribute( "name" );

		int nodeCount = resNode->GetNodeCount( "testNode" );
		for( int num = 0; num < nodeCount; num++ )
		{
			const XmlNode * testNode = resNode->GetNode( "testNode", num );
			std::string typeAtt = testNode->GetAttribute( "type" );
		}
	}
}