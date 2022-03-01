export module SourceExt;

export import MemoryBlock;

export import MemoryUtils;

export namespace hfog::Sources
{


	template <size_t alignment, typename alignFunc = hfog::MemoryUtils::align<alignment>>
	class External
	{

	public:
		External(MemoryBlock extMemory)
			:extMemoryBlock(extMemory)
		{}

		MemoryBlock allocate(mem_t offset, size_t size)
		{

			MemoryBlock memBlock;

			if (this->extMemoryBlock.ptr == nullptr ||
				size == 0)
				return memBlock;

			const auto alignmentSize{ alignFunc()(size) };
			if (offset + alignmentSize > this->extMemoryBlock.size)
				return memBlock;

			memBlock.ptr = extMemoryBlock.ptr + offset;
			if (memBlock.ptr != nullptr)
				memBlock.size = alignmentSize;

			return memBlock;

		}

		void deallocate([[maybe_unused]] MemoryBlock memBlock)
		{

		}

	private:
		MemoryBlock extMemoryBlock;

	};

};