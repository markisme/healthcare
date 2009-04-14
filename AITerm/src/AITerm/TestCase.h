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
	void XMLSaveTest();
	void XMLLoadTest();

	//XmlNode* CreateXMLTag(XmlDocument & xmlDoc, const char *representativeName);
	//void AddTag(XmlNode &XmlNode, const char *tagName, const char *tag);

	static TestCase & GetInstance() { return _instance; }

private:
	static TestCase _instance;
};