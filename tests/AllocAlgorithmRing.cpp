#include "pch.h"
#include <array>

import hfog.Sources.Ext;
import hfog.Sources.Local;
import hfog.MemoryUtils;

import hfog.Core;

import hfog.Algorithms.Ring;

using namespace hfog::MemoryUtils::Literals;

static_assert(hfog::CtAllocator<hfog::Algorithms::Ring<hfog::Sources::External<hfog::GarbageWriter::Default>, 16_B>>);
static_assert(hfog::CtAllocator<hfog::Algorithms::Ring<hfog::Sources::Local<128_B, hfog::GarbageWriter::Default>, 16_B>>);

static constexpr auto ALIGNMENT{ 16_B };

using sourceExternal_t = hfog::Sources::External<hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>>;
using stackExt_t = hfog::Algorithms::Ring<sourceExternal_t, ALIGNMENT>;
static constexpr auto BUFFER_LEN{ 256_B };

static const auto& generateStackExtMemBlock()
{
	
	static byte_t extMemBuffer[BUFFER_LEN];
	std::memset(extMemBuffer, 0, sizeof(extMemBuffer));

	static hfog::MemoryBlock extMemBlock;
	extMemBlock.ptr = extMemBuffer;
	extMemBlock.size = BUFFER_LEN;

	return extMemBlock;

}

static void tsSimpleImpl(auto& algRing)
{

	static constexpr auto ALLOC_SIZE{ 64_B };
	constexpr auto NUM_OF_ALLOCS{ BUFFER_LEN / ALLOC_SIZE };

	for (size_t allocId{ 0 }; allocId < NUM_OF_ALLOCS; ++allocId)
	{
		const auto memBlock{ algRing.allocate(ALLOC_SIZE) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0, ALLOC_SIZE, SET_BYTES));
		EXPECT_TRUE(algRing.getIsOwner(memBlock.ptr));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, ALLOC_SIZE);
	}

	const auto memBlock{ algRing.allocate(ALLOC_SIZE) };
	EXPECT_FALSE(algRing.getIsOwner(memBlock.ptr));
	EXPECT_EQ(memBlock.ptr, nullptr);

}

static void tsDeallocateionImpl(auto& algRing)
{

	static constexpr auto ALLOC_SIZE{ 64_B };

	constexpr auto NUM_OF_ALLOCS{ BUFFER_LEN / ALLOC_SIZE };

	std::array<hfog::MemoryBlock, NUM_OF_ALLOCS> blocks;

	const auto allocateSuccess = [&](size_t allocId)
	{
		const auto memBlock{ algRing.allocate(ALLOC_SIZE) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0, ALLOC_SIZE, SET_BYTES));
		EXPECT_TRUE(algRing.getIsOwner(memBlock.ptr));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, ALLOC_SIZE);
		blocks[allocId] = memBlock;
	};

	const auto allocateFail = [&]()
	{
		const auto memBlock{ algRing.allocate(ALLOC_SIZE) };
		EXPECT_FALSE(algRing.getIsOwner(memBlock.ptr));
		EXPECT_EQ(memBlock.ptr, nullptr);
	};

	for (size_t allocId{ 0 }; allocId < NUM_OF_ALLOCS; ++allocId)
	{
		allocateSuccess(allocId);
	}

	for (size_t allocId{ 0 }; allocId < NUM_OF_ALLOCS; ++allocId)
	{
		allocateFail();
		algRing.deallocate(blocks[allocId]);
		allocateSuccess(allocId);
	}

	for (size_t allocId{ 0 }; allocId < NUM_OF_ALLOCS; ++allocId)
	{
		algRing.deallocate(blocks[allocId]);
	}

	for (size_t allocId{ 0 }; allocId < NUM_OF_ALLOCS; ++allocId)
	{
		allocateSuccess(allocId);
		algRing.deallocate(blocks[allocId]);
	}

}

