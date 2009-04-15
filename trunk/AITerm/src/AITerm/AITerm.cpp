#include "stdafx.h"

#include "NamedEntityRecognition.h"
#include "SemanticTemplateProcessor.h"

int _tmain(int argc, _TCHAR* argv[])
{
	// DB 접속 정보
	DBConnectInfo info;
	info.ip = "211.189.19.160";
	info.user = "root";
	info.pass = "1234";
	info.port = 3306;
	info.dbName = "AITerm";

	// DB 정보 초기화(싱글턴)
	DBConnector::GetInstance().Init( info );

	// 개체명 인식기 초기화
	NamedEntityRecognition namedEntityRecognition;
	namedEntityRecognition.Init();

	// 개체명 인식 처리
	TagList tagList;
	namedEntityRecognition.ProcessQuestion( tagList );

	// 템플릿 매칭 처리
	SemanticTemplateProcessor semanticTemplateProcessor;
	semanticTemplateProcessor.Init( &tagList );

	// Just XML test
	//TestCase testCase;
	//namedEntity를 변수로 값으로 넘겨서 처리
	//testCase.XMLLoadTest( &tagList );
	

	return 0;
}