#include "stdafx.h"
#include "XMLDeclaration.h"
#include "XMLDocument.h"

namespace tinyxml2{

	// --------- XMLDeclaration ---------- //

	XMLDeclaration::XMLDeclaration(XMLDocument* doc) : XMLNode(doc)
	{
	}
	
	XMLDeclaration::~XMLDeclaration()
	{
		//printf( "~XMLDeclaration\n" );
	}
	
	char* XMLDeclaration::ParseDeep(char* p, StrPair*)
	{
		// Declaration parses as text.
		const char* start = p;
		p = _value.ParseText(p, "?>", StrPair::NEEDS_NEWLINE_NORMALIZATION);
		if (p == 0) {
			_document->SetError(XML_ERROR_PARSING_DECLARATION, start, 0);
		}
		return p;
	}

	XMLNode* XMLDeclaration::ShallowClone(XMLDocument* doc) const
	{
		if (!doc) {
			doc = _document;
		}
		XMLDeclaration* dec = doc->NewDeclaration(Value());	// fixme: this will always allocate memory. Intern?
		return dec;
	}

	bool XMLDeclaration::ShallowEqual(const XMLNode* compare) const
	{
		return (compare->ToDeclaration() && XMLUtil::StringEqual(compare->ToDeclaration()->Value(), Value()));
	}
	
	bool XMLDeclaration::Accept(XMLVisitor* visitor) const
	{
		return visitor->Visit(*this);
	}
}