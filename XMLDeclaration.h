/** In correct XML the declaration is the first entry in the file.
	@verbatim
		<?xml version="1.0" standalone="yes"?>
	@endverbatim

	TinyXML-2 will happily read or write files without a declaration,
	however.

	The text of the declaration isn't interpreted. It is parsed
	and written as a string.
*/
#ifndef XMLDECLARATION_H
#define XMLDECLARATION_H

#include "tinyxml.h"
#include "XMLNode.h"
#include "XMLVisitor.h"

namespace tinyxml2{

	class XMLDeclaration : public XMLNode
	{
		friend class XMLDocument;
	public:
		virtual XMLDeclaration*	ToDeclaration()					{
			return this;
		}
		virtual const XMLDeclaration* ToDeclaration() const		{
			return this;
		}

		virtual bool Accept(XMLVisitor* visitor) const;

		char* ParseDeep(char*, StrPair* endTag);
		virtual XMLNode* ShallowClone(XMLDocument* document) const;
		virtual bool ShallowEqual(const XMLNode* compare) const;

	protected:
		XMLDeclaration(XMLDocument* doc);
		virtual ~XMLDeclaration();
		XMLDeclaration(const XMLDeclaration&);	// not supported
		XMLDeclaration& operator=(const XMLDeclaration&);	// not supported
	};
}
#endif