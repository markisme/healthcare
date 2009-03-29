//**********************************************************************************
//
// TITLE	: AI term project 2nd task
// DATE		: 2009/03/27
// AUTHOR	: Lee, Taewoo( ltw1372@lycos.co.kr )
// UPDATE	:
//	- (2009.03.27/08:12) Minipar, wordnet include �Ϸ�
//	- (2009.03.28/05:31) Minipar, Wordnet ���� ������ ���� �׽�Ʈ�ڵ� �ۼ�
//	- (2009.03.28/08:03) 1�������� ���� ���������� �־ ������
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

	printf(">> %s\n", data);									// �־��� ���� ���
	outputParsingResult(Minipar);								// parsing ��� ���
	printf("\n");
	//------------------------
	// Parsing a sentence end
	//------------------------

	
	// ��ü�� �ν� ����
	// 1. �ν��ؾ߸� �ϴ� �ܾ���� ���س��´�. (�������, DB�� ���� �� �׾��� ����� ��� ���Ѵ�)
	//			-> Text ���ϵ�� �����س���.
	// 2. ������ ���̴� ���ξ���� �ν��Ͽ� �����. (what���� �ǹ���, Can you tell me��)

	char *nouns= (char*)malloc(sizeof(SIZE_OF_A_WORD * miniparWordCount));
	memset(nouns, 0, sizeof(SIZE_OF_A_WORD * miniparWordCount));

	//extractNouns( Minipar, miniparWordCount, nouns);		// �������� ���鸸 �����.
	//removeInterrogativePronoun( nouns, miniparWordCount);	// �ǹ����� �����.

	// Can you tell me���� ����տ� �ٴ� ���ʿ��� ������ �����Ѵ�.
	
	// 3. ����� �Ѿ�� �ν��ؾ߸� �ϴ� �ܾ���� ���Ǿ�, ���Ǿ� ���� wordnet�� ���� �˾Ƴ��� ���Ϸ� �����س��´�.
	
	// 4. ������ ���� �����Ϳ��� ������ �κ��� ����� �����Ѵ�.
	// 5. �������� ���� ���� �߿� �׷� �͵��� �ִ��� �˾ƺ���.

	//----------------------------
	// 2. Wordnet Test
	//----------------------------
	wninit();													// wordnet�� �ʱ�ȭ
	
	synsetPtr=(Synset *)malloc(sizeof(Synset));					// ����� ����ü �ʱ�ȭ
	memset( synsetPtr, 0, sizeof(synsetPtr));

	//--------------------------------------------------
	// 2-1. ���ڿ��� ������(fomatted) �����͸� �Ѱ� ���� ��
	//--------------------------------------------------
	char *wordnetData;
	wordnetData= findtheinfo("edible", NOUN, HYPERPTR, ALLSENSES);
	printf("%s\n", wordnetData);


	//--------------------------------------------------
	// 2-2. ������ ������ ���ε��� ������ ��
	//--------------------------------------------------
	// ����ü �ʱ�ȭ (optional ������, ���ִ°� ����� ��)
	synsetPtr=(Synset *)malloc(sizeof(Synset));
	memset( synsetPtr, 0, sizeof(synsetPtr));

	if( (synsetPtr= findtheinfo_ds( "edible", NOUN, HYPERPTR, ALLSENSES )) == NULL )
	{
		perror("serach failure\n");
		return -1;
	}

	// �˻��� ������ �ǹ� (NOUN���ν�)
	for( i=0; i<synsetPtr->wcount; i++)				// synset�� ���Ե� ���Ǿ� ������ŭ
	{
		printf("%s ", (*(synsetPtr->words)++)  );	// synset�� ���Ե� ���Ǿ�� ���
	}
	printf("-- %s\n", synsetPtr->defn);				// �� �ܾ��� ���� ���

	synsetPtr= synsetPtr->ptrlist;					// �˻��� ���� ������ �ܾ�� �̵��Ͽ�

	printf("       => ");
	for( i=0; i<synsetPtr->wcount; i++)				// synset�� ���Ե� ���Ǿ� ������ŭ
	{
		printf("%s ", (*(synsetPtr->words)++)  );	// synset�� ���Ե� ���Ǿ�� ���
	}
	printf("-- %s\n", synsetPtr->defn);				// �� �ܾ��� ���� ���


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

		const XmlNode * testNode = resNode->GetNode( "testNode" );
		std::string typeAtt = testNode->GetAttribute( "type" );
	}

	
	return 0;
}

