// SimpleMinipar.cpp: implementation of the CSimpleMinipar class.
//
//////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "SimpleMinipar.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimpleMinipar::CSimpleMinipar()
{
	Init();
}

void CSimpleMinipar::Init(void)
{
	nSimpleMiniparWord = 0;
	return;
}

void CSimpleMinipar::Add(int nLineNum, char *pstrWord, char *pstrPOS, char *pstrDependence)
{
	SimpleMiniparWord[nSimpleMiniparWord].nLineNum = nLineNum;
	sprintf(SimpleMiniparWord[nSimpleMiniparWord].szWord, "%s\0", pstrWord);
	sprintf(SimpleMiniparWord[nSimpleMiniparWord].szPOS, "%s\0", pstrPOS);
	sprintf(SimpleMiniparWord[nSimpleMiniparWord].szDependence, "%s\0", pstrDependence);

	nSimpleMiniparWord++;
	return;
}

bool CSimpleMinipar::Parse(char *pSentence)
{
	Init();
	FILE * fd = NULL;

	fd = fopen("sentense.dat", "w");
	if (fd == NULL) return false;
	fprintf(fd, "%s\n", pSentence);
	fclose(fd);

	int ids = system("Minipar.EXE > sentence.out");
	if (ids != TRUE) return false;

	char szLine[1025];
	fd = fopen("sentence.out", "r");
	if (fd == NULL) return false;
	while(true)
	{
		if (fgets(szLine, 1024, fd) == NULL) break;
		szLine[strlen(szLine)-1] = '\0';
		if (strlen(szLine) == 0) continue;
		if (szLine[0] == '(' || szLine[0] == ')') continue;

		Add(szLine);
	}
	fclose(fd);
	return true;
}

int CSimpleMinipar::GetWordCount(void)
{
	return nSimpleMiniparWord;
}

void CSimpleMinipar::Add(char *pstrLine)
{
	char szLine[2048];
	sprintf(szLine, "%s", pstrLine);
	char *pBuf;

	CSimpleMiniparWord tmpWord;
	for (int i = 0; true; i++)
	{
		if (i == 0) 
		{
			pBuf = strtok(szLine, " \t");
		} else {
			pBuf = strtok(NULL, " \t");
		}
		if (pBuf == NULL) break;

		if (i == 0) tmpWord.nLineNum = atoi(pBuf);
		if (i == 1) 
		{
			sprintf(tmpWord.szWord, "%s", &pBuf[1]);
		}
		if (i == 2) 
		{
			sprintf(tmpWord.szPOS, "%s", pBuf);
		}
		if (i == 3) 
		{
			pBuf[strlen(pBuf)-1] = '\0';
			sprintf(tmpWord.szDependence, "%s", pBuf);
		}
	}
	Add(tmpWord.nLineNum, tmpWord.szWord, tmpWord.szPOS, tmpWord.szDependence);
	return;
}

CSimpleMiniparWord * CSimpleMinipar::GetAt(int nIdx)
{
	if (nIdx < 0) return NULL;
	if (nIdx >= GetWordCount()) return NULL;

	return &SimpleMiniparWord[nIdx];
}
