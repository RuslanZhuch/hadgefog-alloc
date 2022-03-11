export module StructureSegregator;

import hfogCore;
import MemoryBlock;

export namespace hfog::Structures
{

	template <CtAllocator Left, CtAllocator Right, mem_t threshold>
	class Segregator
	{

	public:
		[[nodiscard]] MemoryBlock allocate(mem_t size) noexcept
		{

			if (size <= threshold)
				return this->left.allocate(size);

			return this->right.allocate(size);

		}

		void deallocate(const MemoryBlock& memBlock) noexcept
		{

			if (memBlock.size <= threshold)
				this->left.deallocate(memBlock);
			else
				this->right.deallocate(memBlock);

		}

		[[nodiscard]] bool getIsOwner(byte_t* ptr) noexcept
		{

			return this->left.getIsOwner(ptr) || this->right.getIsOwner(ptr);

		}

	private:
		Left left;
		Right right;

	};

};