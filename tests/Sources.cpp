#include "pch.h"

import hfog.Sources.Ext;
import hfog.Sources.Stack;
import hfog.Sources.Common;

using namespace hfog::MemoryUtils::Literals;

static_assert(hfog::Sources::CtSource<hfog::Sources::External<hfog::GarbageWriter::Default>>);
static_assert(hfog::Sources::CtSource<hfog::Sources::Stack<128_B, hfog::GarbageWriter::Default>>);

TEST(Sources, tsExtSource)
{

	byte_t val{ 42 };
	byte_t extBuffer[64_B];
	std::memset(extBuffer, 0, sizeof(extBuffer));

	hfog::MemoryBlock extMemBlock;
	extMemBlock.ptr = extBuffer;
	extMemBlock.size = 32_B;

	hfog::Sources::External<hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>> extSource(extMemBlock);

	EXPECT_EQ(extSource.getOffset(&val), invalidMem_t);
	{
		const auto memBlock{ extSource.getMemory(16_B, 16_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 16_B, 0xFA));
		EXPECT_EQ(memBlock.ptr, extMemBlock.ptr + 16_B);
		EXPECT_EQ(memBlock.size, 16_B);
		extSource.releaseMemory(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 16_B, 0xAF));
	}

	{
		const auto memBlock{ extSource.getMemory(16_B, 16_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 16_B, 0xFA));
		EXPECT_EQ(memBlock.ptr, extMemBlock.ptr + 16_B);
		EXPECT_EQ(memBlock.size, 16_B);
		EXPECT_EQ(extSource.getOffset(memBlock.ptr), 16_B);

		const auto memBlock2{ extSource.getMemory(0_B, 16_B) };
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 16_B, 0xFA));
		EXPECT_EQ(memBlock2.ptr, extMemBlock.ptr);
		EXPECT_EQ(memBlock2.size, 16_B);
		EXPECT_EQ(extSource.getOffset(memBlock2.ptr), 0_B);

		extSource.releaseMemory(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 16_B, 0xAF));
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 16_B, 0xFA));
		EXPECT_EQ(extSource.getOffset(memBlock.ptr), 16_B);
		EXPECT_EQ(extSource.getOffset(memBlock2.ptr), 0_B);

		extSource.releaseMemory(memBlock2);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 16_B, 0xAF));
		EXPECT_EQ(extSource.getOffset(memBlock2.ptr), 0_B);

	}

	{
		const auto memBlock{ extSource.getMemory(16_B, 16_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 16_B, 0xFA));
		EXPECT_EQ(memBlock.ptr, extMemBlock.ptr + 16_B);
		EXPECT_EQ(memBlock.size, 16_B);

		const auto memBlock2{ extSource.getMemory(0_B, 16_B) };
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 16_B, 0xFA));
		EXPECT_EQ(memBlock2.ptr, extMemBlock.ptr);
		EXPECT_EQ(memBlock2.size, 16_B);

		extSource.releaseAllMemory();
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 16_B, 0xAF));
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 16_B, 0xAF));
		EXPECT_EQ(extSource.getOffset(memBlock.ptr), 16_B);
		EXPECT_EQ(extSource.getOffset(memBlock2.ptr), 0_B);

	}

	{
		const auto memBlock{ extSource.getMemory(16_B, 32_B) };
		EXPECT_EQ(memBlock.ptr, nullptr);
	}

	EXPECT_EQ(extSource.getOffset(nullptr), invalidMem_t);


}

TEST(Sources, tsStackSource)
{

	byte_t val{ 42 };

	hfog::Sources::Stack<128_B, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>> stackSource;

	EXPECT_EQ(stackSource.getOffset(&val), invalidMem_t);

	{
		const auto memBlock{ stackSource.getMemory(0_B, 16_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 16_B, 0xFA));
		EXPECT_EQ(memBlock.ptr, reinterpret_cast<byte_t*>(&stackSource));
		EXPECT_EQ(memBlock.size, 16_B);
		stackSource.releaseMemory(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 16_B, 0xAF));
	}
	{

		const auto memBlock{ stackSource.getMemory(0_B, 16_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 16_B, 0xFA));
		EXPECT_EQ(memBlock.ptr, reinterpret_cast<byte_t*>(&stackSource));
		EXPECT_EQ(memBlock.size, 16_B);

		const auto memBlock2{ stackSource.getMemory(16_B, 16_B) };
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 16_B, 0xFA));
		EXPECT_EQ(memBlock2.ptr, reinterpret_cast<byte_t*>(&stackSource) + 16_B);
		EXPECT_EQ(memBlock2.size, 16_B);

		stackSource.releaseMemory(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 16_B, 0xAF));
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 16_B, 0xFA));
		stackSource.releaseMemory(memBlock2);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 16_B, 0xAF));

	}

	{
		const auto memBlock{ stackSource.getMemory(0_B, 200_B) };
		EXPECT_EQ(memBlock.ptr, nullptr);
	}

	{

		const auto memBlock{ stackSource.getMemory(16_B, 64_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 64_B, 0xFA));
		EXPECT_EQ(memBlock.ptr, reinterpret_cast<byte_t*>(&stackSource) + 16_B);
		EXPECT_EQ(memBlock.size, 64_B);

		const auto memBlock2{ stackSource.getMemory(64_B, 128_B) };
		EXPECT_EQ(memBlock2.ptr, nullptr);

		stackSource.releaseMemory(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 64_B, 0xAF));

		const auto memBlock3{ stackSource.getMemory(0_B, 128_B) };
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, 128_B, 0xFA));
		EXPECT_EQ(memBlock3.ptr, reinterpret_cast<byte_t*>(&stackSource));
		EXPECT_EQ(memBlock3.size, 128_B);
		stackSource.releaseMemory(memBlock3);
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, 128_B, 0xAF));

	}

	{
		const auto memBlock{ stackSource.getMemory(16_B, 16_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 16_B, 0xFA));
		EXPECT_EQ(memBlock.ptr, reinterpret_cast<byte_t*>(&stackSource) + 16_B);
		EXPECT_EQ(memBlock.size, 16_B);
		EXPECT_EQ(stackSource.getOffset(memBlock.ptr), 16_B);

		const auto memBlock2{ stackSource.getMemory(0_B, 16_B) };
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 16_B, 0xFA));
		EXPECT_EQ(memBlock2.ptr, reinterpret_cast<byte_t*>(&stackSource));
		EXPECT_EQ(memBlock2.size, 16_B);
		EXPECT_EQ(stackSource.getOffset(memBlock2.ptr), 0_B);

		stackSource.releaseMemory(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 16_B, 0xAF));
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 16_B, 0xFA));

		stackSource.releaseMemory(memBlock2);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 16_B, 0xAF));

	}

	{
		const auto memBlock{ stackSource.getMemory(16_B, 16_B) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 16_B, 0xFA));
		EXPECT_EQ(memBlock.ptr, reinterpret_cast<byte_t*>(&stackSource) + 16_B);
		EXPECT_EQ(memBlock.size, 16_B);

		const auto memBlock2{ stackSource.getMemory(0_B, 16_B) };
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 16_B, 0xFA));
		EXPECT_EQ(memBlock2.ptr, reinterpret_cast<byte_t*>(&stackSource));
		EXPECT_EQ(memBlock2.size, 16_B);

		stackSource.releaseAllMemory();
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 16_B, 0xAF));
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 16_B, 0xAF));

	}

	EXPECT_EQ(stackSource.getOffset(nullptr), invalidMem_t);

}