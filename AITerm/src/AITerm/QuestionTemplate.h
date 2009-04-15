#include "NamedEntity.h"

struct TemplateSlot
{
	std::vector<std::string> _tagNameList;
	std::string _type;
	bool _need;
};

typedef std::vector<TemplateSlot> Template;
typedef std::vector<Template> TemplateList;

struct ResultSlot
{
	std::string _slotType;
	std::string _word;
	std::string _tagName;
	bool _need;
	int _wordPos;
};

typedef std::vector<ResultSlot> SlotList;

struct QuestionResult
{
	SlotList _slotList;
	int _tempNo;
};

typedef std::vector<QuestionResult> QuestionResultList;

class QuestionTemplate
{
public:
	QuestionTemplate();
	~QuestionTemplate();

	void Init( TagList * tagList );
	void Uninit();

	void SaveToXML( QuestionResultList & inQuestionResultList );

private:
	bool IsWordInSlot( QuestionResult & questionResult, int n );
	void CompareTagname( TemplateList & tempList, Tags & tags, QuestionResult & outQuestionResult );
	void SelectTemplate( QuestionResultList & questionResultList, QuestionResult & outQuestionResult );

private:
};