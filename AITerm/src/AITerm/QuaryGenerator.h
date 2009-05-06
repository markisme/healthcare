#pragma once

#include "SemanticTemplateProcessor.h"

// 매칭 결과 컨테이너
struct NeedSlot
{
	std::string _slotType;
	std::string _tagName;
};

enum DataElement
{
	FOCUS_TABLE = 0,
	FOCUS_TAG,
	FOCUS_TEXT,
	TARGET_TABLE,
	TARGET_TAG,
	TARGET_TEXT,
	OPTION_TABLE,
	OPTION_TAG,
	OPTION_TEXT,
	COMPONENT1_TABLE,
	COMPONENT1_TAG,
	COMPONENT1_TEXT,
	COMPONENT2_TABLE,
	COMPONENT2_TAG,
	COMPONENT2_TEXT
};

struct QuaryRule
{
	int _tempNo;
	int _subNo;
	std::vector<NeedSlot> _needSlotList;
	std::string _expression;
	std::vector<DataElement> _elementList;
};

typedef std::vector<QuaryRule> QuaryRuleList;

typedef std::vector<std::string> QuaryList;

class QuaryGenerator
{
public:
	QuaryGenerator();
	~QuaryGenerator();

	void Init( ResultMatchedTemplate & resultMatchedTemplate );
	void Uninit();

private:
	void LoadQuaryRule( QuaryRuleList & quaryRuleList );
	void GeneratorQuary( MatchedTemplate & matchedTemplate, QuaryRuleList & quaryRuleList );
	void SetExpression( std::string expression, std::vector<std::string> valueList );

private:
};