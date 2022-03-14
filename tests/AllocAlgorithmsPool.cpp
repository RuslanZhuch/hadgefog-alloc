#include "pch.h"

import AlgPool;

import SourceHeap;
import SourceExt;
import SourceStack;

import hfogCore;

using namespace hfog::MemoryUtils::Literals;

static_assert(hfog::CtAllocator<hfog::Algorithms::Pool<hfog::Sources::Heap<hfog::GarbageWriter::Default>, 16_B, 1>>);
static_assert(hfog::CtAllocator<hfog::Algorithms::Pool<hfog::Sources::External<hfog::GarbageWriter::Default>, 16_B, 4>>);
static_assert(hfog::CtAllocator<hfog::Algorithms::Pool<hfog::Sources::Stack<128_B, hfog::GarbageWriter::Default>, 16_B, 8>>);


TEST(AllocAlgorithms, tsPool)
{

	using source_t = hfog::Sources::Heap<hfog::GarbageWriter::Default>;

	hfog::Algorithms::Pool<source_t, 16_B, 4> pool;

	{

		const auto memBlock1{ pool.allocate(64_B) };
		EXPECT_EQ(memBlock1.ptr, nullptr);
		//EXPECT_EQ(memBlock1.size, 64_B);

		const auto memBlock2{ pool.allocate(16_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 16_B);

		const auto memBlock3{ pool.allocate(16_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 16_B);

		const auto memBlock4{ pool.allocate(16_B) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, 16_B);

		const auto memBlock5{ pool.allocate(16_B) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, 16_B);

		const auto memBlock6{ pool.allocate(16_B) };
		EXPECT_EQ(memBlock6.ptr, nullptr);

		pool.deallocate(memBlock4);

		const auto memBlock7{ pool.allocate(16_B) };
		EXPECT_NE(memBlock7.ptr, nullptr);
		EXPECT_EQ(memBlock7.size, 16_B);

		pool.deallocate(memBlock2);
		pool.deallocate(memBlock3);
		pool.deallocate(memBlock7);
		pool.deallocate(memBlock5);


		const auto memBlock2_1{ pool.allocate(16_B) };
		EXPECT_NE(memBlock2_1.ptr, nullptr);
		EXPECT_EQ(memBlock2_1.size, 16_B);

		const auto memBlock2_2{ pool.allocate(16_B) };
		EXPECT_NE(memBlock2_2.ptr, nullptr);
		EXPECT_EQ(memBlock2_2.size, 16_B);

		const auto memBlock2_3{ pool.allocate(16_B) };
		EXPECT_NE(memBlock2_3.ptr, nullptr);
		EXPECT_EQ(memBlock2_3.size, 16_B);

		const auto memBlock2_4{ pool.allocate(16_B) };
		EXPECT_NE(memBlock2_4.ptr, nullptr);
		EXPECT_EQ(memBlock2_4.size, 16_B);

		auto ownedMemBlock2_1{ pool.getIsOwner(memBlock2_1.ptr) };
		EXPECT_EQ(ownedMemBlock2_1, true);

		auto ownedMemBlock2_2{ pool.getIsOwner(memBlock2_2.ptr) };
		EXPECT_EQ(ownedMemBlock2_2, true);

		auto ownedMemBlock2_3{ pool.getIsOwner(memBlock2_3.ptr) };
		EXPECT_EQ(ownedMemBlock2_3, true);

		auto ownedMemBlock2_4{ pool.getIsOwner(memBlock2_4.ptr) };
		EXPECT_EQ(ownedMemBlock2_4, true);

		pool.deallocate(memBlock2_1);
		ownedMemBlock2_1 = pool.getIsOwner(memBlock2_1.ptr);
		EXPECT_EQ(ownedMemBlock2_1, false);
		ownedMemBlock2_2 = pool.getIsOwner(memBlock2_2.ptr);
		EXPECT_EQ(ownedMemBlock2_2, true);
		ownedMemBlock2_3 =  pool.getIsOwner(memBlock2_3.ptr);
		EXPECT_EQ(ownedMemBlock2_3, true);
		ownedMemBlock2_4 = pool.getIsOwner(memBlock2_4.ptr);
		EXPECT_EQ(ownedMemBlock2_4, true);

		pool.deallocate(memBlock2_3);
		ownedMemBlock2_1 = pool.getIsOwner(memBlock2_1.ptr);
		EXPECT_EQ(ownedMemBlock2_1, false);
		ownedMemBlock2_2 = pool.getIsOwner(memBlock2_2.ptr);
		EXPECT_EQ(ownedMemBlock2_2, true);
		ownedMemBlock2_3 = pool.getIsOwner(memBlock2_3.ptr);
		EXPECT_EQ(ownedMemBlock2_3, false);
		ownedMemBlock2_4 = pool.getIsOwner(memBlock2_4.ptr);
		EXPECT_EQ(ownedMemBlock2_4, true);

		pool.deallocate(memBlock2_2);
		ownedMemBlock2_1 = pool.getIsOwner(memBlock2_1.ptr);
		EXPECT_EQ(ownedMemBlock2_1, false);
		ownedMemBlock2_2 = pool.getIsOwner(memBlock2_2.ptr);
		EXPECT_EQ(ownedMemBlock2_2, false);
		ownedMemBlock2_3 = pool.getIsOwner(memBlock2_3.ptr);
		EXPECT_EQ(ownedMemBlock2_3, false);
		ownedMemBlock2_4 = pool.getIsOwner(memBlock2_4.ptr);
		EXPECT_EQ(ownedMemBlock2_4, true);

		pool.deallocate(memBlock2_4);
		ownedMemBlock2_1 = pool.getIsOwner(memBlock2_1.ptr);
		EXPECT_EQ(ownedMemBlock2_1, false);
		ownedMemBlock2_2 = pool.getIsOwner(memBlock2_2.ptr);
		EXPECT_EQ(ownedMemBlock2_2, false);
		ownedMemBlock2_3 = pool.getIsOwner(memBlock2_3.ptr);
		EXPECT_EQ(ownedMemBlock2_3, false);
		ownedMemBlock2_4 = pool.getIsOwner(memBlock2_4.ptr);
		EXPECT_EQ(ownedMemBlock2_4, false);

	}
}
