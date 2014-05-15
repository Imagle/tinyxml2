/*
Original code by Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.


2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#ifndef TINYXML2_INCLUDED
#define TINYXML2_INCLUDED

#include "stdafx.h"

#if defined(ANDROID_NDK) || defined(__BORLANDC__)
#   include <ctype.h>
#   include <limits.h>
#   include <stdio.h>
#   include <stdlib.h>
#   include <string.h>
#   include <stdarg.h>
#else
#   include <cctype>
#   include <climits>
#   include <cstdio>
#   include <cstdlib>
#   include <cstring>
#   include <cstdarg>
#endif



static const char LINE_FEED = (char)0x0a;			// all line endings are normalized to LF
static const char LF = LINE_FEED;
static const char CARRIAGE_RETURN = (char)0x0d;			// CR gets filtered out
static const char CR = CARRIAGE_RETURN;
static const char SINGLE_QUOTE = '\'';
static const char DOUBLE_QUOTE = '\"';

// Bunch of unicode info at:
//		http://www.unicode.org/faq/utf_bom.html
//	ef bb bf (Microsoft "lead bytes") - designates UTF-8

static const unsigned char TIXML_UTF_LEAD_0 = 0xefU;
static const unsigned char TIXML_UTF_LEAD_1 = 0xbbU;
static const unsigned char TIXML_UTF_LEAD_2 = 0xbfU;


#define DELETE_NODE( node )	{			\
	if ( node ) {						\
	MemPool* pool = node->_memPool;	\
	node->~XMLNode();				\
	pool->Free( node );				\
	}									\
}
#define DELETE_ATTRIBUTE( attrib ) {		\
	if ( attrib ) {							\
	MemPool* pool = attrib->_memPool;	\
	attrib->~XMLAttribute();			\
	pool->Free( attrib );				\
	}										\
}


/*
   TODO: intern strings instead of allocation.
*/
/*
	gcc:
        g++ -Wall -DDEBUG tinyxml2.cpp xmltest.cpp -o gccxmltest.exe

    Formatting, Artistic Style:
        AStyle.exe --style=1tbs --indent-switches --break-closing-brackets --indent-preprocessor tinyxml2.cpp tinyxml2.h
*/

#if defined( _DEBUG ) || defined( DEBUG ) || defined (__DEBUG__)
#   ifndef DEBUG
#       define DEBUG
#   endif
#endif

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable: 4251)
#endif







#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
// Microsoft visual studio, version 2005 and higher.
/*int _snprintf_s(
   char *buffer,
   size_t sizeOfBuffer,
   size_t count,
   const char *format [,
	  argument] ...
);*/

inline int TIXML_SNPRINTF( char* buffer, size_t size, const char* format, ... )
{
    va_list va;
    va_start( va, format );
    int result = vsnprintf_s( buffer, size, _TRUNCATE, format, va );
    va_end( va );
    return result;
}
#define TIXML_SSCANF   sscanf_s
#else
// GCC version 3 and higher
//#warning( "Using sn* functions." )
#define TIXML_SNPRINTF snprintf
#define TIXML_SSCANF   sscanf
#endif

static const int TIXML2_MAJOR_VERSION = 1;
static const int TIXML2_MINOR_VERSION = 0;
static const int TIXML2_PATCH_VERSION = 14;


#if defined(_MSC_VER)
#   pragma warning(pop)
#endif


namespace tinyxml2
{
	/*
	class XMLDocument;
	class XMLElement;
	class XMLAttribute;
	class XMLComment;
	class XMLText;
	class XMLDeclaration;
	class XMLUnknown;
	class XMLPrinter;
	*/
	struct Entity {
		const char* pattern;
		int length;
		char value;
	};

	static const int NUM_ENTITIES = 5;
	static const Entity entities[NUM_ENTITIES] = {
		{ "quot", 4, DOUBLE_QUOTE },
		{ "amp", 3, '&' },
		{ "apos", 4, SINGLE_QUOTE },
		{ "lt", 2, '<' },
		{ "gt", 2, '>' }
	};

	enum XMLError {
		XML_NO_ERROR = 0,
		XML_SUCCESS = 0,

		XML_NO_ATTRIBUTE,
		XML_WRONG_ATTRIBUTE_TYPE,

		XML_ERROR_FILE_NOT_FOUND,
		XML_ERROR_FILE_COULD_NOT_BE_OPENED,
		XML_ERROR_FILE_READ_ERROR,
		XML_ERROR_ELEMENT_MISMATCH,
		XML_ERROR_PARSING_ELEMENT,
		XML_ERROR_PARSING_ATTRIBUTE,
		XML_ERROR_IDENTIFYING_TAG,
		XML_ERROR_PARSING_TEXT,
		XML_ERROR_PARSING_CDATA,
		XML_ERROR_PARSING_COMMENT,
		XML_ERROR_PARSING_DECLARATION,
		XML_ERROR_PARSING_UNKNOWN,
		XML_ERROR_EMPTY_DOCUMENT,
		XML_ERROR_MISMATCHED_ELEMENT,
		XML_ERROR_PARSING,

		XML_CAN_NOT_CONVERT_TEXT,
		XML_NO_TEXT_NODE
	};


	enum Whitespace {
		PRESERVE_WHITESPACE,
		COLLAPSE_WHITESPACE
	};
}	// tinyxml2


#include <assert.h>
#include "DynArray.h"
#include "MemPool.h"
#include "MemPoolT.h"
#include "StrPair.h"
#include "XMLUtil.h"

/*
#include "XMLVisitor.h"
#include "XMLUtil.h"
#include "XMLNode.h"
#include "XMLText.h"
#include "XMLComment.h"
#include "XMLDeclaration.h"
#include "XMLUnknown.h"
#include "XMLAttribute.h"
#include "XMLElement.h"
#include "XMLDocument.h"

#include "XMLHandle.h"
#include "XMLConstHandle.h"

#include "XMLPrinter.h"
*/


#endif // TINYXML2_INCLUDED