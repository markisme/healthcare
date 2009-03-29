// NamedEntityRecognition.h


#ifndef __NAMED_ENTITY_RECOGNITION_H_
#define __NAMED_ENTITY_RECOGNITION_H_


void extractNouns( CSimpleMinipar Minipar, int miniparWordCount, char *nouns);
void removeInterrogativePronoun( char *nouns, int miniparWordCount);


#endif //__NAMED_ENTITY_RECOGNITION_H_

