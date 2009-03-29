// SimpleMinipar.h: interface for the CSimpleMinipar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined _SimpleMinipar_H_
#define _SimpleMinipar_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE					1
#define FALSE					0
#define MAX_WORD_SIZE			64
#define MAX_SMALL_WORD_SIZE		16

// ���� !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Minipar.EXE�� ���� ���丮���� ����Ǿ�� �մϴ�.

class CSimpleMiniparWord
{
	public:
	int nLineNum;							// �ܾ� ��ȣ
	char szWord[MAX_WORD_SIZE];				// �ܾ�
	char szPOS[MAX_SMALL_WORD_SIZE];		// ǰ��
	char szDependence[MAX_SMALL_WORD_SIZE];	// ��������
};

class CSimpleMinipar  
{
public:

	CSimpleMinipar();

	int nSimpleMiniparWord;
	CSimpleMiniparWord SimpleMiniparWord[64];

	void Init(void);
	void Add(int nLineNum, char *pstrWord, char *pstrPOS, char *pstrDependence);
	void Add(char *pstrLine);


/* ����ϰ� �� �Լ��� */

	// ������ parameter�� �Ͽ� �Ľ��� �����ϴ� �Լ�
	bool Parse(char *pSentence);

	// �Ľ̵� ����� �ܾ��� ������ ����� ��ȯ�ϴ� �Լ�
	int GetWordCount(void);

	// �Ľ̵� ������� �־���  �ε����� �ܾ��� ǰ��, �������踦 ��ȯ�ϴ� �Լ�
	CSimpleMiniparWord* GetAt(int nIdx);

};

#endif // !defined(AFX_SIMPLEMINIPAR_H__3A9985AC_8D6C_4D3D_868C_77EEB2D4217A__INCLUDED_)
