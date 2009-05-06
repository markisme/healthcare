#pragma once
#include "NamedEntityRecognition.h"

struct TemplateSlot
{
	std::vector<std::string> _tagNameList;
	std::string _type;
	bool _need;
};

typedef std::vector<TemplateSlot> Template;
typedef std::vector<Template> TemplateList;

// 매칭 결과 컨테이너
struct MatchedSlot
{
	std::string _slotType;
	std::string _word;
	std::string _tagName;
	bool _need;
	int _wordPos;
};

struct MatchedTemplate
{
	std::vector<MatchedSlot> _slotList;
	int _tempNo;
};

typedef std::vector<MatchedTemplate> ResultMatchedTemplate;

class SemanticTemplateProcessor
{
public:
	SemanticTemplateProcessor();
	~SemanticTemplateProcessor();

	void Init( ResultNamedEntityRecognition * resultNamedEntityRecognition );
	void Uninit();

	void LoadTemplateList( TemplateList & tempList );
	void SaveResultSemanticTemplateProcess( ResultMatchedTemplate & inResultMatchedTemplate );
	ResultMatchedTemplate & GetResultMatchedTemplate() { return _resultMatchedTemplate; }

private:
	bool IsMatchWord( NamedEntityList & namedEntityList, std::string & tagName, MatchedTemplate & matchedTemplate, MatchedSlot & outMatchedSlot );
	bool IsWordInSlot( MatchedTemplate & matchedTemplate, int n );
	void CompareTagname( TemplateList & tempList, NamedEntityList & namedEntityList, MatchedTemplate & outMatchedTemplate );
	void SelectTemplate( ResultMatchedTemplate & resultMatchedTemplate, MatchedTemplate & outMatchedTemplate );

private:
	ResultMatchedTemplate _resultMatchedTemplate;
};