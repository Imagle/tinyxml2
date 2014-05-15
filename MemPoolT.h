/*
	Template child class to create pools of the correct type.
*/
#ifndef MEMPOOLT_H
#define MEMPOOLT_H

#include "MemPool.h"
#include "DynArray.h"

namespace tinyxml2{

	template< int SIZE >
	class MemPoolT : public MemPool
	{
	public:
		MemPoolT() : _root(0), _currentAllocs(0), _nAllocs(0), _maxAllocs(0), _nUntracked(0)	{}
		~MemPoolT() {
			// Delete the blocks.
			for (int i = 0; i < _blockPtrs.Size(); ++i) {
				delete _blockPtrs[i];
			}
		}

		virtual int ItemSize() const	{
			return SIZE;
		}
		int CurrentAllocs() const		{
			return _currentAllocs;
		}

		virtual void* Alloc() {
			if (!_root) {
				// Need a new block.
				Block* block = new Block();
				_blockPtrs.Push(block);

				for (int i = 0; i<COUNT - 1; ++i) {
					block->chunk[i].next = &block->chunk[i + 1];
				}
				block->chunk[COUNT - 1].next = 0;
				_root = block->chunk;
			}
			void* result = _root;
			_root = _root->next;

			++_currentAllocs;
			if (_currentAllocs > _maxAllocs) {
				_maxAllocs = _currentAllocs;
			}
			_nAllocs++;
			_nUntracked++;
			return result;
		}
		virtual void Free(void* mem) {
			if (!mem) {
				return;
			}
			--_currentAllocs;
			Chunk* chunk = (Chunk*)mem;
#ifdef DEBUG
			memset( chunk, 0xfe, sizeof(Chunk) );
#endif
			chunk->next = _root;
			_root = chunk;
		}
		void Trace(const char* name) {
			printf("Mempool %s watermark=%d [%dk] current=%d size=%d nAlloc=%d blocks=%d\n",
				name, _maxAllocs, _maxAllocs*SIZE / 1024, _currentAllocs, SIZE, _nAllocs, _blockPtrs.Size());
		}

		void SetTracked() {
			_nUntracked--;
		}

		int Untracked() const {
			return _nUntracked;
		}

		// This number is perf sensitive. 4k seems like a good tradeoff on my machine.
		// The test file is large, 170k.
		// Release:		VS2010 gcc(no opt)
		//		1k:		4000
		//		2k:		4000
		//		4k:		3900	21000
		//		16k:	5200
		//		32k:	4300
		//		64k:	4000	21000
		enum { COUNT = (4 * 1024) / SIZE }; // Some compilers do not accept to use COUNT in private part if COUNT is private

	private:
		union Chunk {
			Chunk*  next;
			char    mem[SIZE];
		};
		struct Block {
			Chunk chunk[COUNT];
		};
		DynArray< Block*, 10 > _blockPtrs;
		Chunk* _root;

		int _currentAllocs;
		int _nAllocs;
		int _maxAllocs;
		int _nUntracked;
	};
}
#endif