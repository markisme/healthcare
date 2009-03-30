//**********************************************************************************
//
// TITLE	: AI term project 2nd task
// DATE		: 2009/03/27
// AUTHOR	: Lee, Taewoo( ltw1372@lycos.co.kr )
// UPDATE	:
//	- (2009.03.27/08:12) Minipar, wordnet include 완료
//	- (2009.03.28/05:31) Minipar, Wordnet 사용법 습득을 위한 테스트코드 작성
//	- (2009.03.28/08:03) 1차보고서의 실제 샘플질문을 넣어서 돌려봄
//
//**********************************************************************************

#include "stdafx.h"
#include <string>
#include "DBDictionary.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int i, j;
	// minipar
	int miniparWordCount;
	CSimpleMinipar Minipar;
	char data[100]= "What is the cost of CPU-A?";				// Given Question
	
	//----------------------
	// System Init start
	//----------------------

	wninit();													// Wordnet 초기화

	DBConnectInfo info;
	info.ip = "211.239.163.46";
	info.user = "naid";
	info.pass = "rbgur0834#!";
	info.port = 3306;
	info.dbName = "AITerm";

	DBConnector::GetInstance().Init( info );

	//----------------------
	// System Init end
	//----------------------

	// DB 사전 만들기
	DBDictionary dbDic;
	dbDic.Init( true );

	// 사전 테스트
	std::string colName = dbDic.GetColName( "E7400" );
	std::string tableName = dbDic.GetTableName( "E7400" );

	//------------------------
	// Parsing a sentence
	//------------------------
	if( !Minipar.Parse(data))									// Parsing
	{
		perror("parsing failure\n");
		return -1;
	}
	miniparWordCount= Minipar.GetWordCount();

	printf(">> %s\n", data);									// 주어진 문장 출력
	outputParsingResult(Minipar);								// parsing 결과 출력
	printf("\n");
	//------------------------
	// Parsing a sentence end
	//------------------------

	
	// 개체명 인식 전략
	// 1. 인식해야만 하는 단어들을 정해놓는다. (예를들면, DB에 없는 건 죽었다 깨어나도 대답 못한다)
	//			-> Text 파일들로 저장해놓음.
	// 2. 질문에 쓰이는 접두어들을 인식하여 떼어낸다. (what등의 의문문, Can you tell me등)

	char *nouns= (char*)malloc(sizeof(SIZE_OF_A_WORD * miniparWordCount));
	memset(nouns, 0, sizeof(SIZE_OF_A_WORD * miniparWordCount));

	//extractNouns( Minipar, miniparWordCount, nouns);		// 질문에서 명사들만 떼어낸다.
	//removeInterrogativePronoun( nouns, miniparWordCount);	// 의문문을 떼어낸다.

	// Can you tell me등의 문장앞에 붙는 불필요한 문장을 제거한다.
	
	// 3. 떼어내고 넘어온 인식해야만 하는 단어들의 동의어, 상의어 들을 wordnet을 통해 알아내고 파일로 저장해놓는다.
	
	// 4. 저장해 놓은 데이터에서 부족한 부분은 사람이 보충한다.
	// 5. 질문으로 들어온 명사들 중에 그런 것들이 있는지 알아본다.
												

	//------------------------
	// Test Code
	//------------------------
	
	// Wordnet 테스트 // 사용시 테스트 코드 참조
	TestCase::GetInstance().WordnetTest();

	// XML 로드 세이브 테스트 // 사용시 테스트 코드 참조
	// TestCase::GetInstance().XMLLoadSaveTest();
	
	return 0;
}

