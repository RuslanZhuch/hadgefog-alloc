#include "pch.h"

import hfog.Algorithms.Stack;
import hfog.Algorithms.Null;
import hfog.Algorithms.Islands;

import hfog.Sources.Local;
import hfog.Sources.Ext;

import hfog.GarbageWriter;
import hfog.Structures.Fallback;

import hfog.Core;

using namespace hfog::MemoryUtils::Literals;

using garbageWriter_t = hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>;

using primaryAlg_t = hfog::Algorithms::Stack<hfog::Sources::Local<32_B, garbageWriter_t>, 16_B>;
using secondaryAlg_t = hfog::Algorithms::Islands<hfog::Sources::Local<32_B, garbageWriter_t>, 16_B, 2, 1>;
static_assert(hfog::CtAllocator<hfog::Structures::Fallback<primaryAlg_t, secondaryAlg_t>>);

TEST(StructuresFallBack, tsSimpleAlloc)
{

	static constexpr auto ALLOC_SIZE{ 16_B };

	hfog::Structures::Fallback<primaryAlg_t, secondaryAlg_t> fallback;

	const auto memBlock{ fallback.allocate(ALLOC_SIZE) };
	EXPECT_NE(memBlock.ptr, nullptr);
	EXPECT_EQ(memBlock.size, ALLOC_SIZE);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, SET_BYTES));
	fallback.deallocate(memBlock);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, CLEAR_BYTES));

}

TEST(StructuresFallBack, tsStackAlloc)
{

	static constexpr auto ALLOC_SIZE{ 16_B };

	hfog::Structures::Fallback<primaryAlg_t, secondaryAlg_t> fallback;

	const auto memBlock{ fallback.allocate(ALLOC_SIZE) };
	EXPECT_NE(memBlock.ptr, nullptr);
	EXPECT_EQ(memBlock.size, ALLOC_SIZE);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, SET_BYTES));

	const auto memBlock2{ fallback.allocate(ALLOC_SIZE) };
	EXPECT_NE(memBlock2.ptr, nullptr);
	EXPECT_EQ(memBlock2.size, ALLOC_SIZE);
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, SET_BYTES));

	EXPECT_EQ(memBlock.ptr + memBlock.size, memBlock2.ptr);

	fallback.deallocate(memBlock2);
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, CLEAR_BYTES));
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, SET_BYTES));

	fallback.deallocate(memBlock);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, CLEAR_BYTES));

}

TEST(StructuresFallBack, tsIslandsFallback)
{

	static constexpr auto ALLOC_SIZE_SMALL{ 16_B };
	static constexpr auto ALLOC_SIZE_LARGE{ 32_B };

	hfog::Structures::Fallback<primaryAlg_t, secondaryAlg_t> fallback;

	const auto memBlock{ fallback.allocate(ALLOC_SIZE_SMALL) };
	EXPECT_NE(memBlock.ptr, nullptr);
	EXPECT_EQ(memBlock.size, ALLOC_SIZE_SMALL);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, SET_BYTES));

	const auto memBlock2{ fallback.allocate(ALLOC_SIZE_LARGE) };
	EXPECT_NE(memBlock2.ptr, nullptr);
	EXPECT_EQ(memBlock2.size, ALLOC_SIZE_LARGE);
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, SET_BYTES));
	EXPECT_FALSE(getValuesAre(memBlock.ptr, memBlock.size, memBlock.size * 2, SET_BYTES));

	fallback.deallocate(memBlock);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, CLEAR_BYTES));
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, SET_BYTES));

	fallback.deallocate(memBlock2);
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, CLEAR_BYTES));

}

