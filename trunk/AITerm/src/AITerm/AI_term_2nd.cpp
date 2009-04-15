#include "stdafx.h"

#include "NamedEntity.h"
#include "QuestionTemplate.h"

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
	NamedEntity namedEntity;
	namedEntity.Init();

	// ���ø� �ε�

	// ������ ó��
	TagList tagList;
	namedEntity.ProcessQuestion( tagList );

	// ���ø� ó��
	QuestionTemplate questionTemplate;
	questionTemplate.Init( &tagList );

	// Just XML test
	//TestCase testCase;
	//namedEntity�� ������ ������ �Ѱܼ� ó��
	//testCase.XMLLoadTest( &tagList );
	

	return 0;
}