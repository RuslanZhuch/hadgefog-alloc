module;
#include <cstdlib>
#include <cstring>
#include <cstddef>

#pragma warning(disable : 5045)

export module SourceHeap;
export import MemoryBlock;

import MemoryUtils;
export import GarbageWriter;

using namespace hfog::MemoryUtils::Literals;


export namespace hfog::Sources
{

	template<GarbageWriter::CtGarbageWriter<byte_t> garbageWriterOp = GarbageWriter::Default>
	class Heap
	{
		struct MemIslandPtr
		{
			mem_t offset{};
			byte_t* island{ nullptr };
			MemIslandPtr* next{ nullptr };
		};

	public:
		[[nodiscard]] MemoryBlock getMemory(mem_t offset, mem_t size) noexcept
		{

			MemoryBlock memBlock;

			if (auto fIsland{ this->findIslandByOffset(offset) }; fIsland != nullptr)
			{
				memBlock.ptr = fIsland->island;
				memBlock.size = size;
				garbageWriterOp::write(memBlock.ptr, 0, size);
				return memBlock;
			}

			MemIslandPtr* newIsland{ static_cast<MemIslandPtr*>(std::malloc(sizeof(MemIslandPtr))) };
			std::memset(newIsland, 0, sizeof(MemIslandPtr));
			if (this->memoryIslands == nullptr)
			{
				this->memoryIslands = newIsland;
				this->lastMemoryIsland = this->memoryIslands;
			}
			else
			{
				this->lastMemoryIsland->next = newIsland;
				this->lastMemoryIsland = newIsland;
			}

			this->lastMemoryIsland->island = static_cast<byte_t*>(std::malloc(size));
			this->lastMemoryIsland->offset = offset;

			memBlock.ptr = this->lastMemoryIsland->island;
			if (memBlock.ptr != nullptr)
				memBlock.size = size;

			garbageWriterOp::init(memBlock.ptr, 0, size);
			garbageWriterOp::write(memBlock.ptr, 0, size);
			return memBlock;

		}

		void releaseMemory(const MemoryBlock& memBlock) noexcept
		{

			auto currIslandPtr{ this->memoryIslands };
			MemIslandPtr* prevIslandPtr{ nullptr };

			while (currIslandPtr != nullptr)
			{
				if (currIslandPtr->island == memBlock.ptr)
				{
					garbageWriterOp::clear(memBlock.ptr, 0, memBlock.size);
					std::free(memBlock.ptr);
					if (prevIslandPtr != nullptr)
					{
						prevIslandPtr->next = currIslandPtr->next;
					}
					else
					{
						if (this->memoryIslands == currIslandPtr)
							this->memoryIslands = this->memoryIslands->next;
					}
					std::free(currIslandPtr);
					return;
				}
				prevIslandPtr = currIslandPtr;
				currIslandPtr = currIslandPtr->next;
			}

		}

		void releaseAllMemory() noexcept
		{
			auto currIslandPtr{ this->memoryIslands };
			while (currIslandPtr != nullptr)
			{
				std::free(currIslandPtr->island);
				currIslandPtr = currIslandPtr->next;
			}
			std::free(this->memoryIslands);
			this->memoryIslands = nullptr;
			this->lastMemoryIsland = nullptr;
		}

		[[nodiscard]] mem_t getOffset(byte_t* ptr) const noexcept
		{

			auto currIsland{ this->memoryIslands };
			while (currIsland != nullptr)
			{
				if (currIsland->island == ptr)
					return currIsland->offset;
				currIsland = currIsland->next;
			}

			return invalidMem_t;

		}

	private:
		[[nodiscard]] MemIslandPtr* findIslandByOffset(mem_t offset) const noexcept
		{
			auto currIsland{ this->memoryIslands };
			while (currIsland != nullptr)
			{
				if (currIsland->offset == offset)
					return currIsland;
				currIsland = currIsland->next;
			}
			return nullptr;
		}

	private:
		MemIslandPtr* memoryIslands{ nullptr };
		MemIslandPtr* lastMemoryIsland{ nullptr };

	};

};