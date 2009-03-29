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

// 주의 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Minipar.EXE와 같은 디렉토리에서 실행되어야 합니다.

class CSimpleMiniparWord
{
	public:
	int nLineNum;							// 단어 번호
	char szWord[MAX_WORD_SIZE];				// 단어
	char szPOS[MAX_SMALL_WORD_SIZE];		// 품사
	char szDependence[MAX_SMALL_WORD_SIZE];	// 의존관계
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


/* 사용하게 될 함수들 */

	// 문장을 parameter로 하여 파싱을 수행하는 함수
	bool Parse(char *pSentence);

	// 파싱된 결과에 단어의 갯수가 몇개인지 반환하는 함수
	int GetWordCount(void);

	// 파싱된 결과에서 주어진  인덱스의 단어의 품사, 의존관계를 반환하는 함수
	CSimpleMiniparWord* GetAt(int nIdx);

};

#endif // !defined(AFX_SIMPLEMINIPAR_H__3A9985AC_8D6C_4D3D_868C_77EEB2D4217A__INCLUDED_)
