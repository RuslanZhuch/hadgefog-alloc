#include "pch.h"

import hfog.Heap;

TEST(Supports, tsMinHeap)
{

	hfog::Supports::Heap<int, 6> minHeap;

	EXPECT_TRUE(minHeap.push(4));
	EXPECT_EQ(minHeap.pick().value(), 4);
	EXPECT_EQ(minHeap.pick().value(), 4);
	minHeap.pop();
	EXPECT_FALSE(minHeap.pick().has_value());
	minHeap.pop();

	EXPECT_TRUE(minHeap.push(4));
	EXPECT_EQ(minHeap.pick().value(), 4);

	EXPECT_TRUE(minHeap.push(8));
	EXPECT_EQ(minHeap.pick().value(), 4);

	minHeap.pop();
	EXPECT_EQ(minHeap.pick().value(), 8);

	minHeap.pop();
	EXPECT_FALSE(minHeap.pick().has_value());
	minHeap.pop();


	EXPECT_TRUE(minHeap.push(4));
	EXPECT_TRUE(minHeap.push(8));
	EXPECT_TRUE(minHeap.push(2));
	EXPECT_EQ(minHeap.pick().value(), 2);
	minHeap.pop();
	minHeap.pop();
	minHeap.pop();

}

TEST(Supports, tsMinHeapLarge)
{

	hfog::Supports::Heap<int, 6> minHeap;

	EXPECT_TRUE(minHeap.push(4));
	EXPECT_EQ(minHeap.pick().value(), 4);

	EXPECT_TRUE(minHeap.push(7));
	EXPECT_EQ(minHeap.pick().value(), 4);

	EXPECT_TRUE(minHeap.push(6));
	EXPECT_EQ(minHeap.pick().value(), 4);

	EXPECT_TRUE(minHeap.push(2));
	EXPECT_EQ(minHeap.pick().value(), 2);

	EXPECT_TRUE(minHeap.push(8));
	EXPECT_EQ(minHeap.pick().value(), 2);

	EXPECT_TRUE(minHeap.push(0));
	EXPECT_EQ(minHeap.pick().value(), 0);

	minHeap.pop();
	EXPECT_EQ(minHeap.pick().value(), 2);

	minHeap.pop();
	EXPECT_EQ(minHeap.pick().value(), 4);

	minHeap.pop();
	EXPECT_EQ(minHeap.pick().value(), 6);

	minHeap.pop();
	EXPECT_EQ(minHeap.pick().value(), 7);

	minHeap.pop();
	EXPECT_EQ(minHeap.pick().value(), 8);

	minHeap.pop();
	EXPECT_FALSE(minHeap.pick().has_value());
}

TEST(Supports, tsHeapOverflow)
{

	hfog::Supports::Heap<int, 6> minHeap;

	EXPECT_TRUE(minHeap.push(4));
	EXPECT_TRUE(minHeap.push(7));
	EXPECT_TRUE(minHeap.push(6));
	EXPECT_TRUE(minHeap.push(2));
	EXPECT_TRUE(minHeap.push(8));
	EXPECT_TRUE(minHeap.push(0));
	EXPECT_FALSE(minHeap.push(5));

	EXPECT_EQ(minHeap.pick().value(), 0);

	minHeap.pop();
	EXPECT_EQ(minHeap.pick().value(), 2);

	EXPECT_TRUE(minHeap.push(5));

	minHeap.pop();
	EXPECT_EQ(minHeap.pick().value(), 4);

	minHeap.pop();
	EXPECT_EQ(minHeap.pick().value(), 5);

	minHeap.pop();
	EXPECT_EQ(minHeap.pick().value(), 6);

	minHeap.pop();
	EXPECT_EQ(minHeap.pick().value(), 7);

	minHeap.pop();
	EXPECT_EQ(minHeap.pick().value(), 8);

	minHeap.pop();
	EXPECT_FALSE(minHeap.pick().has_value());

}