#include "pch.h"

import hfog.Algorithms.Islands;

import hfog.Sources.Stack;
import hfog.Sources.Ext;

import hfog.GarbageWriter;
import hfog.Structures.Segregator;

import hfog.Core;

using namespace hfog::MemoryUtils::Literals;


static constexpr auto gLeftInitBytes{ 0xFA };
static constexpr auto gLeftReleaseBytes{ 0xAF };

static constexpr auto gRightInitBytes{ 0xEB };
static constexpr auto gRightReleaseBytes{ 0xBE };

using garbageWriterLeft_t = hfog::GarbageWriter::ByteWriter<gLeftInitBytes, gLeftReleaseBytes>;
using garbageWriterRight_t = hfog::GarbageWriter::ByteWriter<gRightInitBytes, gRightReleaseBytes>;

using leftAlg_t = hfog::Algorithms::Islands<hfog::Sources::Stack<128_B, garbageWriterLeft_t>, 16_B, 4, 2>;
using rightAlg_t = hfog::Algorithms::Islands<hfog::Sources::Stack<640_B, garbageWriterRight_t>, 80_B, 4, 2>;
static_assert(hfog::CtAllocator<hfog::Structures::Segregator<leftAlg_t, rightAlg_t, 64_B>>);

TEST(Structures, tsSegregator)
{

	hfog::Structures::Segregator<leftAlg_t, rightAlg_t, 64_B> segregator;

	{

		const auto memBlock1{ segregator.allocate(16_B) };
		EXPECT_NE(memBlock1.ptr, nullptr);
		EXPECT_EQ(memBlock1.size, 16_B);
		EXPECT_TRUE(getValuesAre(memBlock1.ptr, 0_B, memBlock1.size, gLeftInitBytes));

		const auto memBlock2{ segregator.allocate(16_B) };
		EXPECT_NE(memBlock2.ptr, nullptr);
		EXPECT_EQ(memBlock2.size, 16_B);
		EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, memBlock2.size, gLeftInitBytes));

		const auto memBlock3{ segregator.allocate(64_B) };
		EXPECT_NE(memBlock3.ptr, nullptr);
		EXPECT_EQ(memBlock3.size, 64_B);
		EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, memBlock3.size, gLeftInitBytes));

		{
			const auto memBlock{ segregator.allocate(64_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}

		const auto memBlock4{ segregator.allocate(65_B) };
		EXPECT_NE(memBlock4.ptr, nullptr);
		EXPECT_EQ(memBlock4.size, 80_B);
		EXPECT_TRUE(getValuesAre(memBlock4.ptr, 0_B, memBlock4.size, gRightInitBytes));

		const auto memBlock5{ segregator.allocate(320_B) };
		EXPECT_NE(memBlock5.ptr, nullptr);
		EXPECT_EQ(memBlock5.size, 320_B);
		EXPECT_TRUE(getValuesAre(memBlock5.ptr, 0_B, memBlock5.size, gRightInitBytes));

		{
			const auto memBlock{ segregator.allocate(320_B) };
			EXPECT_EQ(memBlock.ptr, nullptr);
		}

		const auto memBlock6{ segregator.allocate(32_B) };
		EXPECT_NE(memBlock6.ptr, nullptr);
		EXPECT_EQ(memBlock6.size, 32_B);
		EXPECT_TRUE(getValuesAre(memBlock6.ptr, 0_B, memBlock6.size, gLeftInitBytes));

		segregator.deallocate(memBlock3);

		const auto memBlock7{ segregator.allocate(64_B) };
		EXPECT_NE(memBlock7.ptr, nullptr);
		EXPECT_EQ(memBlock7.size, 64_B);
		EXPECT_TRUE(getValuesAre(memBlock7.ptr, 0_B, memBlock7.size, gLeftInitBytes));

		segregator.deallocate(memBlock5);

		const auto memBlock8{ segregator.allocate(320_B) };
		EXPECT_NE(memBlock8.ptr, nullptr);
		EXPECT_EQ(memBlock8.size, 320_B);
		EXPECT_TRUE(getValuesAre(memBlock8.ptr, 0_B, memBlock8.size, gRightInitBytes));

	}

}