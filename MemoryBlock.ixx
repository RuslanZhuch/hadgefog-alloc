module;
#include <memory>
export module MemoryBlock;
import MemoryUtils;

export namespace hfog
{

	struct MemoryBlock
	{

		byte_t* ptr{ nullptr };
		size_t size{ 0 };

	};

};