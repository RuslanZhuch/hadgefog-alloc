#include "pch.h"

import AlgStack;
import AlgNull;
import AlgUnified;

import SourceStack;
import SourceExt;

import GarbageWriter;
import StructureFallback;

import hfogCore;

using namespace hfog::MemoryUtils::Literals;

using garbageWriter_t = hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>;

using primaryAlg_t = hfog::Algorithms::Stack<hfog::Sources::Stack<32_B, garbageWriter_t>, 16_B>;
using secondaryAlg_t = hfog::Algorithms::Unified<hfog::Sources::Stack<32_B, garbageWriter_t>, 16_B, 2, 1>;
static_assert(hfog::CtAllocator<hfog::Structures::Fallback<primaryAlg_t, secondaryAlg_t>>);

TEST(Structures, tsFallback)
{

	hfog::Structures::Fallback<primaryAlg_t, secondaryAlg_t> fallback;

	{

		const auto memBlock{ fallback.allocate(16_B) };
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 16_B);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, 0xFA));
		fallback.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, 0xAF));

	}

	{

		const auto memBlock{ fallback.allocate(16_B) };
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 16_B);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, 0xFA));

		const auto memBlock2{ fallback.allocate(16_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 16_B);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, 0xFA));

		EXPECT_EQ(memBlock.ptr + memBlock.size, memBlock2.ptr);

		fallback.deallocate(memBlock2);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, 0xAF));
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, 0xFA));

		fallback.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, 0xAF));

	}

	{

		const auto memBlock{ fallback.allocate(16_B) };
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 16_B);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, 0xFA));

		const auto memBlock2{ fallback.allocate(32_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 32_B);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, 0xFA));
		EXPECT_FALSE(getValuesAre(memBlock.ptr, memBlock.size, memBlock.size * 2, 0xFA));

		fallback.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, 0xAF));
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, 0xFA));

		fallback.deallocate(memBlock2);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, 0xAF));

	}

	{

		const auto memBlock{ fallback.allocate(16_B) };
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 16_B);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, 0xFA));

		const auto memBlock2{ fallback.allocate(32_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 32_B);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, 0xFA));
		EXPECT_FALSE(getValuesAre(memBlock.ptr, memBlock.size, memBlock.size * 2, 0xFA));

		fallback.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, 0xAF));
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, 0xFA));

		const auto memBlock3{ fallback.allocate(32_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 32_B);
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, memBlock3.size, 0xFA));
		EXPECT_EQ(memBlock3.ptr, memBlock.ptr);

		fallback.deallocate(memBlock2);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, 0xAF));
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, memBlock3.size, 0xFA));

		fallback.deallocate(memBlock3);
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, memBlock3.size, 0xAF));

	}

	{

		const auto memBlock{ fallback.allocate(32_B) };
		EXPECT_NE(memBlock.ptr, nullptr);
		EXPECT_EQ(memBlock.size, 32_B);
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, 0xFA));

		const auto memBlock2{ fallback.allocate(32_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 32_B);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, 0xFA));

		const auto memBlock3{ fallback.allocate(32_B) };
		EXPECT_EQ(memBlock3.ptr, nullptr);

		fallback.deallocate(memBlock2);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, 0xAF));
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, 0xFA));

		fallback.deallocate(memBlock);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, 0xAF));
		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, memBlock.size, 0xAF));

	}

}

TEST(Structures, tsFallbackOwn)
{

	byte_t val{ 42 };

	hfog::Structures::Fallback<primaryAlg_t, secondaryAlg_t> fallback;

	EXPECT_FALSE(fallback.getIsOwner(&val));
	EXPECT_FALSE(fallback.getIsOwner(nullptr));

	{

		const auto memBlock{ fallback.allocate(16_B) };
		EXPECT_TRUE(fallback.getIsOwner(memBlock.ptr));

		fallback.deallocate(memBlock);
		EXPECT_FALSE(fallback.getIsOwner(memBlock.ptr));

	}

	{

		const auto memBlock{ fallback.allocate(32_B) };
		EXPECT_TRUE(fallback.getIsOwner(memBlock.ptr));

		const auto memBlock2{ fallback.allocate(32_B) };
		EXPECT_TRUE(fallback.getIsOwner(memBlock2.ptr));

		const auto memBlock3{ fallback.allocate(32_B) };
		EXPECT_FALSE(fallback.getIsOwner(memBlock3.ptr));

		fallback.deallocate(memBlock2);
		EXPECT_TRUE(fallback.getIsOwner(memBlock.ptr));
		EXPECT_FALSE(fallback.getIsOwner(memBlock2.ptr));

		fallback.deallocate(memBlock);
		EXPECT_FALSE(fallback.getIsOwner(memBlock.ptr));
		EXPECT_FALSE(fallback.getIsOwner(memBlock2.ptr));

	}

}