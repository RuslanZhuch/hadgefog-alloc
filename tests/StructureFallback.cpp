#include "pch.h"

import AlgStack;
import AlgLinear;
import AlgNull;

import SourceStack;
import SourceExt;

import GarbageWriter;

using namespace hfog::MemoryUtils::Literals;

//TEST(Structures, tsFallback)
//{
//
//	using garbageWriter_t = hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>;
//
//	using primaryAlg_t = hfog::Algorithms::Stack<hfog::Sources::Stack<32_B, garbageWriter_t>, 16_B>;
//	using secondaryAlg_t = hfog::Algorithms::Linear<hfog::Sources::Stack<32_B, garbageWriter_t>, 16_B>;
//
//	hfog::Structures::Fallback<primaryAlg_t, secondaryAlg_t> fallback;
//
//	{
//
//		const auto memBlock{ fallback.allocate(16_B) };
//		EXPECT_NE(memBlock.ptr, nullptr);
//		EXPECT_EQ(memBlock.size, 16_B);
//		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0, memBlock.size(), 0xFA));
//		fallback.deallocate(memBlock);
//		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0, memBlock.size(), 0xAF));
//
//	}
//
//	{
//
//		const auto memBlock{ fallback.allocate(16_B) };
//		EXPECT_NE(memBlock.ptr, nullptr);
//		EXPECT_EQ(memBlock.size, 16_B);
//		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0, memBlock.size(), 0xFA));
//
//		const auto memBlock2{ fallback.allocate(16_B) };
//		EXPECT_NE(memBlock2.ptr, nullptr);
//		EXPECT_EQ(memBlock2.size, 16_B);
//		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0, memBlock2.size(), 0xFA));
//
//		EXPECT_EQ(memBlock.ptr + memBlock.size, memBlock2.ptr);
//
//		fallback.deallocate(memBlock2);
//		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0, memBlock2.size(), 0xAF));
//		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0, memBlock.size(), 0xFA));
//
//		fallback.deallocate(memBlock);
//		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0, memBlock.size(), 0xAF));
//
//	}
//
//	{
//
//		const auto memBlock{ fallback.allocate(16_B) };
//		EXPECT_NE(memBlock.ptr, nullptr);
//		EXPECT_EQ(memBlock.size, 16_B);
//		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0, memBlock.size(), 0xFA));
//
//		const auto memBlock2{ fallback.allocate(32_B) };
//		EXPECT_NE(memBlock2.ptr, nullptr);
//		EXPECT_EQ(memBlock2.size, 32_B);
//		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0, memBlock2.size(), 0xFA));
//		EXPECT_FALSE(getValuesAre(memBlock.ptr, memBlock.size(), memBlock.size(), 0xFA));
//
//		fallback.deallocate(memBlock);
//		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0, memBlock.size(), 0xAF));
//		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0, memBlock2.size(), 0xFA));
//
//		fallback.deallocate(memBlock2);
//		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0, memBlock2.size(), 0xAF));
//
//	}
//
//	{
//
//		const auto memBlock{ fallback.allocate(16_B) };
//		EXPECT_NE(memBlock.ptr, nullptr);
//		EXPECT_EQ(memBlock.size, 16_B);
//		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0, memBlock.size(), 0xFA));
//
//		const auto memBlock2{ fallback.allocate(32_B) };
//		EXPECT_NE(memBlock2.ptr, nullptr);
//		EXPECT_EQ(memBlock2.size, 32_B);
//		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0, memBlock2.size(), 0xFA));
//		EXPECT_FALSE(getValuesAre(memBlock.ptr, memBlock.size(), memBlock.size(), 0xFA));
//
//		fallback.deallocate(memBlock);
//		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0, memBlock.size(), 0xAF));
//		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0, memBlock2.size(), 0xFA));
//
//		const auto memBlock3{ fallback.allocate(32_B) };
//		EXPECT_NE(memBlock3.ptr, nullptr);
//		EXPECT_EQ(memBlock3.size, 32_B);
//		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0, memBlock3.size(), 0xFA));
//		EXPECT_EQ(memBlock3.ptr, memBlock.ptr);
//
//		fallback.deallocate(memBlock2);
//		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0, memBlock2.size(), 0xAF));
//		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0, memBlock3.size(), 0xFA));
//
//		fallback.deallocate(memBlock3);
//		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0, memBlock3.size(), 0xAF));
//
//	}
//
//	{
//
//		const auto memBlock{ fallback.allocate(32_B) };
//		EXPECT_NE(memBlock.ptr, nullptr);
//		EXPECT_EQ(memBlock.size, 32_B);
//		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0, memBlock.size(), 0xFA));
//
//		const auto memBlock2{ fallback.allocate(32_B) };
//		EXPECT_NE(memBlock2.ptr, nullptr);
//		EXPECT_EQ(memBlock2.size, 32_B);
//		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0, memBlock2.size(), 0xFA));
//
//		const auto memBlock3{ fallback.allocate(32_B) };
//		EXPECT_EQ(memBlock3.ptr, nullptr);
//
//		fallback.deallocate(memBlock2);
//		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0, memBlock2.size(), 0xAF));
//		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0, memBlock.size(), 0xFA));
//
//		fallback.deallocate(memBlock);
//		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0, memBlock2.size(), 0xAF));
//		EXPECT_TRUE(getValuesAre(memBlock.ptr, 0, memBlock.size(), 0xAF));
//
//	}
//
//}