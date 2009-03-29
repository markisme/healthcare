//
#include <vector>
#include <string>

#include "DBConnector.h"

// 기본 매크로 함수
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }

//
typedef unsigned int uint;