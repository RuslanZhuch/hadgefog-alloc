#include "pch.h"

import SourceHeap;
import SourceExt;
import SourceStack;
import SourcesCommon;

using namespace hfog::MemoryUtils::Literals;

static_assert(hfog::Sources::CtSource<hfog::Sources::Heap<hfog::GarbageWriter::Default>>);
static_assert(hfog::Sources::CtSource<hfog::Sources::External<hfog::GarbageWriter::Default>>);
static_assert(hfog::Sources::CtSource<hfog::Sources::Stack<128_B, hfog::GarbageWriter::Default>>);

TEST(Sources, tsHeapSource)
{

	hfog::Sources::Heap<hfog::GarbageWriter::IncDecWriter> heapSource;

	{
		const auto memBlock{ heapSource.getMemory(0, 32_B) };
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 32_B);
		EXPECT_EQ(heapSource.getOffset(memBlock.ptr), 0_B);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 32_B, 0x01));
		heapSource.releaseMemory(memBlock);
		EXPECT_EQ(heapSource.getOffset(memBlock.ptr), invalidMem_t);
	}

	{
		const auto memBlock{ heapSource.getMemory(0, 18_B) };
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 18_B);
		EXPECT_EQ(heapSource.getOffset(memBlock.ptr), 0_B);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, 18_B, 0x01));
		heapSource.releaseMemory(memBlock);
		EXPECT_EQ(heapSource.getOffset(memBlock.ptr), invalidMem_t);

		const auto memBlock2{ heapSource.getMemory(18_B, 18_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 18_B);
		EXPECT_EQ(heapSource.getOffset(memBlock2.ptr), 18_B);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 18_B, 0x01));
		heapSource.releaseMemory(memBlock2);
		EXPECT_EQ(heapSource.getOffset(memBlock2.ptr), invalidMem_t);

	}

	{
		const auto memBlock{ heapSource.getMemory(0_B, 0_B) };
		EXPECT_EQ(memBlock.size, 0_B);
		heapSource.releaseMemory(memBlock);
	}

	EXPECT_EQ(heapSource.getOffset(nullptr), invalidMem_t);
	byte_t val{ 42 };
	EXPECT_EQ(heapSource.getOffset(&val), invalidMem_t);

	//getMemory garantees that it returns initialized memory with garbageWriter write value (0xFA)
	//Also it would write clear value (0xAF) when memory been released to source
	//But after deallocation it cannot garantee that data in memory block stay the same (0xAF)
	//	so we cannot check that

	//Heap source uses offset parameter to find allocated memory island,
	//	but it doesn't garantee that mamory would be allocated continuously 
	{
		const auto memBlock1{ heapSource.getMemory(0, 16_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 16_B);
		EXPECT_TRUE(getValuesAre(memBlock1.ptr, 0_B, 16_B, 0x01));

		const auto memBlock2{ heapSource.getMemory(16_B, 16_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 16_B);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 16_B, 0x01));

		heapSource.releaseMemory(memBlock1);
		heapSource.releaseMemory(memBlock2);
	}

	{
		const auto memBlock1{ heapSource.getMemory(0, 16_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 16_B);
		EXPECT_TRUE(getValuesAre(memBlock1.ptr, 0_B, 16_B, 0x01));

		const auto memBlock2{ heapSource.getMemory(16_B, 16_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 16_B);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 16_B, 0x01));

		const auto memBlock3{ heapSource.getMemory(32_B, 16_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 16_B);
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, 16_B, 0x01));

		heapSource.releaseMemory(memBlock3);
		heapSource.releaseMemory(memBlock1);
		heapSource.releaseMemory(memBlock2);
	}

	{
		const auto memBlock1{ heapSource.getMemory(0, 16_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 16_B);
		EXPECT_TRUE(getValuesAre(memBlock1.ptr, 0_B, 16_B, 0x01));

		const auto memBlock2{ heapSource.getMemory(16_B, 16_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 16_B);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 16_B, 0x01));

		const auto memBlock3{ heapSource.getMemory(32_B, 16_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 16_B);
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, 16_B, 0x01));

		heapSource.releaseAllMemory();

		const auto memBlock4{ heapSource.getMemory(0_B, 16_B) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, 16_B);
		EXPECT_TRUE(getValuesAre(memBlock4.ptr, 0_B, 16_B, 0x01));

		heapSource.releaseAllMemory();
	}

	{
		const auto memBlock1{ heapSource.getMemory(0_B, 16_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 16_B);
		EXPECT_TRUE(getValuesAre(memBlock1.ptr, 0_B, 16_B, 0x01));

		const auto memBlock2{ heapSource.getMemory(0_B, 16_B) };
		EXPECT_EQ(memBlock2.ptr, memBlock1.ptr);
		EXPECT_EQ(memBlock2.size, 16_B);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 16_B, 0x02));

		heapSource.releaseAllMemory();
	}

}

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