#include "stdafx.h"
#include "SemanticTemplateProcessor.h"

SemanticTemplateProcessor::SemanticTemplateProcessor()
{
}

SemanticTemplateProcessor::~SemanticTemplateProcessor()
{
}

void SemanticTemplateProcessor::Init( ResultNamedEntityRecognition * resultNamedEntityRecognition )
{
	// 템플릿 로드
	TemplateList tempList;
	LoadTemplateList( tempList );

	// 매칭되는 템플릿 찾기
	int count = resultNamedEntityRecognition->size();
	for( int num = 0; num < count; num++ )
	{
		NamedEntityList & namedEntityList = (*resultNamedEntityRecognition)[ num ];
		MatchedTemplate matchedTemplate;
		CompareTagname( tempList, namedEntityList, matchedTemplate );
		_resultMatchedTemplate.push_back( matchedTemplate );
	}

	// 저장
	SaveResultSemanticTemplateProcess( _resultMatchedTemplate );
}

void SemanticTemplateProcessor::Uninit()
{
}

void SemanticTemplateProcessor::LoadTemplateList( TemplateList & tempList )
{
	// 질문 템플릿 xml 로드
	XmlDocument xmlDoc;
	std::string path = "./resource/SemanticTemplate.xml";
	xmlDoc.LoadFile( path.c_str() );

	const XmlNode *resNode			= xmlDoc.GetNode( "resource");

	// QuestionTemplate xml파일의 
	int nodeCount = resNode->GetNodeCount( "template" );
	for( int num = 0; num < nodeCount; num++ )
	{	
		// Question template node들을 돌며,
		const XmlNode * templateNode = resNode->GetNode( "template", num );
		std::string templateNum = templateNode->GetAttribute( "no" );

		//
		Template temp;

		// 만약 num번째 Question template에서,
		int slotCount = templateNode->GetNodeCount( "slot" );
		for( int num2 = 0; num2 < slotCount; num2++ )
		{	
			// slot의 개수만큼 돌며 
			// 속성들을 읽어오고,
			const XmlNode * slotNode = templateNode->GetNode( "slot", num2 );
			std::string slotType = slotNode->GetAttribute( "type" );
			std::string slotNeed = slotNode->GetAttribute( "need" );

			//
			TemplateSlot slot;
			slot._type = slotType;

			if( slotNeed == "true" )
			{
				slot._need = true;
			}
			else if( slotNeed == "false" )
			{
				slot._need = false;
			}

			// 슬롯 안의 tag들을 가져와서,
			int tagCount = slotNode->GetNodeCount( "tag" );
			for (int tagNum=0; tagNum<tagCount; tagNum++ )
			{
				const XmlNode *tagNode_Question= slotNode->GetNode( "tag", tagNum);
				std::string tagEntityOfQuestion = tagNode_Question->GetAttribute( "name" );
				slot._tagNameList.push_back( tagEntityOfQuestion );
			}

			temp.push_back( slot );
		}

		tempList.push_back( temp );
	}
}

void SemanticTemplateProcessor::SaveResultSemanticTemplateProcess( ResultMatchedTemplate & inResultMatchedTemplate )
{
	XmlDocument xmlDoc;
	XmlNode * resNode = xmlDoc.AddNode( "resource" );

	int count = inResultMatchedTemplate.size();
	for( int num = 0; num < count; num++ )
	{
		MatchedTemplate & matchedTemplate = inResultMatchedTemplate[ num ];

		char buf[8];
		
		itoa( num, buf, 10 );
		std::string questionNo = buf;

		itoa( matchedTemplate._tempNo, buf, 10 );
		std::string templateNo = buf;

		XmlNode * questionNode = resNode->AddNode( "question" );
		questionNode->SetAttribute( "questionno", questionNo.c_str() );
		questionNode->SetAttribute( "templateno", templateNo.c_str() );

		int size = matchedTemplate._slotList.size();
		for( int cnt = 0; cnt < size; cnt++)
		{
			MatchedSlot matchedSlot = matchedTemplate._slotList[ cnt ];

			XmlNode * slotNode = questionNode->AddNode( "slot" );
			slotNode->SetAttribute( "type", matchedSlot._slotType.c_str() );
			slotNode->SetAttribute( "tagname", matchedSlot._tagName.c_str() );
			slotNode->SetText( matchedSlot._word.c_str(), XmlNode::NUMBER );
		}
	}

	std::string path = "./resource/ResultSemanticTemplateProcess.xml";
	xmlDoc.SaveFile( path.c_str() );
}

