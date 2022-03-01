export module SourceStack;

export import MemoryBlock;

export import MemoryUtils;

export namespace hfog::Sources
{
	
	template <mem_t buffSize, mem_t alignment, typename alignFunc = hfog::MemoryUtils::align<alignment>>
	class Stack
	{

	public:
		MemoryBlock allocate(mem_t offset, mem_t size)
		{

			MemoryBlock memBlock;

			if (size == 0)
				return memBlock;

			const auto alignmentSize{ alignFunc()(size) };
			if (offset + alignmentSize > buffSize)
				return memBlock;

			memBlock.ptr = memBuffer + offset;
			if (memBlock.ptr != nullptr)
				memBlock.size = alignmentSize;

			return memBlock;

		}

		void deallocate([[maybe_unused]] MemoryBlock memBlock)
		{

		}

	private:
		std::byte memBuffer[buffSize];

	};

};