export module hfog.Algorithms.Ring;
import hfog.Sources.Common;
import hfog.MemoryUtils;
import hfog.MemoryBlock;

export namespace hfog::Algorithms
{

	template <Sources::CtSource Source, mem_t alignment, MemoryUtils::CtAlign alignFunc = MemoryUtils::Align<alignment>>
	class Ring
	{

	public:
		Ring(const Ring&) = delete;
		Ring& operator=(const Ring&) = delete;

		Ring(Ring&&) = default;
		Ring& operator=(Ring&&) = default;

		template <typename ... Args>
		Ring(Args ... args) noexcept
			:source(args...)
		{}

		[[nodiscard]] MemoryBlock allocate(mem_t numOfBytes) noexcept
		{
			const auto alignNumOfBytes{ alignFunc::compute(numOfBytes) };

			if (this->allocatedBytes + alignNumOfBytes > this->source.getMemorySize())
				return {};

			const auto leftSizeFreeBytes{ this->source.getMemorySize() - this->allocateMemPoint };
			
			mem_t newRoundupPoint{ invalidMem_t };
			mem_t tryAllocatePoint{ this->allocateMemPoint };
			if (leftSizeFreeBytes < alignNumOfBytes)
			{
				if (this->deallocateMemPoint < alignNumOfBytes)
					return {};
				newRoundupPoint = this->allocateMemPoint;
				tryAllocatePoint = 0;
			}

			const auto outMemory{ this->source.getMemory(tryAllocatePoint, alignNumOfBytes) };
			if (outMemory.ptr != nullptr)
			{
				this->allocateMemPoint = tryAllocatePoint;

				if (this->roundupPoint == invalidMem_t)
					this->roundupPoint = newRoundupPoint;

				this->allocateMemPoint = this->shiftPoint(this->allocateMemPoint, alignNumOfBytes);
				this->allocatedBytes += alignNumOfBytes;
			}
			return outMemory;
		}

		void deallocate() noexcept
		{
			this->source.releaseAllMemory();
			this->allocateMemPoint = 0;
			this->deallocateMemPoint = 0;
			this->allocatedBytes = 0;
		}

		void deallocate(const MemoryBlock& block) noexcept
		{
			if (this->allocatedBytes == 0)
				return;

			if (this->deallocateMemPoint == this->roundupPoint)
			{
				this->deallocateMemPoint = 0;
				this->roundupPoint = invalidMem_t;
			}

			const auto blockPtr{ block.ptr };
			const auto blockSize{ block.size };

			const auto sourcePtr{ this->source.getMemory(this->deallocateMemPoint, blockSize) };
			if (blockPtr == sourcePtr.ptr)
			{
				this->source.releaseMemory(block);
				this->deallocateMemPoint = this->shiftPoint(this->deallocateMemPoint, blockSize);

				this->allocatedBytes -= blockSize;
			}
		}

		[[nodiscard]] bool getIsOwner(byte_t* ptr) const noexcept
		{
			const auto offset{ this->source.getOffset(ptr) };
			const auto bOwnerNormal{ (offset < this->allocateMemPoint) && (offset >= this->deallocateMemPoint) };
			const auto bOwnerRightRound{ (offset > this->allocateMemPoint) && (offset >= this->deallocateMemPoint) };
			const auto bOwnerLeftRound{ (offset < this->allocateMemPoint) && (offset < this->deallocateMemPoint) };
			return (offset != invalidMem_t) && (bOwnerNormal || bOwnerRightRound || bOwnerLeftRound);
		}

	private:
		auto shiftPoint(mem_t currPoint, mem_t amount)
		{
			const auto newPointRaw{ currPoint + amount };
			const auto bTailExist{ newPointRaw >= this->source.getMemorySize() };
			if (!bTailExist)
				return newPointRaw;

			const auto tail{ newPointRaw - this->source.getMemorySize() };
			const auto newPoint{ tail };
			return newPoint;
		}

	private:
		Source source;
		mem_t allocatedBytes{ 0 };
		mem_t allocateMemPoint{ 0 };
		mem_t deallocateMemPoint{ 0 };
		mem_t roundupPoint{ invalidMem_t };

	};

};