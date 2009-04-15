#include "stdafx.h"
#include "SemanticTemplateProcessor.h"

SemanticTemplateProcessor::SemanticTemplateProcessor()
{
}

SemanticTemplateProcessor::~SemanticTemplateProcessor()
{
}

void SemanticTemplateProcessor::Init( TagList * tagList )
{
	// 템플릿 로드
	TemplateList tempList;
	LoadTemplateList( tempList );

	// 매칭되는 템플릿 찾기
	QuestionResultList questionResultList;

	int count = tagList->size();
	for( int num = 0; num < count; num++ )
	{
		Tags & tags = (*tagList)[ num ];
		QuestionResult questionResult;
		CompareTagname( tempList, tags, questionResult );
		questionResultList.push_back( questionResult );
	}

	// 저장
	SaveResultSemanticTemplate( questionResultList );
}

void SemanticTemplateProcessor::Uninit()
{
}

void SemanticTemplateProcessor::LoadTemplateList( TemplateList & tempList )
{
	// 질문 템플릿 xml 로드
	XmlDocument xmlDoc;
	std::string path = "./resource/QuestionTemplate.xml";
	xmlDoc.LoadFile( path.c_str() );

	// 결과 xml 파일 로드
	XmlDocument xmlDoc2;
	std::string path2 = "./resource/Result.xml";
	xmlDoc2.LoadFile( path2.c_str() );

	const XmlNode *resNode			= xmlDoc.GetNode( "resource");
	const XmlNode *resNodeOfResult	= xmlDoc2.GetNode( "resource");

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
				printf("%d = %s\n", tagNum, tagEntityOfQuestion.c_str() );
				slot._tagNameList.push_back( tagEntityOfQuestion );
			}

			temp.push_back( slot );
		}

		tempList.push_back( temp );
	}
}

void SemanticTemplateProcessor::SaveResultSemanticTemplate( QuestionResultList & inQuestionResultList )
{
	XmlDocument xmlDoc;
	XmlNode * resNode = xmlDoc.AddNode( "resource" );

	int count = inQuestionResultList.size();
	for( int num = 0; num < count; num++ )
	{
		QuestionResult & questionResult = inQuestionResultList[ num ];
		;
		char buf[8];
		
		itoa( num, buf, 10 );
		std::string questionNo = buf;

		itoa( questionResult._tempNo, buf, 10 );
		std::string templateNo = buf;

		XmlNode * questionNode = resNode->AddNode( "question" );
		questionNode->SetAttribute( "questionno", questionNo.c_str() );
		questionNode->SetAttribute( "templateno", templateNo.c_str() );

		int size = questionResult._slotList.size();
		for( int cnt = 0; cnt < size; cnt++)
		{
			ResultSlot resultSlot = questionResult._slotList[ cnt ];

			XmlNode * slotNode = questionNode->AddNode( "slot" );
			slotNode->SetAttribute( "type", resultSlot._slotType.c_str() );
			slotNode->SetAttribute( "tagname", resultSlot._tagName.c_str() );
			slotNode->SetText( resultSlot._word.c_str(), XmlNode::NUMBER );
		}
	}

	std::string path = "./resource/ResultSemanticTemplate.xml";
	xmlDoc.SaveFile( path.c_str() );
}

void SemanticTemplateProcessor::CompareTagname( TemplateList & tempList, Tags & tags, QuestionResult & outQuestionResult )
{
	QuestionResultList questionResultList;

	int count = tempList.size();
	for( int num = 0; num < count; num++ )
	{
		Template & temp = tempList[ num ];
		bool matchTemplate = false;
		QuestionResult questionResult;
		questionResult._tempNo = num;

		int size = temp.size();
		for( int cnt = 0; cnt < size; cnt++ )
		{
			TemplateSlot & slot = temp[ cnt ];
			bool needSlot = slot._need;
			std::string type = slot._type;

			bool matchSlot = false;
			ResultSlot resultSlot;
			resultSlot._need = slot._need;
			resultSlot._slotType = type;

			int slotSize = slot._tagNameList.size();
			for( int k = 0; k < slotSize; k++ )
			{
				std::string tagName = ToLowerCase( (char*)slot._tagNameList[ k ].c_str() );
				matchSlot = IsMatchWord( tags, tagName, questionResult, resultSlot );
				if( matchSlot == true )
				{
					break;
				}
			}

			if( matchSlot == true )
			{
				questionResult._slotList.push_back( resultSlot );
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
			questionResultList.push_back( questionResult );
		}
	}

	SelectTemplate( questionResultList, outQuestionResult );
}

bool SemanticTemplateProcessor::IsMatchWord( Tags & tags, std::string & tagName, QuestionResult & questionResult, ResultSlot & outResultSlot )
{
	int tagsSize = tags.size();
	for( int n = 0; n < tagsSize; n++ )
	{
		if( IsWordInSlot( questionResult, n ) == true )
		{
			continue;
		}

		TagElement & tag = tags[ n ];
		std::string name = tag._name;
		if( name == tagName )
		{
			outResultSlot._word = tag._word;
			outResultSlot._tagName = tag._name;
			outResultSlot._wordPos = n;
			return true;
		}
	}

	return false;
}

bool SemanticTemplateProcessor::IsWordInSlot( QuestionResult & questionResult, int n )
{
	int matchSlotCount = questionResult._slotList.size();
	for( int m = 0; m < matchSlotCount; m++ )
	{
		if( n == questionResult._slotList[m]._wordPos )
		{
			return true;
		}
	}

	return false;
}

void SemanticTemplateProcessor::SelectTemplate( QuestionResultList & questionResultList, QuestionResult & outQuestionResult )
{
	int matchTemplate = -1;
	int maxMatchSlot = 0;

	int count = questionResultList.size();
	for( int num = 0; num < count; num++ )
	{
		QuestionResult questionResult = questionResultList[ num ];
		int tempNo = questionResult._tempNo;

		int matchPoint = 0;

		int size = questionResult._slotList.size();
		for( int cnt = 0; cnt < size; cnt++ )
		{
			ResultSlot resultSlot = questionResult._slotList[cnt];

			bool need = resultSlot._need;
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
		QuestionResult & questionResult = questionResultList[ num ];
		int tempNo = questionResult._tempNo;
		if( matchTemplate == tempNo )
		{
			outQuestionResult = questionResult;
		}
	}
}