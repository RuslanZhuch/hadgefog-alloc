#include "pch.h"

#include "Protection.h"

import hfog.MemoryUtils;
import hfog.Algorithms.Islands;

import hfog.Sources.Local;
import hfog.Sources.Ext;

import hfog.Core;

import hfog.Alloc;

using namespace hfog::MemoryUtils::Literals;

static_assert(hfog::CtAllocator<hfog::Algorithms::Islands<hfog::Sources::External<hfog::GarbageWriter::Default>, 16_B, 4, 1>>);
static_assert(hfog::CtAllocator<hfog::Algorithms::Islands<hfog::Sources::Local<128_B, hfog::GarbageWriter::Default>, 16_B, 4, 1>>);

static void tsContimuousAllocationImpl(auto& islands)
{

	static constexpr auto ALLOCK_SIZE{ 64_B };

	const auto memBlock1{ islands.allocate(ALLOCK_SIZE) };
	EXPECT_NE(memBlock1.ptr, nullptr);
	EXPECT_EQ(memBlock1.size, ALLOCK_SIZE);

	const auto memBlock2{ islands.allocate(ALLOCK_SIZE) };
	EXPECT_NE(memBlock2.ptr, nullptr);
	EXPECT_EQ(memBlock2.size, ALLOCK_SIZE);

	const auto memBlock3{ islands.allocate(ALLOCK_SIZE) };
	EXPECT_NE(memBlock3.ptr, nullptr);
	EXPECT_EQ(memBlock3.size, ALLOCK_SIZE);

	const auto memBlock4{ islands.allocate(ALLOCK_SIZE) };
	EXPECT_NE(memBlock4.ptr, nullptr);
	EXPECT_EQ(memBlock4.size, ALLOCK_SIZE);

	const auto memBlock5{ islands.allocate(ALLOCK_SIZE) };
	EXPECT_EQ(memBlock5.ptr, nullptr);

	islands.deallocate(memBlock4);

	const auto memBlock6{ islands.allocate(ALLOCK_SIZE) };
	EXPECT_NE(memBlock6.ptr, nullptr);
	EXPECT_EQ(memBlock6.size, ALLOCK_SIZE);

	islands.deallocate(memBlock6);
	islands.deallocate(memBlock3);
	islands.deallocate(memBlock2);
	islands.deallocate(memBlock1);

}

static void tsHeterogeniousAllocationImpl(auto& islands)
{

	static constexpr auto ALLOCK_SIZE_SMALL{ 32_B };
	static constexpr auto ALLOCK_SIZE_LARGE{ 64_B };

	const auto memBlock1{ islands.allocate(ALLOCK_SIZE_SMALL) };
	EXPECT_NE(memBlock1.ptr, nullptr);
	EXPECT_EQ(memBlock1.size, ALLOCK_SIZE_SMALL);

	const auto memBlock2{ islands.allocate(ALLOCK_SIZE_LARGE) };
	EXPECT_NE(memBlock2.ptr, nullptr);
	EXPECT_EQ(memBlock2.size, ALLOCK_SIZE_LARGE);

	const auto memBlock3{ islands.allocate(ALLOCK_SIZE_LARGE) };
	EXPECT_NE(memBlock3.ptr, nullptr);
	EXPECT_EQ(memBlock3.size, ALLOCK_SIZE_LARGE);

	const auto memBlock4{ islands.allocate(ALLOCK_SIZE_LARGE) };
	EXPECT_NE(memBlock4.ptr, nullptr);
	EXPECT_EQ(memBlock4.size, ALLOCK_SIZE_LARGE);

	const auto memBlock5{ islands.allocate(ALLOCK_SIZE_SMALL) };
	EXPECT_NE(memBlock5.ptr, nullptr);
	EXPECT_EQ(memBlock5.size, ALLOCK_SIZE_SMALL);

	islands.deallocate(memBlock1);

	const auto memBlock6{ islands.allocate(ALLOCK_SIZE_LARGE) };
	EXPECT_EQ(memBlock6.ptr, nullptr);

	islands.deallocate(memBlock5);

	const auto memBlock7{ islands.allocate(ALLOCK_SIZE_LARGE) };
	EXPECT_NE(memBlock7.ptr, nullptr);
	EXPECT_EQ(memBlock7.size, ALLOCK_SIZE_LARGE);

}

