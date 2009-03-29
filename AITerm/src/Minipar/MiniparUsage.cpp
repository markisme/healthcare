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
	printf("wordCount= %d\n", Minipar.GetWordCount());		// �ܾ��� ���� ���
	
	for( i=0; i<Minipar.GetWordCount(); i++)				// �ܾ��� ������ŭ ����
	{
		MiniparWord= Minipar.GetAt(i);						// �� �ܾ��� ������ �����Ͽ�
		printf("%d %s/%s/%s\n", 
			MiniparWord->nLineNum,							// ���° �ܾ�(1~ )
			MiniparWord->szWord,							// �ܾ� ��ü
			MiniparWord->szPOS,								// part of sentense(ǰ��)
			MiniparWord->szDependence						// ��������
			);												// �� ���
	}
}