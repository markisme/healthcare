// NamedEntityRecognition.cpp

#include "stdafx.h"




//=================================================================================
//
// FUNCTION	: extractNouns
// DATE		: 2009/03/29
// AUTHOR	: Lee, Taewoo( ltw1372@lycos.co.kr )
// Input	: Minipar(파싱된 미니파구조체), miniparWordCount(파싱한 단어의개수)
//			: nouns(추출된 명사들이 저장될 공간의 시작주소)
// Output	: nouns에 명사들만 저장됨.
// Descriptinon	: parsing된 Minipar구조체를 받아서 거기에서 명사만 추출해내는 함수
// UPDATE	:
//
//=================================================================================
void extractNouns( CSimpleMinipar Minipar, int miniparWordCount, char *nouns)
{
	int i;
	printf("명사인 단어들만 추출\n");

	for( i=0; i<miniparWordCount; i++)
	{
		if ( !strcmp(Minipar.GetAt(i)->szPOS, "N") )			// 명사인 단어들만 추출
		{
			strcat( nouns, Minipar.GetAt(i)->szWord);
			strcat( nouns, " ");
			//printf("%s\n", Minipar.GetAt(i)->szWord);
		}
	}
	printf("%s\n", nouns);
}



//=================================================================================
//
// FUNCTION	: removeInterrogativePronoun
// DATE		: 2009/03/29
// AUTHOR	: Lee, Taewoo( ltw1372@lycos.co.kr )
// Input	: miniparWordCount(파싱한 단어의개수)
//			: nouns(추출된 명사들이 저장될 공간의 시작주소)
// Output	: nouns에 의문사를 떼어낸 명사들만 저장됨.
// Descriptinon	: 추출한 단어들 중에서 의문사가 있는지 검사하여 있으면 떼어낸다.
// UPDATE	:
//
//=================================================================================
void removeInterrogativePronoun( char *nouns, int miniparWordCount)
{
	// 파일을 돌며, nouns 문자열 내에 일치하는 단어가 있는지 검색한다.
	FILE *fp;
	int numOfWordInFile, i;
	char *ch;
	char buf[SIZE_OF_A_WORD]={0};
	char removedWord[SIZE_OF_A_WORD]={0};

	char *nounsBuf= (char*)malloc(sizeof(SIZE_OF_A_WORD * miniparWordCount));
	memset(nounsBuf, 0, sizeof(SIZE_OF_A_WORD * miniparWordCount));
	
	if( (fp= fopen(INTERROGATIVE_PRONOUN, "r")) == NULL )
	{
		perror("Cannot file open");
		return;
	}
	
	numOfWordInFile= countFileElement(INTERROGATIVE_PRONOUN);		// file안의 단어수가 몇개인지 카운트
	
	nouns= ToLowerCase(nouns);										// 찾은 명사들을 소문자로 변환하고

	for(i=0; i<numOfWordInFile; i++)								// 파일 안의 단어 개수만 루프돌며
	{
		fscanf( fp, "%s", buf);										// 파일안의 각 단어를 읽어들여
		
		if( (ch= strstr( nouns, buf)) != NULL)						// 찾고자 하는 단어가 추출된 명사들 안에 존재하면
		{
			//printf("%s\n", ch);								
			while( *ch != ' ')										// nouns에서 해당 문자열을 제거한다.
			{														// 이때 제거된 단어는 나중에 도움이 될 수 있으므로 추출해 놓는다.
				strncat(removedWord, ch++, 1);
			}
			
			strcpy( nounsBuf, ++ch);								// 뺀 단어 다음의 문장을 Buf에 저장해두고,
			memset( nouns, 0, sizeof(SIZE_OF_A_WORD * miniparWordCount));	// nouns를 비운다음
			strcpy( nouns, nounsBuf);								// Buf에 있는 것을 다시 nouns에 넣는다.
		}	
	}
	
	printf("nouns=%s\n", nouns);
	printf("removedWord=%s\n", removedWord);


	free(nounsBuf);
}