static void tsGapsImpl(auto& islands)
{

	static constexpr auto ALLOCK_SIZE_SMALL{ 16_B };
	static constexpr auto ALLOCK_SIZE_LARGE{ 48_B };

	const auto memBlock1{ islands.allocate(ALLOCK_SIZE_LARGE) };
	EXPECT_NE(memBlock1.ptr, nullptr);
	EXPECT_EQ(memBlock1.size, ALLOCK_SIZE_LARGE);

	const auto memBlock2{ islands.allocate(ALLOCK_SIZE_LARGE) };
	EXPECT_NE(memBlock2.ptr, nullptr);
	EXPECT_EQ(memBlock2.size, ALLOCK_SIZE_LARGE);

	const auto memBlock3{ islands.allocate(ALLOCK_SIZE_LARGE) };
	EXPECT_NE(memBlock3.ptr, nullptr);
	EXPECT_EQ(memBlock3.size, ALLOCK_SIZE_LARGE);

	const auto memBlock4{ islands.allocate(ALLOCK_SIZE_LARGE) };
	EXPECT_NE(memBlock4.ptr, nullptr);
	EXPECT_EQ(memBlock4.size, ALLOCK_SIZE_LARGE);

	const auto memBlock5{ islands.allocate(ALLOCK_SIZE_LARGE) };
	EXPECT_EQ(memBlock5.ptr, nullptr);

	const auto memBlock6{ islands.allocate(ALLOCK_SIZE_SMALL) };
	EXPECT_NE(memBlock6.ptr, nullptr);
	EXPECT_EQ(memBlock6.size, ALLOCK_SIZE_SMALL);

	const auto memBlock7{ islands.allocate(ALLOCK_SIZE_SMALL) };
	EXPECT_NE(memBlock7.ptr, nullptr);
	EXPECT_EQ(memBlock7.size, ALLOCK_SIZE_SMALL);

	const auto memBlock8{ islands.allocate(ALLOCK_SIZE_SMALL) };
	EXPECT_NE(memBlock8.ptr, nullptr);
	EXPECT_EQ(memBlock8.size, ALLOCK_SIZE_SMALL);

	const auto memBlock9{ islands.allocate(ALLOCK_SIZE_SMALL) };
	EXPECT_NE(memBlock9.ptr, nullptr);
	EXPECT_EQ(memBlock9.size, ALLOCK_SIZE_SMALL);

	{
		const auto memBlock{ islands.allocate(ALLOCK_SIZE_SMALL) };
		EXPECT_EQ(memBlock.ptr, nullptr);
	}

	islands.deallocate(memBlock1);
	{
		const auto memBlock{ islands.allocate(ALLOCK_SIZE_SMALL) };
		EXPECT_EQ(memBlock.ptr, nullptr);
	}
	islands.deallocate(memBlock6);

	const auto memBlock10{ islands.allocate(ALLOCK_SIZE_SMALL) };
	EXPECT_NE(memBlock10.ptr, nullptr);
	EXPECT_EQ(memBlock10.size, ALLOCK_SIZE_SMALL);

	const auto memBlock11{ islands.allocate(ALLOCK_SIZE_LARGE) };
	EXPECT_NE(memBlock11.ptr, nullptr);
	EXPECT_EQ(memBlock11.size, ALLOCK_SIZE_LARGE);
	{
		const auto memBlock{ islands.allocate(ALLOCK_SIZE_SMALL) };
		EXPECT_EQ(memBlock.ptr, nullptr);
	}

}

static void tsBadDataImp(auto& islands)
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

static void tsOneChunckImpl(auto& islands)
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

