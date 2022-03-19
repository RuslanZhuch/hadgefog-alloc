#include "pch.h"

#include <type_traits>

import hfogAlloc;

using namespace hfog::MemoryUtils::Literals;

TEST(Predefines, tsAlgStack)
{

	static_assert(std::is_same_v<
		hfog::Alloc::Stack<16_B, 128_B>,
		hfog::Algorithms::Stack<hfog::Sources::Stack<128_B, hfog::GarbageWriter::Default>, 16_B>
	>);

	static_assert(std::is_same_v<
		hfog::Alloc::StackExt<16_B>,
		hfog::Algorithms::Stack<hfog::Sources::External<hfog::GarbageWriter::Default>, 16_B>
	>);

}

TEST(Predefines, tsAlgNull)
{

	static_assert(std::is_same_v<
		hfog::Alloc::Null,
		hfog::Algorithms::Null
	>);

}


TEST(Predefines, tsAlgUnified)
{

	static_assert(std::is_same_v<
		hfog::Alloc::Unified<16_B, 4, 512_B>,
		hfog::Algorithms::Unified<hfog::Sources::Stack<512_B, hfog::GarbageWriter::Default>, 16_B, 4, 8>
	>);

	static_assert(std::is_same_v<
		hfog::Alloc::UnifiedExt<16_B, 4, 512_B>,
		hfog::Algorithms::Unified<hfog::Sources::External<hfog::GarbageWriter::Default>, 16_B, 4, 8>
	>);

}