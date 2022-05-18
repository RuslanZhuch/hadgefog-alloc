#include "pch.h"

#include "Protection.h"

import hfog.MemoryUtils;
import hfog.Algorithms.Unified;

import hfog.Sources.Local;
import hfog.Sources.Ext;

import hfog.Core;

using namespace hfog::MemoryUtils::Literals;

static_assert(hfog::CtAllocator<hfog::Algorithms::Unified<hfog::Sources::External<hfog::GarbageWriter::Default>, 128_B, 4_kB>>);
static_assert(hfog::CtAllocator<hfog::Algorithms::Unified<hfog::Sources::Local<128_kB, hfog::GarbageWriter::Default>, 128_B, 8_kB>>);

static void tsContinuousAllocationImpl(auto& unified)
{

	static constexpr auto ALLOC_SIZE{ 80_kB };

	for (size_t numOfIters{ 0 }; numOfIters < 2; ++numOfIters)
	{

		const auto memBlock1{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, ALLOC_SIZE);

		const auto memBlock2{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, ALLOC_SIZE);

		const auto memBlock3{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, ALLOC_SIZE);

		const auto memBlock4{ unified.allocate(ALLOC_SIZE) };
		EXPECT_EQ(memBlock4.ptr, nullptr);

		unified.deallocate(memBlock3);

		const auto memBlock5{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, ALLOC_SIZE);

		unified.deallocate(memBlock5);
		unified.deallocate(memBlock2);
		unified.deallocate(memBlock1);

	}

}

static void tsFragmentationImpl(auto& unified)
{

	static constexpr auto ALLOC_SIZE{ 32_B };

	for (size_t numOfIters{ 0 }; numOfIters < 2; ++numOfIters)
	{

		const auto memBlock1{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, ALLOC_SIZE);

		const auto memBlock2{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, ALLOC_SIZE);

		const auto memBlock3{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, ALLOC_SIZE);

		const auto memBlock4{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, ALLOC_SIZE);

		const auto memBlock5{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, ALLOC_SIZE);

		unified.deallocate(memBlock1);
		unified.deallocate(memBlock3);
		unified.deallocate(memBlock5);

		unified.deallocate(memBlock2);
		unified.deallocate(memBlock4);

	}

}

static void tsRefillImpl(auto& unified)
{

	static constexpr auto ALLOC_SIZE{ 32_B };

	for (size_t numOfIters{ 0 }; numOfIters < 2; ++numOfIters)
	{

		const auto memBlock1{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, ALLOC_SIZE);

		const auto memBlock2{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, ALLOC_SIZE);

		const auto memBlock3{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, ALLOC_SIZE);

		const auto memBlock4{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, ALLOC_SIZE);

		const auto memBlock5{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, ALLOC_SIZE);

		const auto memBlockOB1{ unified.allocate(ALLOC_SIZE) };
		EXPECT_EQ(memBlockOB1.ptr, nullptr);

		unified.deallocate(memBlock1);
		unified.deallocate(memBlock3);
		unified.deallocate(memBlock5);

		const auto memBlock6{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock6.ptr, nullptr);
		EXPECT_EQ(memBlock6.size, ALLOC_SIZE);

		const auto memBlock7{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock7.ptr, nullptr);
		EXPECT_EQ(memBlock7.size, ALLOC_SIZE);

		const auto memBlock8{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock8.ptr, nullptr);
		EXPECT_EQ(memBlock8.size, ALLOC_SIZE);

		const auto memBlockOB2{ unified.allocate(ALLOC_SIZE) };
		EXPECT_EQ(memBlockOB2.ptr, nullptr);

		unified.deallocate();

	}

}