static void tsMemoryCheckImpl(auto& islands)
{
	const auto memBlock1{ islands.allocate(64_B) };
	EXPECT_TRUE(getValuesAre(memBlock1.ptr, 0_B, 64_B, SET_BYTES));

	const auto memBlock2{ islands.allocate(40_B) };
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 48_B, SET_BYTES));

	const auto memBlock3{ islands.allocate(20_B) };
	EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, 32_B, SET_BYTES));

	const auto memBlock4{ islands.allocate(8_B) };
	EXPECT_TRUE(getValuesAre(memBlock4.ptr, 0_B, 16_B, SET_BYTES));

	const auto memBlock5{ islands.allocate(12_B) };
	EXPECT_TRUE(getValuesAre(memBlock5.ptr, 0_B, 16_B, SET_BYTES));

	const auto memBlock6{ islands.allocate(28_B) };
	EXPECT_TRUE(getValuesAre(memBlock6.ptr, 0_B, 32_B, SET_BYTES));

	islands.deallocate(memBlock2);
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 48_B, CLEAR_BYTES));

	const auto memBlock7{ islands.allocate(28_B) };
	EXPECT_TRUE(getValuesAre(memBlock7.ptr, 0_B, 32_B, SET_BYTES));

	islands.deallocate(memBlock4);
	EXPECT_TRUE(getValuesAre(memBlock4.ptr, 0_B, 16_B, CLEAR_BYTES));

	const auto memBlock9{ islands.allocate(16_B) };
	EXPECT_TRUE(getValuesAre(memBlock9.ptr, 0_B, 16_B, SET_BYTES));

	const auto memBlock10{ islands.allocate(64_B) };
	EXPECT_TRUE(getValuesAre(memBlock10.ptr, 0_B, 64_B, SET_BYTES));
}

static void tsOwnImpl(auto& islands, auto& val)
{

	EXPECT_FALSE(islands.getIsOwner(&val));
	EXPECT_FALSE(islands.getIsOwner(nullptr));

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

static void tsFullDeallocationImpl(auto& islands)
{

	for (size_t itersLeft{ 3 }; itersLeft > 0; --itersLeft)
	{

		const auto memBlock1{ islands.allocate(64_B) };
		EXPECT_TRUE(getValuesAre(memBlock1.ptr, 0_B, 64_B, SET_BYTES));

		const auto memBlock2{ islands.allocate(40_B) };
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 48_B, SET_BYTES));

		const auto memBlock3{ islands.allocate(20_B) };
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, 32_B, SET_BYTES));

		const auto memBlock4{ islands.allocate(8_B) };
		EXPECT_TRUE(getValuesAre(memBlock4.ptr, 0_B, 16_B, SET_BYTES));

		const auto memBlock5{ islands.allocate(12_B) };
		EXPECT_TRUE(getValuesAre(memBlock5.ptr, 0_B, 16_B, SET_BYTES));

		const auto memBlock6{ islands.allocate(28_B) };
		EXPECT_TRUE(getValuesAre(memBlock6.ptr, 0_B, 32_B, SET_BYTES));

		islands.deallocate();
		EXPECT_TRUE(getValuesAre(memBlock1.ptr, 0_B, 64_B, CLEAR_BYTES));
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, 48_B, CLEAR_BYTES));
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, 32_B, CLEAR_BYTES));
		EXPECT_TRUE(getValuesAre(memBlock4.ptr, 0_B, 16_B, CLEAR_BYTES));
		EXPECT_TRUE(getValuesAre(memBlock5.ptr, 0_B, 16_B, CLEAR_BYTES));
		EXPECT_TRUE(getValuesAre(memBlock6.ptr, 0_B, 32_B, CLEAR_BYTES));

	}

}

