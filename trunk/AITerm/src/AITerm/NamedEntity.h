#pragma once

#include "DBDictionary.h"
#include "WNDictionary.h"

struct TagElement
{
	std::string _name;
	std::string _part;
	std::string _currentPos;
	std::string _dependencePos;
	std::string _word;
};

typedef std::vector<TagElement> Tags;
typedef std::vector<Tags> TagList;

typedef std::vector<std::string> QuestionList;

class NamedEntity
{
public:
	NamedEntity();
	~NamedEntity();

	void Init();
	void Uninit();

	void ProcessQuestion();

private:
	void LoadQuestions( QuestionList & qsList );
	void SaveResult( TagList & tagList );
	void GenerateTag( std::string question, Tags & tags );
	std::string GetAdjective( int curPos );

private:
	CSimpleMinipar _Minipar;
	DBDictionary _dbDic;
	WNDictionary _wnDic;
};