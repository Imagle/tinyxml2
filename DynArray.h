/*
	A dynamic array of Plain Old Data. Doesn't support constructors, etc.
	Has a small initial memory pool, so that low or no usage will not
	cause a call to new/delete
*/
#ifndef DYNARRAY_H
#define DYNARRAY_H

#include "stdafx.h"
#include "tinyxml.h"

namespace tinyxml2{

	template <class T, int INIT>
	class DynArray
	{
	public:
		DynArray< T, INIT >() {
			_mem = _pool;
			_allocated = INIT;
			_size = 0;
		}

		~DynArray() {
			if (_mem != _pool) {
				delete[] _mem;
			}
		}

		void Clear() {
			_size = 0;
		}

		void Push(T t) {
			EnsureCapacity(_size + 1);
			_mem[_size++] = t;
		}

		T* PushArr(int count) {
			EnsureCapacity(_size + count);
			T* ret = &_mem[_size];
			_size += count;
			return ret;
		}

		T Pop() {
			return _mem[--_size];
		}

		void PopArr(int count) {
			TIXMLASSERT(_size >= count);
			_size -= count;
		}

		bool Empty() const					{
			return _size == 0;
		}

		T& operator[](int i)				{
			assert(i >= 0 && i < _size);
			return _mem[i];
		}

		const T& operator[](int i) const	{
			assert(i >= 0 && i < _size);
			return _mem[i];
		}

		const T& PeekTop() const                            {
			TIXMLASSERT(_size > 0);
			return _mem[_size - 1];
		}

		int Size() const					{
			return _size;
		}

		int Capacity() const				{
			return _allocated;
		}

		const T* Mem() const				{
			return _mem;
		}

		T* Mem()							{
			return _mem;
		}

	private:
		void EnsureCapacity(int cap) {
			if (cap > _allocated) {
				int newAllocated = cap * 2;
				T* newMem = new T[newAllocated];
				memcpy(newMem, _mem, sizeof(T)*_size);	// warning: not using constructors, only works for PODs
				if (_mem != _pool) {
					delete[] _mem;
				}
				_mem = newMem;
				_allocated = newAllocated;
			}
		}

		T*  _mem;
		T   _pool[INIT];
		int _allocated;		// objects allocated
		int _size;			// number objects in use
	};
}
#endif