static void tsCrashImpl(auto& islands)
{
	const auto memBlock1_1{ islands.allocate(12_B) };
	const auto memBlock1_2{ islands.allocate(24_B) };
	islands.deallocate(memBlock1_1);
	const auto memBlock1_3{ islands.allocate(48_B) };
	islands.deallocate(memBlock1_2);
	const auto memBlock1_4{ islands.allocate(32_B) };
	islands.deallocate(memBlock1_3);

	const auto memBlock2_1{ islands.allocate(64_B) };

	islands.deallocate(memBlock1_4);

	const auto memBlock2_2{ islands.allocate(64_B) };
	islands.deallocate(memBlock2_1);

	const auto memBlock3_1{ islands.allocate(128_B) };
}

static auto& generateIslandsExtMemoryBlock()
{

	static byte_t extBuffer[256];
	std::memset(extBuffer, 0, sizeof(extBuffer));

	static hfog::MemoryBlock extBlock;
	extBlock.ptr = extBuffer;
	extBlock.size = sizeof(extBuffer);

	return extBlock;

}

static constexpr auto BUFFER_SIZE{ 256_B };
static constexpr auto ALIGNMENT{ 16_B };
static constexpr auto NUM_OF_SEGMENTS{ 4 };
static constexpr auto NUM_OF_CHUNCKS{ 4 };

using sourceExt_t = hfog::Sources::External<hfog::GarbageWriter::Default>;
using islandsExt_t = hfog::Algorithms::Islands<sourceExt_t, ALIGNMENT, NUM_OF_SEGMENTS, NUM_OF_CHUNCKS>;

template <typename Garbage_t>
using sourceExtG_t = hfog::Sources::External<Garbage_t>;

template <typename Garbage_t>
using islandsExtG_t = hfog::Algorithms::Islands<sourceExtG_t<Garbage_t>, ALIGNMENT, NUM_OF_SEGMENTS, NUM_OF_CHUNCKS>;


using sourceLocal_t = hfog::Sources::Local<BUFFER_SIZE, hfog::GarbageWriter::Default>;
using islandsLocal_t = hfog::Algorithms::Islands<sourceLocal_t, ALIGNMENT, NUM_OF_SEGMENTS, NUM_OF_CHUNCKS>;

template <typename Garbage_t>
using sourceLocalG_t = hfog::Sources::Local<BUFFER_SIZE, Garbage_t>;

template <typename Garbage_t>
using islandsLocalG_t = hfog::Algorithms::Islands<sourceLocalG_t<Garbage_t>, ALIGNMENT, NUM_OF_SEGMENTS, NUM_OF_CHUNCKS>;

TEST(AllocAlgorithmsIslands, tsCheckProtection)
{

	{
		islandsExt_t islands(generateIslandsExtMemoryBlock());
		const auto failedMemBlock{ islands.allocate(ALIGNMENT * NUM_OF_CHUNCKS * 2) };
		EXPECT_EQ(hfogprotect::instance()->getLastError().value().code, hfogprotect::ErrorCodes::NOT_IN_RANGE);

		islands.allocate(ALIGNMENT * NUM_OF_CHUNCKS);
		islands.allocate(ALIGNMENT * NUM_OF_CHUNCKS);
		islands.allocate(ALIGNMENT * NUM_OF_CHUNCKS);
		islands.allocate(ALIGNMENT * NUM_OF_CHUNCKS);
		EXPECT_FALSE(hfogprotect::instance()->getLastError().has_value());
		islands.allocate(ALIGNMENT * NUM_OF_CHUNCKS);
		EXPECT_EQ(hfogprotect::instance()->getLastError().value().code, hfogprotect::ErrorCodes::INVALID_PTR);
	}

	{
		byte_t extBuffer[1];
		hfog::MemoryBlock extBlock;
		extBlock.ptr = extBuffer;
		extBlock.size = sizeof(extBuffer);

		hfog::Algorithms::Islands<sourceExt_t, ALIGNMENT, NUM_OF_SEGMENTS, NUM_OF_CHUNCKS> islands(extBlock);

		const auto failedMemBlock{ islands.allocate(ALIGNMENT * NUM_OF_CHUNCKS) };
		EXPECT_EQ(hfogprotect::instance()->getLastError().value().code, hfogprotect::ErrorCodes::INVALID_PTR);
	}
	
}