static void tsGapsImpl(auto& unified)
{

	static constexpr auto ALLOC_SIZE_SMALL{ 32_B };
	static constexpr auto ALLOC_SIZE_LARGE{ 64_B };

	for (size_t numOfIters{ 0 }; numOfIters < 2; ++numOfIters)
	{

		const auto memBlock1{ unified.allocate(ALLOC_SIZE_SMALL) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, ALLOC_SIZE_SMALL);

		const auto memBlock2{ unified.allocate(ALLOC_SIZE_SMALL) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, ALLOC_SIZE_SMALL);

		const auto memBlock3{ unified.allocate(ALLOC_SIZE_SMALL) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, ALLOC_SIZE_SMALL);

		const auto memBlock4{ unified.allocate(ALLOC_SIZE_SMALL) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, ALLOC_SIZE_SMALL);

		const auto memBlock5{ unified.allocate(ALLOC_SIZE_SMALL) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, ALLOC_SIZE_SMALL);

		const auto memBlockOB1{ unified.allocate(ALLOC_SIZE_SMALL) };
		EXPECT_EQ(memBlockOB1.ptr, nullptr);

		unified.deallocate(memBlock1);

		const auto memBlockOB2{ unified.allocate(ALLOC_SIZE_LARGE) };
		EXPECT_EQ(memBlockOB2.ptr, nullptr);

		unified.deallocate(memBlock2);

		const auto memBlock6{ unified.allocate(ALLOC_SIZE_LARGE) };
		EXPECT_NE(memBlock6.ptr, nullptr);
		EXPECT_EQ(memBlock6.size, ALLOC_SIZE_LARGE);

		unified.deallocate();

	}

}

static void tsFillingImpl(auto& unified)
{

	static constexpr auto ALLOC_SIZE_SMALL{ 32_B };
	static constexpr auto ALLOC_SIZE_LARGE{ 64_B };

	for (size_t numOfIters{ 0 }; numOfIters < 2; ++numOfIters)
	{

		const auto memBlock1{ unified.allocate(ALLOC_SIZE_SMALL) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, ALLOC_SIZE_SMALL);

		const auto memBlock2{ unified.allocate(ALLOC_SIZE_LARGE) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, ALLOC_SIZE_LARGE);

		const auto memBlock3{ unified.allocate(ALLOC_SIZE_SMALL) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, ALLOC_SIZE_SMALL);

		const auto memBlockOB1{ unified.allocate(ALLOC_SIZE_LARGE) };
		EXPECT_EQ(memBlockOB1.ptr, nullptr);

		const auto memBlock4{ unified.allocate(ALLOC_SIZE_SMALL) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, ALLOC_SIZE_SMALL);

		const auto memBlockOB2{ unified.allocate(ALLOC_SIZE_SMALL) };
		EXPECT_EQ(memBlockOB2.ptr, nullptr);

		unified.deallocate(memBlock2);

		const auto memBlock5{ unified.allocate(ALLOC_SIZE_SMALL) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, ALLOC_SIZE_SMALL);

		const auto memBlockOB3{ unified.allocate(ALLOC_SIZE_LARGE) };
		EXPECT_EQ(memBlockOB3.ptr, nullptr);

		const auto memBlock6{ unified.allocate(ALLOC_SIZE_SMALL) };
		EXPECT_NE(memBlock6.ptr, nullptr);
		EXPECT_EQ(memBlock6.size, ALLOC_SIZE_SMALL);

		const auto memBlockOB4{ unified.allocate(ALLOC_SIZE_SMALL) };
		EXPECT_EQ(memBlockOB4.ptr, nullptr);

		unified.deallocate();

	}

}

