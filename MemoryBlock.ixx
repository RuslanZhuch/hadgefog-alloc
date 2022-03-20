module;
#include <utility>
export module MemoryBlock;
export import MemoryUtils;

export namespace hfog
{

	struct MemoryBlock
	{

		constexpr MemoryBlock() noexcept = default;
		constexpr MemoryBlock(byte_t* ptr, mem_t size) noexcept
			:ptr(ptr), size(size)
		{	}

		byte_t* ptr{ nullptr };
		mem_t size{ 0 };

	};

};