TEST(AllocAlgorithmsIslands, tsSourcesExtContimuousAllocation)
{
	islandsExt_t islands(generateIslandsExtMemoryBlock());
	tsContimuousAllocationImpl(islands);
}

TEST(AllocAlgorithmsIslands, tsSourcesLocalContimuousAllocation)
{
	islandsLocal_t islands;
	tsContimuousAllocationImpl(islands);
}

TEST(AllocAlgorithmsIslands, tsSourcesExtHeterogeniousAllocation)
{
	islandsExt_t islands(generateIslandsExtMemoryBlock());
	tsHeterogeniousAllocationImpl(islands);
}

TEST(AllocAlgorithmsIslands, tsSourcesLocalHeterogeniousAllocation)
{
	islandsLocal_t islands;
	tsHeterogeniousAllocationImpl(islands);
}

TEST(AllocAlgorithmsIslands, tsSourcesExtGaps)
{
	islandsExt_t islands(generateIslandsExtMemoryBlock());
	tsGapsImpl(islands);
}

TEST(AllocAlgorithmsIslands, tsSourcesLocalGaps)
{
	islandsLocal_t islands;
	tsGapsImpl(islands);
}

TEST(AllocAlgorithmsIslands, tsSourcesExtBadData)
{
	islandsExt_t islands(generateIslandsExtMemoryBlock());
	tsBadDataImp(islands);
}

TEST(AllocAlgorithmsIslands, tsSourcesLocalBadData)
{
	islandsLocal_t islands;
	tsBadDataImp(islands);
}

TEST(AllocAlgorithmsIslands, tsSourcesExtOneChunck)
{
	hfog::Algorithms::Islands<sourceExt_t, ALIGNMENT, NUM_OF_SEGMENTS, 1> islands(generateIslandsExtMemoryBlock());
	tsOneChunckImpl(islands);
}

TEST(AllocAlgorithmsIslands, tsSourcesLocalOneChunck)
{
	hfog::Algorithms::Islands<sourceLocal_t, ALIGNMENT, NUM_OF_SEGMENTS, 1> islands;
	tsOneChunckImpl(islands);
}

TEST(AllocAlgorithmsIslands, tsSourcesExtMemoryCheck)
{
	islandsExtG_t<hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>> islands(generateIslandsExtMemoryBlock());
	tsMemoryCheckImpl(islands);
}

TEST(AllocAlgorithmsIslands, tsSourcesLocalMemoryCheck)
{
	islandsLocalG_t<hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>> islands;
	tsMemoryCheckImpl(islands);
}

TEST(AllocAlgorithmsIslands, tsSourcesExtOwn)
{
	byte_t val{ 42 };
	islandsExt_t islands(generateIslandsExtMemoryBlock());
	tsOwnImpl(islands, val);
}

TEST(AllocAlgorithmsIslands, tsSourcesLocalOwn)
{
	byte_t val{ 42 };
	islandsLocal_t islands;
	tsOwnImpl(islands, val);
}

TEST(AllocAlgorithmsIslands, tsSourcesExtFullDeallocation)
{
	islandsExtG_t<hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>> islands(generateIslandsExtMemoryBlock());
	tsFullDeallocationImpl(islands);
}

TEST(AllocAlgorithmsIslands, tsSourcesLocalFullDeallocation)
{
	islandsLocalG_t<hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>> islands;
	tsFullDeallocationImpl(islands);
}

TEST(AllocAlgorithmsIslands, tsSourcesExtCrash)
{

	byte_t extBuffer[512_B];
	using allocator_t = hfog::Alloc::IslandsExt<4_B, 128_B, 4, hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>>;
	allocator_t alloc(hfog::MemoryBlock(extBuffer, sizeof(extBuffer)));

	tsCrashImpl(alloc);

}