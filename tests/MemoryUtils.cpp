#include "pch.h"

import hfog.MemoryUtils;

using namespace hfog::MemoryUtils::Literals;

TEST(MemoryUtils, tsLiterals)
{
	EXPECT_EQ(1_B, static_cast<mem_t>(1));
	EXPECT_EQ(1_kB, static_cast<mem_t>(1024));
	EXPECT_EQ(1_MB, static_cast<mem_t>(1048576));
	EXPECT_EQ(1_GB, static_cast<mem_t>(1073741824));
	EXPECT_EQ(1_TB, static_cast<mem_t>(1099511627776));

	EXPECT_EQ(10_B, static_cast<mem_t>(10));
	EXPECT_EQ(10_kB, static_cast<mem_t>(10240));
	EXPECT_EQ(10_MB, static_cast<mem_t>(10485760));
	EXPECT_EQ(10_GB, static_cast<mem_t>(10737418240));
	EXPECT_EQ(10_TB, static_cast<mem_t>(10995116277760));
}

TEST(MemoryUtils, tsAlighnment)
{

	{
		const auto input{ 1_B };
		EXPECT_EQ(hfog::MemoryUtils::Align<16_B>::compute(input), 16_B);
	}

	{
		const auto input{ 8_B };
		EXPECT_EQ(hfog::MemoryUtils::Align<16_B>::compute(input), 16_B);
	}

	{
		const auto input{ 16_B };
		EXPECT_EQ(hfog::MemoryUtils::Align<16_B>::compute(input), 16_B);
	}

	{
		const auto input{ 18_B };
		EXPECT_EQ(hfog::MemoryUtils::Align<16_B>::compute(input), 32_B);
	}

	{
		const auto input{ 40_B };
		EXPECT_EQ(hfog::MemoryUtils::Align<16_B>::compute(input), 48_B);
	}

	static_assert(hfog::MemoryUtils::Align<16_B>::compute(40_B) == 48_B);

	static_assert(hfog::MemoryUtils::CtAlign<hfog::MemoryUtils::Align<16_B>>);

}