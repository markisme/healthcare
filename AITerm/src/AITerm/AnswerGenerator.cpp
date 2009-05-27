#include "stdafx.h"
#include "AnswerGenerator.h"
#include "DBDictionary.h"

AnswerGenerator::AnswerGenerator()
{
}

AnswerGenerator::~AnswerGenerator()
{
}

void AnswerGenerator::Start( const ResultMatchedTemplate & resultMatchedTemplate, const DBResultList & dbResultList )
{
	// 쿼리룰 로드
	AnswerRuleList answerRuleList;
	LoadAnswerRule( answerRuleList );

	// 쿼리 리스트 만들기
	AnswerList answerList;

	int count = resultMatchedTemplate.size();
	for( int num = 0; num < count; num++ )
	{
		const MatchedTemplate & matchedTemplate = resultMatchedTemplate[ num ];
		std::string result = GenerateAnswer( matchedTemplate, dbResultList, answerRuleList );
		answerList.push_back( result );
	}

	// 쿼리 저장
	SaveAnswer( answerList );
}

void AnswerGenerator::LoadAnswerRule( AnswerRuleList & answerRuleList )
{
	// 질문 템플릿 xml 로드
	XmlDocument xmlDoc;
	std::string path = "./resource/AnswerRule.xml";
	xmlDoc.LoadFile( path.c_str() );

	const XmlNode *resNode = xmlDoc.GetNode( "resource");

	//
	int nodeCount = resNode->GetNodeCount( "ans" );
	for( int num = 0; num < nodeCount; num++ )
	{	
		// Question template node들을 돌며,
		const XmlNode * sqlNode = resNode->GetNode( "ans", num );
		std::string templateNum = sqlNode->GetAttribute( "tempNo" );
		std::string subNum = sqlNode->GetAttribute( "no" );

		//
		AnswerRule rule;
		rule._tempNo = atoi( templateNum.c_str() );
		rule._subNo = atoi( subNum.c_str() );

		const XmlNode * needListNode = sqlNode->GetNode( "need_slot_list" );

		// 만약 num번째 Question template에서,
		int slotCount = needListNode->GetNodeCount( "slot" );
		for( int num2 = 0; num2 < slotCount; num2++ )
		{	
			// slot의 개수만큼 돌며 
			// 속성들을 읽어오고,
			const XmlNode * slotNode = needListNode->GetNode( "slot", num2 );
			std::string slotType = slotNode->GetAttribute( "type" );
			std::string tagName = slotNode->GetAttribute( "value" );
			std::string addText = slotNode->GetAttribute( "add" );

			//
			AnswerNeedSlot slot;
			slot._slotType = slotType;
			slot._tagName = tagName;
			slot._addText = addText;

			rule._needSlotList.push_back( slot );
		}

		const XmlNode * expressionNode = sqlNode->GetNode( "expression" );
		std::string expression = expressionNode->GetAttribute( "type" );
		rule._expression = expression;

		int expCount = expressionNode->GetNodeCount( "exp" );
		for( int num2 = 0; num2 < expCount; num2++ )
		{	
			//
			const XmlNode * expNode = expressionNode->GetNode( "exp", num2 );
			std::string expType = expNode->GetAttribute( "type" );

			if( expType == "$f.va" )
			{
				rule._elementList.push_back(FOCUS_TAG);
			}
			else if( expType == "$f.tx" )
			{
				rule._elementList.push_back(FOCUS_TEXT);
			}
			else if( expType == "$t.va" )
			{
				rule._elementList.push_back(TARGET_TAG);
			}
			else if( expType == "$t.tx" )
			{
				rule._elementList.push_back(TARGET_TEXT);
			}
			else if( expType == "$c1.va" )
			{
				rule._elementList.push_back(COMPONENT1_TAG);
			}
			else if( expType == "$c1.tx" )
			{
				rule._elementList.push_back(COMPONENT1_TEXT);
			}
			else if( expType == "$c2.va" )
			{
				rule._elementList.push_back(COMPONENT2_TAG);
			}
			else if( expType == "$c2.tx" )
			{
				rule._elementList.push_back(COMPONENT2_TEXT);
			}
			else if( expType == "$r.va" )
			{
				rule._elementList.push_back(RESULT_VALUE);
			}
			else if( expType == "$r.li" )
			{
				rule._elementList.push_back(RESULT_LIST);
			}
		}

		answerRuleList.push_back( rule );
	}
}

void AnswerGenerator::SaveAnswer( AnswerList & answerList )
{
	XmlDocument xmlDoc;
	XmlNode * resNode = xmlDoc.AddNode( "resource" );

	int count = answerList.size();
	for( int num = 0; num < count; num++ )
	{
		std::string ans = answerList[ num ];

		XmlNode * answerNode = resNode->AddNode( "answer" );
		answerNode->SetText( ans.c_str(), XmlNode::NUMBER );

		char buf[8]; itoa(num, buf, 10);
		answerNode->SetAttribute( "no", buf );
	}

	std::string path = "./resource/ResultAnswer.xml";
	xmlDoc.SaveFile( path.c_str() );
}

