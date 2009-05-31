#include "stdafx.h"
#include "QueryGenerator.h"
#include "DBDictionary.h"

QueryGenerator::QueryGenerator()
{
}

QueryGenerator::~QueryGenerator()
{
}

void QueryGenerator::Start( ResultMatchedTemplate & resultMatchedTemplate, DBDictionary * dbDic )
{
	// 디비 사전 셋
	_dbDic = dbDic;

	// 쿼리룰 로드
	QueryRuleList queryRuleList;
	LoadQueryRule( queryRuleList );

	int count = resultMatchedTemplate.size();
	for( int num = 0; num < count; num++ )
	{
		MatchedTemplate & matchedTemplate = resultMatchedTemplate[ num ];
		std::string result = GeneratorQuery( matchedTemplate, queryRuleList );
		_queryList.push_back( result );
	}

	// 쿼리 저장
	SaveQuery( _queryList );

	// 디비에 접속해서 쿼리 결과 얻어 오기
	//SaveResultQuery( _queryList );	
}

void QueryGenerator::LoadQueryRule( QueryRuleList & queryRuleList )
{
	// 질문 템플릿 xml 로드
	XmlDocument xmlDoc;
	std::string path = "./resource/SQLRule.xml";
	xmlDoc.LoadFile( path.c_str() );

	const XmlNode *resNode = xmlDoc.GetNode( "resource");

	//
	int nodeCount = resNode->GetNodeCount( "sql" );
	for( int num = 0; num < nodeCount; num++ )
	{	
		// Question template node들을 돌며,
		const XmlNode * sqlNode = resNode->GetNode( "sql", num );
		std::string templateNum = sqlNode->GetAttribute( "tempNo" );
		std::string subNum = sqlNode->GetAttribute( "no" );

		//
		QueryRule rule;
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

			//
			NeedSlot slot;
			slot._slotType = slotType;
			slot._tagName = tagName;

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

			if( expType == "$f.tb" )
			{
				rule._elementList.push_back(FOCUS_TABLE);
			}
			else if( expType == "$f.va" )
			{
				rule._elementList.push_back(FOCUS_TAG);
			}
			else if( expType == "$f.tx" )
			{
				rule._elementList.push_back(FOCUS_TEXT);
			}
			else if( expType == "$t.tb" )
			{
				rule._elementList.push_back(TARGET_TABLE);
			}
			else if( expType == "$t.va" )
			{
				rule._elementList.push_back(TARGET_TAG);
			}
			else if( expType == "$t.tx" )
			{
				rule._elementList.push_back(TARGET_TEXT);
			}
			else if( expType == "$o.tb" )
			{
				rule._elementList.push_back(OPTION_TABLE);
			}
			else if( expType == "$o.va" )
			{
				rule._elementList.push_back(OPTION_TAG);
			}
			else if( expType == "$o.tx" )
			{
				rule._elementList.push_back(OPTION_TEXT);
			}
			else if( expType == "$c1.tb" )
			{
				rule._elementList.push_back(COMPONENT1_TABLE);
			}
			else if( expType == "$c1.va" )
			{
				rule._elementList.push_back(COMPONENT1_TAG);
			}
			else if( expType == "$c1.tx" )
			{
				rule._elementList.push_back(COMPONENT1_TEXT);
			}
			else if( expType == "$c2.tb" )
			{
				rule._elementList.push_back(COMPONENT2_TABLE);
			}
			else if( expType == "$c2.va" )
			{
				rule._elementList.push_back(COMPONENT2_TAG);
			}
			else if( expType == "$c2.tx" )
			{
				rule._elementList.push_back(COMPONENT2_TEXT);
			}
		}

		queryRuleList.push_back( rule );
	}
}

void QueryGenerator::SaveQuery( QueryList & queryList )
{
	XmlDocument xmlDoc;
	XmlNode * resNode = xmlDoc.AddNode( "resource" );

	int count = queryList.size();
	for( int num = 0; num < count; num++ )
	{
		std::string sql = queryList[ num ];

		XmlNode * questionNode = resNode->AddNode( "question" );
		questionNode->SetText( sql.c_str(), XmlNode::NUMBER );

		char buf[8]; itoa(num, buf, 10);
		questionNode->SetAttribute( "no", buf );
	}

	std::string path = "./resource/ResultSQL.xml";
	xmlDoc.SaveFile( path.c_str() );
}

