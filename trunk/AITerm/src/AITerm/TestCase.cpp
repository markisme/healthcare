#include "stdafx.h"
#include "TestCase.h"

TestCase TestCase::_instance;

TestCase::TestCase()
{
}

TestCase::~TestCase()
{
}

void TestCase::Init()
{
}

void TestCase::Uninit()
{
}

void TestCase::WordnetTest()
{
	Synset synset, *synsetPtr;

	//--------------------------------------------------
	// ���ڿ��� ������(fomatted) �����͸� �Ѱ� ���� ��
	//--------------------------------------------------
	char *wordnetData;
	wordnetData= findtheinfo("edible", NOUN, HYPERPTR, ALLSENSES);
	printf("%s\n", wordnetData);

	//--------------------------------------------------
	// ������ ������ ���ε��� ������ ��
	//--------------------------------------------------
	synsetPtr=(Synset *)malloc(sizeof(Synset));
	memset( synsetPtr, 0, sizeof(synsetPtr));

	if( (synsetPtr= findtheinfo_ds( "edible", NOUN, HYPERPTR, ALLSENSES )) == NULL )
	{
		perror("serach failure\n");
		return;
	}

	// �˻��� ������ �ǹ� (NOUN���ν�)
	for( int i=0; i<synsetPtr->wcount; i++)			// synset�� ���Ե� ���Ǿ� ������ŭ
	{
		printf("%s ", (*(synsetPtr->words)++)  );	// synset�� ���Ե� ���Ǿ�� ���
	}
	printf("-- %s\n", synsetPtr->defn);				// �� �ܾ��� ���� ���

	synsetPtr= synsetPtr->ptrlist;					// �˻��� ���� ������ �ܾ�� �̵��Ͽ�

	printf("       => ");
	for( int i=0; i<synsetPtr->wcount; i++)			// synset�� ���Ե� ���Ǿ� ������ŭ
	{
		printf("%s ", (*(synsetPtr->words)++)  );	// synset�� ���Ե� ���Ǿ�� ���
	}
	printf("-- %s\n", synsetPtr->defn);				// �� �ܾ��� ���� ���
}

void TestCase::XMLLoadSaveTest()
{
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

		int nodeCount = resNode->GetNodeCount( "testNode" );
		for( int num = 0; num < nodeCount; num++ )
		{
			const XmlNode * testNode = resNode->GetNode( "testNode", num );
			std::string typeAtt = testNode->GetAttribute( "type" );
		}
	}
}


void TestCase::XMLSaveTest()
{
	// XML ����
	{
		XmlDocument xmlDoc;

		XmlNode *resNode = xmlDoc.AddNode( "TestNode" );		// TAG�� 
		
		resNode->SetText( "test", XmlNode::NUMBER );			// ��ü�� �߰�

		//resNode->SetAttribute( "attr", "No" );
		//resNode->SetAttribute( "Name", "No_name" );
		//XmlNode *testNode = resNode->AddNode( "testNode" );
		//testNode->SetAttribute( "type", "type_attTest" );
		//XmlNode *testNode2 = testNode->AddNode( "testNode2");

		std::string path = "test.xml";
		xmlDoc.SaveFile( path.c_str() );
	}
}

void TestCase::XMLLoadTest()
{
	// XML �ε�
	{
		XmlDocument xmlDoc;
		std::string path = "test.xml";
		xmlDoc.LoadFile( path.c_str() );

		const XmlNode * resNode = xmlDoc.GetNode( "resource" );
		std::string type = resNode->GetAttribute( "type" );
		std::string name = resNode->GetAttribute( "name" );

		int nodeCount = resNode->GetNodeCount( "testNode" );
		for( int num = 0; num < nodeCount; num++ )
		{
			const XmlNode * testNode = resNode->GetNode( "testNode", num );
			std::string typeAtt = testNode->GetAttribute( "type" );
		}
	}
}

//XmlNode* CreateXMLTag( XmlDocument &xmlDoc, const char *representativeName)
//{
//	XmlNode *resNode = xmlDoc.AddNode( representativeName);		// TAG�� 
//	return resNode;
//}
//
//void AddTag(XmlNode &XmlNode, const char *tagName, const char *tag)
//{
//	
//	XmlNode *tagNode = resNode->AddNode( tagName);
//	tagNode->SetText( tag, XmlNode::NUMBER);
//}