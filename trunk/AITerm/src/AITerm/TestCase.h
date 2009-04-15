#pragma once
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

	static TestCase & GetInstance() { return _instance; }

private:
	static TestCase _instance;
};