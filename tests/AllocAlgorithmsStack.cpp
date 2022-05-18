#include "pch.h"

import hfog.Sources.Ext;
import hfog.Sources.Local;
import hfog.MemoryUtils;

import hfog.Core;

import hfog.Algorithms.Stack;

using namespace hfog::MemoryUtils::Literals;

static_assert(hfog::CtAllocator<hfog::Algorithms::Stack<hfog::Sources::External<hfog::GarbageWriter::Default>, 16_B>>);
static_assert(hfog::CtAllocator<hfog::Algorithms::Stack<hfog::Sources::Local<128_B, hfog::GarbageWriter::Default>, 16_B>>);

static constexpr auto ALIGNMENT{ 16_B };

using sourceExternal_t = hfog::Sources::External<hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>>;
using stackExt_t = hfog::Algorithms::Stack<sourceExternal_t, ALIGNMENT>;
static constexpr auto BUFFER_LEN{ 128_B };

static const auto& generateStackExtMemBlock()
{
	static byte_t extMemBuffer[BUFFER_LEN];
	std::memset(extMemBuffer, 0, sizeof(extMemBuffer));

	static hfog::MemoryBlock extMemBlock;
	extMemBlock.ptr = extMemBuffer;
	extMemBlock.size = BUFFER_LEN;

	return extMemBlock;

}

static void tsSimpleImpl(auto& algStack)
{

	static constexpr auto ALLOC_SIZE{ 32_B };

	const auto memBlock{ algStack.allocate(ALLOC_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, ALLOC_SIZE, SET_BYTES));
	EXPECT_TRUE(algStack.getIsOwner(memBlock.ptr));
	EXPECT_NE(memBlock.ptr, nullptr);
	EXPECT_EQ(memBlock.size, ALLOC_SIZE);
	algStack.deallocate(memBlock);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, ALLOC_SIZE, CLEAR_BYTES));
	EXPECT_FALSE(algStack.getIsOwner(memBlock.ptr));
}

static void tsDeallocateionImpl(auto& algStack)
{
	
	static constexpr auto ALLOC_SIZE{ 32_B };

	const auto memBlock{ algStack.allocate(ALLOC_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, ALLOC_SIZE, SET_BYTES));
	EXPECT_TRUE(algStack.getIsOwner(memBlock.ptr));
	EXPECT_NE(memBlock.ptr, nullptr);
	EXPECT_EQ(memBlock.size, ALLOC_SIZE);

	const auto memBlock2{ algStack.allocate(ALLOC_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, ALLOC_SIZE, SET_BYTES));
	EXPECT_TRUE(algStack.getIsOwner(memBlock2.ptr));
	EXPECT_NE(memBlock2.ptr, nullptr);
	EXPECT_EQ(memBlock2.size, ALLOC_SIZE);

	algStack.deallocate(memBlock);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, ALLOC_SIZE, SET_BYTES));
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, ALLOC_SIZE, SET_BYTES));
	EXPECT_TRUE(algStack.getIsOwner(memBlock.ptr));
	EXPECT_TRUE(algStack.getIsOwner(memBlock2.ptr));

	algStack.deallocate(memBlock2);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, ALLOC_SIZE, SET_BYTES));
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, ALLOC_SIZE, CLEAR_BYTES));
	EXPECT_TRUE(algStack.getIsOwner(memBlock.ptr));
	EXPECT_FALSE(algStack.getIsOwner(memBlock2.ptr));

	algStack.deallocate(memBlock);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, ALLOC_SIZE, CLEAR_BYTES));
	EXPECT_FALSE(algStack.getIsOwner(memBlock.ptr));

	const auto memBlock3{ algStack.allocate(ALLOC_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, ALLOC_SIZE, SET_BYTES));
	EXPECT_TRUE(algStack.getIsOwner(memBlock3.ptr));
	EXPECT_NE(memBlock3.ptr, nullptr);
	EXPECT_EQ(memBlock3.size, ALLOC_SIZE);

	algStack.deallocate(memBlock3);
	EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, ALLOC_SIZE, CLEAR_BYTES));
	EXPECT_FALSE(algStack.getIsOwner(memBlock3.ptr));
}

static void tsOutOfMemoryImpl(auto& algStack)
{


	static constexpr auto ALLOC_SIZE_LARGE{ 96_B };
	static constexpr auto ALLOC_SIZE_SMALL { 36_B };

	const auto memBlock{ algStack.allocate(ALLOC_SIZE_LARGE) };
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, ALLOC_SIZE_LARGE, SET_BYTES));
	EXPECT_NE(memBlock.ptr, nullptr);
	EXPECT_EQ(memBlock.size, ALLOC_SIZE_LARGE);

	const auto memBlock2{ algStack.allocate(BUFFER_LEN) };
	EXPECT_EQ(memBlock2.ptr, nullptr);
	EXPECT_EQ(memBlock2.size, 0_B);

	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, ALLOC_SIZE_SMALL, SET_BYTES));

	algStack.deallocate(memBlock);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, ALLOC_SIZE_SMALL, CLEAR_BYTES));

	const auto memBlock3{ algStack.allocate(BUFFER_LEN) };
	EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, BUFFER_LEN, SET_BYTES));
	EXPECT_NE(memBlock3.ptr, nullptr);
	EXPECT_EQ(memBlock3.size, BUFFER_LEN);

	algStack.deallocate(memBlock3);
	EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, BUFFER_LEN, CLEAR_BYTES));

}

TEST(AllocAlgorithmsStack, tsSourceExtSimple)
{

	stackExt_t algStack(generateStackExtMemBlock());
	tsSimpleImpl(algStack);
	
}

TEST(AllocAlgorithmsStack, tsSourceExtDeallocation)
{

	stackExt_t algStack(generateStackExtMemBlock());
	tsDeallocateionImpl(algStack);

}

TEST(AllocAlgorithmsStack, tsSourcesExtOutOfMemory)
{

	stackExt_t algStack(generateStackExtMemBlock());
	tsOutOfMemoryImpl(algStack);

}

TEST(AllocAlgorithmsStack, tsSourceExtGetIsOwner)
{

	byte_t val{ 42 };
	hfog::Algorithms::Stack<sourceExternal_t, ALIGNMENT> algStack(generateStackExtMemBlock());

	EXPECT_FALSE(algStack.getIsOwner(&val));
	EXPECT_FALSE(algStack.getIsOwner(nullptr));

}

using sourceLocal_t = hfog::Sources::Local<BUFFER_LEN, hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>>;
using stackLocal_t = hfog::Algorithms::Stack<sourceLocal_t, ALIGNMENT>;

TEST(AllocAlgorithmsStack, tsSourceLocalSimple)
{

	stackLocal_t algStack;
	tsSimpleImpl(algStack);

}

TEST(AllocAlgorithmsStack, tsSourceLocalDeallocation)
{

	stackLocal_t algStack;
	tsDeallocateionImpl(algStack);

}

TEST(AllocAlgorithms, tsSourcesLocalOutOfMemory)
{

	stackLocal_t algStack;
	tsOutOfMemoryImpl(algStack);

}