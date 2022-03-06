#include "pch.h"

import AlgNull;
import MemoryUtils;

import hfogCore;

using namespace hfog::MemoryUtils::Literals;

static_assert(hfog::CtAllocator<hfog::Algorithms::Null>);

TEST(AllocAlgorithms, tsNull)
{

	hfog::Algorithms::Null algNull;

	{
		const auto memBlock{ algNull.allocate(1_B) };
		EXPECT_EQ(memBlock.ptr, nullptr);
		algNull.deallocate(memBlock);
	}

	EXPECT_FALSE(algNull.getIsOwner(nullptr));

	byte_t val{ 42 };
	EXPECT_FALSE(algNull.getIsOwner(&val));

}