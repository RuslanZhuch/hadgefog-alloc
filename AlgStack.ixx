export module hfog.Algorithms.Stack;
import hfog.Sources.Common;
import hfog.MemoryUtils;
import hfog.MemoryBlock;

export namespace hfog::Algorithms
{

	template <Sources::CtSource Source, mem_t alignment, MemoryUtils::CtAlign alignFunc = MemoryUtils::Align<alignment>>
	class Stack
	{

	public:
		Stack(const Stack&) = delete;
		Stack& operator=(const Stack&) = delete;

		Stack(Stack&&) = default;
		Stack& operator=(Stack&&) = default;

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

		void deallocate() noexcept
		{
			this->source.releaseAllMemory();
			this->currMemPoint = 0;
		}

		void deallocate(const MemoryBlock& block) noexcept
		{
			const auto blockPtr{ block.ptr };
			const auto blockSize{ block.size };

			const auto memoryPoint{ this->currMemPoint - blockSize };
			const auto sourcePtr{ this->source.getMemory(memoryPoint, blockSize) };

			if (sourcePtr.ptr == blockPtr)
			{
				this->source.releaseMemory(sourcePtr);
				this->currMemPoint = memoryPoint;
			}
		}

		[[nodiscard]] bool getIsOwner(byte_t* ptr) const noexcept
		{
			const auto offset{ this->source.getOffset(ptr) };
			return offset < this->currMemPoint;
		}

	private:
		Source source;
		mem_t currMemPoint{ 0 };

	};

};