TEST(StructuresFallBack, tsReallocateFromPrimary)
{

	static constexpr auto ALLOC_SIZE_SMALL{ 16_B };
	static constexpr auto ALLOC_SIZE_LARGE{ 32_B };

	hfog::Structures::Fallback<primaryAlg_t, secondaryAlg_t> fallback;

	const auto memBlock{ fallback.allocate(ALLOC_SIZE_SMALL) };
	EXPECT_NE(memBlock.ptr, nullptr);
	EXPECT_EQ(memBlock.size, ALLOC_SIZE_SMALL);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, SET_BYTES));

	const auto memBlock2{ fallback.allocate(ALLOC_SIZE_LARGE) };
	EXPECT_NE(memBlock2.ptr, nullptr);
	EXPECT_EQ(memBlock2.size, ALLOC_SIZE_LARGE);
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, SET_BYTES));
	EXPECT_FALSE(getValuesAre(memBlock.ptr, memBlock.size, memBlock.size * 2, SET_BYTES));

	fallback.deallocate(memBlock);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, CLEAR_BYTES));
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, SET_BYTES));

	const auto memBlock3{ fallback.allocate(ALLOC_SIZE_LARGE) };
	EXPECT_NE(memBlock3.ptr, nullptr);
	EXPECT_EQ(memBlock3.size, ALLOC_SIZE_LARGE);
	EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, memBlock3.size, SET_BYTES));
	EXPECT_EQ(memBlock3.ptr, memBlock.ptr);

	fallback.deallocate(memBlock2);
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, CLEAR_BYTES));
	EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, memBlock3.size, SET_BYTES));

	fallback.deallocate(memBlock3);
	EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, memBlock3.size, CLEAR_BYTES));

}

TEST(StructuresFallBack, tsOutOfMemory)
{

	static constexpr auto ALLOC_SIZE{ 32_B };

	hfog::Structures::Fallback<primaryAlg_t, secondaryAlg_t> fallback;

	const auto memBlock{ fallback.allocate(ALLOC_SIZE) };
	EXPECT_NE(memBlock.ptr, nullptr);
	EXPECT_EQ(memBlock.size, ALLOC_SIZE);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, SET_BYTES));

	const auto memBlock2{ fallback.allocate(ALLOC_SIZE) };
	EXPECT_NE(memBlock2.ptr, nullptr);
	EXPECT_EQ(memBlock2.size, ALLOC_SIZE);
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, SET_BYTES));

	const auto memBlock3{ fallback.allocate(ALLOC_SIZE) };
	EXPECT_EQ(memBlock3.ptr, nullptr);

	fallback.deallocate(memBlock2);
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, CLEAR_BYTES));
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, SET_BYTES));

	fallback.deallocate(memBlock);
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, CLEAR_BYTES));
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, CLEAR_BYTES));

}

TEST(StructuresFallBack, tsIsOwner)
{

	static constexpr auto ALLOC_SIZE_SMALL{ 16_B };
	static constexpr auto ALLOC_SIZE_LARGE{ 32_B };

	byte_t val{ 42 };

	hfog::Structures::Fallback<primaryAlg_t, secondaryAlg_t> fallback;

	EXPECT_FALSE(fallback.getIsOwner(&val));
	EXPECT_FALSE(fallback.getIsOwner(nullptr));

	{

		const auto memBlock{ fallback.allocate(ALLOC_SIZE_SMALL) };
		EXPECT_TRUE(fallback.getIsOwner(memBlock.ptr));

		fallback.deallocate(memBlock);
		EXPECT_FALSE(fallback.getIsOwner(memBlock.ptr));

	}

	{

		const auto memBlock{ fallback.allocate(ALLOC_SIZE_LARGE) };
		EXPECT_TRUE(fallback.getIsOwner(memBlock.ptr));

		const auto memBlock2{ fallback.allocate(ALLOC_SIZE_LARGE) };
		EXPECT_TRUE(fallback.getIsOwner(memBlock2.ptr));

		const auto memBlock3{ fallback.allocate(ALLOC_SIZE_LARGE) };
		EXPECT_FALSE(fallback.getIsOwner(memBlock3.ptr));

		fallback.deallocate(memBlock2);
		EXPECT_TRUE(fallback.getIsOwner(memBlock.ptr));
		EXPECT_FALSE(fallback.getIsOwner(memBlock2.ptr));

		fallback.deallocate(memBlock);
		EXPECT_FALSE(fallback.getIsOwner(memBlock.ptr));
		EXPECT_FALSE(fallback.getIsOwner(memBlock2.ptr));

	}

}