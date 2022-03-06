#include "pch.h"

import SourceHeap;
import SourceExt;
import SourceStack;
import MemoryUtils;

import hfogCore;

import AlgStack;

using namespace hfog::MemoryUtils::Literals;

static_assert(hfog::CtAllocator<hfog::Algorithms::Stack<hfog::Sources::Heap<hfog::GarbageWriter::Default>, 16_B>>);
static_assert(hfog::CtAllocator<hfog::Algorithms::Stack<hfog::Sources::External<hfog::GarbageWriter::Default>, 16_B>>);
static_assert(hfog::CtAllocator<hfog::Algorithms::Stack<hfog::Sources::Stack<128_B, hfog::GarbageWriter::Default>, 16_B>>);

TEST(AllocAlgorithms, tsStackHeap)
{

	using source_t = hfog::Sources::Heap<hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>;
	const auto alignment{ 16_B };

	hfog::Algorithms::Stack<source_t, alignment> algStack;

	{
		const auto memBlock{ algStack.allocate(32_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xFA));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 32_B);
		algStack.deallocate(memBlock);
	}

	{

		const auto memBlock{ algStack.allocate(32_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xFA));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 32_B);

		const auto memBlock2{ algStack.allocate(32_B) };
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 32_B, 0xFA));
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 32_B);

		algStack.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xFA));
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 32_B, 0xFA));

		algStack.deallocate(memBlock2);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xFA));

		algStack.deallocate(memBlock);

		const auto memBlock3{ algStack.allocate(32_B) };
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, 32_B, 0xFA));
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 32_B);

		algStack.deallocate(memBlock3);

	}

}

TEST(AllocAlgorithms, tsStackExt)
{

	using source_t = hfog::Sources::External<hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>;
	const auto alignment{ 16_B };

	static constexpr auto extBufferLen{ 128_B };
	byte_t extMemBuffer[extBufferLen];

	hfog::MemoryBlock extMemBlock;
	extMemBlock.ptr = extMemBuffer;
	extMemBlock.size = extBufferLen;

	hfog::Algorithms::Stack<source_t, alignment> algStack(extMemBlock);

	{
		const auto memBlock{ algStack.allocate(32_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xFA));
		EXPECT_TRUE(algStack.getIsOwner(memBlock.ptr));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 32_B);
		algStack.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xAF));
		EXPECT_FALSE(algStack.getIsOwner(memBlock.ptr));
	}

	{

		const auto memBlock{ algStack.allocate(32_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xFA));
		EXPECT_TRUE(algStack.getIsOwner(memBlock.ptr));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 32_B);

		const auto memBlock2{ algStack.allocate(32_B) };
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 32_B, 0xFA));
		EXPECT_TRUE(algStack.getIsOwner(memBlock2.ptr));
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 32_B);

		algStack.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xFA));
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 32_B, 0xFA));
		EXPECT_TRUE(algStack.getIsOwner(memBlock.ptr));
		EXPECT_TRUE(algStack.getIsOwner(memBlock2.ptr));

		algStack.deallocate(memBlock2);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xFA));
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 32_B, 0xAF));
		EXPECT_TRUE(algStack.getIsOwner(memBlock.ptr));
		EXPECT_FALSE(algStack.getIsOwner(memBlock2.ptr));

		algStack.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xAF));
		EXPECT_FALSE(algStack.getIsOwner(memBlock.ptr));

		const auto memBlock3{ algStack.allocate(32_B) };
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, 32_B, 0xFA));
		EXPECT_TRUE(algStack.getIsOwner(memBlock3.ptr));
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 32_B);

		algStack.deallocate(memBlock3);
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, 32_B, 0xAF));
		EXPECT_FALSE(algStack.getIsOwner(memBlock3.ptr));

	}

	{

		const auto memBlock{ algStack.allocate(96_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 96_B, 0xFA));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 96_B);

		const auto memBlock2{ algStack.allocate(extBufferLen) };
		EXPECT_EQ(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 0_B);

		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 36_B, 0xFA));

		algStack.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 36_B, 0xAF));

		const auto memBlock3{ algStack.allocate(extBufferLen) };
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, extBufferLen, 0xFA));
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, extBufferLen);

		algStack.deallocate(memBlock3);
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, extBufferLen, 0xAF));

	}

	byte_t val{ 42 };
	EXPECT_FALSE(algStack.getIsOwner(&val));

	EXPECT_FALSE(algStack.getIsOwner(nullptr));

}

TEST(AllocAlgorithms, tsStackStack)
{

	static constexpr auto extBufferLen{ 128_B };
	using source_t = hfog::Sources::Stack<extBufferLen, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>;
	const auto alignment{ 16_B };

	hfog::Algorithms::Stack<source_t, alignment> algStack;

	{

		const auto memBlock{ algStack.allocate(32_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xFA));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 32_B);
		algStack.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xAF));

	}

	{

		const auto memBlock{ algStack.allocate(32_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xFA));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 32_B);

		const auto memBlock2{ algStack.allocate(32_B) };
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 32_B, 0xFA));
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 32_B);

		algStack.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xFA));
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 32_B, 0xFA));

		algStack.deallocate(memBlock2);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xFA));
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 32_B, 0xAF));

		algStack.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0xAF));

		const auto memBlock3{ algStack.allocate(32_B) };
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, 32_B, 0xFA));
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 32_B);

		algStack.deallocate(memBlock3);
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, 32_B, 0xAF));

	}

	{

		const auto memBlock{ algStack.allocate(96_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 96_B, 0xFA));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 96_B);

		const auto memBlock2{ algStack.allocate(extBufferLen) };
		EXPECT_EQ(memBlock2.ptr, nullptr);

		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 36_B, 0xFA));

		algStack.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 36_B, 0xAF));

		const auto memBlock3{ algStack.allocate(extBufferLen) };
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, extBufferLen, 0xFA));
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, extBufferLen);

		algStack.deallocate(memBlock3);
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, extBufferLen, 0xAF));

	}

}