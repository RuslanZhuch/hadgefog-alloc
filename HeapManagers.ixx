module;
#include <cstdlib>

export module HeapManagers;
export import MemoryBlock;

export namespace hfog::Managers
{

	class HeapManager
	{

	public:
		MemoryBlock allocate(size_t numOfBytes)
		{
			MemoryBlock memBlock;
			memBlock.ptr = std::malloc(numOfBytes);
			if (memBlock.ptr != nullptr)
				memBlock.size = numOfBytes;
			return memBlock;
		}

		void deallocate(MemoryBlock memBlock)
		{
			std::free(memBlock.ptr);
		}

	};

};