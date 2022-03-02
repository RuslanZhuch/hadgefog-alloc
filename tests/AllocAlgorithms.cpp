#include "pch.h"

import AlgLinear;

import SourceHeap;

using namespace hfog::MemoryUtils::Literals;

TEST(AllocAlgorithms, tsLinear)
{

	using source_t = hfog::Sources::Heap<hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>;
	const auto alignment{ 16_B };

	hfog::Algorithms::Linear<source_t, alignment> algLinear;

	{
		const auto memBlock{ algLinear.allocate(32_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xFA));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 32_B);
		algLinear.deallocate(memBlock);
	}
	{
		const auto memBlock{ algLinear.allocate(18_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xFA));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 32_B);
		algLinear.deallocate(memBlock);
	}
	{
		const auto memBlock{ algLinear.allocate(40_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 48_B, 0xFA));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 48_B);
		algLinear.deallocate(memBlock);
	}
	{
		const auto memBlock{ algLinear.allocate(0_B) };
		EXPECT_EQ(memBlock.size, 0_B);
		algLinear.deallocate(memBlock);
	}
	{
		const auto memBlock{ algLinear.allocate(40_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 48_B, 0xFA));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 48_B);

		const auto memBlock2{ algLinear.allocate(40_B) };
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 48_B, 0xFA));
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 48_B);

		algLinear.deallocate(memBlock);
	}

}