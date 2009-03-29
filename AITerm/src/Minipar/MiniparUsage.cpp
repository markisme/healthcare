//**********************************************************************************
//
// TITLE	: MiniparUsage.cpp
// DATE		: 2009/03/28
// AUTHOR	: Lee, Taewoo( ltw1372@lycos.co.kr )
// UPDATE	:
//
//**********************************************************************************
#include "stdafx.h"

void outputParsingResult(CSimpleMinipar Minipar)
{
	int i;
	CSimpleMiniparWord *MiniparWord;

	printf("< MiniPar Parsing result >\n");
	printf("wordCount= %d\n", Minipar.GetWordCount());		// 단어의 개수 출력
	
	for( i=0; i<Minipar.GetWordCount(); i++)				// 단어의 개수만큼 돌며
	{
		MiniparWord= Minipar.GetAt(i);						// 각 단어의 정보를 추출하여
		printf("%d %s/%s/%s\n", 
			MiniparWord->nLineNum,							// 몇번째 단어(1~ )
			MiniparWord->szWord,							// 단어 자체
			MiniparWord->szPOS,								// part of sentense(품사)
			MiniparWord->szDependence						// 의존관계
			);												// 를 출력
	}
}