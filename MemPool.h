/*
	Parent virtual class of a pool for fast allocation
	and deallocation of objects.
*/
#ifndef MEMPOOL_H
#define MEMPOOL_H

#include "stdafx.h"

namespace tinyxml2{

	class MemPool
	{
	public:
		MemPool() {}
		virtual ~MemPool() {}

		virtual int ItemSize() const = 0;
		virtual void* Alloc() = 0;
		virtual void Free(void*) = 0;
		virtual void SetTracked() = 0;
	};
}
#endif