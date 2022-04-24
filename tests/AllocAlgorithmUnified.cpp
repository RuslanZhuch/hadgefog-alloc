#include "pch.h"

import hfog.MemoryUtils;
import hfog.Algorithms.Unified;

import hfog.Sources.Stack;
import hfog.Sources.Ext;

import hfog.Core;

using namespace hfog::MemoryUtils::Literals;

static_assert(hfog::CtAllocator<hfog::Algorithms::Unified<hfog::Sources::External<hfog::GarbageWriter::Default>, 128_B, 4_kB>>);
static_assert(hfog::CtAllocator<hfog::Algorithms::Unified<hfog::Sources::Stack<128_kB, hfog::GarbageWriter::Default>, 128_B, 8_kB>>);

TEST(AllocAlgorithms, tsUnified)
{

	byte_t extBuffer[512_kB];
	hfog::MemoryBlock extBlock;
	extBlock.ptr = extBuffer;
	extBlock.size = sizeof(extBuffer);

	using source_t = hfog::Sources::External<hfog::GarbageWriter::Default>;

	hfog::Algorithms::Unified<source_t, 128_B, 256_kB> unified(extBlock);

	for (size_t numOfIters{ 0 }; numOfIters < 2; ++numOfIters)
	{

		const auto memBlock1{ unified.allocate(80_kB) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 80_kB);

		const auto memBlock2{ unified.allocate(80_kB) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 80_kB);

		const auto memBlock3{ unified.allocate(80_kB) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 80_kB);

		const auto memBlock4{ unified.allocate(80_kB) };
		EXPECT_EQ(memBlock4.ptr, nullptr);

		unified.deallocate(memBlock3);

		const auto memBlock5{ unified.allocate(80_kB) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, 80_kB);

		unified.deallocate(memBlock5);
		unified.deallocate(memBlock2);
		unified.deallocate(memBlock1);

	}

}

