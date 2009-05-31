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
	// ��ü�� �νı� �ʱ�ȭ
	NamedEntityRecognition namedEntityRecognition;
	namedEntityRecognition.Init();
	DBDictionary * dbDic = namedEntityRecognition.GetDBDic();

	// ��ü�� �ν� ó��
	ResultNamedEntityRecognition resultNamedEntityRecognition;
	namedEntityRecognition.LoadQuestions( path/*"./resource/Questions.xml"*/ );
	namedEntityRecognition.ProcessQuestion( resultNamedEntityRecognition );

	// ���ø� ��Ī ó��
	SemanticTemplateProcessor semanticTemplateProcessor;
	semanticTemplateProcessor.Start( &resultNamedEntityRecognition );
	ResultMatchedTemplate & resultMatchedTemplate = semanticTemplateProcessor.GetResultMatchedTemplate();

	// ���� ������
	QueryGenerator queryGenerator;
	queryGenerator.Start( resultMatchedTemplate, dbDic );
	const DBResultList & dbResultList = queryGenerator.GetDBResultList();

	// db ��� �ε� (�ð������������� �׽�Ʈ �ڵ�)
	//DBResultList dbResultList;
	//LoadResultDB( dbResultList );

	// �亯 ������
	AnswerGenerator answerGenerator;
	answerGenerator.Start( resultMatchedTemplate, dbResultList );
}

void TestCase::WordnetTest()
{
	Synset synset, *synsetPtr;

	//--------------------------------------------------
	// ���ڿ��� ������(fomatted) �����͸� �Ѱ� ���� ��
	//--------------------------------------------------
	char *wordnetData;
	wordnetData= findtheinfo("edible", NOUN, HYPERPTR, ALLSENSES);
	printf("%s\n", wordnetData);

	//--------------------------------------------------
	// ������ ������ ���ε��� ������ ��
	//--------------------------------------------------
	synsetPtr=(Synset *)malloc(sizeof(Synset));
	memset( synsetPtr, 0, sizeof(synsetPtr));

	if( (synsetPtr= findtheinfo_ds( "edible", NOUN, HYPERPTR, ALLSENSES )) == NULL )
	{
		perror("serach failure\n");
		return;
	}

	// �˻��� ������ �ǹ� (NOUN���ν�)
	for( int i=0; i<synsetPtr->wcount; i++)			// synset�� ���Ե� ���Ǿ� ������ŭ
	{
		printf("%s ", (*(synsetPtr->words)++)  );	// synset�� ���Ե� ���Ǿ�� ���
	}
	printf("-- %s\n", synsetPtr->defn);				// �� �ܾ��� ���� ���

	synsetPtr= synsetPtr->ptrlist;					// �˻��� ���� ������ �ܾ�� �̵��Ͽ�

	printf("       => ");
	for( int i=0; i<synsetPtr->wcount; i++)			// synset�� ���Ե� ���Ǿ� ������ŭ
	{
		printf("%s ", (*(synsetPtr->words)++)  );	// synset�� ���Ե� ���Ǿ�� ���
	}
	printf("-- %s\n", synsetPtr->defn);				// �� �ܾ��� ���� ���
}

void TestCase::XMLLoadSaveTest()
{
	// XML ����
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

	// XML �ε�
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
	// ���� ���ø� xml �ε�
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

		// ���� num��° Question template����,
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