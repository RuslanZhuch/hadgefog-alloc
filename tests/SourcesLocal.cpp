#include "pch.h"

#include "Protection.h"

import hfog.Sources.Local;
import hfog.Sources.Common;

using namespace hfog::MemoryUtils::Literals;

static constexpr auto BUFFER_SIZE{ 128_B };

static_assert(hfog::Sources::CtSource<hfog::Sources::Local<BUFFER_SIZE, hfog::GarbageWriter::Default>>);

using local_t = hfog::Sources::Local<BUFFER_SIZE, hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>>;

TEST(SourcesLocal, tsProtectionReleaseMemory)
{

	local_t localSource;
	hfog::MemoryBlock memBlock;
	localSource.releaseMemory(memBlock);
	EXPECT_EQ(hfogprotect::getLastError().value().code, hfogprotect::ErrorCodes::INVALID_PTR);

	EXPECT_FALSE(hfogprotect::getLastError().has_value());

	const auto memBlockOffLimits{ localSource.getMemory(0_B, 16_B) };
	[[maybe_unused]] const auto offset{ localSource.getOffset(memBlockOffLimits.ptr + BUFFER_SIZE + 16_B) };
	EXPECT_EQ(hfogprotect::getLastError().value().code, hfogprotect::ErrorCodes::NOT_IN_RANGE);

}

TEST(SourcesLocal, tsSimpleMemReceiving)
{

	local_t localSource;

	static constexpr auto OFFSET_SIZE{ 0_B };
	static constexpr auto EL_SIZE{ 16_B };

	const auto memBlock{ localSource.getMemory(OFFSET_SIZE, EL_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, SET_BYTES));
	EXPECT_EQ(memBlock.ptr, reinterpret_cast<byte_t*>(&localSource));
	EXPECT_EQ(memBlock.size, EL_SIZE);
	localSource.releaseMemory(memBlock);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, CLEAR_BYTES));

}

TEST(SourcesLocal, tsContinuousReceiving)
{

	local_t localSource;

	static constexpr auto OFFSET_SIZE{ 16_B };
	static constexpr auto EL_SIZE{ 16_B };

	const auto memBlock{ localSource.getMemory(0_B, EL_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, SET_BYTES));
	EXPECT_EQ(memBlock.ptr, reinterpret_cast<byte_t*>(&localSource));
	EXPECT_EQ(memBlock.size, EL_SIZE);

	const auto memBlock2{ localSource.getMemory(OFFSET_SIZE, EL_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, EL_SIZE, SET_BYTES));
	EXPECT_EQ(memBlock2.ptr, reinterpret_cast<byte_t*>(&localSource) + OFFSET_SIZE);
	EXPECT_EQ(memBlock2.size, EL_SIZE);

	localSource.releaseMemory(memBlock);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, CLEAR_BYTES));
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, EL_SIZE, SET_BYTES));
	localSource.releaseMemory(memBlock2);
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, EL_SIZE, CLEAR_BYTES));

}

TEST(SourcesLocal, tsOverlapReceiving)
{

	local_t localSource;

	static constexpr auto OFFSET_SIZE{ 16_B };
	static constexpr auto EL_SIZE{ 64_B };

	const auto memBlock{ localSource.getMemory(OFFSET_SIZE, EL_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, SET_BYTES));
	EXPECT_EQ(memBlock.ptr, reinterpret_cast<byte_t*>(&localSource) + OFFSET_SIZE);
	EXPECT_EQ(memBlock.size, EL_SIZE);

	const auto memBlock2{ localSource.getMemory(EL_SIZE, BUFFER_SIZE) };
	EXPECT_EQ(memBlock2.ptr, nullptr);

	localSource.releaseMemory(memBlock);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, CLEAR_BYTES));

	const auto memBlock3{ localSource.getMemory(0_B, BUFFER_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, BUFFER_SIZE, SET_BYTES));
	EXPECT_EQ(memBlock3.ptr, reinterpret_cast<byte_t*>(&localSource));
	EXPECT_EQ(memBlock3.size, BUFFER_SIZE);
	localSource.releaseMemory(memBlock3);
	EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, BUFFER_SIZE, CLEAR_BYTES));

}

TEST(SourcesLocal, tsReverseReceiving)
{

	local_t localSource;

	static constexpr auto OFFSET_SIZE{ 16_B };
	static constexpr auto EL_SIZE{ 16_B };

	const auto memBlock{ localSource.getMemory(OFFSET_SIZE, EL_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, SET_BYTES));
	EXPECT_EQ(memBlock.ptr, reinterpret_cast<byte_t*>(&localSource) + OFFSET_SIZE);
	EXPECT_EQ(memBlock.size, EL_SIZE);
	EXPECT_EQ(localSource.getOffset(memBlock.ptr), OFFSET_SIZE);

	const auto memBlock2{ localSource.getMemory(0_B, EL_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, EL_SIZE, SET_BYTES));
	EXPECT_EQ(memBlock2.ptr, reinterpret_cast<byte_t*>(&localSource));
	EXPECT_EQ(memBlock2.size, EL_SIZE);
	EXPECT_EQ(localSource.getOffset(memBlock2.ptr), 0_B);

	localSource.releaseMemory(memBlock);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, CLEAR_BYTES));
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, EL_SIZE, SET_BYTES));

	localSource.releaseMemory(memBlock2);
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, EL_SIZE, CLEAR_BYTES));

}

TEST(SourcesLocal, tsReleaseAll)
{

	local_t localSource;

	static constexpr auto OFFSET_SIZE{ 16_B };
	static constexpr auto EL_SIZE{ 16_B };

	const auto memBlock{ localSource.getMemory(OFFSET_SIZE, EL_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, SET_BYTES));
	EXPECT_EQ(memBlock.ptr, reinterpret_cast<byte_t*>(&localSource) + OFFSET_SIZE);
	EXPECT_EQ(memBlock.size, EL_SIZE);

	const auto memBlock2{ localSource.getMemory(0_B, EL_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, EL_SIZE, SET_BYTES));
	EXPECT_EQ(memBlock2.ptr, reinterpret_cast<byte_t*>(&localSource));
	EXPECT_EQ(memBlock2.size, EL_SIZE);

	localSource.releaseAllMemory();
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, CLEAR_BYTES));
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, EL_SIZE, CLEAR_BYTES));

}

TEST(SourcesLocal, tsOutOfMemory)
{

	local_t localSource;

	const auto memBlock{ localSource.getMemory(0_B, 200_B) };
	EXPECT_EQ(memBlock.ptr, nullptr);

}

TEST(SourcesLocal, tsInvalidNullptr)
{

	local_t localSource;
	EXPECT_EQ(localSource.getOffset(nullptr), invalidMem_t);

}

TEST(SourcesLocal, tsInvalidOffset)
{

	byte_t invalidVal{ 42 };
	local_t localSource;
	EXPECT_EQ(localSource.getOffset(&invalidVal), invalidMem_t);

}