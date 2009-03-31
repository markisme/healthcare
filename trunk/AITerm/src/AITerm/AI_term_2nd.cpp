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

struct TagElement
{
	std::string tagName;
	std::string word;
};

typedef std::vector<TagElement> TagList;

void XMLSaveTest( TagList & tagList );

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

	TagList tagList;

	int count = Minipar.GetWordCount();
	for( int num = 0; num < count; num++ )
	{
		std::string word = Minipar.GetAt( num )->szWord;

		std::string colName = dbDic.GetColName( word );
		if( colName.length() != 0 )
		{
			TagElement tag;
			tag.tagName = colName;
			tag.word = word;

			tagList.push_back( tag );
			continue;
		}

		std::string tagName = wnDic.GetTagName( word );
		if( tagName.length() != 0 )
		{
			TagElement tag;
			tag.tagName = tagName;
			tag.word = word;

			tagList.push_back( tag );
			continue;
		}
	}

	printf(">> %s\n", data);									// �־��� ���� ���
	outputParsingResult(Minipar);								// parsing ��� ���
	printf("\n");

	XMLSaveTest( tagList );

	//XmlDocument xmlDoc;
	//XmlNode *XmlNode= TestCase::GetInstance().CreateXMLTag( xmlDoc, "Resource");

	return 0;
}

void XMLSaveTest( TagList & tagList )
{
	// XML ����
	{
		XmlDocument xmlDoc;
		XmlNode *resNode = xmlDoc.AddNode( "resource" );

		int size = tagList.size();
		for( int num = 0; num < size; num++ )
		{
			TagElement tag = tagList[ num ];
			std::string tagName = tag.tagName;
			std::string word = tag.word;

			XmlNode * tagNode = resNode->AddNode( tagName.c_str() );		// TAG�� 		
			tagNode->SetText( word.c_str(), XmlNode::NUMBER );			// ��ü�� �߰�
		}

		std::string path = "test.xml";
		xmlDoc.SaveFile( path.c_str() );
	}
}

