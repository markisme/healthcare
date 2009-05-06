#include "stdafx.h"

#include "NamedEntityRecognition.h"
#include "SemanticTemplateProcessor.h"
#include "QuaryGenerator.h"

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

	// ��ü�� �ν� ó��
	ResultNamedEntityRecognition resultNamedEntityRecognition;
	namedEntityRecognition.ProcessQuestion( resultNamedEntityRecognition );

	// ���ø� ��Ī ó��
	SemanticTemplateProcessor semanticTemplateProcessor;
	semanticTemplateProcessor.Init( &resultNamedEntityRecognition );
	ResultMatchedTemplate & resultMatchedTemplate = semanticTemplateProcessor.GetResultMatchedTemplate();

	// ���� ������
	//QuaryGenerator quaryGenerator;
	//quaryGenerator.Init( resultMatchedTemplate );

	// Just XML test
	//TestCase testCase;
	//namedEntity�� ������ ������ �Ѱܼ� ó��
	//testCase.XMLLoadTest( &tagList );
	
	return 0;
}