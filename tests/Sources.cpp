#include "pch.h"

import SourceHeap;
import SourceExt;
import SourceStack;

using namespace hfog::MemoryUtils::Literals;

TEST(Sources, tsHeapSource)
{

	hfog::Sources::Heap heapSource;

	EXPECT_EQ(heapSource.getNumOfAllocatedBytes(), 0_B);
	{
		const auto memBlock{ heapSource.allocate(32) };
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 32_B);
		EXPECT_EQ(heapSource.getNumOfAllocatedBytes(), 32_B);
		heapSource.deallocate(memBlock);
		EXPECT_EQ(heapSource.getNumOfAllocatedBytes(), 0_B);
	}

	{
		const auto memBlock{ heapSource.allocate(18) };
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 18_B);
		EXPECT_EQ(heapSource.getNumOfAllocatedBytes(), 18_B);
		heapSource.deallocate(memBlock);
		EXPECT_EQ(heapSource.getNumOfAllocatedBytes(), 0_B);
	}

	{
		const auto memBlock{ heapSource.allocate(0) };
		EXPECT_EQ(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 0_B);
		EXPECT_EQ(heapSource.getNumOfAllocatedBytes(), 0_B);
	}

}

TEST(Sources, tsExtSource)
{

	std::byte extBuffer[64_B];
	std::memset(extBuffer, 0, sizeof(extBuffer));

	hfog::MemoryBlock extMemBlock;
	extMemBlock.ptr = extBuffer;
	extMemBlock.size = 32_B;

	hfog::Sources::External<16> extSource(extMemBlock);

	{
		const auto memBlock{ extSource.allocate(16_B, 16_B) };
		EXPECT_EQ(memBlock.ptr, extMemBlock.ptr + 16_B);
		EXPECT_EQ(memBlock.size, size_t(16_B));
		extSource.deallocate(memBlock);
	}

//	{
//		const auto memBlock{ heapSource.allocate(18) };
//		EXPECT_NE(memBlock.ptr, nullptr);
//		EXPECT_EQ(memBlock.size, size_t(32));
//		heapSource.deallocate(memBlock);
//	}

}

TEST(Sources, tsStackSource)
{

	hfog::Sources::Stack<128_B, 16_B> stackSource;

	{
		const auto memBlock{ stackSource.allocate(mem_t(0_B), mem_t(16_B)) };
		EXPECT_EQ(memBlock.ptr, reinterpret_cast<std::byte*>(&stackSource));
		EXPECT_EQ(memBlock.size, size_t(16_B));
		stackSource.deallocate(memBlock);
	}
	{

		const auto memBlock{ stackSource.allocate(mem_t(0_B), mem_t(16_B)) };
		EXPECT_EQ(memBlock.ptr, reinterpret_cast<std::byte*>(&stackSource));
		EXPECT_EQ(memBlock.size, size_t(16_B));

		const auto memBlock2{ stackSource.allocate(mem_t(16_B), mem_t(16_B)) };
		EXPECT_EQ(memBlock2.ptr, reinterpret_cast<std::byte*>(&stackSource) + 16_B);
		EXPECT_EQ(memBlock2.size, size_t(16_B));

		stackSource.deallocate(memBlock);
		stackSource.deallocate(memBlock2);

	}

	{
		const auto memBlock{ stackSource.allocate(mem_t(0_B), mem_t(200_B)) };
		EXPECT_EQ(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, size_t(0));
	}

	{

		const auto memBlock{ stackSource.allocate(mem_t(16_B), mem_t(64_B)) };
		EXPECT_EQ(memBlock.ptr, reinterpret_cast<std::byte*>(&stackSource) + mem_t(16_B));
		EXPECT_EQ(memBlock.size, size_t(64_B));

		const auto memBlock2{ stackSource.allocate(mem_t(64_B), mem_t(128_B)) };
		EXPECT_EQ(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, size_t(0));

		stackSource.deallocate(memBlock);

		const auto memBlock3{ stackSource.allocate(mem_t(0_B), mem_t(128_B)) };
		EXPECT_EQ(memBlock3.ptr, reinterpret_cast<std::byte*>(&stackSource));
		EXPECT_EQ(memBlock3.size, size_t(128_B));
		stackSource.deallocate(memBlock3);

	}

}