#include "stdafx.h"
#include "TestCase.h"

#include "NamedEntityRecognition.h"
#include "SemanticTemplateProcessor.h"
#include "QueryGenerator.h"
#include "AnswerGenerator.h"

int _tmain(int argc, _TCHAR* argv[])
{
	TestCase testCase;

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
	DBDictionary * dbDic = namedEntityRecognition.GetDBDic();

	// 개체명 인식 처리
	ResultNamedEntityRecognition resultNamedEntityRecognition;
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
	//testCase.LoadResultDB( dbResultList );

	// 답변 생성기
	AnswerGenerator answerGenerator;
	answerGenerator.Start( resultMatchedTemplate, dbResultList );

	// Just XML test
	//TestCase testCase;
	//namedEntity를 변수로 값으로 넘겨서 처리
	//testCase.XMLLoadTest( &tagList );
	
	return 0;
}