#pragma once
#include "QueryGenerator.h"
class XmlDocument;
class NamedEntity;
class XmlNode;

class TestCase
{
public:
	TestCase();
	~TestCase();

	void Init();
	void Uninit();
	
	void WordnetTest();
	void XMLLoadSaveTest();
	void LoadResultDB( DBResultList & dbResultList );

	static TestCase & GetInstance() { return _instance; }

private:
	static TestCase _instance;
};