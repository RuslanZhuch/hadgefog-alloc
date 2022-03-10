#include "pch.h"

import MemoryUtils;
import AlgUnified;

import SourceHeap;

using namespace hfog::MemoryUtils::Literals;

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

