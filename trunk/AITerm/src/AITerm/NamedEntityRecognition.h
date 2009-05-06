#pragma once

#include "DBDictionary.h"
#include "WNDictionary.h"

// 개체명 인식을 위한 컨네이너들
struct NamedEntityElement
{
	std::string _name;
	std::string _part;
	std::string _currentPos;
	std::string _dependencePos;
	std::string _word;
};

typedef std::vector<NamedEntityElement> NamedEntityList;
typedef std::vector<NamedEntityList> ResultNamedEntityRecognition;

// 질문 리스트
typedef std::vector<std::string> QuestionList;

//
class NamedEntityRecognition
{
public:
	NamedEntityRecognition();
	~NamedEntityRecognition();

	void Init();
	void Uninit();

	void ProcessQuestion( ResultNamedEntityRecognition & resultNamedEntityRecognition );

	DBDictionary * GetDBDic() { return &_dbDic;}
	WNDictionary * GetWNDic() { return &_wnDic;}

private:
	void LoadQuestions( QuestionList & qsList );
	void SaveResultNamedEntityRecognition( ResultNamedEntityRecognition & resultNamedEntityRecognition );
	void GenerateTag( std::string question, NamedEntityList & namedEntityList );
	std::string GetAdjective( int curPos );

private:
	CSimpleMinipar _Minipar;
	DBDictionary _dbDic;
	WNDictionary _wnDic;
};