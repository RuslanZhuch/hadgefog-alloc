#include <memory>
export module MemoryBlock;

export namespace hfog
{

	struct MemoryBlock
	{

		std::byte* ptr{ nullptr };
		size_t size{ 0 };

	};

};