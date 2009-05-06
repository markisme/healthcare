#include "stdafx.h"

#include "NamedEntityRecognition.h"
#include "SemanticTemplateProcessor.h"
#include "QueryGenerator.h"

int _tmain(int argc, _TCHAR* argv[])
{
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
	semanticTemplateProcessor.Init( &resultNamedEntityRecognition );
	ResultMatchedTemplate & resultMatchedTemplate = semanticTemplateProcessor.GetResultMatchedTemplate();

	// ���� ������
	QueryGenerator queryGenerator;
	queryGenerator.Init( resultMatchedTemplate, dbDic );

	// Just XML test
	//TestCase testCase;
	//namedEntity�� ������ ������ �Ѱܼ� ó��
	//testCase.XMLLoadTest( &tagList );
	
	return 0;
}