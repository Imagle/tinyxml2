#include "stdafx.h"
#include "tinyxml.h"
#include "XMLText.h"
#include "XMLDeclaration.h"
#include "XMLComment.h"
#include "XMLElement.h"
#include "XMLUnknown.h"
#include "XMLDocument.h"


namespace tinyxml2{

	// --------- XMLNode ----------- //

	XMLNode::XMLNode(XMLDocument* doc) :
		_document(doc),
		_parent(0),
		_firstChild(0), _lastChild(0),
		_prev(0), _next(0),
		_memPool(0)
	{
	}


	XMLNode::~XMLNode()
	{
		DeleteChildren();
		if (_parent) {
			_parent->Unlink(this);
		}
	}

	const char* XMLNode::Value() const
	{
		return _value.GetStr();
	}

	void XMLNode::SetValue(const char* str, bool staticMem)
	{
		if (staticMem) {
			_value.SetInternedStr(str);
		}
		else {
			_value.SetStr(str);
		}
	}


	void XMLNode::DeleteChildren()
	{
		while (_firstChild) {
			XMLNode* node = _firstChild;
			Unlink(node);

			DELETE_NODE(node);
		}
		_firstChild = _lastChild = 0;
	}


	void XMLNode::Unlink(XMLNode* child)
	{
		if (child == _firstChild) {
			_firstChild = _firstChild->_next;
		}
		if (child == _lastChild) {
			_lastChild = _lastChild->_prev;
		}

		if (child->_prev) {
			child->_prev->_next = child->_next;
		}
		if (child->_next) {
			child->_next->_prev = child->_prev;
		}
		child->_parent = 0;
	}


	void XMLNode::DeleteChild(XMLNode* node)
	{
		assert(node->_parent == this);
		DELETE_NODE(node);
	}


	XMLNode* XMLNode::InsertEndChild(XMLNode* addThis)
	{
		if (addThis->_document != _document)
			return 0;

		if (addThis->_parent)
			addThis->_parent->Unlink(addThis);
		else
			addThis->_memPool->SetTracked();

		if (_lastChild) {
			assert(_firstChild);
			assert(_lastChild->_next == 0);
			_lastChild->_next = addThis;
			addThis->_prev = _lastChild;
			_lastChild = addThis;

			addThis->_next = 0;
		}
		else {
			assert(_firstChild == 0);
			_firstChild = _lastChild = addThis;

			addThis->_prev = 0;
			addThis->_next = 0;
		}
		addThis->_parent = this;
		return addThis;
	}


	XMLNode* XMLNode::InsertFirstChild(XMLNode* addThis)
	{
		if (addThis->_document != _document)
			return 0;

		if (addThis->_parent)
			addThis->_parent->Unlink(addThis);
		else
			addThis->_memPool->SetTracked();

		if (_firstChild) {
			assert(_lastChild);
			assert(_firstChild->_prev == 0);

			_firstChild->_prev = addThis;
			addThis->_next = _firstChild;
			_firstChild = addThis;

			addThis->_prev = 0;
		}
		else {
			assert(_lastChild == 0);
			_firstChild = _lastChild = addThis;

			addThis->_prev = 0;
			addThis->_next = 0;
		}
		addThis->_parent = this;
		return addThis;
	}


	XMLNode* XMLNode::InsertAfterChild(XMLNode* afterThis, XMLNode* addThis)
	{
		if (addThis->_document != _document)
			return 0;

		assert(afterThis->_parent == this);

		if (afterThis->_parent != this) {
			return 0;
		}

		if (afterThis->_next == 0) {
			// The last node or the only node.
			return InsertEndChild(addThis);
		}
		if (addThis->_parent)
			addThis->_parent->Unlink(addThis);
		else
			addThis->_memPool->SetTracked();
		addThis->_prev = afterThis;
		addThis->_next = afterThis->_next;
		afterThis->_next->_prev = addThis;
		afterThis->_next = addThis;
		addThis->_parent = this;
		return addThis;
	}


