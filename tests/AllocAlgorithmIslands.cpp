#include "pch.h"

import hfog.MemoryUtils;
import hfog.Algorithms.Islands;

import hfog.Sources.Stack;
import hfog.Sources.Ext;

import hfog.Core;

import hfog.Alloc;

using namespace hfog::MemoryUtils::Literals;

static_assert(hfog::CtAllocator<hfog::Algorithms::Islands<hfog::Sources::External<hfog::GarbageWriter::Default>, 16_B, 4, 1>>);
static_assert(hfog::CtAllocator<hfog::Algorithms::Islands<hfog::Sources::Stack<128_B, hfog::GarbageWriter::Default>, 16_B, 4, 1>>);

TEST(AllocAlgorithms, tsIslands)
{

	byte_t extBuffer[256];
	hfog::MemoryBlock extBlock;
	extBlock.ptr = extBuffer;
	extBlock.size = sizeof(extBuffer);

	using source_t = hfog::Sources::External<hfog::GarbageWriter::Default>;

	hfog::Algorithms::Islands<source_t, 16_B, 4, 4> islands(extBlock);

	{

		const auto memBlock1{ islands.allocate(64_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 64_B);

		const auto memBlock2{ islands.allocate(64_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 64_B);

		const auto memBlock3{ islands.allocate(64_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 64_B);

		const auto memBlock4{ islands.allocate(64_B) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, 64_B);

		const auto memBlock5{ islands.allocate(64_B) };
		EXPECT_EQ(memBlock5.ptr, nullptr);

		islands.deallocate(memBlock4);

		const auto memBlock6{ islands.allocate(64_B) };
		EXPECT_NE(memBlock6.ptr, nullptr);
		EXPECT_EQ(memBlock6.size, 64_B);

		islands.deallocate(memBlock6);
		islands.deallocate(memBlock3);
		islands.deallocate(memBlock2);
		islands.deallocate(memBlock1);

	}

	{

		const auto memBlock1{ islands.allocate(32_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 32_B);

		const auto memBlock2{ islands.allocate(64_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 64_B);

		const auto memBlock3{ islands.allocate(64_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 64_B);

		const auto memBlock4{ islands.allocate(64_B) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, 64_B);

		const auto memBlock5{ islands.allocate(32_B) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, 32_B);

		islands.deallocate(memBlock1);

		const auto memBlock6{ islands.allocate(64_B) };
		EXPECT_EQ(memBlock6.ptr, nullptr);

		islands.deallocate(memBlock5);

		const auto memBlock7{ islands.allocate(64_B) };
		EXPECT_NE(memBlock7.ptr, nullptr);
		EXPECT_EQ(memBlock7.size, 64_B);

	}

}

TEST(AllocAlgorithms, tsIslandsGaps)
{

	byte_t extBuffer[256];
	hfog::MemoryBlock extBlock;
	extBlock.ptr = extBuffer;
	extBlock.size = sizeof(extBuffer);

	using source_t = hfog::Sources::External<hfog::GarbageWriter::Default>;

	hfog::Algorithms::Islands<source_t, 16_B, 4, 4> islands(extBlock);

	{

		const auto memBlock1{ islands.allocate(48_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 48_B);

		const auto memBlock2{ islands.allocate(48_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 48_B);

		const auto memBlock3{ islands.allocate(48_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 48_B);

		const auto memBlock4{ islands.allocate(48_B) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, 48_B);

		const auto memBlock5{ islands.allocate(48_B) };
		EXPECT_EQ(memBlock5.ptr, nullptr);

		const auto memBlock6{ islands.allocate(16_B) };
		EXPECT_NE(memBlock6.ptr, nullptr);
		EXPECT_EQ(memBlock6.size, 16_B);

		const auto memBlock7{ islands.allocate(16_B) };
		EXPECT_NE(memBlock7.ptr, nullptr);
		EXPECT_EQ(memBlock7.size, 16_B);

		const auto memBlock8{ islands.allocate(16_B) };
		EXPECT_NE(memBlock8.ptr, nullptr);
		EXPECT_EQ(memBlock8.size, 16_B);

		const auto memBlock9{ islands.allocate(16_B) };
		EXPECT_NE(memBlock9.ptr, nullptr);
		EXPECT_EQ(memBlock9.size, 16_B);

		{
			const auto memBlock{ islands.allocate(16_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}


		islands.deallocate(memBlock1);
		{
			const auto memBlock{ islands.allocate(16_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}
		islands.deallocate(memBlock6);

		const auto memBlock10{ islands.allocate(16_B) };
		EXPECT_NE(memBlock10.ptr, nullptr);
		EXPECT_EQ(memBlock10.size, 16_B);

		const auto memBlock11{ islands.allocate(48_B) };
		EXPECT_NE(memBlock11.ptr, nullptr);
		EXPECT_EQ(memBlock11.size, 48_B);
		{
			const auto memBlock{ islands.allocate(16_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}

	}

}

TEST(AllocAlgorithms, tsIslandsBadData)
{

	byte_t extBuffer[256];
	hfog::MemoryBlock extBlock;
	extBlock.ptr = extBuffer;
	extBlock.size = sizeof(extBuffer);

	using source_t = hfog::Sources::External<hfog::GarbageWriter::Default>;

	hfog::Algorithms::Islands<source_t, 16_B, 4, 4> islands(extBlock);

	{
		{
			const auto memBlock{ islands.allocate(65_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}

		const auto memBlock1{ islands.allocate(64_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 64_B);

		const auto memBlock2{ islands.allocate(40_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 48_B);

		const auto memBlock3{ islands.allocate(20_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 32_B);

		const auto memBlock4{ islands.allocate(8_B) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, 16_B);

		const auto memBlock5{ islands.allocate(12_B) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, 16_B);

		{
			const auto memBlock{ islands.allocate(65_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}

		const auto memBlock6{ islands.allocate(28_B) };
		EXPECT_NE(memBlock6.ptr, nullptr);
		EXPECT_EQ(memBlock6.size, 32_B);

		{
			const auto memBlock{ islands.allocate(48_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}

		islands.deallocate(memBlock2);

		{
			const auto memBlock{ islands.allocate(48_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}

		const auto memBlock7{ islands.allocate(28_B) };
		EXPECT_NE(memBlock7.ptr, nullptr);
		EXPECT_EQ(memBlock7.size, 32_B);

		islands.deallocate(memBlock4);

		const auto memBlock9{ islands.allocate(16_B) };
		EXPECT_NE(memBlock9.ptr, nullptr);
		EXPECT_EQ(memBlock9.size, 16_B);

		const auto memBlock10{ islands.allocate(64_B) };
		EXPECT_NE(memBlock10.ptr, nullptr);
		EXPECT_EQ(memBlock10.size, 64_B);

	}

}

TEST(AllocAlgorithms, tsIslandsOneChunck)
{

	byte_t extBuffer[256];
	hfog::MemoryBlock extBlock;
	extBlock.ptr = extBuffer;
	extBlock.size = sizeof(extBuffer);

	using source_t = hfog::Sources::External<hfog::GarbageWriter::Default>;

	hfog::Algorithms::Islands<source_t, 16_B, 4, 1> islands(extBlock);

	{
		{
			const auto memBlock{ islands.allocate(65_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}

		const auto memBlock1{ islands.allocate(64_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 64_B);

		{
			const auto memBlock{ islands.allocate(64_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}

		islands.deallocate(memBlock1);

		const auto memBlock2{ islands.allocate(32_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 32_B);

		const auto memBlock3{ islands.allocate(16_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 16_B);

		const auto memBlock4{ islands.allocate(16_B) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, 16_B);

		islands.deallocate(memBlock2);
		{
			const auto memBlock{ islands.allocate(32_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}
		islands.deallocate(memBlock3);
		islands.deallocate(memBlock4);

		const auto memBlock5{ islands.allocate(32_B) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, 32_B);

		const auto memBlock6{ islands.allocate(32_B) };
		EXPECT_NE(memBlock6.ptr, nullptr);
		EXPECT_EQ(memBlock6.size, 32_B);

	}

}

TEST(AllocAlgorithms, tsIslandsMemoryCheck)
{

	using source_t = hfog::Sources::Stack<16_B * 4 * 4, 
		hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>;

	hfog::Algorithms::Islands<source_t, 16_B, 4, 4> islands;

	{

		const auto memBlock1{ islands.allocate(64_B) };
		EXPECT_TRUE(getValuesAre(memBlock1.ptr, 0_B, 64_B, 0xFA));

		const auto memBlock2{ islands.allocate(40_B) };
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 48_B, 0xFA));

		const auto memBlock3{ islands.allocate(20_B) };
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, 32_B, 0xFA));

		const auto memBlock4{ islands.allocate(8_B) };
		EXPECT_TRUE(getValuesAre(memBlock4.ptr, 0_B, 16_B, 0xFA));

		const auto memBlock5{ islands.allocate(12_B) };
		EXPECT_TRUE(getValuesAre(memBlock5.ptr, 0_B, 16_B, 0xFA));

		const auto memBlock6{ islands.allocate(28_B) };
		EXPECT_TRUE(getValuesAre(memBlock6.ptr, 0_B, 32_B, 0xFA));

		islands.deallocate(memBlock2);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 48_B, 0xAF));

		const auto memBlock7{ islands.allocate(28_B) };
		EXPECT_TRUE(getValuesAre(memBlock7.ptr, 0_B, 32_B, 0xFA));

		islands.deallocate(memBlock4);
		EXPECT_TRUE(getValuesAre(memBlock4.ptr, 0_B, 16_B, 0xAF));

		const auto memBlock9{ islands.allocate(16_B) };
		EXPECT_TRUE(getValuesAre(memBlock9.ptr, 0_B, 16_B, 0xFA));

		const auto memBlock10{ islands.allocate(64_B) };
		EXPECT_TRUE(getValuesAre(memBlock10.ptr, 0_B, 64_B, 0xFA));

	}

}

TEST(AllocAlgorithms, tsIslandsOwn)
{

	byte_t val{ 42 };

	using source_t = hfog::Sources::Stack<16_B * 4 * 4>;

	hfog::Algorithms::Islands<source_t, 16_B, 4, 4> islands;

	EXPECT_FALSE(islands.getIsOwner(&val));
	EXPECT_FALSE(islands.getIsOwner(nullptr));

	{

		const auto memBlock1{ islands.allocate(64_B) };
		EXPECT_TRUE(islands.getIsOwner(memBlock1.ptr));

		const auto memBlock2{ islands.allocate(40_B) };
		EXPECT_TRUE(islands.getIsOwner(memBlock2.ptr));

		const auto memBlock3{ islands.allocate(20_B) };
		EXPECT_TRUE(islands.getIsOwner(memBlock3.ptr));

		const auto memBlock4{ islands.allocate(8_B) };
		EXPECT_TRUE(islands.getIsOwner(memBlock4.ptr));

		const auto memBlock5{ islands.allocate(12_B) };
		EXPECT_TRUE(islands.getIsOwner(memBlock5.ptr));

		const auto memBlock6{ islands.allocate(28_B) };
		EXPECT_TRUE(islands.getIsOwner(memBlock6.ptr));

		islands.deallocate(memBlock2);
		EXPECT_TRUE(islands.getIsOwner(memBlock2.ptr));

		const auto memBlock7{ islands.allocate(28_B) };
		EXPECT_TRUE(islands.getIsOwner(memBlock7.ptr));

		islands.deallocate(memBlock4);
		EXPECT_FALSE(islands.getIsOwner(memBlock2.ptr));
		EXPECT_FALSE(islands.getIsOwner(memBlock4.ptr));

		const auto memBlock8{ islands.allocate(16_B) };
		EXPECT_TRUE(islands.getIsOwner(memBlock8.ptr));

		const auto memBlock9{ islands.allocate(64_B) };
		EXPECT_TRUE(islands.getIsOwner(memBlock9.ptr));

		EXPECT_TRUE(islands.getIsOwner(memBlock1.ptr));
		EXPECT_TRUE(islands.getIsOwner(memBlock3.ptr));
		EXPECT_TRUE(islands.getIsOwner(memBlock5.ptr));
		EXPECT_TRUE(islands.getIsOwner(memBlock6.ptr));
		EXPECT_TRUE(islands.getIsOwner(memBlock7.ptr));
		EXPECT_TRUE(islands.getIsOwner(memBlock8.ptr));
		EXPECT_TRUE(islands.getIsOwner(memBlock9.ptr));

		//It is not garanteed that this partucalar block use this memory address
		//In this case some other memBlock use this address
		EXPECT_TRUE(islands.getIsOwner(memBlock2.ptr));
		EXPECT_TRUE(islands.getIsOwner(memBlock4.ptr));

	}

}

TEST(AllocAlgorithms, tsIslandsFullDeallocation)
{

	using source_t = hfog::Sources::Stack<16_B * 4 * 4,
		hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>;

	hfog::Algorithms::Islands<source_t, 16_B, 4, 4> islands;

	for (size_t itersLeft{3}; itersLeft > 0; --itersLeft)
	{

		const auto memBlock1{ islands.allocate(64_B) };
		EXPECT_TRUE(getValuesAre(memBlock1.ptr, 0_B, 64_B, 0xFA));

		const auto memBlock2{ islands.allocate(40_B) };
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 48_B, 0xFA));

		const auto memBlock3{ islands.allocate(20_B) };
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, 32_B, 0xFA));

		const auto memBlock4{ islands.allocate(8_B) };
		EXPECT_TRUE(getValuesAre(memBlock4.ptr, 0_B, 16_B, 0xFA));

		const auto memBlock5{ islands.allocate(12_B) };
		EXPECT_TRUE(getValuesAre(memBlock5.ptr, 0_B, 16_B, 0xFA));

		const auto memBlock6{ islands.allocate(28_B) };
		EXPECT_TRUE(getValuesAre(memBlock6.ptr, 0_B, 32_B, 0xFA));

		islands.deallocate();
		EXPECT_TRUE(getValuesAre(memBlock1.ptr, 0_B, 64_B, 0xAF));
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 48_B, 0xAF));
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, 32_B, 0xAF));
		EXPECT_TRUE(getValuesAre(memBlock4.ptr, 0_B, 16_B, 0xAF));
		EXPECT_TRUE(getValuesAre(memBlock5.ptr, 0_B, 16_B, 0xAF));
		EXPECT_TRUE(getValuesAre(memBlock6.ptr, 0_B, 32_B, 0xAF));

	}

}

TEST(AllocAlgorithms, tsIslandsCrash)
{

	byte_t extBuffer[512_B];
	//	std::vector<byte_t> extBuffer(512_B);

	using allocator_t = hfog::Alloc::IslandsExt<4_B, 128_B, 4, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>;
	allocator_t alloc(hfog::MemoryBlock(extBuffer, sizeof(extBuffer)));

	{

		const auto memBlock1_1{ alloc.allocate(12_B) };
		const auto memBlock1_2{ alloc.allocate(24_B) };
		alloc.deallocate(memBlock1_1);
		const auto memBlock1_3{ alloc.allocate(48_B) };
		alloc.deallocate(memBlock1_2);
		const auto memBlock1_4{ alloc.allocate(32_B) };
		alloc.deallocate(memBlock1_3);

		const auto memBlock2_1{ alloc.allocate(64_B) };

		alloc.deallocate(memBlock1_4);

		const auto memBlock2_2{ alloc.allocate(64_B) };
		alloc.deallocate(memBlock2_1);

		const auto memBlock3_1{ alloc.allocate(128_B) };

	}

}