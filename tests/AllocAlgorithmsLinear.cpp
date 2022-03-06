#include "pch.h"

import AlgLinear;

import SourceHeap;
import SourceExt;
import SourceStack;

import hfogCore;

using namespace hfog::MemoryUtils::Literals;

static_assert(hfog::CtAllocator<hfog::Algorithms::Linear<hfog::Sources::Heap<hfog::GarbageWriter::Default>, 16_B>>);
static_assert(hfog::CtAllocator<hfog::Algorithms::Linear<hfog::Sources::External<hfog::GarbageWriter::Default>, 16_B>>);
static_assert(hfog::CtAllocator<hfog::Algorithms::Linear<hfog::Sources::Stack<128_B, hfog::GarbageWriter::Default>, 16_B>>);

TEST(AllocAlgorithms, tsLinearHeap)
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

TEST(AllocAlgorithms, tsLinearExt)
{

	using source_t = hfog::Sources::External<hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>;
	const auto alignment{ 16_B };

	byte_t extBuffer[128];

	hfog::MemoryBlock extMemBlock;
	extMemBlock.ptr = extBuffer;
	extMemBlock.size = sizeof(extBuffer);

	hfog::Algorithms::Linear<source_t, alignment> algLinear(extMemBlock);

	{
		const auto memBlock{ algLinear.allocate(32_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xFA));
		EXPECT_TRUE(algLinear.getIsOwner(memBlock.ptr));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 32_B);
		algLinear.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xAF));
		EXPECT_FALSE(algLinear.getIsOwner(memBlock.ptr));
	}
	{
		const auto memBlock{ algLinear.allocate(18_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xFA));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 32_B);
		algLinear.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xAF));
	}
	{
		const auto memBlock{ algLinear.allocate(40_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 48_B, 0xFA));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 48_B);
		algLinear.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 48_B, 0xAF));
	}
	{
		const auto memBlock{ algLinear.allocate(0_B) };
		EXPECT_EQ(memBlock.size, 0_B);
		algLinear.deallocate(memBlock);
	}
	{
		const auto memBlock{ algLinear.allocate(sizeof(extBuffer) * 2) };
		EXPECT_EQ(memBlock.ptr, nullptr);
	}
	{
		const auto memBlock{ algLinear.allocate(40_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 48_B, 0xFA));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 48_B);

		const auto memBlock2{ algLinear.allocate(100_B) };
		EXPECT_EQ(memBlock2.ptr, nullptr);

		algLinear.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 48_B, 0xAF));

		const auto memBlock3{ algLinear.allocate(100_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 112_B, 0xFA));
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 112_B);
		algLinear.deallocate(memBlock3);
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, 112_B, 0xAF));

	}
	{
		const auto memBlock{ algLinear.allocate(40_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 48_B, 0xFA));
		EXPECT_TRUE(algLinear.getIsOwner(memBlock.ptr));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 48_B);

		const auto memBlock2{ algLinear.allocate(40_B) };
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 48_B, 0xFA));
		EXPECT_TRUE(algLinear.getIsOwner(memBlock2.ptr));
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 48_B);

		algLinear.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 48_B, 0xAF));
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 48_B, 0xAF));
		EXPECT_FALSE(algLinear.getIsOwner(memBlock.ptr));
		EXPECT_FALSE(algLinear.getIsOwner(memBlock2.ptr));
	}

	EXPECT_FALSE(algLinear.getIsOwner(nullptr));

	byte_t val{ 42 };
	EXPECT_FALSE(algLinear.getIsOwner(&val));

}

TEST(AllocAlgorithms, tsLinearStack)
{

	static constexpr auto stackSize{ 128_B };
	using source_t = hfog::Sources::Stack<stackSize, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>;
	const auto alignment{ 16_B };

	hfog::Algorithms::Linear<source_t, alignment> algLinear;

	{
		const auto memBlock{ algLinear.allocate(32_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xFA));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 32_B);
		algLinear.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xAF));
	}
	{
		const auto memBlock{ algLinear.allocate(18_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xFA));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 32_B);
		algLinear.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xAF));
	}
	{
		const auto memBlock{ algLinear.allocate(40_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 48_B, 0xFA));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 48_B);
		algLinear.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 48_B, 0xAF));
	}
	{
		const auto memBlock{ algLinear.allocate(0_B) };
		EXPECT_EQ(memBlock.size, 0_B);
		algLinear.deallocate(memBlock);
	}
	{
		const auto memBlock{ algLinear.allocate(stackSize * 2) };
		EXPECT_EQ(memBlock.ptr, nullptr);
	}
	{
		const auto memBlock{ algLinear.allocate(40_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 48_B, 0xFA));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 48_B);

		const auto memBlock2{ algLinear.allocate(100_B) };
		EXPECT_EQ(memBlock2.ptr, nullptr);

		algLinear.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 48_B, 0xAF));

		const auto memBlock3{ algLinear.allocate(100_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 112_B, 0xFA));
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 112_B);
		algLinear.deallocate(memBlock3);
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, 112_B, 0xAF));

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
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 48_B, 0xAF));
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 48_B, 0xAF));
	}

}