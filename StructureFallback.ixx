export module StructureFallback;

import MemoryBlock;
import MemoryUtils;

import hfogCore;

export namespace hfog::Structures
{

	template <CtAllocator PrimaryAlg, CtAllocator SecondaryAlg>
	class Fallback
	{

	public:
		[[nodiscard]] MemoryBlock allocate(mem_t size) noexcept
		{
			const auto primaryBlock{ this->primary.allocate(size) };
			if (primaryBlock.ptr == nullptr)
				return this->secondary.allocate(size);
			return primaryBlock;
		}

		void deallocate(MemoryBlock memBlock) noexcept
		{

			if (this->primary.getIsOwner(memBlock.ptr))
			{
				this->primary.deallocate(memBlock);
				return;
			}

			this->secondary.deallocate(memBlock);
			
		}

		[[nodiscard]] bool getIsOwner(byte_t* ptr) noexcept
		{
			return this->primary.getIsOwner(ptr) || this->secondary.getIsOwner(ptr);
		}

	private:
		PrimaryAlg primary;
		SecondaryAlg secondary;

	};

};