#include "pch.h"

import HeapManagers;

TEST(HeapManagers, HeapManager) 
{

	hfog::Managers::HeapManager heapManager;
	const auto dataPtr{ heapManager.allocate(100) };
	EXPECT_NE(dataPtr.ptr, nullptr);
	EXPECT_EQ(dataPtr.size, size_t(100));
	heapManager.deallocate(dataPtr);

}