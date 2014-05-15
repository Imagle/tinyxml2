/**
	Implements the interface to the "Visitor pattern" (see the Accept() method.)
	If you call the Accept() method, it requires being passed a XMLVisitor
	class to handle callbacks. For nodes that contain other nodes (Document, Element)
	you will get called with a VisitEnter/VisitExit pair. Nodes that are always leafs
	are simply called with Visit().

	If you return 'true' from a Visit method, recursive parsing will continue. If you return
	false, <b>no children of this node or its siblings</b> will be visited.

	All flavors of Visit methods have a default implementation that returns 'true' (continue
	visiting). You need to only override methods that are interesting to you.

	Generally Accept() is called on the XMLDocument, although all nodes support visiting.

	You should never change the document from a callback.

	@sa XMLNode::Accept()
*/
#ifndef XMLVISITOR_H
#define XMLVISITOR_H

//#include "stdafx.h"
#include "tinyxml.h"

namespace tinyxml2{
	
	class XMLDocument;
	class XMLElement;
	class XMLAttribute;
	class XMLDeclaration;
	class XMLText;
	class XMLComment;
	class XMLUnknown;

	class XMLVisitor
	{
	public :

	public:
		virtual ~XMLVisitor() {}

		/// Visit a document.
		virtual bool VisitEnter(const XMLDocument& doc)			{
			return true;
		}
		/// Visit a document.
		virtual bool VisitExit(const XMLDocument& doc)			{
			return true;
		}

		/// Visit an element.
		virtual bool VisitEnter(const XMLElement& element, const XMLAttribute* firstAttribute)	{
			return true;
		}
		/// Visit an element.
		virtual bool VisitExit(const XMLElement& element)			{
			return true;
		}

		/// Visit a declaration.
		virtual bool Visit(const XMLDeclaration& declaration)		{
			return true;
		}
		/// Visit a text node.
		virtual bool Visit(const XMLText& text)					{
			return true;
		}
		/// Visit a comment node.
		virtual bool Visit(const XMLComment& comment)				{
			return true;
		}
		/// Visit an unknown node.
		virtual bool Visit(const XMLUnknown& unknown)				{
			return true;
		}
	};
}
#endif