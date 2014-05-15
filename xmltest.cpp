#include "stdafx.h"
#include <cstring>
#include <cstdlib>
#include <ctime>
/**/
#include <direct.h>
#include <crtdbg.h>
//#include <windows.h>
_CrtMemState startMemState;
_CrtMemState endMemState;


//#include "XMLNode.h"
#include "XMLDocument.h"
#include "XMLPrinter.h"



using namespace tinyxml2;

int gPass = 0;
int gFail = 0;


bool XMLTest(const char* testString, const char* expected, const char* found, bool echo = true, bool extraNL = false)
{
	bool pass = !strcmp(expected, found);
	if (pass)
		printf("[pass]");
	else
		printf("[fail]");

	if (!echo) {
		printf(" %s\n", testString);
	}
	else {
		if (extraNL) {
			printf(" %s\n", testString);
			printf("%s\n", expected);
			printf("%s\n", found);
		}
		else {
			printf(" %s [%s][%s]\n", testString, expected, found);
		}
	}

	if (pass)
		++gPass;
	else
		++gFail;
	return pass;
}


template< class T > bool XMLTest(const char* testString, T expected, T found, bool echo = true)
{
	bool pass = (expected == found);
	if (pass)
		printf("[pass]");
	else
		printf("[fail]");

	if (!echo)
		printf(" %s\n", testString);
	else
		printf(" %s [%d][%d]\n", testString, static_cast<int>(expected), static_cast<int>(found));

	if (pass)
		++gPass;
	else
		++gFail;
	return pass;
}


void NullLineEndings(char* p)
{
	while (p && *p) {
		if (*p == '\n' || *p == '\r') {
			*p = 0;
			return;
		}
		++p;
	}
}

/** @page Example-1 Load an XML File
*  @dontinclude ./xmltest.cpp
*  Basic XML file loading.
*  The basic syntax to load an XML file from
*  disk and check for an error. (ErrorID()
*  will return 0 for no error.)
*  @skip example_1()
*  @until }
*/
int example_1()
{
	XMLDocument doc;
	doc.LoadFile("resources/dream.xml");

	return doc.ErrorID();
}

/** @page Example-2 Parse an XML from char buffer
*  @dontinclude ./xmltest.cpp
*  Basic XML string parsing.
*  The basic syntax to parse an XML for
*  a char* and check for an error. (ErrorID()
*  will return 0 for no error.)
*  @skip example_2()
*  @until }
*/
int example_2()
{
	static const char* xml = "<element/>";	
	XMLDocument doc;
	doc.Parse(xml);

	return doc.ErrorID();
}

/** @page Example-3 Get information out of XML
@dontinclude ./xmltest.cpp
In this example, we navigate a simple XML
file, and read some interesting text. Note
that this example doesn't use error
checking; working code should check for null
pointers when walking an XML tree, or use
XMLHandle.

(The XML is an excerpt from "dream.xml").

@skip example_3()
@until </PLAY>";

The structure of the XML file is:

<ul>
<li>(declaration)</li>
<li>(dtd stuff)</li>
<li>Element "PLAY"</li>
<ul>
<li>Element "TITLE"</li>
<ul>
<li>Text "A Midsummer Night's Dream"</li>
</ul>
</ul>
</ul>

For this example, we want to print out the
title of the play. The text of the title (what
we want) is child of the "TITLE" element which
is a child of the "PLAY" element.

We want to skip the declaration and dtd, so the
method FirstChildElement() is a good choice. The
FirstChildElement() of the Document is the "PLAY"
Element, the FirstChildElement() of the "PLAY" Element
is the "TITLE" Element.

@until ( "TITLE" );

We can then use the convenience function GetText()
to get the title of the play.

@until title );

Text is just another Node in the XML DOM. And in
fact you should be a little cautious with it, as
text nodes can contain elements.

@verbatim
Consider: A Midsummer Night's <b>Dream</b>
@endverbatim

It is more correct to actually query the Text Node
if in doubt:

@until title );

Noting that here we use FirstChild() since we are
looking for XMLText, not an element, and ToText()
is a cast from a Node to a XMLText.
*/
int example_3()
{
	static const char* xml =
		"<?xml version=\"1.0\"?>"
		"<!DOCTYPE PLAY SYSTEM \"play.dtd\">"
		"<PLAY>"
		"<TITLE>A Midsummer Night's Dream</TITLE>"
		"</PLAY>";

	XMLDocument doc;
	doc.Parse(xml);

	XMLElement* titleElement = doc.FirstChildElement("PLAY")->FirstChildElement("TITLE");
	const char* title = titleElement->GetText();
	printf("Name of play (1): %s\n", title);

	XMLText* textNode = titleElement->FirstChild()->ToText();
	title = textNode->Value();
	printf("Name of play (2): %s\n", title);

	return doc.ErrorID();
}

