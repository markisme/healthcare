#include "stdafx.h"
#include "QuestionTemplate.h"

QuestionTemplate::QuestionTemplate()
{
}

QuestionTemplate::~QuestionTemplate()
{
}

void QuestionTemplate::Init( TagList * tagList )
{
	// ���� ���ø� xml �ε�
	XmlDocument xmlDoc;
	std::string path = "./resource/QuestionTemplate.xml";
	xmlDoc.LoadFile( path.c_str() );

	// ��� xml ���� �ε�
	XmlDocument xmlDoc2;
	std::string path2 = "./resource/Result.xml";
	xmlDoc2.LoadFile( path2.c_str() );

	const XmlNode *resNode			= xmlDoc.GetNode( "resource");
	const XmlNode *resNodeOfResult	= xmlDoc2.GetNode( "resource");

	//
	TemplateList tempList;

	// QuestionTemplate xml������ 
	int nodeCount = resNode->GetNodeCount( "template" );
	for( int num = 0; num < nodeCount; num++ )
	{	
		// Question template node���� ����,
		const XmlNode * templateNode = resNode->GetNode( "template", num );
		std::string templateNum = templateNode->GetAttribute( "no" );
		//printf("no[%d] = %s\n", num, templateNum.c_str() );

		//
		Template temp;

		// ���� num��° Question template����,
		int slotCount = templateNode->GetNodeCount( "slot" );
		for( int num2 = 0; num2 < slotCount; num2++ )
		{	
			// slot�� ������ŭ ���� 
			// �Ӽ����� �о����,
			const XmlNode * slotNode = templateNode->GetNode( "slot", num2 );
			std::string slotType = slotNode->GetAttribute( "type" );
			std::string slotNeed = slotNode->GetAttribute( "need" );
			//printf("no[%d] = %s\t%s\n", num, slotType.c_str(), slotNeed.c_str() );

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

			// ���� ���� tag���� �����ͼ�,
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

	QuestionResultList questionResultList;

	// ��Ī�Ǵ� ���ø� ã��
	int count = tagList->size();
	for( int num = 0; num < count; num++ )
	{
		Tags & tags = (*tagList)[ num ];
		QuestionResult questionResult;
		CompareTagname( tempList, tags, questionResult );
		questionResultList.push_back( questionResult );
	}

	// ����
	SaveToXML( questionResultList );
}

void QuestionTemplate::Uninit()
{
}

void QuestionTemplate::SaveToXML( QuestionResultList & inQuestionResultList )
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

void QuestionTemplate::CompareTagname( TemplateList & tempList, Tags & tags, QuestionResult & outQuestionResult )
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
						matchSlot = true;
						resultSlot._word = tag._word;
						resultSlot._tagName = tag._name;
						resultSlot._wordPos = n;
						break;
					}
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

bool QuestionTemplate::IsWordInSlot( QuestionResult & questionResult, int n )
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

void QuestionTemplate::SelectTemplate( QuestionResultList & questionResultList, QuestionResult & outQuestionResult )
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