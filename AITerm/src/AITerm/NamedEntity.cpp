#include "stdafx.h"
#include "NamedEntity.h"

NamedEntity::NamedEntity()
{
}

NamedEntity::~NamedEntity()
{
}

void NamedEntity::Init()
{
	// 사전 초기화
	_dbDic.Init();
	_wnDic.Init();
}

void NamedEntity::Uninit()
{
	_dbDic.Uninit();
	_wnDic.Uninit();
}

void NamedEntity::ProcessQuestion()
{
	// 질문 로드
	QuestionList qsList;
	LoadQuestions( qsList );

	// 질문 분석
	TagList tagList;

	int size = qsList.size();
	for( int num = 0; num < size; num++ )
	{
		std::string question = qsList[ num ];
		printf( "Process Question %d: %s\n", num, question.c_str() );

		Tags tags;
		GenerateTag( question, tags );
		tagList.push_back( tags );
	}

	// 결과 저장
	SaveResult( tagList );
}

void NamedEntity::LoadQuestions( QuestionList & qsList )
{
	XmlDocument xmlDoc;
	std::string path = "./resource/Questions.xml";
	xmlDoc.LoadFile( path.c_str() );

	const XmlNode * resNode = xmlDoc.GetNode( "resource" );

	int qsCount = resNode->GetNodeCount( "question" );
	for( int num = 0; num < qsCount; num++ )
	{
		const XmlNode * questionNode = resNode->GetNode( "question", num );
		std::string question = questionNode->GetText();
		qsList.push_back( question );		
	}
}

void NamedEntity::SaveResult( TagList & tagList )
{
	XmlDocument xmlDoc;
	XmlNode * resNode = xmlDoc.AddNode( "resource" );

	int tagsCount = tagList.size();
	for( int num = 0; num < tagsCount; num++ )
	{
		Tags & tags = tagList[ num ];
		
		char no[8];
		itoa( num, no, 10 );

		XmlNode * questionNode = resNode->AddNode( "question" );
		questionNode->SetAttribute( "no", no );

		int size = tags.size();
		for( int cnt = 0; cnt < size; cnt++ )
		{
			TagElement tag = tags[ cnt ];
			std::string currentPos = tag._currentPos;
			std::string tagName = tag._tagName;
			std::string type = tag._type;
			std::string dependencePos = tag._dependencePos;
			std::string word = tag._word;

			XmlNode * tagNode = questionNode->AddNode( "tag" );
			tagNode->SetAttribute( "type", tagName.c_str() );
			tagNode->SetAttribute( "curpos", currentPos.c_str() );
			tagNode->SetAttribute( "deppos", dependencePos.c_str() );
			tagNode->SetAttribute( "part", type.c_str() );
			tagNode->SetText( word.c_str(), XmlNode::NUMBER );
		}
	}	

	std::string path = "./resource/Result.xml";
	xmlDoc.SaveFile( path.c_str() );
}

void NamedEntity::GenerateTag( std::string question, Tags & tags )
{
	if( !_Minipar.Parse((char*)question.c_str()))
	{
		perror("parsing failure\n");
		return;
	}

	int count = _Minipar.GetWordCount();
	for( int num = 0; num < count; num++ )
	{
		char no[8];
		itoa( _Minipar.GetAt( num )->nLineNum, no, 10 );

		std::string word = ToLowerCase( _Minipar.GetAt( num )->szWord );

		TagElement tag;
		tag._currentPos = no;
		tag._type = _Minipar.GetAt( num )->szPOS;
		tag._dependencePos = _Minipar.GetAt( num )->szDependence;
		tag._word = word;

		tag._tagName = _dbDic.GetTagName( tag._word );
		if( tag._tagName.length() != 0 )
		{
			tags.push_back( tag );
			continue;
		}

		tag._tagName = _wnDic.GetTagName( tag._word );
		if( tag._tagName.length() != 0 )
		{
			tags.push_back( tag );
			continue;
		}

		// 명사에 뜻이 없으면 형용사 찾아서 붙여 봄
		std::string adjWord = GetAdjective( _Minipar.GetAt( num )->nLineNum );
		tag._word = adjWord + " " + word;
		
		tag._tagName = _wnDic.GetTagName( tag._word  );
		if( tag._tagName.length() != 0 )
		{
			tags.push_back( tag );
			continue;
		}

		// 형용사는 뜻이 없는 경우 명사에 붙임
		if( tag._type != "A" )
		{
			tag._tagName = "N/A";
			tags.push_back( tag );
		}
	}
}

std::string NamedEntity::GetAdjective( int curPos )
{
	char pos[8];
	itoa( curPos, pos, 10 );

	std::string posStr = pos;

	int count = _Minipar.GetWordCount();
	for( int num = 0; num < count; num++ )
	{
		std::string part = _Minipar.GetAt( num )->szPOS;
		std::string depPos = _Minipar.GetAt( num )->szDependence;
		if( part == "A" && depPos == posStr )
		{
			 return ToLowerCase( _Minipar.GetAt( num )->szWord );
		}
	}

	return "";
}