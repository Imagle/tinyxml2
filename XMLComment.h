/** An XML Comment. */

#ifndef XMLCOMMENT_H
#define XMLCOMMENT_H

#include "tinyxml.h"
#include "XMLNode.h"
#include "XMLVisitor.h"


namespace tinyxml2{

	class XMLComment : public XMLNode
	{
		friend class XMLDocument;
	public:
		virtual XMLComment*	ToComment()					{
			return this;
		}
		virtual const XMLComment* ToComment() const		{
			return this;
		}

		virtual bool Accept(XMLVisitor* visitor) const;

		char* ParseDeep(char*, StrPair* endTag);
		virtual XMLNode* ShallowClone(XMLDocument* document) const;
		virtual bool ShallowEqual(const XMLNode* compare) const;

	protected:
		XMLComment(XMLDocument* doc);
		virtual ~XMLComment();
		XMLComment(const XMLComment&);	// not supported
		XMLComment& operator=(const XMLComment&);	// not supported

	private:
	};
}
#endif