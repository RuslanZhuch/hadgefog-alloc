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