TEST(AllocAlgorithms, tsUnifiedFragmentation)
{

	byte_t extBuffer[256_B];
	hfog::MemoryBlock extBlock;
	extBlock.ptr = extBuffer;
	extBlock.size = sizeof(extBuffer);

	using source_t = hfog::Sources::External<hfog::GarbageWriter::Default>;

	hfog::Algorithms::Unified<source_t, 32_B, 160_B> unified(extBlock);

	for (size_t numOfIters{ 0 }; numOfIters < 2; ++numOfIters)
	{

		const auto memBlock1{ unified.allocate(32_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 32_B);

		const auto memBlock2{ unified.allocate(32_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 32_B);

		const auto memBlock3{ unified.allocate(32_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 32_B);

		const auto memBlock4{ unified.allocate(32_B) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, 32_B);

		const auto memBlock5{ unified.allocate(32_B) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, 32_B);

		unified.deallocate(memBlock1);
		unified.deallocate(memBlock3);
		unified.deallocate(memBlock5);

		unified.deallocate(memBlock2);
		unified.deallocate(memBlock4);

	}

}

TEST(AllocAlgorithms, tsUnifiedRefill)
{

	byte_t extBuffer[256_B];
	hfog::MemoryBlock extBlock;
	extBlock.ptr = extBuffer;
	extBlock.size = sizeof(extBuffer);

	using source_t = hfog::Sources::External<hfog::GarbageWriter::Default>;

	hfog::Algorithms::Unified<source_t, 32_B, 160_B> unified(extBlock);

	for (size_t numOfIters{ 0 }; numOfIters < 2; ++numOfIters)
	{

		const auto memBlock1{ unified.allocate(32_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 32_B);

		const auto memBlock2{ unified.allocate(32_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 32_B);

		const auto memBlock3{ unified.allocate(32_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 32_B);

		const auto memBlock4{ unified.allocate(32_B) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, 32_B);

		const auto memBlock5{ unified.allocate(32_B) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, 32_B);

		const auto memBlockOB1{ unified.allocate(32_B) };
		EXPECT_EQ(memBlockOB1.ptr, nullptr);

		unified.deallocate(memBlock1);
		unified.deallocate(memBlock3);
		unified.deallocate(memBlock5);

		const auto memBlock6{ unified.allocate(32_B) };
		EXPECT_NE(memBlock6.ptr, nullptr);
		EXPECT_EQ(memBlock6.size, 32_B);

		const auto memBlock7{ unified.allocate(32_B) };
		EXPECT_NE(memBlock7.ptr, nullptr);
		EXPECT_EQ(memBlock7.size, 32_B);

		const auto memBlock8{ unified.allocate(32_B) };
		EXPECT_NE(memBlock8.ptr, nullptr);
		EXPECT_EQ(memBlock8.size, 32_B);

		const auto memBlockOB2{ unified.allocate(32_B) };
		EXPECT_EQ(memBlockOB2.ptr, nullptr);

		unified.deallocate();

	}

}

TEST(AllocAlgorithms, tsUnifiedGaps)
{

	byte_t extBuffer[256_B];
	hfog::MemoryBlock extBlock;
	extBlock.ptr = extBuffer;
	extBlock.size = sizeof(extBuffer);

	using source_t = hfog::Sources::External<hfog::GarbageWriter::Default>;

	hfog::Algorithms::Unified<source_t, 32_B, 160_B> unified(extBlock);

	for (size_t numOfIters{ 0 }; numOfIters < 2; ++numOfIters)
	{

		const auto memBlock1{ unified.allocate(32_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 32_B);

		const auto memBlock2{ unified.allocate(32_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 32_B);

		const auto memBlock3{ unified.allocate(32_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 32_B);

		const auto memBlock4{ unified.allocate(32_B) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, 32_B);

		const auto memBlock5{ unified.allocate(32_B) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, 32_B);

		const auto memBlockOB1{ unified.allocate(32_B) };
		EXPECT_EQ(memBlockOB1.ptr, nullptr);

		unified.deallocate(memBlock1);

		const auto memBlockOB2{ unified.allocate(64_B) };
		EXPECT_EQ(memBlockOB2.ptr, nullptr);

		unified.deallocate(memBlock2);

		const auto memBlock6{ unified.allocate(64_B) };
		EXPECT_NE(memBlock6.ptr, nullptr);
		EXPECT_EQ(memBlock6.size, 64_B);

		unified.deallocate();

	}

}

TEST(AllocAlgorithms, tsUnifiedFilling)
{

	byte_t extBuffer[256_B];
	hfog::MemoryBlock extBlock;
	extBlock.ptr = extBuffer;
	extBlock.size = sizeof(extBuffer);

	using source_t = hfog::Sources::External<hfog::GarbageWriter::Default>;

	hfog::Algorithms::Unified<source_t, 32_B, 160_B> unified(extBlock);

	for (size_t numOfIters{ 0 }; numOfIters < 2; ++numOfIters)
	{

		const auto memBlock1{ unified.allocate(32_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 32_B);

		const auto memBlock2{ unified.allocate(64_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 64_B);

		const auto memBlock3{ unified.allocate(32_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 32_B);

		const auto memBlockOB1{ unified.allocate(64_B) };
		EXPECT_EQ(memBlockOB1.ptr, nullptr);

		const auto memBlock4{ unified.allocate(32_B) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, 32_B);

		const auto memBlockOB2{ unified.allocate(32_B) };
		EXPECT_EQ(memBlockOB2.ptr, nullptr);

		unified.deallocate(memBlock2);

		const auto memBlock5{ unified.allocate(32_B) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, 32_B);

		const auto memBlockOB3{ unified.allocate(64_B) };
		EXPECT_EQ(memBlockOB3.ptr, nullptr);

		const auto memBlock6{ unified.allocate(32_B) };
		EXPECT_NE(memBlock6.ptr, nullptr);
		EXPECT_EQ(memBlock6.size, 32_B);

		const auto memBlockOB4{ unified.allocate(32_B) };
		EXPECT_EQ(memBlockOB4.ptr, nullptr);

		unified.deallocate();

	}

}

TEST(AllocAlgorithms, tsUnifiedIsOwner)
{

	byte_t extBuffer[256_B];
	hfog::MemoryBlock extBlock;
	extBlock.ptr = extBuffer;
	extBlock.size = sizeof(extBuffer);

	using source_t = hfog::Sources::External<hfog::GarbageWriter::Default>;

	hfog::Algorithms::Unified<source_t, 32_B, 160_B> unified(extBlock);

	for (size_t numOfIters{ 0 }; numOfIters < 2; ++numOfIters)
	{

		const auto memBlock1{ unified.allocate(32_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 32_B);

		const auto memBlock2{ unified.allocate(32_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 32_B);

		const auto memBlock3{ unified.allocate(32_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 32_B);

		const auto memBlock4{ unified.allocate(32_B) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, 32_B);

		const auto memBlock5{ unified.allocate(32_B) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, 32_B);

		EXPECT_TRUE(unified.getIsOwner(memBlock1.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock2.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock3.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock4.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock5.ptr));

		unified.deallocate(memBlock1);
		EXPECT_FALSE(unified.getIsOwner(memBlock1.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock2.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock3.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock4.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock5.ptr));

		unified.deallocate(memBlock3);
		EXPECT_FALSE(unified.getIsOwner(memBlock1.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock2.ptr));
		EXPECT_FALSE(unified.getIsOwner(memBlock3.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock4.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock5.ptr));

		unified.deallocate(memBlock5);
		EXPECT_FALSE(unified.getIsOwner(memBlock1.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock2.ptr));
		EXPECT_FALSE(unified.getIsOwner(memBlock3.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock4.ptr));
		EXPECT_FALSE(unified.getIsOwner(memBlock5.ptr));

		unified.deallocate(memBlock2);
		EXPECT_FALSE(unified.getIsOwner(memBlock1.ptr));
		EXPECT_FALSE(unified.getIsOwner(memBlock2.ptr));
		EXPECT_FALSE(unified.getIsOwner(memBlock3.ptr));
		EXPECT_TRUE(unified.getIsOwner(memBlock4.ptr));
		EXPECT_FALSE(unified.getIsOwner(memBlock5.ptr));

		unified.deallocate();

		EXPECT_FALSE(unified.getIsOwner(memBlock1.ptr));
		EXPECT_FALSE(unified.getIsOwner(memBlock2.ptr));
		EXPECT_FALSE(unified.getIsOwner(memBlock3.ptr));
		EXPECT_FALSE(unified.getIsOwner(memBlock4.ptr));
		EXPECT_FALSE(unified.getIsOwner(memBlock5.ptr));

	}

}