// FileProcss.cpp

#include "stdafx.h"



//=================================================================================
//
// FUNCTION	: countFileElement
// DATE		: 2009/03/28
// AUTHOR	: Lee, Taewoo( ltw1372@lycos.co.kr )
// Input	: FileName -> ���ϸ�
// Output	: ���Ͼ��� �ܾ��
// Descriptinon	: �־��� ���Ͼȿ� �ִ� ��Ұ� ������ ����
//					�ܾ��� ���̰� 1024byte�̻��̸� �ȵ�.
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

	while( fscanf(fp,"%s", &data) != EOF)						// ���Ͼ��� ���ڰ� ����� ī��Ʈ
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
// Input	: wordToFind (ã���ܾ�)
//			: FileName (ã�� ����)
// Output	: ã����, ���° ��ҿ� �ִ���.
//			: ��ã����, -1
// Descriptinon	: �־��� ���Ͼȿ� ã�� �ܾ �ִ��� �˻��Ͽ�
//					������ �� �ܾ��� �ּҸ� �����ϰ�, ������ NULL�� ����.
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
// Input	: memoryName (������ �޸�)
//			: FileName (ã�� ����)
// Output	: ���Ͽ��� �о���� ������ �޸𸮿� �����.
// UPDATE	:
//
//=================================================================================
void readFromFileToMemory( char *memoryName, const char *fileName)
{
	FILE *fp;
	int numOfWordInFile;
	char buf[SIZE_OF_A_WORD]={0};

	// �����͸� �Ͼ���� �޸� �ʱ�ȭ
	numOfWordInFile= countFileElement(fileName);
	memset( memoryName, 0, sizeof(numOfWordInFile * SIZE_OF_A_WORD));

	// ������ ���
	if( (fp= fopen( fileName, "r")) == NULL )
	{
		perror("Cannot file open");
		return;
	}

	// �޸𸮿� ���
	while( (fscanf(fp, "%s", buf)) != EOF)
	{
		strcat( buf, " ");
		strcat( memoryName, buf);	
	}
	printf("%s\n", memoryName);
}


