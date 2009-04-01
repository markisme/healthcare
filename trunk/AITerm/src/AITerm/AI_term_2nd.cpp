#include "stdafx.h"

#include "NamedEntity.h"

int _tmain(int argc, _TCHAR* argv[])
{
	// DB 접속 정보
	DBConnectInfo info;
	info.ip = "211.189.19.160";
	info.user = "root";
	info.pass = "1234";
	info.port = 3306;
	info.dbName = "AITerm";

	// DB 정보 초기화(싱글턴)
	DBConnector::GetInstance().Init( info );

	// 개체명 인식기 초기화
	NamedEntity namedEntity;
	namedEntity.Init();

	// 질문들 처리
	namedEntity.ProcessQuestion();

	return 0;
}