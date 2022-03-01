#include "pch.h"

import AlgLinear;

import SourceHeap;

using namespace hfog::MemoryUtils::Literals;

TEST(AllocAlgorithms, tsLinear)
{

	using source_t = hfog::Sources::Heap;
	const auto alignment{ 16_B };

	hfog::Algorithms::Linear<source_t, alignment> algLinear;

	{
		const auto memBlock{ algLinear.allocate(32_B) };
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 32_B);
		algLinear.deallocate(memBlock);
	}
	{
		const auto memBlock{ algLinear.allocate(18_B) };
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 32_B);
		algLinear.deallocate(memBlock);
	}
	{
		const auto memBlock{ algLinear.allocate(40_B) };
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 48_B);
		algLinear.deallocate(memBlock);
	}
	{
		const auto memBlock{ algLinear.allocate(0_B) };
		EXPECT_EQ(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 0_B);
		algLinear.deallocate(memBlock);
	}

}