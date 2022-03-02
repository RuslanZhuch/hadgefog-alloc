export module AlgLinear;

export import MemoryUtils;
import MemoryBlock;
import SourcesCommon;

export namespace hfog::Algorithms
{

	template <Sources::CtSource Source, mem_t alignment, typename alignFunc = hfog::MemoryUtils::align<alignment>>
	class Linear
	{
	public:
		MemoryBlock allocate(mem_t numOfBytes)
		{
			const auto alignNumOfBytes{ alignFunc()(numOfBytes) };
			const auto outMemory{ this->source.getMemory(this->currMemPoint, alignNumOfBytes) };
			if (outMemory.ptr != nullptr)
			{
				this->currMemPoint += alignNumOfBytes;
			}
			return outMemory;
		}

		void deallocate([[maybe_unused]] MemoryBlock block)
		{
			this->source.releaseAllMemory();
			this->currMemPoint = 0;
		}

	private:
		Source source;
		mem_t currMemPoint{ 0 };

	};

};