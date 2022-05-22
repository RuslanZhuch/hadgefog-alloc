module;
#include <utility>
export module hfog.MemoryBlock;
export import hfog.MemoryUtils;

export namespace hfog
{

	struct MemoryBlock
	{

		MemoryBlock() noexcept = default;
		constexpr MemoryBlock(byte_t* ptr, mem_t size) noexcept
			:ptr(ptr), size(size)
		{	}

		byte_t* ptr{ nullptr };
		mem_t size{ 0 };

	};

};