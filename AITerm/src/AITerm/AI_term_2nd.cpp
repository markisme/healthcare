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

	// DB ���� �����
	DBDictionary dbDic;
	dbDic.Init( true );

	// ����� ���� �����
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

	printf(">> %s\n", data);									// �־��� ���� ���
	outputParsingResult(Minipar);								// parsing ��� ���
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
	
	// Wordnet �׽�Ʈ // ���� �׽�Ʈ �ڵ� ����
	// TestCase::GetInstance().WordnetTest();

	// XML �ε� ���̺� �׽�Ʈ // ���� �׽�Ʈ �ڵ� ����
	// TestCase::GetInstance().XMLLoadSaveTest();
	
	return 0;
}

