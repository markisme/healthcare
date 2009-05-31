#include "stdafx.h"
#include "TestCase.h"

#include "NamedEntityRecognition.h"
#include "SemanticTemplateProcessor.h"
#include "QueryGenerator.h"
#include "AnswerGenerator.h"

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

void TestCase::TestStart( std::string path )
{
	// 개체명 인식기 초기화
	NamedEntityRecognition namedEntityRecognition;
	namedEntityRecognition.Init();
	DBDictionary * dbDic = namedEntityRecognition.GetDBDic();

	// 개체명 인식 처리
	ResultNamedEntityRecognition resultNamedEntityRecognition;
	namedEntityRecognition.LoadQuestions( path/*"./resource/Questions.xml"*/ );
	namedEntityRecognition.ProcessQuestion( resultNamedEntityRecognition );

	// 템플릿 매칭 처리
	SemanticTemplateProcessor semanticTemplateProcessor;
	semanticTemplateProcessor.Start( &resultNamedEntityRecognition );
	ResultMatchedTemplate & resultMatchedTemplate = semanticTemplateProcessor.GetResultMatchedTemplate();

	// 쿼리 생성기
	QueryGenerator queryGenerator;
	queryGenerator.Start( resultMatchedTemplate, dbDic );
	const DBResultList & dbResultList = queryGenerator.GetDBResultList();

	// db 결과 로드 (시간지연으로인한 테스트 코드)
	//DBResultList dbResultList;
	//LoadResultDB( dbResultList );

	// 답변 생성기
	AnswerGenerator answerGenerator;
	answerGenerator.Start( resultMatchedTemplate, dbResultList );
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

	if( (synsetPtr= findtheinfo_ds( "edible", NOUN, HYPERPTR, ALLSENSES )) == NULL )
	{
		perror("serach failure\n");
		return;
	}

	// 검색어 차제의 의미 (NOUN으로써)
	for( int i=0; i<synsetPtr->wcount; i++)			// synset에 포함된 동의어 개수만큼
	{
		printf("%s ", (*(synsetPtr->words)++)  );	// synset에 포함된 동의어들 출력
	}
	printf("-- %s\n", synsetPtr->defn);				// 그 단어의 정의 출력

	synsetPtr= synsetPtr->ptrlist;					// 검색된 상위 개념의 단어로 이동하여

	printf("       => ");
	for( int i=0; i<synsetPtr->wcount; i++)			// synset에 포함된 동의어 개수만큼
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
			printf("%d= %s\n", num, typeAtt.c_str());
		}
	}
}


void TestCase::LoadResultDB( DBResultList & dbResultList )
{
	// 질문 템플릿 xml 로드
	XmlDocument xmlDoc;
	std::string path = "./resource/ResultDB.xml";
	xmlDoc.LoadFile( path.c_str() );

	const XmlNode *resNode = xmlDoc.GetNode( "resource");

	//
	int nodeCount = resNode->GetNodeCount( "question" );
	for( int num = 0; num < nodeCount; num++ )
	{	
		//
		const XmlNode * questionNode = resNode->GetNode( "question", num );
		std::string qsNum = questionNode->GetAttribute( "no" );

		//
		DataList dataList;

		// 만약 num번째 Question template에서,
		int rowCount = questionNode->GetNodeCount( "row" );
		for( int num2 = 0; num2 < rowCount; num2++ )
		{	
			const XmlNode * rowNode = questionNode->GetNode( "row", num2 );

			//
			RowData row;

			int dataCount = rowNode->GetNodeCount( "data" );
			for( int k = 0; k < dataCount; k++ )
			{
				const XmlNode * dataNode = rowNode->GetNode( "data", k );

				std::string data = dataNode->GetText();
				row._data.push_back( data );

				std::string col = dataNode->GetAttribute( "name" );
				row._col.push_back( col );
			}

			dataList.push_back( row );
		}

		dbResultList.push_back( dataList );
	}
}