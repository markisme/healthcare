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


int _tmain(int argc, _TCHAR* argv[])
{
	int i, j;
	// minipar
	int miniparWordCount;
	CSimpleMinipar Minipar;
	char data[100]= "What is the cost of CPU-A?";				// Given Question
	// wordnet
	Synset synset, *synsetPtr;
	int numOfWordInFile;

	
	//----------------------
	// System Init start
	//----------------------
	
	//----------------------
	// System Init end
	//----------------------


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

	//----------------------------
	// 2. Wordnet Test
	//----------------------------
	wninit();													// wordnet을 초기화
	
	synsetPtr=(Synset *)malloc(sizeof(Synset));					// 사용할 구조체 초기화
	memset( synsetPtr, 0, sizeof(synsetPtr));

	//--------------------------------------------------
	// 2-1. 문자열로 정리된(fomatted) 데이터를 넘겨 받을 때
	//--------------------------------------------------
	char *wordnetData;
	wordnetData= findtheinfo("edible", NOUN, HYPERPTR, ALLSENSES);
	printf("%s\n", wordnetData);


	//--------------------------------------------------
	// 2-2. 각각의 정보에 따로따로 접근할 때
	//--------------------------------------------------
	// 구조체 초기화 (optional 하지만, 해주는게 깔끔할 듯)
	synsetPtr=(Synset *)malloc(sizeof(Synset));
	memset( synsetPtr, 0, sizeof(synsetPtr));

	if( (synsetPtr= findtheinfo_ds( "edible", NOUN, HYPERPTR, ALLSENSES )) == NULL )
	{
		perror("serach failure\n");
		return -1;
	}

	// 검색어 차제의 의미 (NOUN으로써)
	for( i=0; i<synsetPtr->wcount; i++)				// synset에 포함된 동의어 개수만큼
	{
		printf("%s ", (*(synsetPtr->words)++)  );	// synset에 포함된 동의어들 출력
	}
	printf("-- %s\n", synsetPtr->defn);				// 그 단어의 정의 출력

	synsetPtr= synsetPtr->ptrlist;					// 검색된 상위 개념의 단어로 이동하여

	printf("       => ");
	for( i=0; i<synsetPtr->wcount; i++)				// synset에 포함된 동의어 개수만큼
	{
		printf("%s ", (*(synsetPtr->words)++)  );	// synset에 포함된 동의어들 출력
	}
	printf("-- %s\n", synsetPtr->defn);				// 그 단어의 정의 출력


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

		const XmlNode * testNode = resNode->GetNode( "testNode" );
		std::string typeAtt = testNode->GetAttribute( "type" );
	}

	
	return 0;
}

