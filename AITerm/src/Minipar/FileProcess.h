// FileProcess.h

#ifndef __FILE_PROCESS_H_
#define __FILE_PROCESS_H_

#define SIZE_OF_A_WORD		255

int countFileElement(const char *fileName);
int findWordInFile(const char *wordToFind, const char *fileName);
void readFromFileToMemory( char *memoryName, const char *fileName);


#endif //__FILE_PROCESS_H_
