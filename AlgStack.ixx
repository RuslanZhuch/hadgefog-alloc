export module AlgStack;
import SourcesCommon;
import MemoryUtils;
import MemoryBlock;

export namespace hfog::Algorithms
{

	template <Sources::CtSource Source, mem_t alignment, MemoryUtils::CtAlign alignFunc = MemoryUtils::Align<alignment>>
	class Stack
	{

	public:
		template <typename ... Args>
		Stack(Args ... args) noexcept
			:source(args...)
		{}

		[[nodiscard]] MemoryBlock allocate(mem_t numOfBytes) noexcept
		{
			const auto alignNumOfBytes{ alignFunc::compute(numOfBytes) };
			const auto outMemory{ this->source.getMemory(this->currMemPoint, alignNumOfBytes) };
			if (outMemory.ptr != nullptr)
			{
				this->currMemPoint += alignNumOfBytes;
			}
			return outMemory;
		}

		void deallocate(MemoryBlock block) noexcept
		{
			const auto memoryPoint{ this->currMemPoint - block.size };
			const auto sourcePtr{ this->source.getMemory(memoryPoint, block.size) };
			if (sourcePtr.ptr == block.ptr)
			{
				this->source.releaseMemory(block);
				currMemPoint = memoryPoint;
			}
		}

	private:
		Source source;
		mem_t currMemPoint{ 0 };

	};

};