#include "pch.h"

#include <type_traits>

import MemoryBlock;

using namespace hfog::MemoryUtils::Literals;

TEST(MemoryBlock, tsMemBlockStruct)
{

	static_assert(std::is_pointer_v<decltype(hfog::MemoryBlock::ptr)>);
	static_assert(std::is_integral_v<decltype(hfog::MemoryBlock::size)>);

	hfog::MemoryBlock memBlock;
	EXPECT_EQ(memBlock.ptr, nullptr);
	EXPECT_EQ(memBlock.size, 0_B);

}

TEST(MemoryBlock, tsMemBlockCreation)
{

	byte_t data[16_B];

	hfog::MemoryBlock memBlock(data, sizeof(data));
	EXPECT_EQ(memBlock.ptr, data);
	EXPECT_EQ(memBlock.size, sizeof(data));

}