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

		void deallocate(const MemoryBlock& block) noexcept
		{
			const auto blockPtr{ block.ptr };
			const auto blockSize{ block.size };

			const auto memoryPoint{ this->currMemPoint - blockSize };
			const auto sourcePtr{ this->source.getMemory(memoryPoint, blockSize) };

			//Remove branch for speedup
			//Doest solve x64 slowdown
			if (sourcePtr.ptr == blockPtr)
			{
				this->source.releaseMemory(sourcePtr);
				this->currMemPoint = memoryPoint;
			}
		}

		[[nodiscard]] bool getIsOwner(byte_t* ptr) noexcept
		{
			const auto offset{ this->source.getOffset(ptr) };
			return offset < this->currMemPoint;
		}

	private:
		Source source;
		mem_t currMemPoint{ 0 };

	};

};