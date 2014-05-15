/**
	A variant of the XMLHandle class for working with const XMLNodes and Documents. It is the
	same in all regards, except for the 'const' qualifiers. See XMLHandle for API.
*/
#ifndef XMLCONSTHANDLE_H
#define XMLCONSTHANDLE_H

#include "tinyxml.h"
#include "XMLNode.h"
#include "XMLElement.h"

namespace tinyxml2{

	class XMLConstHandle
	{
	public:
		XMLConstHandle(const XMLNode* node)											{
			_node = node;
		}
		XMLConstHandle(const XMLNode& node)											{
			_node = &node;
		}
		XMLConstHandle(const XMLConstHandle& ref)										{
			_node = ref._node;
		}

		XMLConstHandle& operator=(const XMLConstHandle& ref)							{
			_node = ref._node;
			return *this;
		}

		const XMLConstHandle FirstChild() const											{
			return XMLConstHandle(_node ? _node->FirstChild() : 0);
		}
		const XMLConstHandle FirstChildElement(const char* value = 0) const				{
			return XMLConstHandle(_node ? _node->FirstChildElement(value) : 0);
		}
		const XMLConstHandle LastChild()	const										{
			return XMLConstHandle(_node ? _node->LastChild() : 0);
		}
		const XMLConstHandle LastChildElement(const char* _value = 0) const				{
			return XMLConstHandle(_node ? _node->LastChildElement(_value) : 0);
		}
		const XMLConstHandle PreviousSibling() const									{
			return XMLConstHandle(_node ? _node->PreviousSibling() : 0);
		}
		const XMLConstHandle PreviousSiblingElement(const char* _value = 0) const		{
			return XMLConstHandle(_node ? _node->PreviousSiblingElement(_value) : 0);
		}
		const XMLConstHandle NextSibling() const										{
			return XMLConstHandle(_node ? _node->NextSibling() : 0);
		}
		const XMLConstHandle NextSiblingElement(const char* _value = 0) const			{
			return XMLConstHandle(_node ? _node->NextSiblingElement(_value) : 0);
		}


		const XMLNode* ToNode() const				{
			return _node;
		}
		const XMLElement* ToElement() const			{
			return ((_node && _node->ToElement()) ? _node->ToElement() : 0);
		}
		const XMLText* ToText() const				{
			return ((_node && _node->ToText()) ? _node->ToText() : 0);
		}
		const XMLUnknown* ToUnknown() const			{
			return ((_node && _node->ToUnknown()) ? _node->ToUnknown() : 0);
		}
		const XMLDeclaration* ToDeclaration() const	{
			return ((_node && _node->ToDeclaration()) ? _node->ToDeclaration() : 0);
		}

	private:
		const XMLNode* _node;
	};
}
#endif