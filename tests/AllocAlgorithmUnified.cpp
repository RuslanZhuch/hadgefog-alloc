#include "pch.h"

import MemoryUtils;
import AlgUnified;

import SourceHeap;
import SourceStack;
import SourceExt;

import hfogCore;

using namespace hfog::MemoryUtils::Literals;

static_assert(hfog::CtAllocator<hfog::Algorithms::Unified<hfog::Sources::Heap<hfog::GarbageWriter::Default>, 16_B, 4, 1>>);
static_assert(hfog::CtAllocator<hfog::Algorithms::Unified<hfog::Sources::External<hfog::GarbageWriter::Default>, 16_B, 4, 1>>);
static_assert(hfog::CtAllocator<hfog::Algorithms::Unified<hfog::Sources::Stack<128_B, hfog::GarbageWriter::Default>, 16_B, 4, 1>>);

TEST(AllocAlgorithms, tsUnified)
{

	using source_t = hfog::Sources::Heap<hfog::GarbageWriter::Default>;

	hfog::Algorithms::Unified<source_t, 16_B, 4, 4> unified;

	{

		const auto memBlock1{ unified.allocate(64_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 64_B);

		const auto memBlock2{ unified.allocate(64_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 64_B);

		const auto memBlock3{ unified.allocate(64_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 64_B);

		const auto memBlock4{ unified.allocate(64_B) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, 64_B);

		const auto memBlock5{ unified.allocate(64_B) };
		EXPECT_EQ(memBlock5.ptr, nullptr);

		unified.deallocate(memBlock4);

		const auto memBlock6{ unified.allocate(64_B) };
		EXPECT_NE(memBlock6.ptr, nullptr);
		EXPECT_EQ(memBlock6.size, 64_B);

		unified.deallocate(memBlock6);
		unified.deallocate(memBlock3);
		unified.deallocate(memBlock2);
		unified.deallocate(memBlock1);

	}

	{

		const auto memBlock1{ unified.allocate(32_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 32_B);

		const auto memBlock2{ unified.allocate(64_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 64_B);

		const auto memBlock3{ unified.allocate(64_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 64_B);

		const auto memBlock4{ unified.allocate(64_B) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, 64_B);

		const auto memBlock5{ unified.allocate(32_B) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, 32_B);

		unified.deallocate(memBlock1);

		const auto memBlock6{ unified.allocate(64_B) };
		EXPECT_EQ(memBlock6.ptr, nullptr);

		unified.deallocate(memBlock5);

		const auto memBlock7{ unified.allocate(64_B) };
		EXPECT_NE(memBlock7.ptr, nullptr);
		EXPECT_EQ(memBlock7.size, 64_B);

	}

}

TEST(AllocAlgorithms, tsUnifiedGaps)
{

	using source_t = hfog::Sources::Heap<hfog::GarbageWriter::Default>;

	hfog::Algorithms::Unified<source_t, 16_B, 4, 4> unified;

	{

		const auto memBlock1{ unified.allocate(48_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 48_B);

		const auto memBlock2{ unified.allocate(48_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 48_B);

		const auto memBlock3{ unified.allocate(48_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 48_B);

		const auto memBlock4{ unified.allocate(48_B) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, 48_B);

		const auto memBlock5{ unified.allocate(48_B) };
		EXPECT_EQ(memBlock5.ptr, nullptr);

		const auto memBlock6{ unified.allocate(16_B) };
		EXPECT_NE(memBlock6.ptr, nullptr);
		EXPECT_EQ(memBlock6.size, 16_B);

		const auto memBlock7{ unified.allocate(16_B) };
		EXPECT_NE(memBlock7.ptr, nullptr);
		EXPECT_EQ(memBlock7.size, 16_B);

		const auto memBlock8{ unified.allocate(16_B) };
		EXPECT_NE(memBlock8.ptr, nullptr);
		EXPECT_EQ(memBlock8.size, 16_B);

		const auto memBlock9{ unified.allocate(16_B) };
		EXPECT_NE(memBlock9.ptr, nullptr);
		EXPECT_EQ(memBlock9.size, 16_B);

		{
			const auto memBlock{ unified.allocate(16_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}


		unified.deallocate(memBlock1);
		{
			const auto memBlock{ unified.allocate(16_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}
		unified.deallocate(memBlock6);

		const auto memBlock10{ unified.allocate(16_B) };
		EXPECT_NE(memBlock10.ptr, nullptr);
		EXPECT_EQ(memBlock10.size, 16_B);

		const auto memBlock11{ unified.allocate(48_B) };
		EXPECT_NE(memBlock11.ptr, nullptr);
		EXPECT_EQ(memBlock11.size, 48_B);
		{
			const auto memBlock{ unified.allocate(16_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}

	}

}

TEST(AllocAlgorithms, tsUnifiedBadData)
{

	using source_t = hfog::Sources::Heap<hfog::GarbageWriter::Default>;

	hfog::Algorithms::Unified<source_t, 16_B, 4, 4> unified;

	{
		{
			const auto memBlock{ unified.allocate(65_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}

		const auto memBlock1{ unified.allocate(64_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 64_B);

		const auto memBlock2{ unified.allocate(40_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 48_B);

		const auto memBlock3{ unified.allocate(20_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 32_B);

		const auto memBlock4{ unified.allocate(8_B) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, 16_B);

		const auto memBlock5{ unified.allocate(12_B) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, 16_B);

		{
			const auto memBlock{ unified.allocate(65_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}

		const auto memBlock6{ unified.allocate(28_B) };
		EXPECT_NE(memBlock6.ptr, nullptr);
		EXPECT_EQ(memBlock6.size, 32_B);

		{
			const auto memBlock{ unified.allocate(48_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}

		unified.deallocate(memBlock2);

		{
			const auto memBlock{ unified.allocate(48_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}

		const auto memBlock7{ unified.allocate(28_B) };
		EXPECT_NE(memBlock7.ptr, nullptr);
		EXPECT_EQ(memBlock7.size, 32_B);

		unified.deallocate(memBlock4);

		const auto memBlock9{ unified.allocate(16_B) };
		EXPECT_NE(memBlock9.ptr, nullptr);
		EXPECT_EQ(memBlock9.size, 16_B);

		const auto memBlock10{ unified.allocate(64_B) };
		EXPECT_NE(memBlock10.ptr, nullptr);
		EXPECT_EQ(memBlock10.size, 64_B);

	}

}

TEST(AllocAlgorithms, tsUnifiedOneChunck)
{

	using source_t = hfog::Sources::Heap<hfog::GarbageWriter::Default>;

	hfog::Algorithms::Unified<source_t, 16_B, 4, 1> unified;

	{
		{
			const auto memBlock{ unified.allocate(65_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}

		const auto memBlock1{ unified.allocate(64_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 64_B);

		{
			const auto memBlock{ unified.allocate(64_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}

		unified.deallocate(memBlock1);

		const auto memBlock2{ unified.allocate(32_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 32_B);

		const auto memBlock3{ unified.allocate(16_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 16_B);

		const auto memBlock4{ unified.allocate(16_B) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, 16_B);

		unified.deallocate(memBlock2);
		{
			const auto memBlock{ unified.allocate(32_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}
		unified.deallocate(memBlock3);
		unified.deallocate(memBlock4);

		const auto memBlock5{ unified.allocate(32_B) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, 32_B);

		const auto memBlock6{ unified.allocate(32_B) };
		EXPECT_NE(memBlock6.ptr, nullptr);
		EXPECT_EQ(memBlock6.size, 32_B);

	}

}

TEST(AllocAlgorithms, tsUnifiedMemoryCheck)
{

	using source_t = hfog::Sources::Stack<16_B * 4 * 4, 
		hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>;

	hfog::Algorithms::Unified<source_t, 16_B, 4, 4> unified;

	{

		const auto memBlock1{ unified.allocate(64_B) };
		EXPECT_TRUE(getValuesAre(memBlock1.ptr, 0_B, 64_B, 0xFA));

		const auto memBlock2{ unified.allocate(40_B) };
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 48_B, 0xFA));

		const auto memBlock3{ unified.allocate(20_B) };
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, 32_B, 0xFA));

		const auto memBlock4{ unified.allocate(8_B) };
		EXPECT_TRUE(getValuesAre(memBlock4.ptr, 0_B, 16_B, 0xFA));

		const auto memBlock5{ unified.allocate(12_B) };
		EXPECT_TRUE(getValuesAre(memBlock5.ptr, 0_B, 16_B, 0xFA));

		const auto memBlock6{ unified.allocate(28_B) };
		EXPECT_TRUE(getValuesAre(memBlock6.ptr, 0_B, 32_B, 0xFA));

		unified.deallocate(memBlock2);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 48_B, 0xAF));

		const auto memBlock7{ unified.allocate(28_B) };
		EXPECT_TRUE(getValuesAre(memBlock7.ptr, 0_B, 32_B, 0xFA));

		unified.deallocate(memBlock4);
		EXPECT_TRUE(getValuesAre(memBlock4.ptr, 0_B, 16_B, 0xAF));

		const auto memBlock9{ unified.allocate(16_B) };
		EXPECT_TRUE(getValuesAre(memBlock9.ptr, 0_B, 16_B, 0xFA));

		const auto memBlock10{ unified.allocate(64_B) };
		EXPECT_TRUE(getValuesAre(memBlock10.ptr, 0_B, 64_B, 0xFA));

	}

}

TEST(AllocAlgorithms, tsUnifiedOwn)
{

	byte_t val{ 42 };

	using source_t = hfog::Sources::Stack<16_B * 4 * 4>;

	hfog::Algorithms::Unified<source_t, 16_B, 4, 4> unified;

	EXPECT_FALSE(unified.getIsOwner(&val));
	EXPECT_FALSE(unified.getIsOwner(nullptr));

	{

		const auto memBlock1{ unified.allocate(64_B) };
		EXPECT_TRUE(unified.getIsOwner(memBlock1.ptr));

		const auto memBlock2{ unified.allocate(40_B) };
		EXPECT_TRUE(unified.getIsOwner(memBlock2.ptr));

		const auto memBlock3{ unified.allocate(20_B) };
		EXPECT_TRUE(unified.getIsOwner(memBlock3.ptr));

		const auto memBlock4{ unified.allocate(8_B) };
		EXPECT_TRUE(unified.getIsOwner(memBlock4.ptr));

		const auto memBlock5{ unified.allocate(12_B) };
		EXPECT_TRUE(unified.getIsOwner(memBlock5.ptr));

		const auto memBlock6{ unified.allocate(28_B) };
		EXPECT_TRUE(unified.getIsOwner(memBlock6.ptr));

		unified.deallocate(memBlock2);
		EXPECT_TRUE(unified.getIsOwner(memBlock2.ptr));

		const auto memBlock7{ unified.allocate(28_B) };
		EXPECT_TRUE(unified.getIsOwner(memBlock7.ptr));

		unified.deallocate(memBlock4);
		EXPECT_FALSE(unified.getIsOwner(memBlock2.ptr));
		EXPECT_FALSE(unified.getIsOwner(memBlock4.ptr));

		const auto memBlock8{ unified.allocate(16_B) };
		EXPECT_TRUE(unified.getIsOwner(memBlock8.ptr));

		const auto memBlock9{ unified.allocate(64_B) };
		EXPECT_TRUE(unified.getIsOwner(memBlock9.ptr));

		EXPECT_TRUE(unified.getIsOwner(memBlock1.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock3.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock5.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock6.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock7.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock8.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock9.ptr));

		//It is not garanteed that this partucalar block use this memory address
		//In this case some other memBlock use this address
		EXPECT_TRUE(unified.getIsOwner(memBlock2.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock4.ptr));

	}

}