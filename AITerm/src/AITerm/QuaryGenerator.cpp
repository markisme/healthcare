#include "stdafx.h"
#include "QuaryGenerator.h"

QuaryGenerator::QuaryGenerator()
{
}

QuaryGenerator::~QuaryGenerator()
{
}

void QuaryGenerator::Init( ResultMatchedTemplate & resultMatchedTemplate )
{
	QuaryRuleList quaryRuleList;
	LoadQuaryRule( quaryRuleList );

	int count = resultMatchedTemplate.size();
	for( int num = 0; num < count; num++ )
	{
		MatchedTemplate & matchedTemplate = resultMatchedTemplate[ num ];
		GeneratorQuary( matchedTemplate, quaryRuleList );
	}
}

void QuaryGenerator::Uninit()
{
}

void QuaryGenerator::LoadQuaryRule( QuaryRuleList & quaryRuleList )
{
	// ���� ���ø� xml �ε�
	XmlDocument xmlDoc;
	std::string path = "./resource/SQLRule.xml";
	xmlDoc.LoadFile( path.c_str() );

	const XmlNode *resNode = xmlDoc.GetNode( "resource");

	//
	int nodeCount = resNode->GetNodeCount( "sql" );
	for( int num = 0; num < nodeCount; num++ )
	{	
		// Question template node���� ����,
		const XmlNode * sqlNode = resNode->GetNode( "sql", num );
		std::string templateNum = sqlNode->GetAttribute( "tempNo" );
		std::string subNum = sqlNode->GetAttribute( "no" );

		//
		QuaryRule rule;
		rule._tempNo = atoi( templateNum.c_str() );
		rule._subNo = atoi( subNum.c_str() );

		const XmlNode * needListNode = sqlNode->GetNode( "need_slot_list" );

		// ���� num��° Question template����,
		int slotCount = needListNode->GetNodeCount( "slot" );
		for( int num2 = 0; num2 < slotCount; num2++ )
		{	
			// slot�� ������ŭ ���� 
			// �Ӽ����� �о����,
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

		quaryRuleList.push_back( rule );
	}
}

void QuaryGenerator::GeneratorQuary( MatchedTemplate & matchedTemplate, QuaryRuleList & quaryRuleList )
{
	int count = quaryRuleList.size();
	for( int num = 0; num < count; num++)
	{
		bool matched = false;
		QuaryRule & quaryRule = quaryRuleList[ num ];
		if( matchedTemplate._tempNo == quaryRule._tempNo )
		{
			// �ϵ� ���� ����Ʈ Ȯ��
			// ��ġ ���ø� ����
			int slotCount = quaryRule._needSlotList.size();
			for( int cnt = 0; cnt < slotCount; cnt++ )
			{
				NeedSlot & slot = quaryRule._needSlotList[ cnt ];

				int tempSlotCount = matchedTemplate._slotList.size();
				for( int k = 0; k < tempSlotCount; k++ )
				{
					MatchedSlot & matcdedSlot = matchedTemplate._slotList[k];
					if( matcdedSlot._slotType == slot._slotType && matcdedSlot._tagName == slot._tagName )
					{
						// ���� ��ġ
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

		// ��ġ�� �ɶ����� ���ƶ�
		if( matched == false )
		{
			continue;
		}

		// ��ġ�� �Ǿ����� ����
		std::vector<std::string> valueList;

		int elementCount = quaryRule._elementList.size();
		for( int cnt = 0; cnt < elementCount; cnt++)
		{
			DataElement element = quaryRule._elementList[ cnt ];
			// �ٷ���Ʈ ���� ����Ʈ ä���
			switch( element )
			{
			case FOCUS_TABLE:
				break;
			case FOCUS_TAG:
				break;
			case FOCUS_TEXT:
				break;
			case TARGET_TABLE:
				break;
			case TARGET_TAG:
				break;
			case TARGET_TEXT:
				break;
			case OPTION_TABLE:
				break;
			case OPTION_TAG:
				break;
			case OPTION_TEXT:
				break;
			case COMPONENT1_TABLE:
				break;
			case COMPONENT1_TAG:
				break;
			case COMPONENT1_TEXT:
				break;
			case COMPONENT2_TABLE:
				break;
			case COMPONENT2_TAG:
				break;
			case COMPONENT2_TEXT:
				break;
			}
		}
		
		valueList.push_back("test2");
		valueList.push_back("test3");
		valueList.push_back("test4");

		// ä�� ����Ʈ�� �Ŀ� ����
		SetExpression( quaryRule._expression, valueList );

		if( matched == true )
		{
			break;
		}
	}
}

void QuaryGenerator::SetExpression( std::string expression, std::vector<std::string> valueList )
{
	int count = valueList.size();
	for( int num = 0; num < count; num++)
	{
		std::string str = valueList[ num ];
		int index = expression.find("%s");
		expression.erase(index,2);
		expression.insert(index,str);
	}	
}