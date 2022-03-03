module;
#include <cstring>
export module MemoryBlock;
export import MemoryUtils;

export namespace hfog
{

	struct MemoryBlock
	{

		byte_t* ptr{ nullptr };
		mem_t size{ 0 };

	};

};