static void tsEdgeAllocationImpl(auto& algRing)
{

	static constexpr auto ALLOC_SIZE{ 64_B };
	static constexpr auto ALLOC_SIZE_LARGE{ 128_B };
	static constexpr auto ALLOC_SIZE_SMALL{ 32_B };

	constexpr auto NUM_OF_ALLOCS{ BUFFER_LEN / ALLOC_SIZE };

	std::array<hfog::MemoryBlock, NUM_OF_ALLOCS> blocks;

	const auto allocateSuccess = [&](mem_t allocSize)
	{
		const auto memBlock{ algRing.allocate(allocSize) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0, allocSize, SET_BYTES));
		EXPECT_TRUE(algRing.getIsOwner(memBlock.ptr));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, allocSize);
		return memBlock;
	};

	const auto blockLeft{ allocateSuccess(ALLOC_SIZE_SMALL) };
	const auto blockLeftMiddle{ allocateSuccess(ALLOC_SIZE_SMALL) };
	const auto blockMiddle{ allocateSuccess(ALLOC_SIZE_LARGE) };
	const auto blockRight{ allocateSuccess(ALLOC_SIZE_SMALL) };

	EXPECT_EQ(algRing.allocate(ALLOC_SIZE).ptr, nullptr);

	algRing.deallocate(blockLeft);

	EXPECT_EQ(algRing.allocate(ALLOC_SIZE).ptr, nullptr);

	algRing.deallocate(blockLeftMiddle);

	const auto blockRound{ allocateSuccess(ALLOC_SIZE) };

	algRing.deallocate(blockMiddle);
	EXPECT_TRUE(getValuesAre(blockRound.ptr, 0, ALLOC_SIZE, SET_BYTES));
	algRing.deallocate(blockRight);
	EXPECT_TRUE(getValuesAre(blockRound.ptr, 0, ALLOC_SIZE, SET_BYTES));
	algRing.deallocate(blockRound);
	EXPECT_FALSE(getValuesAre(blockRound.ptr, 0, ALLOC_SIZE, SET_BYTES));

}

static void tsOneByOneWithWrappingImpl(auto& algRing)
{

	static constexpr auto ALLOC_SIZE{ 240_B };

	constexpr auto NUM_OF_ALLOCS{ BUFFER_LEN / ALLOC_SIZE };

	std::array<hfog::MemoryBlock, NUM_OF_ALLOCS> blocks;

	const auto allocateSuccess = [&](mem_t allocSize)
	{
		const auto memBlock{ algRing.allocate(allocSize) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0, allocSize, SET_BYTES));
		EXPECT_TRUE(algRing.getIsOwner(memBlock.ptr));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, allocSize);
		return memBlock;
	};

	constexpr auto NUM_OF_ITERS{ 100 };

	for (size_t iterId{ 0 }; iterId < NUM_OF_ITERS; ++iterId)
	{
		const auto blockLeft{ allocateSuccess(ALLOC_SIZE) };
		algRing.deallocate(blockLeft);
		const auto blockWrapped{ allocateSuccess(ALLOC_SIZE) };
		algRing.deallocate(blockWrapped);
		EXPECT_TRUE(getValuesAre(blockWrapped.ptr, 0, ALLOC_SIZE, CLEAR_BYTES));
	}

}

static void tsBatchWrappingImpl(auto& algRing)
{

	static constexpr auto ALLOC_SIZE{ 128_B };
	static constexpr auto ALLOC_SIZE_LARGE{ 144_B };
	static constexpr auto ALLOC_SIZE_SMALL{ 64_B };

	constexpr auto NUM_OF_ALLOCS{ BUFFER_LEN / ALLOC_SIZE };

	std::array<hfog::MemoryBlock, NUM_OF_ALLOCS> blocks;

	const auto allocateSuccess = [&](mem_t allocSize)
	{
		const auto memBlock{ algRing.allocate(allocSize) };
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0, allocSize, SET_BYTES));
		EXPECT_TRUE(algRing.getIsOwner(memBlock.ptr));
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, allocSize);
		return memBlock;
	};

	constexpr auto NUM_OF_ITERS{ 100 };

	for (size_t iterId{ 0 }; iterId < NUM_OF_ITERS; ++iterId)
	{
		const auto blockLeft{ allocateSuccess(ALLOC_SIZE_LARGE) };
		algRing.deallocate(blockLeft);
		const auto blockWrapped{ allocateSuccess(ALLOC_SIZE) };
		const auto blockWrappedAdd{ allocateSuccess(ALLOC_SIZE_SMALL) };
		algRing.deallocate(blockWrapped);
		EXPECT_TRUE(getValuesAre(blockWrapped.ptr, 0, ALLOC_SIZE, CLEAR_BYTES));
		algRing.deallocate(blockWrappedAdd);
		EXPECT_TRUE(getValuesAre(blockWrappedAdd.ptr, 0, ALLOC_SIZE_SMALL, CLEAR_BYTES));
	}

}

