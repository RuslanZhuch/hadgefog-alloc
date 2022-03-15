#include "pch.h"

#include <type_traits>

import hfogAlloc;

using namespace hfog::MemoryUtils::Literals;

TEST(Predefines, tsAlgLinear)
{

	static_assert(std::is_same_v<
		hfog::Alloc::Linear<16_B, 128_B>,
		hfog::Algorithms::Linear<hfog::Sources::Stack<128_B, hfog::GarbageWriter::Default>, 16_B>
	>);

	static_assert(std::is_same_v<
		hfog::Alloc::LinearExt<16_B>,
		hfog::Algorithms::Linear<hfog::Sources::External<hfog::GarbageWriter::Default>, 16_B>
	>);

	static_assert(std::is_same_v<
		hfog::Alloc::LinearHeap<16_B>,
		hfog::Algorithms::Linear<hfog::Sources::Heap<hfog::GarbageWriter::Default>, 16_B>
	>);

}

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

	static_assert(std::is_same_v<
		hfog::Alloc::StackHeap<16_B>,
		hfog::Algorithms::Stack<hfog::Sources::Heap<hfog::GarbageWriter::Default>, 16_B>
	>);

}