/** @page Example-4 Read attributes and text information.
@dontinclude ./xmltest.cpp

There are fundamentally 2 ways of writing a key-value
pair into an XML file. (Something that's always annoyed
me about XML.) Either by using attributes, or by writing
the key name into an element and the value into
the text node wrapped by the element. Both approaches
are illustrated in this example, which shows two ways
to encode the value "2" into the key "v":

@skip example_4()
@until "</information>";

TinyXML-2 has accessors for both approaches.

When using an attribute, you navigate to the XMLElement
with that attribute and use the QueryIntAttribute()
group of methods. (Also QueryFloatAttribute(), etc.)

@skip XMLElement* attributeApproachElement
@until &v0 );

When using the text approach, you need to navigate
down one more step to the XMLElement that contains
the text. Note the extra FirstChildElement( "v" )
in the code below. The value of the text can then
be safely queried with the QueryIntText() group
of methods. (Also QueryFloatText(), etc.)

@skip XMLElement* textApproachElement
@until &v1 );
*/
bool example_4()
{
	static const char* xml =
		"<information>"
		"	<attributeApproach v='2' />"
		"	<textApproach>"
		"		<v>2</v>"
		"	</textApproach>"
		"</information>";

	XMLDocument doc;
	doc.Parse(xml);

	int v0 = 0;
	int v1 = 0;

	XMLElement* attributeApproachElement = doc.FirstChildElement()->FirstChildElement("attributeApproach");
	attributeApproachElement->QueryIntAttribute("v", &v0);

	XMLElement* textApproachElement = doc.FirstChildElement()->FirstChildElement("textApproach");
	textApproachElement->FirstChildElement("v")->QueryIntText(&v1);

	printf("Both values are the same: %d and %d\n", v0, v1);

	return !doc.Error() && (v0 == v1);
}

int main(int argc, const char **argv){
	//_MSC_VER visual studio 的版本号
#if defined( _MSC_VER ) && defined( DEBUG )
	//_CrtMemCheckpoint 函数在指定的任何时间创建调试堆的当前状态的快照
	_CrtMemCheckpoint(&startMemState);
#endif
	
	#if defined(_MSC_VER) || defined(MINGW32) || defined(__MINGW32__)
	#if defined __MINGW64_VERSION_MAJOR && defined __MINGW64_VERSION_MINOR
	//MINGW64: both 32 and 64-bit
	mkdir( "resources/out/" );
	#else
	_mkdir( "resources/out/" );
	#endif
	#else
	mkdir( "resources/out/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	#endif

	if (argc > 1) {
		XMLDocument* doc = new XMLDocument();
		clock_t startTime = clock();
		doc->LoadFile(argv[1]);
		clock_t loadTime = clock();
		int errorID = doc->ErrorID();
		delete doc; doc = 0;
		clock_t deleteTime = clock();

		printf("Test file '%s' loaded. ErrorID=%d\n", argv[1], errorID);
		if (!errorID) {
			printf("Load time=%u\n", (unsigned)(loadTime - startTime));
			printf("Delete time=%u\n", (unsigned)(deleteTime - loadTime));
			printf("Total time=%u\n", (unsigned)(deleteTime - startTime));
		}
		exit(0);
	}

	FILE* fp = fopen( "resources/dream.xml", "r" );
	if (!fp) {
		printf("Error opening test file 'dream.xml'.\n"
			"Is your working directory the same as where \n"
			"the xmltest.cpp and dream.xml file are?\n\n"
#if defined( _MSC_VER )
			"In windows Visual Studio you may need to set\n"
			"Properties->Debugging->Working Directory to '..'\n"
#endif
			);
		exit(1);
	}
	fclose( fp );
	
	XMLTest( "Example-1", 0, example_1() );
	XMLTest( "Example-2", 0, example_2() );
	XMLTest( "Example-3", 0, example_3() );
	XMLTest("Example-4", true, example_4());

	/* ------ Example 2: Lookup information. ----

	{
		static const char* test[] = { "<element />",
			"<element></element>",
			"<element><subelement/></element>",
			"<element><subelement></subelement></element>",
			"<element><subelement><subsub/></subelement></element>",
			"<!--comment beside elements--><element><subelement></subelement></element>",
			"<!--comment beside elements, this time with spaces-->  \n <element>  <subelement> \n </subelement> </element>",
			"<element attrib1='foo' attrib2=\"bar\" ></element>",
			"<element attrib1='foo' attrib2=\"bar\" ><subelement attrib3='yeehaa' /></element>",
			"<element>Text inside element.</element>",
			"<element><b></b></element>",
			"<element>Text inside and <b>bolded</b> in the element.</element>",
			"<outer><element>Text inside and <b>bolded</b> in the element.</element></outer>",
			"<element>This &amp; That.</element>",
			"<element attrib='This&lt;That' />",
			0
		};
		for (int i = 0; test[i]; ++i) {
			XMLDocument doc;
			doc.Parse(test[i]);
			printf("test[%d] is %s\n doc is ", i, test[i]);
			doc.Print();
			printf("--------------------%d--------------------------\n", i);
		}

	}
	*/
	getchar();
	return 0;
}