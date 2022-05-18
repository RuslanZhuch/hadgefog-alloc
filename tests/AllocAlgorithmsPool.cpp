#include "pch.h"

import hfog.Algorithms.Pool;

import hfog.Sources.Ext;
import hfog.Sources.Local;

import hfog.Core;

using namespace hfog::MemoryUtils::Literals;

static_assert(hfog::CtAllocator<hfog::Algorithms::Pool<hfog::Sources::External<hfog::GarbageWriter::Default>, 16_B, 4>>);
static_assert(hfog::CtAllocator<hfog::Algorithms::Pool<hfog::Sources::Local<128_B, hfog::GarbageWriter::Default>, 16_B, 8>>);

static void tsAllocateContImpl(auto& pool)
{

	static constexpr auto OUT_OF_BOUNDS_ALLOCATION_SIZE{ 64_B };
	static constexpr auto NORMAL_ALLOCATION_SIZE{ 16_B };

	const auto memBlock1{ pool.allocate(OUT_OF_BOUNDS_ALLOCATION_SIZE) };
	EXPECT_EQ(memBlock1.ptr, nullptr);
	//EXPECT_EQ(memBlock1.size, 64_B);

	const auto memBlock2{ pool.allocate(NORMAL_ALLOCATION_SIZE) };
	EXPECT_NE(memBlock2.ptr, nullptr);
	EXPECT_EQ(memBlock2.size, NORMAL_ALLOCATION_SIZE);

	const auto memBlock3{ pool.allocate(NORMAL_ALLOCATION_SIZE) };
	EXPECT_NE(memBlock3.ptr, nullptr);
	EXPECT_EQ(memBlock3.size, NORMAL_ALLOCATION_SIZE);

	const auto memBlock4{ pool.allocate(NORMAL_ALLOCATION_SIZE) };
	EXPECT_NE(memBlock4.ptr, nullptr);
	EXPECT_EQ(memBlock4.size, NORMAL_ALLOCATION_SIZE);

	const auto memBlock5{ pool.allocate(NORMAL_ALLOCATION_SIZE) };
	EXPECT_NE(memBlock5.ptr, nullptr);
	EXPECT_EQ(memBlock5.size, NORMAL_ALLOCATION_SIZE);

	const auto memBlock6{ pool.allocate(NORMAL_ALLOCATION_SIZE) };
	EXPECT_EQ(memBlock6.ptr, nullptr);

	pool.deallocate(memBlock4);

	const auto memBlock7{ pool.allocate(NORMAL_ALLOCATION_SIZE) };
	EXPECT_NE(memBlock7.ptr, nullptr);
	EXPECT_EQ(memBlock7.size, NORMAL_ALLOCATION_SIZE);

	pool.deallocate(memBlock2);
	pool.deallocate(memBlock3);
	pool.deallocate(memBlock7);
	pool.deallocate(memBlock5);

}

static void tsOwnerCheckImpl(auto& pool)
{

	static constexpr auto NORMAL_ALLOCATION_SIZE{ 16_B };

	const auto memBlock2_1{ pool.allocate(NORMAL_ALLOCATION_SIZE) };
	EXPECT_NE(memBlock2_1.ptr, nullptr);
	EXPECT_EQ(memBlock2_1.size, NORMAL_ALLOCATION_SIZE);

	const auto memBlock2_2{ pool.allocate(NORMAL_ALLOCATION_SIZE) };
	EXPECT_NE(memBlock2_2.ptr, nullptr);
	EXPECT_EQ(memBlock2_2.size, NORMAL_ALLOCATION_SIZE);

	const auto memBlock2_3{ pool.allocate(NORMAL_ALLOCATION_SIZE) };
	EXPECT_NE(memBlock2_3.ptr, nullptr);
	EXPECT_EQ(memBlock2_3.size, NORMAL_ALLOCATION_SIZE);

	const auto memBlock2_4{ pool.allocate(NORMAL_ALLOCATION_SIZE) };
	EXPECT_NE(memBlock2_4.ptr, nullptr);
	EXPECT_EQ(memBlock2_4.size, NORMAL_ALLOCATION_SIZE);

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
	ownedMemBlock2_3 = pool.getIsOwner(memBlock2_3.ptr);
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

static constexpr auto ALIGNMENT{ 16_B };
static constexpr auto NUM_OF_CHUNCKS{ 4 };

using sourceExternal_t = hfog::Sources::External<hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>>;
using poolExt_t = hfog::Algorithms::Pool<sourceExternal_t, ALIGNMENT, NUM_OF_CHUNCKS>;
static constexpr auto BUFFER_LEN{ 64_B };

static const auto& generatePoolExtMemBlock()
{
	static byte_t extMemBuffer[BUFFER_LEN];
	std::memset(extMemBuffer, 0, sizeof(extMemBuffer));

	static hfog::MemoryBlock extMemBlock;
	extMemBlock.ptr = extMemBuffer;
	extMemBlock.size = BUFFER_LEN;

	return extMemBlock;

}

TEST(AllocAlgorithmsPool, tsSoruceExtAllocateCont)
{

	poolExt_t pool(generatePoolExtMemBlock());
	tsAllocateContImpl(pool);

}

TEST(AllocAlgorithmsPool, tsSoruceExtOwnerCheck)
{

	poolExt_t pool(generatePoolExtMemBlock());
	tsOwnerCheckImpl(pool);

}

using sourceLocal_t = hfog::Sources::Local<BUFFER_LEN, hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>>;
using poolLocal_t = hfog::Algorithms::Pool<sourceExternal_t, ALIGNMENT, NUM_OF_CHUNCKS>;

TEST(AllocAlgorithmsPool, tsSoruceLocalAllocateCont)
{

	poolLocal_t pool(generatePoolExtMemBlock());
	tsAllocateContImpl(pool);

}

TEST(AllocAlgorithmsPool, tsSoruceLocalOwnerCheck)
{

	poolLocal_t pool(generatePoolExtMemBlock());
	tsOwnerCheckImpl(pool);

}
