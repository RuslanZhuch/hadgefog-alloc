export module hfog.Structures.Segregator;

import hfog.Core;
import hfog.MemoryBlock;

export namespace hfog::Structures
{

	template <CtAllocator Left, CtAllocator Right, mem_t threshold>
	class Segregator
	{

	public:
		Segregator() = default;
		Segregator(const Segregator&) = delete;
		Segregator& operator=(const Segregator&) = delete;

		Segregator(Segregator&&) = default;
		Segregator& operator=(Segregator&&) = default;

		[[nodiscard]] MemoryBlock allocate(mem_t size) noexcept
		{

			if (size <= threshold)
				return this->left.allocate(size);

			return this->right.allocate(size);

		}

		void deallocate() noexcept
		{

			this->primary.deallocate();
			this->secondary.deallocate();

		}

		void deallocate(const MemoryBlock& memBlock) noexcept
		{

			if (memBlock.size <= threshold)
				this->left.deallocate(memBlock);
			else
				this->right.deallocate(memBlock);

		}

		[[nodiscard]] bool getIsOwner(byte_t* ptr) const noexcept
		{

			return this->left.getIsOwner(ptr) || this->right.getIsOwner(ptr);

		}

	private:
		Left left;
		Right right;

	};

};