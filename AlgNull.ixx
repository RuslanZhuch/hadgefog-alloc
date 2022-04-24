export module hfog.Algorithms.Null;

export import hfog.MemoryBlock;

export namespace hfog::Algorithms
{

	class Null
	{
	public:
		Null() = default;
		Null(const Null&) = delete;
		Null& operator=(const Null&) = delete;

		Null(Null&&) = default;
		Null& operator=(Null&&) = default;

		[[nodiscard]] constexpr MemoryBlock allocate([[maybe_unused]] mem_t numOfBytes) const noexcept
		{
			return MemoryBlock();
		}

		constexpr void deallocate() const noexcept
		{	}

		constexpr void deallocate([[maybe_unused]] const MemoryBlock& block) const noexcept
		{	}

		[[nodiscard]] constexpr bool getIsOwner([[maybe_unused]] byte_t* ptr) const noexcept
		{
			return false;
		}

	};

};