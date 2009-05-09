
#include "TinyXML/tinyxml.h"


class XmlNode : private TiXmlNode
{
public:
	const XmlNode * GetNode( const char * tagName, unsigned int idx = 0 ) const;
	// �±װ� ��ġ�ϴ� �ڽ��� ������ ��ȯ�Ѵ�.
	unsigned int GetNodeCount( const char * tagName ) const;
	XmlNode * AddNode( const char * tagName );

	const char * GetAttribute( const char * attName ) const;
	void SetAttribute( const char * attName, const char * attValue );

	enum TextType
	{
		STRING = 0, NUMBER
	};

	const char * GetText() const;
	void SetText( const char * text, TextType textType );
};


class XmlDocument : private TiXmlDocument
{
public:
	bool LoadFile( const char * filename );
	bool SaveFile( const char * filename ) const;

	const XmlNode * GetNode( const char * tagName, unsigned int idx = 0 ) const;
	XmlNode * AddNode( const char * tagName );
};

