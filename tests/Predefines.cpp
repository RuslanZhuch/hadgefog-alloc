#include "pch.h"

#include <type_traits>

import hfogAlloc;

using namespace hfog::MemoryUtils::Literals;

TEST(Predefines, tsAlgLinear)
{

	static_assert(std::is_same_v<
		hfog::Alloc::LinearStack<16_B, 128_B>,
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