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

struct Expression
{
	std::string _expression;
	std::vector<DataElement> _elementList;
};

struct AnswerRule
{
	int _tempNo;
	int _subNo;
	std::vector<AnswerNeedSlot> _needSlotList;
	Expression _reference;
	Expression _answer;
};

typedef std::vector<AnswerRule> AnswerRuleList;

struct AnswerData
{
	std::string _reference;
	std::string _answer;
};

typedef std::vector<AnswerData> AnswerList;

class AnswerGenerator
{
public:
	AnswerGenerator();
	~AnswerGenerator();

	void Start( const ResultMatchedTemplate & resultMatchedTemplate, const DBResultList & dbResultList );

private:
	void LoadAnswerRule( AnswerRuleList & answerRuleList );
	void SaveAnswer( AnswerList & answerList );
	AnswerData GenerateAnswer( const MatchedTemplate & matchedTemplate, const DBResultList & dbResultList, AnswerRuleList & answerRuleList );
	std::string GetExpression( std::string expression, std::vector<std::string> valueList );
	DataElement GetElement( std::string expType );
	std::string GetElement( DataElement element, const MatchedTemplate & matchedTemplate, const DBResultList & dbResultList );
	std::string GetAddText( DataElement element, AnswerNeedSlot & matchedSlot );
	std::string GetTagName( std::string slotType, const MatchedTemplate & matchedTemplate );
	std::string GetWord( std::string slotType, const MatchedTemplate & matchedTemplate );
	std::string GetResultData( int questionNo, const DBResultList & dbResultList );
	std::string GetResultDataList( int questionNo, const DBResultList & dbResultList );
};