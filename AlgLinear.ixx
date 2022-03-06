export module AlgLinear;

export import MemoryUtils;
import MemoryBlock;
import SourcesCommon;

export namespace hfog::Algorithms
{

	template <Sources::CtSource Source, mem_t alignment, MemoryUtils::CtAlign alignFunc = MemoryUtils::Align<alignment>>
	class Linear
	{
	public:
		template <typename ... Args>
		Linear(Args ... args) noexcept
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

		void deallocate([[maybe_unused]] MemoryBlock block) noexcept
		{
			this->source.releaseAllMemory();
			this->currMemPoint = 0;
		}

		[[nodiscard]] bool getIsOwner(byte_t* ptr)
		{
			const auto offset{ this->source.getOffset(ptr) };
			return offset < this->currMemPoint;
		}

	private:
		Source source;
		mem_t currMemPoint{ 0 };

	};

};