void SemanticTemplateProcessor::CompareTagname( TemplateList & tempList, NamedEntityList & namedEntityList, MatchedTemplate & outMatchedTemplate )
{
	ResultMatchedTemplate resultMatchedTemplate;

	int count = tempList.size();
	for( int num = 0; num < count; num++ )
	{
		Template & temp = tempList[ num ];
		bool matchTemplate = false;
		MatchedTemplate matchedTemplate;
		matchedTemplate._tempNo = num;

		int size = temp.size();
		for( int cnt = 0; cnt < size; cnt++ )
		{
			TemplateSlot & slot = temp[ cnt ];
			bool needSlot = slot._need;
			std::string type = slot._type;

			bool matchSlot = false;
			MatchedSlot matchedSlot;
			matchedSlot._need = slot._need;
			matchedSlot._slotType = type;

			int slotSize = slot._tagNameList.size();
			for( int k = 0; k < slotSize; k++ )
			{
				std::string tagName = ToLowerCase( (char*)slot._tagNameList[ k ].c_str() );
				matchSlot = IsMatchWord( namedEntityList, tagName, matchedTemplate, matchedSlot );
				if( matchSlot == true )
				{
					break;
				}
			}

			if( matchSlot == true )
			{
				matchedTemplate._slotList.push_back( matchedSlot );
			}

			if( matchSlot == false && slot._need == true )
			{
				matchTemplate = false;
				break;
			}

			if( matchSlot == true && slot._need == true )
			{
				matchTemplate = true;
			}
		}

		if( matchTemplate == true )
		{
			resultMatchedTemplate.push_back( matchedTemplate );
		}
	}

	SelectTemplate( resultMatchedTemplate, outMatchedTemplate );
}

bool SemanticTemplateProcessor::IsMatchWord( NamedEntityList & namedEntityList, std::string & tagName, MatchedTemplate & matchedTemplate, MatchedSlot & outMatchedSlot )
{
	int tagsSize = namedEntityList.size();
	for( int n = 0; n < tagsSize; n++ )
	{
		if( IsWordInSlot( matchedTemplate, n ) == true )
		{
			continue;
		}

		NamedEntityElement & namedEntityElement = namedEntityList[ n ];
		std::string name = namedEntityElement._name;
		if( name == tagName )
		{
			outMatchedSlot._word = namedEntityElement._word;
			outMatchedSlot._tagName = namedEntityElement._name;
			outMatchedSlot._wordPos = n;
			return true;
		}
	}

	return false;
}

bool SemanticTemplateProcessor::IsWordInSlot( MatchedTemplate & matchedTemplate, int n )
{
	int matchSlotCount = matchedTemplate._slotList.size();
	for( int m = 0; m < matchSlotCount; m++ )
	{
		if( n == matchedTemplate._slotList[m]._wordPos )
		{
			return true;
		}
	}

	return false;
}

void SemanticTemplateProcessor::SelectTemplate( ResultMatchedTemplate & resultMatchedTemplate, MatchedTemplate & outMatchedTemplate )
{
	int matchTemplate = -1;
	int maxMatchSlot = 0;

	int count = resultMatchedTemplate.size();
	for( int num = 0; num < count; num++ )
	{
		MatchedTemplate matchedTemplate = resultMatchedTemplate[ num ];
		int tempNo = matchedTemplate._tempNo;

		int matchPoint = 0;

		int size = matchedTemplate._slotList.size();
		for( int cnt = 0; cnt < size; cnt++ )
		{
			MatchedSlot matchedSlot = matchedTemplate._slotList[cnt];

			bool need = matchedSlot._need;
			if( need )
			{
				matchPoint += 10;
			}
			else
			{
				matchPoint += 3;
			}
		}

		if( maxMatchSlot < matchPoint )
		{
			maxMatchSlot = matchPoint;
			matchTemplate = tempNo;
		}
	}

	for( int num = 0; num < count; num++ )
	{
		MatchedTemplate & matchedTemplate = resultMatchedTemplate[ num ];
		int tempNo = matchedTemplate._tempNo;
		if( matchTemplate == tempNo )
		{
			outMatchedTemplate = matchedTemplate;
		}
	}
}