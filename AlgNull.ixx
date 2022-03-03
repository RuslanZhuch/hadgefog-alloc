export module AlgNull;

export import MemoryBlock;

export namespace hfog::Algorithms
{

	class Null
	{
	public:
		[[nodiscard]] MemoryBlock allocate([[maybe_unused]] mem_t numOfBytes) noexcept
		{
			return MemoryBlock();
		}

		void deallocate([[maybe_unused]] MemoryBlock block) noexcept
		{	}

		[[nodiscard]] bool getIsOwner([[maybe_unused]] byte_t* ptr) noexcept
		{
			return false;
		}

	};

};