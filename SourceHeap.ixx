module;
#include <cstdlib>

export module SourceHeap;
export import MemoryBlock;

import MemoryUtils;

using namespace hfog::MemoryUtils::Literals;

export namespace hfog::Sources
{

	class Heap
	{

	public:
		MemoryBlock allocate(size_t size)
		{
			MemoryBlock memBlock;
			if (size == 0_B)
				return memBlock;

			memBlock.ptr = static_cast<std::byte*>(std::malloc(size));
			if (memBlock.ptr != nullptr)
				memBlock.size = size;

			return memBlock;
		}

		void deallocate(MemoryBlock memBlock)
		{
			std::free(memBlock.ptr);
		}

		[[nodiscard]] auto getNumOfAllocatedBytes() const noexcept
		{

		}

	private:
		mem_t

	};

};