static void tsDeallocationImpl(auto& algRing)
{

	constexpr auto NUM_OF_ITERS{ 10 };

	for (size_t iterId{ 0 }; iterId < NUM_OF_ITERS; ++iterId)
	{
		static constexpr auto ALLOC_SIZE{ 64_B };
		constexpr auto NUM_OF_ALLOCS{ BUFFER_LEN / ALLOC_SIZE };

		for (size_t allocId{ 0 }; allocId < NUM_OF_ALLOCS; ++allocId)
		{
			const auto memBlock{ algRing.allocate(ALLOC_SIZE) };
			EXPECT_TRUE(getValuesAre(memBlock.ptr, 0, ALLOC_SIZE, SET_BYTES));
			EXPECT_TRUE(algRing.getIsOwner(memBlock.ptr));
			EXPECT_NE(memBlock.ptr, nullptr);
			EXPECT_EQ(memBlock.size, ALLOC_SIZE);
		}

		algRing.deallocate();
	}
		
}

TEST(AllocAlgorithmsRing, tsSourceExtSimple)
{

	stackExt_t algRing(generateStackExtMemBlock());
	tsSimpleImpl(algRing);

}

TEST(AllocAlgorithmsRing, tsSourceExtDeallocation)
{

	stackExt_t algRing(generateStackExtMemBlock());
	tsDeallocateionImpl(algRing);

}

TEST(AllocAlgorithmsRing, tsEdgeAllocation)
{

	stackExt_t algRing(generateStackExtMemBlock());
	tsEdgeAllocationImpl(algRing);

}

TEST(AllocAlgorithmsRing, tsOneByOneWithWrapping)
{

	stackExt_t algRing(generateStackExtMemBlock());
	tsOneByOneWithWrappingImpl(algRing);

}
TEST(AllocAlgorithmsRing, tsBatchWrapping)
{

	stackExt_t algRing(generateStackExtMemBlock());
	tsBatchWrappingImpl(algRing);

}

TEST(AllocAlgorithmsRing, tsDeallocation)
{

	stackExt_t algRing(generateStackExtMemBlock());
	tsDeallocationImpl(algRing);

}

TEST(AllocAlgorithmsRing, tsSourceExtGetIsOwner)
{

	byte_t val{ 42 };
	hfog::Algorithms::Ring<sourceExternal_t, ALIGNMENT> algRing(generateStackExtMemBlock());

	EXPECT_FALSE(algRing.getIsOwner(&val));
	EXPECT_FALSE(algRing.getIsOwner(nullptr));

}

using sourceLocal_t = hfog::Sources::Local<BUFFER_LEN, hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>>;
using stackLocal_t = hfog::Algorithms::Ring<sourceLocal_t, ALIGNMENT>;

TEST(AllocAlgorithmsRing, tsSourceLocalSimple)
{

	stackLocal_t algRing;
	tsSimpleImpl(algRing);

}

TEST(AllocAlgorithmsRing, tsSourceLocalDeallocation)
{

	stackLocal_t algRing;
	tsDeallocateionImpl(algRing);

}

TEST(AllocAlgorithms, tsSourcesEdgeAllocation)
{

	stackLocal_t algRing;
	tsEdgeAllocationImpl(algRing);

}