#include "pch.h"

import MemoryUtils;

using namespace hfog::MemoryUtils::Literals;

TEST(MemoryUtils, tsLiterals)
{
	EXPECT_EQ(1_B, mem_t(1));
	EXPECT_EQ(1_kB, mem_t(1024));
	EXPECT_EQ(1_MB, mem_t(1048576));
	EXPECT_EQ(1_GB, mem_t(1073741824));
	EXPECT_EQ(1_TB, mem_t(1099511627776));

	EXPECT_EQ(10_B, mem_t(10));
	EXPECT_EQ(10_kB, mem_t(10240));
	EXPECT_EQ(10_MB, mem_t(10485760));
	EXPECT_EQ(10_GB, mem_t(10737418240));
	EXPECT_EQ(10_TB, mem_t(10995116277760));
}

TEST(MemoryUtils, tsAlighnment)
{

	{
		const auto input{ 1_B };
		EXPECT_EQ(hfog::MemoryUtils::align<16_B>()(input), mem_t(16_B));
	}

	{
		const auto input{ 8_B };
		EXPECT_EQ(hfog::MemoryUtils::align<16_B>()(input), mem_t(16_B));
	}

	{
		const auto input{ 16_B };
		EXPECT_EQ(hfog::MemoryUtils::align<16_B>()(input), mem_t(16_B));
	}

	{
		const auto input{ 18_B };
		EXPECT_EQ(hfog::MemoryUtils::align<16_B>()(input), mem_t(32_B));
	}

	{
		const auto input{ 40_B };
		EXPECT_EQ(hfog::MemoryUtils::align<16_B>()(input), mem_t(48_B));
	}

	static_assert(hfog::MemoryUtils::align<16_B>()(40_B) == 48_B);

}