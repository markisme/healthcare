// FileProcss.cpp

#include "stdafx.h"



//=================================================================================
//
// FUNCTION	: countFileElement
// DATE		: 2009/03/28
// AUTHOR	: Lee, Taewoo( ltw1372@lycos.co.kr )
// Input	: FileName -> 파일명
// Output	: 파일안의 단어수
// Descriptinon	: 주어진 파일안에 있는 요소개 개수를 리턴
//					단어의 길이가 1024byte이상이면 안됨.
// UPDATE	:
//
//=================================================================================
int countFileElement(const char *fileName)
{
	FILE *fp;
	char data[1024]={0};
	int fileNumOfWord=0;

	if( (fp= fopen(fileName, "r")) == NULL )
	{
		perror("Cannot file open");
		return -1;
	}

	while( fscanf(fp,"%s", &data) != EOF)						// 파일안의 인자가 몇개인지 카운트
	{
		fileNumOfWord++;
		//printf("%s\n", data);
	}
	//printf("fileNumOfWord= %d\n", fileNumOfWord);

	fclose(fp);
	return fileNumOfWord;
}




//=================================================================================
//
// FUNCTION	: countFileElement
// DATE		: 2009/03/28
// AUTHOR	: Lee, Taewoo( ltw1372@lycos.co.kr )
// Input	: wordToFind (찾을단어)
//			: FileName (찾을 파일)
// Output	: 찾으면, 몇번째 요소에 있는지.
//			: 못찾으면, -1
// Descriptinon	: 주어진 파일안에 찾을 단어가 있는지 검색하여
//					있으면 그 단어의 주소를 리턴하고, 없으면 NULL을 리턴.
// UPDATE	:
//
//=================================================================================
int findWordInFile(const char *wordToFind, const char *fileName)
{
	FILE *fp;
	char data[1024]={0};
	char *ch;
	int fileNumOfWord=0;

	if( (fp= fopen(fileName, "r")) == NULL )
	{
		perror("Cannot file open");
		return -1;
	}

	//strstr( wordToFind, 

	fclose(fp);
	return fileNumOfWord;
}



//=================================================================================
//
// FUNCTION	: readFromFileToMemory
// DATE		: 2009/03/28
// AUTHOR	: Lee, Taewoo( ltw1372@lycos.co.kr )
// Input	: memoryName (저장할 메모리)
//			: FileName (찾을 파일)
// Output	: 파일에서 읽어들인 값들이 메모리에 저장됨.
// UPDATE	:
//
//=================================================================================
void readFromFileToMemory( char *memoryName, const char *fileName)
{
	FILE *fp;
	int numOfWordInFile;
	char buf[SIZE_OF_A_WORD]={0};

	// 데이터를 일어들인 메모리 초기화
	numOfWordInFile= countFileElement(fileName);
	memset( memoryName, 0, sizeof(numOfWordInFile * SIZE_OF_A_WORD));

	// 파일을 열어서
	if( (fp= fopen( fileName, "r")) == NULL )
	{
		perror("Cannot file open");
		return;
	}

	// 메모리에 기록
	while( (fscanf(fp, "%s", buf)) != EOF)
	{
		strcat( buf, " ");
		strcat( memoryName, buf);	
	}
	printf("%s\n", memoryName);
}


