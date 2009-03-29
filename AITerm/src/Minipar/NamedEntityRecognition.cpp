// NamedEntityRecognition.cpp

#include "stdafx.h"




//=================================================================================
//
// FUNCTION	: extractNouns
// DATE		: 2009/03/29
// AUTHOR	: Lee, Taewoo( ltw1372@lycos.co.kr )
// Input	: Minipar(�Ľ̵� �̴��ı���ü), miniparWordCount(�Ľ��� �ܾ��ǰ���)
//			: nouns(����� ������ ����� ������ �����ּ�)
// Output	: nouns�� ���鸸 �����.
// Descriptinon	: parsing�� Minipar����ü�� �޾Ƽ� �ű⿡�� ��縸 �����س��� �Լ�
// UPDATE	:
//
//=================================================================================
void extractNouns( CSimpleMinipar Minipar, int miniparWordCount, char *nouns)
{
	int i;
	printf("����� �ܾ�鸸 ����\n");

	for( i=0; i<miniparWordCount; i++)
	{
		if ( !strcmp(Minipar.GetAt(i)->szPOS, "N") )			// ����� �ܾ�鸸 ����
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
// Input	: miniparWordCount(�Ľ��� �ܾ��ǰ���)
//			: nouns(����� ������ ����� ������ �����ּ�)
// Output	: nouns�� �ǹ��縦 ��� ���鸸 �����.
// Descriptinon	: ������ �ܾ�� �߿��� �ǹ��簡 �ִ��� �˻��Ͽ� ������ �����.
// UPDATE	:
//
//=================================================================================
void removeInterrogativePronoun( char *nouns, int miniparWordCount)
{
	// ������ ����, nouns ���ڿ� ���� ��ġ�ϴ� �ܾ �ִ��� �˻��Ѵ�.
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
	
	numOfWordInFile= countFileElement(INTERROGATIVE_PRONOUN);		// file���� �ܾ���� ����� ī��Ʈ
	
	nouns= ToLowerCase(nouns);										// ã�� ������ �ҹ��ڷ� ��ȯ�ϰ�

	for(i=0; i<numOfWordInFile; i++)								// ���� ���� �ܾ� ������ ��������
	{
		fscanf( fp, "%s", buf);										// ���Ͼ��� �� �ܾ �о�鿩
		
		if( (ch= strstr( nouns, buf)) != NULL)						// ã���� �ϴ� �ܾ ����� ���� �ȿ� �����ϸ�
		{
			//printf("%s\n", ch);								
			while( *ch != ' ')										// nouns���� �ش� ���ڿ��� �����Ѵ�.
			{														// �̶� ���ŵ� �ܾ�� ���߿� ������ �� �� �����Ƿ� ������ ���´�.
				strncat(removedWord, ch++, 1);
			}
			
			strcpy( nounsBuf, ++ch);								// �� �ܾ� ������ ������ Buf�� �����صΰ�,
			memset( nouns, 0, sizeof(SIZE_OF_A_WORD * miniparWordCount));	// nouns�� ������
			strcpy( nouns, nounsBuf);								// Buf�� �ִ� ���� �ٽ� nouns�� �ִ´�.
		}	
	}
	
	printf("nouns=%s\n", nouns);
	printf("removedWord=%s\n", removedWord);


	free(nounsBuf);
}