std::string AnswerGenerator::GenerateAnswer( const MatchedTemplate & matchedTemplate, const DBResultList & dbResultList, AnswerRuleList & answerRuleList )
{
	int count = answerRuleList.size();
	for( int num = 0; num < count; num++)
	{
		bool matched = false;
		AnswerNeedSlot matchedSlot;
		AnswerRule & answerRule = answerRuleList[ num ];
		if( matchedTemplate._tempNo == answerRule._tempNo )
		{
			// 니드 슬롯 리스트 확인
			// 매치 템플릿 선택
			int slotCount = answerRule._needSlotList.size();
			for( int cnt = 0; cnt < slotCount; cnt++ )
			{
				AnswerNeedSlot & slot = answerRule._needSlotList[ cnt ];

				int tempSlotCount = matchedTemplate._slotList.size();
				for( int k = 0; k < tempSlotCount; k++ )
				{
					const MatchedSlot & matcdedSlot = matchedTemplate._slotList[k];
					if( matcdedSlot._slotType == slot._slotType && matcdedSlot._tagName == slot._tagName )
					{
						// 응답 매치
						matched = true;
						matchedSlot = slot;
						break;
					}
				}

				if( matched == true )
				{
					break;
				}
			}

			if( slotCount == 0 )
			{
				// 응답 매치
				matched = true;
			}
		}

		// 매치가 될때까지 돌아라
		if( matched == false )
		{
			continue;
		}

		// 매치가 되었으면 진행
		std::vector<std::string> valueList;

		int elementCount = answerRule._elementList.size();
		for( int cnt = 0; cnt < elementCount; cnt++)
		{
			DataElement element = answerRule._elementList[ cnt ];
			std::string value = GetElement( element, matchedTemplate, dbResultList ) + GetAddText( element, matchedSlot );
			valueList.push_back( value );
		}

		// 채운 리스트를 식에 대입
		return GetExpression( answerRule._expression, valueList );
	}

	return "";
}

std::string AnswerGenerator::GetExpression( std::string expression, std::vector<std::string> valueList )
{
	//
	std::string exp = expression;

	//
	int count = valueList.size();
	for( int num = 0; num < count; num++)
	{
		std::string str = valueList[ num ];
		int index = exp.find("%s");
		if( index < 0 )
		{
			break;
		}
		exp.erase(index,2);
		exp.insert(index,str);
	}

	return exp;
}

std::string AnswerGenerator::GetElement( DataElement element, const MatchedTemplate & matchedTemplate, const DBResultList & dbResultList )
{
	switch( element )
	{
	case FOCUS_TAG:
		return GetTagName( "focus", matchedTemplate );
	case FOCUS_TEXT:
		return GetWord( "focus", matchedTemplate );
	case TARGET_TAG:
		return GetTagName( "target", matchedTemplate );
	case TARGET_TEXT:
		return GetWord( "target", matchedTemplate );
	case COMPONENT1_TAG:
		return GetTagName( "component1", matchedTemplate );
	case COMPONENT1_TEXT:
		return GetWord( "component1", matchedTemplate );
	case COMPONENT2_TAG:
		return GetTagName( "component2", matchedTemplate );
	case COMPONENT2_TEXT:
		return GetWord( "component2", matchedTemplate );
	case RESULT_VALUE:
		return GetResultData( matchedTemplate._questionNo, dbResultList );
	case RESULT_LIST:
		return GetResultDataList( matchedTemplate._questionNo, dbResultList );
	}
	return "%?%";
}

std::string AnswerGenerator::GetAddText( DataElement element, AnswerNeedSlot & matchedSlot )
{
	if( ( matchedSlot._slotType == "focus" && ( element == FOCUS_TAG || element == FOCUS_TEXT ) ) ||
		( matchedSlot._slotType == "target" && ( element == TARGET_TAG || element == TARGET_TEXT ) ) ||
		( matchedSlot._slotType == "component1" && ( element == COMPONENT1_TAG || element == COMPONENT1_TEXT ) ) ||
		( matchedSlot._slotType == "component2" && ( element == COMPONENT2_TAG || element == COMPONENT2_TEXT ) ) )
	{
		return " " + matchedSlot._addText;
	}

	return "";
}

std::string AnswerGenerator::GetTagName( std::string slotType, const MatchedTemplate & matchedTemplate )
{
	int count = matchedTemplate._slotList.size();
	for( int num = 0; num < count; num++)
	{
		const MatchedSlot & slot = matchedTemplate._slotList[ num ];
		if( slot._slotType == slotType )
		{
			return slot._tagName;
		}
	}

	return "";
}

std::string AnswerGenerator::GetWord( std::string slotType, const MatchedTemplate & matchedTemplate )
{
	int count = matchedTemplate._slotList.size();
	for( int num = 0; num < count; num++)
	{
		const MatchedSlot & slot = matchedTemplate._slotList[ num ];
		if( slot._slotType == slotType )
		{
			return slot._word;
		}
	}

	return "";
}

std::string AnswerGenerator::GetResultData( int questionNo, const DBResultList & dbResultList )
{
	std::string ret;

	int resSize = dbResultList.size();
	if(  resSize < questionNo )
	{
		return "%?%";
	}

	const DataList & dataList = dbResultList[ questionNo ];
	//if( dataList.size() < 1 )
	//{
	//	return "%?%";
	//}

	int count = dataList.size();
	for( int num = 0; num < count; num++)
	{
		const RowData & rowData = dataList[ num ];
		
		int dataSize = rowData._data.size();
		for( int cnt = 0; cnt < dataSize; cnt++ )
		{
			std::string data = rowData._data[ cnt ];
			ret += data + " "; 
		}
	}

	return ret;
}

std::string AnswerGenerator::GetResultDataList( int questionNo, const DBResultList & dbResultList )
{
	std::string ret;

	int resSize = dbResultList.size();
	if(  resSize < questionNo )
	{
		return "%?%";
	}

	const DataList & dataList = dbResultList[ questionNo ];
	//if( dataList.size() < 2 )
	//{
	//	return "%?%";
	//}

	int count = dataList.size();
	for( int num = 0; num < count; num++)
	{
		const RowData & rowData = dataList[ num ];
		
		int dataSize = rowData._data.size();
		for( int cnt = 0; cnt < dataSize; cnt++ )
		{
			std::string data = rowData._data[ cnt ];
			ret += data + " ";
		}
	}

	return ret;
}