static void tsIsOwnerImpl(auto& unified)
{

	static constexpr auto ALLOC_SIZE{ 32_B };

	for (size_t numOfIters{ 0 }; numOfIters < 2; ++numOfIters)
	{

		const auto memBlock1{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, ALLOC_SIZE);

		const auto memBlock2{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, ALLOC_SIZE);

		const auto memBlock3{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, ALLOC_SIZE);

		const auto memBlock4{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, ALLOC_SIZE);

		const auto memBlock5{ unified.allocate(ALLOC_SIZE) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, ALLOC_SIZE);

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

template <mem_t bufferSize>
static auto& generateUnifiedExtMemBlock()
{

	static byte_t extBuffer[bufferSize];
	std::memset(extBuffer, 0, sizeof(extBuffer));

	static hfog::MemoryBlock extBlock;
	extBlock.ptr = extBuffer;
	extBlock.size = sizeof(extBuffer);

	return extBlock;

}

static auto& generateUnifiedExtMemBlockLarge()
{
	return generateUnifiedExtMemBlock<512_kB>();
}

static auto& generateUnifiedExtMemBlockSmall()
{
	return generateUnifiedExtMemBlock<256_B>();
}

static constexpr auto BUFFER_SIZE{ 160_B };
static constexpr auto ALIGNMENT{ 32_B };

using sourceExt_t = hfog::Sources::External<hfog::GarbageWriter::Default>;
using unifiedExt_t = hfog::Algorithms::Unified<sourceExt_t, ALIGNMENT, BUFFER_SIZE>;

using sourceLocal_t = hfog::Sources::Local<256_B, hfog::GarbageWriter::Default>;
using unifiedLocal_t = hfog::Algorithms::Unified<sourceLocal_t, ALIGNMENT, BUFFER_SIZE>;

TEST(AllocAlgorithmsUnified, tsSourceExtProtection)
{

	hfogprotect::instance()->getLastError();

	{
		unifiedExt_t unified(generateUnifiedExtMemBlockSmall());

		unified.allocate(BUFFER_SIZE);
		EXPECT_FALSE(hfogprotect::instance()->getLastError().has_value());
		unified.allocate(BUFFER_SIZE);
		EXPECT_EQ(hfogprotect::instance()->getLastError().value().code, hfogprotect::ErrorCodes::INVALID_PTR);
	}

	{
		unifiedExt_t unified(generateUnifiedExtMemBlockSmall());

		unified.allocate(BUFFER_SIZE / 2);
		EXPECT_FALSE(hfogprotect::instance()->getLastError().has_value());
		unified.allocate(BUFFER_SIZE);
		EXPECT_EQ(hfogprotect::instance()->getLastError().value().code, hfogprotect::ErrorCodes::NOT_IN_RANGE);
	}

}

TEST(AllocAlgorithmsUnified, tsSourceExtContinuousAllocation)
{
	hfog::Algorithms::Unified<sourceExt_t, 128_B, 256_kB> unified(generateUnifiedExtMemBlockLarge());
	tsContinuousAllocationImpl(unified);
}

TEST(AllocAlgorithmsUnified, tsSourceLocalContinuousAllocation)
{
	hfog::Algorithms::Unified<hfog::Sources::Local<512_kB, hfog::GarbageWriter::Default>, 128_B, 256_kB> unified;
	tsContinuousAllocationImpl(unified);
}

TEST(AllocAlgorithmsUnified, tsSorutcesExtFragmentation)
{
	unifiedExt_t unified(generateUnifiedExtMemBlockSmall());
	tsFragmentationImpl(unified);
}

TEST(AllocAlgorithmsUnified, tsSorutcesLocalFragmentation)
{
	unifiedLocal_t unified;
	tsFragmentationImpl(unified);
}

TEST(AllocAlgorithmsUnified, tsSourcesExtRefill)
{
	unifiedExt_t unified(generateUnifiedExtMemBlockSmall());
	tsRefillImpl(unified);
}

TEST(AllocAlgorithmsUnified, tsSourcesLocalRefill)
{
	unifiedLocal_t unified;
	tsRefillImpl(unified);
}

TEST(AllocAlgorithmsUnified, tsSourcesExtGaps)
{
	unifiedExt_t unified(generateUnifiedExtMemBlockSmall());
	tsGapsImpl(unified);
}

TEST(AllocAlgorithmsUnified, tsSourcesLocalGaps)
{
	unifiedLocal_t unified;
	tsGapsImpl(unified);
}

TEST(AllocAlgorithmsUnified, tsSourcesExtFilling)
{
	unifiedExt_t unified(generateUnifiedExtMemBlockSmall());
	tsFillingImpl(unified);
}

TEST(AllocAlgorithmsUnified, tsSourcesLocalFilling)
{
	unifiedLocal_t unified;
	tsFillingImpl(unified);
}

TEST(AllocAlgorithmsUnified, tsSourcesExtIsOwner)
{
	unifiedExt_t unified(generateUnifiedExtMemBlockSmall());
	tsIsOwnerImpl(unified);
}

TEST(AllocAlgorithmsUnified, tsSourcesLocalIsOwner)
{
	unifiedLocal_t unified;
	tsIsOwnerImpl(unified);
}