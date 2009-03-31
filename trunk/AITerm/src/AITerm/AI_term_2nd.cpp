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
#include "WNDictionary.h"

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

	// 워드넷 사전 만들기
	WNDictionary wnDic;
	wnDic.Init( true );

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

	// std::string tagName = wnDic.GetTagName( "constitute" );
	//if( tagName.length() != 0 )
	//{
	//}
												
	//------------------------
	// Test Code
	//------------------------
	
	// Wordnet 테스트 // 사용시 테스트 코드 참조
	// TestCase::GetInstance().WordnetTest();

	// XML 로드 세이브 테스트 // 사용시 테스트 코드 참조
	// TestCase::GetInstance().XMLLoadSaveTest();
	
	return 0;
}