	const XMLElement* XMLNode::FirstChildElement(const char* value) const
	{
		for (XMLNode* node = _firstChild; node; node = node->_next) {
			XMLElement* element = node->ToElement();
			if (element) {
				if (!value || XMLUtil::StringEqual(element->Name(), value)) {
					return element;
				}
			}
		}
		return 0;
	}


	const XMLElement* XMLNode::LastChildElement(const char* value) const
	{
		for (XMLNode* node = _lastChild; node; node = node->_prev) {
			XMLElement* element = node->ToElement();
			if (element) {
				if (!value || XMLUtil::StringEqual(element->Name(), value)) {
					return element;
				}
			}
		}
		return 0;
	}


	const XMLElement* XMLNode::NextSiblingElement(const char* value) const
	{
		for (XMLNode* element = this->_next; element; element = element->_next) {
			if (element->ToElement()
				&& (!value || XMLUtil::StringEqual(value, element->Value()))) {
				return element->ToElement();
			}
		}
		return 0;
	}


	const XMLElement* XMLNode::PreviousSiblingElement(const char* value) const
	{
		for (XMLNode* element = _prev; element; element = element->_prev) {
			if (element->ToElement()
				&& (!value || XMLUtil::StringEqual(value, element->Value()))) {
				return element->ToElement();
			}
		}
		return 0;
	}


	char* XMLNode::ParseDeep(char* p, StrPair* parentEnd)
	{
		// This is a recursive method, but thinking about it "at the current level"
		// it is a pretty simple flat list:
		//		<foo/>
		//		<!-- comment -->
		//
		// With a special case:
		//		<foo>
		//		</foo>
		//		<!-- comment -->
		//
		// Where the closing element (/foo) *must* be the next thing after the opening
		// element, and the names must match. BUT the tricky bit is that the closing
		// element will be read by the child.
		//
		// 'endTag' is the end tag for this node, it is returned by a call to a child.
		// 'parentEnd' is the end tag for the parent, which is filled in and returned.

		while (p && *p) {
			XMLNode* node = 0;

			p = _document->Identify(p, &node);
			if (p == 0 || node == 0) {
				break;
			}

			StrPair endTag;
			p = node->ParseDeep(p, &endTag);
			if (!p) {
				DELETE_NODE(node);
				node = 0;
				if (!_document->Error()) {
					_document->SetError(XML_ERROR_PARSING, 0, 0);
				}
				break;
			}

			// We read the end tag. Return it to the parent.
			if (node->ToElement() && node->ToElement()->ClosingType() == XMLElement::CLOSING) {
				if (parentEnd) {
					*parentEnd = static_cast<XMLElement*>(node)->_value;
				}
				node->_memPool->SetTracked();	// created and then immediately deleted.
				DELETE_NODE(node);
				return p;
			}

			// Handle an end tag returned to this level.
			// And handle a bunch of annoying errors.
			XMLElement* ele = node->ToElement();
			if (ele) {
				if (endTag.Empty() && ele->ClosingType() == XMLElement::OPEN) {
					_document->SetError(XML_ERROR_MISMATCHED_ELEMENT, node->Value(), 0);
					p = 0;
				}
				else if (!endTag.Empty() && ele->ClosingType() != XMLElement::OPEN) {
					_document->SetError(XML_ERROR_MISMATCHED_ELEMENT, node->Value(), 0);
					p = 0;
				}
				else if (!endTag.Empty()) {
					if (!XMLUtil::StringEqual(endTag.GetStr(), node->Value())) {
						_document->SetError(XML_ERROR_MISMATCHED_ELEMENT, node->Value(), 0);
						p = 0;
					}
				}
			}
			if (p == 0) {
				DELETE_NODE(node);
				node = 0;
			}
			if (node) {
				this->InsertEndChild(node);
			}
		}
		return 0;
	}

}