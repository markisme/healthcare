#pragma once

#include "SemanticTemplateProcessor.h"
#include "QueryGenerator.h"

// 매칭 결과 컨테이너
struct AnswerNeedSlot
{
	std::string _slotType;
	std::string _tagName;
	std::string _addText;
};

struct AnswerRule
{
	int _tempNo;
	int _subNo;
	std::vector<AnswerNeedSlot> _needSlotList;
	std::string _expression;
	std::vector<DataElement> _elementList;
};

typedef std::vector<AnswerRule> AnswerRuleList;

typedef std::vector<std::string> AnswerList;

class AnswerGenerator
{
public:
	AnswerGenerator();
	~AnswerGenerator();

	void Start( const ResultMatchedTemplate & resultMatchedTemplate, const DBResultList & dbResultList );

private:
	void LoadAnswerRule( AnswerRuleList & answerRuleList );
	void SaveAnswer( AnswerList & answerList );
	std::string GenerateAnswer( const MatchedTemplate & matchedTemplate, const DBResultList & dbResultList, AnswerRuleList & answerRuleList );
	std::string GetExpression( std::string expression, std::vector<std::string> valueList );
	std::string GetElement( DataElement element, const MatchedTemplate & matchedTemplate, const DBResultList & dbResultList );
	std::string GetTagName( std::string slotType, const MatchedTemplate & matchedTemplate );
	std::string GetWord( std::string slotType, const MatchedTemplate & matchedTemplate );
	std::string GetResultData( int questionNo, const DBResultList & dbResultList );
	std::string GetResultDataList( int questionNo, const DBResultList & dbResultList );
};