#include "stdafx.h"
#include "NamedEntityRecognition.h"

NamedEntityRecognition::NamedEntityRecognition()
{
}

NamedEntityRecognition::~NamedEntityRecognition()
{
}

void NamedEntityRecognition::Init()
{
	// 사전 초기화
	_dbDic.Init();
	_wnDic.Init();
}

void NamedEntityRecognition::Uninit()
{
	// 사전 해제
	_dbDic.Uninit();
	_wnDic.Uninit();
}

void NamedEntityRecognition::ProcessQuestion( ResultNamedEntityRecognition & resultNamedEntityRecognition )
{
	// 질문 로드
	//QuestionList qsList;
	//LoadQuestions( "./resource/Questions.xml" );

	// 질문 분석
	int size = _qsList.size();
	for( int num = 0; num < size; num++ )
	{
		std::string question = _qsList[ num ];
		printf( "Process Question %d: %s\n", num, question.c_str() );

		NamedEntityList namedEntityList;
		GenerateTag( question, namedEntityList );
		resultNamedEntityRecognition.push_back( namedEntityList );
	}

	// 결과 저장
	SaveResultNamedEntityRecognition( resultNamedEntityRecognition );
}

void NamedEntityRecognition::LoadQuestions( std::string path )
{
	_qsList.clear();

	XmlDocument xmlDoc;
	xmlDoc.LoadFile( path.c_str() );

	const XmlNode * resNode = xmlDoc.GetNode( "resource" );

	int qsCount = resNode->GetNodeCount( "question" );
	for( int num = 0; num < qsCount; num++ )
	{
		const XmlNode * questionNode = resNode->GetNode( "question", num );
		std::string question = questionNode->GetText();
		_qsList.push_back( question );		
	}
}

void NamedEntityRecognition::SaveResultNamedEntityRecognition( ResultNamedEntityRecognition & resultNamedEntityRecognition )
{
	XmlDocument xmlDoc;
	XmlNode * resNode = xmlDoc.AddNode( "resource" );

	int tagsCount = resultNamedEntityRecognition.size();
	for( int num = 0; num < tagsCount; num++ )
	{
		NamedEntityList & namedEntityList = resultNamedEntityRecognition[ num ];
		
		char no[8];
		itoa( num, no, 10 );

		XmlNode * questionNode = resNode->AddNode( "question" );
		questionNode->SetAttribute( "no", no );

		int size = namedEntityList.size();
		for( int cnt = 0; cnt < size; cnt++ )
		{
			NamedEntityElement namedEntityElement = namedEntityList[ cnt ];
			std::string currentPos = namedEntityElement._currentPos;
			std::string tagName = namedEntityElement._name;
			std::string part = namedEntityElement._part;
			std::string dependencePos = namedEntityElement._dependencePos;
			std::string word = namedEntityElement._word;

			XmlNode * tagNode = questionNode->AddNode( "tag" );
			tagNode->SetAttribute( "name", tagName.c_str() );
			tagNode->SetAttribute( "curpos", currentPos.c_str() );
			tagNode->SetAttribute( "deppos", dependencePos.c_str() );
			tagNode->SetAttribute( "part", part.c_str() );
			tagNode->SetText( word.c_str(), XmlNode::NUMBER );
		}
	}	

	std::string path = "./resource/ResultNamedEntityRecognition.xml";
	xmlDoc.SaveFile( path.c_str() );
}

void NamedEntityRecognition::GenerateTag( std::string question, NamedEntityList & namedEntityList )
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

		NamedEntityElement namedEntityElement;
		namedEntityElement._currentPos = no;
		namedEntityElement._part = _Minipar.GetAt( num )->szPOS;
		namedEntityElement._dependencePos = _Minipar.GetAt( num )->szDependence;
		namedEntityElement._word = word;

		// 자신의 뒤 단어들을 리스트로 조합
		std::vector<std::string> words;
		for( int cnt = num; cnt < count; cnt++)
		{
			std::string word = ToLowerCase( _Minipar.GetAt( cnt )->szWord );
			words.push_back( word );
		}

		//namedEntityElement._name = _dbDic.GetTagName( word );
		int count = _dbDic.GetTagName( words, namedEntityElement._name, namedEntityElement._word );
		if( namedEntityElement._name.length() != 0 )
		{
			namedEntityList.push_back( namedEntityElement );
			num += count;
			continue;
		}

		count = _wnDic.GetTagName( words, namedEntityElement._name, namedEntityElement._word );
		if( namedEntityElement._name.length() != 0 )
		{
			namedEntityList.push_back( namedEntityElement );
			num += count;
			continue;
		}

		namedEntityElement._name = "N/A";
		namedEntityList.push_back( namedEntityElement );

		//// 명사에 뜻이 없으면 형용사 찾아서 붙여 봄
		//std::string adjWord = GetAdjective( _Minipar.GetAt( num )->nLineNum );
		//tag._word = adjWord + " " + word;
		//
		//tag._name = _wnDic.GetTagName( tag._word  );
		//if( tag._name.length() != 0 )
		//{
		//	tags.push_back( tag );
		//	continue;
		//}

		//// 형용사는 뜻이 없는 경우 명사에 붙임
		//if( tag._part != "A" )
		//{
		//	tag._name = "N/A";
		//	tags.push_back( tag );
		//}
	}
}

std::string NamedEntityRecognition::GetAdjective( int curPos )
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