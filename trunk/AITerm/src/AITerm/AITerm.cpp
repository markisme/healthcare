#include "stdafx.h"
#include "TestCase.h"

#include "NamedEntityRecognition.h"
#include "SemanticTemplateProcessor.h"
#include "QueryGenerator.h"
#include "AnswerGenerator.h"

int _tmain(int argc, _TCHAR* argv[])
{
	TestCase testCase;

	// DB ���� ����
	DBConnectInfo info;
	info.ip = "211.189.19.160";
	info.user = "root";
	info.pass = "1234";
	info.port = 3306;
	info.dbName = "AITerm";

	// DB ���� �ʱ�ȭ(�̱���)
	DBConnector::GetInstance().Init( info );

	// ��ü�� �νı� �ʱ�ȭ
	NamedEntityRecognition namedEntityRecognition;
	namedEntityRecognition.Init();
	DBDictionary * dbDic = namedEntityRecognition.GetDBDic();

	// ��ü�� �ν� ó��
	ResultNamedEntityRecognition resultNamedEntityRecognition;
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
	//testCase.LoadResultDB( dbResultList );

	// �亯 ������
	AnswerGenerator answerGenerator;
	answerGenerator.Start( resultMatchedTemplate, dbResultList );

	// Just XML test
	//TestCase testCase;
	//namedEntity�� ������ ������ �Ѱܼ� ó��
	//testCase.XMLLoadTest( &tagList );
	
	return 0;
}