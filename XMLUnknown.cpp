#include "stdafx.h"
#include "XMLUnknown.h"
#include "XMLDocument.h"

namespace tinyxml2{

	// --------- XMLUnknown ---------- //

	XMLUnknown::XMLUnknown(XMLDocument* doc) : XMLNode(doc)
	{
	}


	XMLUnknown::~XMLUnknown()
	{
	}


	char* XMLUnknown::ParseDeep(char* p, StrPair*)
	{
		// Unknown parses as text.
		const char* start = p;

		p = _value.ParseText(p, ">", StrPair::NEEDS_NEWLINE_NORMALIZATION);
		if (!p) {
			_document->SetError(XML_ERROR_PARSING_UNKNOWN, start, 0);
		}
		return p;
	}


	XMLNode* XMLUnknown::ShallowClone(XMLDocument* doc) const
	{
		if (!doc) {
			doc = _document;
		}
		XMLUnknown* text = doc->NewUnknown(Value());	// fixme: this will always allocate memory. Intern?
		return text;
	}


	bool XMLUnknown::ShallowEqual(const XMLNode* compare) const
	{
		return (compare->ToUnknown() && XMLUtil::StringEqual(compare->ToUnknown()->Value(), Value()));
	}


	bool XMLUnknown::Accept(XMLVisitor* visitor) const
	{
		return visitor->Visit(*this);
	}


}