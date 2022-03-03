export module StructureFallback;

import MemoryBlock;
import MemoryUtils;

export namespace hfog::Structures
{

	template <typename PrimaryAlg, typename SecondaryAlg>
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
			}
			else if (this->secondary.getIsOwner(memBlock.ptr))
			{
				this->secondary.deallocate(memBlock);
			}
		}

	private:
		PrimaryAlg primary;
		SecondaryAlg secondary;

	};

};