void QueryGenerator::SaveResultQuery( QueryList & queryList )
{
	// 디비에 쿼리 날려서 결과 얻어 오기
	int queryCount = queryList.size();
	for( int num = 0; num < queryCount; num++)
	{
		std::string sql = queryList[ num ];
		
		DataList dataList;
		DBConnector::GetInstance().MysqlQuery( sql.c_str(), dataList );

		printf( "Get DB Data : Question Number %d\n", num );
		_dbResultList.push_back( dataList );
	}

	// 결과 저장 하기
	XmlDocument xmlDoc;
	XmlNode * resNode = xmlDoc.AddNode( "resource" );

	int count = _dbResultList.size();
	for( int num = 0; num < count; num++ )
	{
		XmlNode * questionNode = resNode->AddNode( "question" );

		char buf[8]; itoa(num, buf, 10);
		questionNode->SetAttribute( "no", buf );

		DataList & dataList = _dbResultList[ num ];
		
		int dataCount = dataList.size();
		for( int cnt = 0; cnt < dataCount; cnt++ )
		{
			XmlNode * rowNode = questionNode->AddNode( "row" );

			RowData row = dataList[ cnt ];

			int rowCount = row._data.size();
			for( int k = 0; k < rowCount; k++ )
			{
				XmlNode * dataNode = rowNode->AddNode( "data" );

				std::string name = row._col[ k ];
				dataNode->SetAttribute( "name", name.c_str() );

				std::string data = row._data[ k ];
				dataNode->SetText( data.c_str(), XmlNode::NUMBER );
			}
		}
	}

	std::string path = "./resource/ResultDB.xml";
	xmlDoc.SaveFile( path.c_str() );
}

std::string QueryGenerator::GeneratorQuery( MatchedTemplate & matchedTemplate, QueryRuleList & queryRuleList )
{
	int count = queryRuleList.size();
	for( int num = 0; num < count; num++)
	{
		bool matched = false;
		QueryRule & queryRule = queryRuleList[ num ];
		if( matchedTemplate._tempNo == queryRule._tempNo )
		{
			// 니드 슬롯 리스트 확인
			// 매치 템플릿 선택
			int slotCount = queryRule._needSlotList.size();
			for( int cnt = 0; cnt < slotCount; cnt++ )
			{
				NeedSlot & slot = queryRule._needSlotList[ cnt ];

				int tempSlotCount = matchedTemplate._slotList.size();
				for( int k = 0; k < tempSlotCount; k++ )
				{
					MatchedSlot & matcdedSlot = matchedTemplate._slotList[k];
					if( matcdedSlot._slotType == slot._slotType && matcdedSlot._tagName == slot._tagName )
					{
						// 쿼리 매치
						matched = true;
						break;
					}
				}

				if( matched == true )
				{
					break;
				}
			}
		}

		// 매치가 될때까지 돌아라
		if( matched == false )
		{
			continue;
		}

		// 매치가 되었으면 진행
		std::vector<std::string> valueList;

		int elementCount = queryRule._elementList.size();
		for( int cnt = 0; cnt < elementCount; cnt++)
		{
			DataElement element = queryRule._elementList[ cnt ];
			std::string value = GetElement( element, matchedTemplate );
			valueList.push_back( value );
		}

		// 채운 리스트를 식에 대입
		return GetExpression( queryRule._expression, valueList );
	}

	return "";
}

std::string QueryGenerator::GetExpression( std::string expression, std::vector<std::string> valueList )
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

std::string QueryGenerator::GetElement( DataElement element, MatchedTemplate & matchedTemplate )
{
	switch( element )
	{
	case FOCUS_TABLE:
		return GetTableName( "focus", matchedTemplate );
	case FOCUS_TAG:
		return GetTagName( "focus", matchedTemplate );
	case FOCUS_TEXT:
		return GetWord( "focus", matchedTemplate );
	case TARGET_TABLE:
		return GetTableName( "target", matchedTemplate );
	case TARGET_TAG:
		return GetTagName( "target", matchedTemplate );
	case TARGET_TEXT:
		return GetWord( "target", matchedTemplate );
	case OPTION_TABLE:
		return GetTableName( "option", matchedTemplate );
	case OPTION_TAG:
		return GetTagName( "option", matchedTemplate );
	case OPTION_TEXT:
		return GetWord( "option", matchedTemplate );
	case COMPONENT1_TABLE:
		return GetTableName( "component1", matchedTemplate );
	case COMPONENT1_TAG:
		return GetTagName( "component1", matchedTemplate );
	case COMPONENT1_TEXT:
		return GetWord( "component1", matchedTemplate );
	case COMPONENT2_TABLE:
		return GetTableName( "component2", matchedTemplate );
	case COMPONENT2_TAG:
		return GetTagName( "component2", matchedTemplate );
	case COMPONENT2_TEXT:
		return GetWord( "component2", matchedTemplate );
	}
	return "%?%";
}

std::string QueryGenerator::GetTableName( std::string slotType, MatchedTemplate & matchedTemplate )
{
	std::string word = GetWord( slotType, matchedTemplate );
	std::string tableName = _dbDic->GetTableName( word );
	return tableName;
}

std::string QueryGenerator::GetTagName( std::string slotType, MatchedTemplate & matchedTemplate )
{
	int count = matchedTemplate._slotList.size();
	for( int num = 0; num < count; num++)
	{
		MatchedSlot & slot = matchedTemplate._slotList[ num ];
		if( slot._slotType == slotType )
		{
			return slot._tagName;
		}
	}

	return "";
}

std::string QueryGenerator::GetWord( std::string slotType, MatchedTemplate & matchedTemplate )
{
	int count = matchedTemplate._slotList.size();
	for( int num = 0; num < count; num++)
	{
		MatchedSlot & slot = matchedTemplate._slotList[ num ];
		if( slot._slotType == slotType )
		{
			return slot._word;
		}
	}

	return "";
}