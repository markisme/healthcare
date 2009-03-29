#include "TinyXML/tinyxml.h"
#include "XmlLib.h"
#include <string>


#define IE_ASSERT( value )


//////////////////////////////////////////////////////////////////////////
// XmlNode

const XmlNode * XmlNode::GetNode( const char * tagName, unsigned int idx ) const
{
	const TiXmlNode * node = FirstChild( tagName );
	if ( node->Type() != ELEMENT )
	{
		node = node->NextSiblingElement( tagName );
		if ( node == NULL )
		{
			return NULL;
		}
	}

	for ( unsigned int i = 0; i < idx; ++i )
	{
		node = node->NextSiblingElement( tagName );
		if ( node == NULL )
		{
			return NULL;
		}
	}

	return reinterpret_cast<const XmlNode*>( node );
}

// 태그가 일치하는 자식의 개수를 반환한다.
unsigned int XmlNode::GetNodeCount( const char * tagName ) const
{
	unsigned int nodeCount = 0;

	const TiXmlNode * node = FirstChild( tagName );
	if ( node == NULL )
	{
		return 0;
	}

	if ( node->Type() != ELEMENT )
	{
		node = node->NextSiblingElement( tagName );
	}

	while ( node )
	{
		++nodeCount;
		node = node->NextSiblingElement( tagName );
	}

	return nodeCount;
}

XmlNode * XmlNode::AddNode( const char * tagName )
{
	TiXmlElement element( tagName );
	XmlNode * node = reinterpret_cast<XmlNode*>( InsertEndChild( element ) );
	IE_ASSERT( node );
	return node;
}

const char * XmlNode::GetAttribute( const char * attName ) const
{
	const TiXmlElement * element = ToElement();
	if( element == NULL )
	{
		return NULL;
	}

	return element->Attribute( attName );
}

void XmlNode::SetAttribute( const char * attName, const char * attValue )
{
	TiXmlElement * element = ToElement();
	element->SetAttribute( attName, attValue );
}

const char * XmlNode::GetText() const
{
	const TiXmlNode * childNode = FirstChild();
	if ( childNode == NULL )
	{
		return NULL;
	}
	IE_ASSERT( childNode->Type() == TEXT );

	const TiXmlText * text = childNode->ToText();
	IE_ASSERT( text );

	return text->Value();
}

void XmlNode::SetText( const char * srcText, TextType textType )
{
	std::string text = srcText;
	if ( textType == STRING )
	{
		text = '"' + text;
		text += '"';
	}
	TiXmlText textNode( text.c_str() );
	if ( InsertEndChild( textNode ) == NULL )
	{
		IE_ASSERT( false );
	}
}


//////////////////////////////////////////////////////////////////////////
// XmlDocument

bool XmlDocument::LoadFile( const char * filename )
{
	return TiXmlDocument::LoadFile( filename );
}

bool XmlDocument::SaveFile( const char * filename ) const
{
	return TiXmlDocument::SaveFile( filename );
}

const XmlNode * XmlDocument::GetNode( const char * tagName, unsigned int idx ) const
{
	const TiXmlNode * node = FirstChild( tagName );
	if ( node->Type() != ELEMENT )
	{
		node = node->NextSiblingElement( tagName );
		if ( node == NULL )
		{
			return NULL;
		}
	}

	for ( unsigned int i = 0; i < idx; ++i )
	{
		node = node->NextSiblingElement( tagName );
		if ( node == NULL )
		{
			return NULL;
		}
	}

	return reinterpret_cast<const XmlNode*>( node );
}

XmlNode * XmlDocument::AddNode( const char * tagName )
{
	TiXmlElement element( tagName );
	XmlNode * node = reinterpret_cast<XmlNode*>( InsertEndChild( element ) );
	IE_ASSERT( node );
	return node;
}
