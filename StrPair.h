/*
	A class that wraps strings. Normally stores the start and end
	pointers into the XML file itself, and will apply normalization
	and entity translation if actually read. Can also store (and memory
	manage) a traditional char[]
*/

#ifndef STRPAIR_H
#define STRPAIR_H

#include "stdafx.h"

namespace tinyxml2{

	class StrPair
	{
	public:
		enum {
			NEEDS_ENTITY_PROCESSING = 0x01,
			NEEDS_NEWLINE_NORMALIZATION = 0x02,
			COLLAPSE_WHITESPACE = 0x04,

			TEXT_ELEMENT = NEEDS_ENTITY_PROCESSING | NEEDS_NEWLINE_NORMALIZATION,
			TEXT_ELEMENT_LEAVE_ENTITIES = NEEDS_NEWLINE_NORMALIZATION,
			ATTRIBUTE_NAME = 0,
			ATTRIBUTE_VALUE = NEEDS_ENTITY_PROCESSING | NEEDS_NEWLINE_NORMALIZATION,
			ATTRIBUTE_VALUE_LEAVE_ENTITIES = NEEDS_NEWLINE_NORMALIZATION,
			COMMENT = NEEDS_NEWLINE_NORMALIZATION
		};

		StrPair() : _flags(0), _start(0), _end(0) {}
		~StrPair();

		void Set(char* start, char* end, int flags) {
			Reset();
			_start = start;
			_end = end;
			_flags = flags | NEEDS_FLUSH;
		}

		const char* GetStr();

		bool Empty() const {
			return _start == _end;
		}

		void SetInternedStr(const char* str) {
			Reset();
			_start = const_cast<char*>(str);
		}

		void SetStr(const char* str, int flags = 0);

		char* ParseText(char* in, const char* endTag, int strFlags);
		char* ParseName(char* in);

	private:
		void Reset();
		void CollapseWhitespace();

		enum {
			NEEDS_FLUSH = 0x100,
			NEEDS_DELETE = 0x200
		};

		// After parsing, if *_end != 0, it can be set to zero.
		int     _flags;
		char*   _start;
		char*   _end;
	};
}
#endif