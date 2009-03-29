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

	// Wordnet �ʱ�ȭ
	wninit();													
	
	// Wordnet �׽�Ʈ // ���� �׽�Ʈ �ڵ� ����
	TestCase::GetInstance().WordnetTest();

	// XML �ε� ���̺� �׽�Ʈ // ���� �׽�Ʈ �ڵ� ����
	TestCase::GetInstance().XMLLoadSaveTest();
	
	return 0;
}

