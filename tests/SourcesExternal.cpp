#include "pch.h"
#include "Protection.h"

import hfog.Sources.Ext;
import hfog.Sources.Common;

using namespace hfog::MemoryUtils::Literals;

static_assert(hfog::Sources::CtSource<hfog::Sources::External<hfog::GarbageWriter::Default>>);

using external_t = hfog::Sources::External<hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>>;

static constexpr auto BUFFER_SIZE{ 128_B };

static auto& prepareExtBuffer(auto* extBuffer)
{
	std::memset(extBuffer, 0, sizeof(extBuffer));

	static hfog::MemoryBlock extMemBlock;
	extMemBlock.ptr = extBuffer;
	extMemBlock.size = BUFFER_SIZE;

	return extMemBlock;
}

TEST(SourcesExternal, tsProtectionReleaseMemory)
{

	static byte_t extBuffer[BUFFER_SIZE];
	external_t localSource(prepareExtBuffer(extBuffer));

	hfog::MemoryBlock memBlock;
	localSource.releaseMemory(memBlock);
	EXPECT_EQ(hfogprotect::instance()->getLastError().value().code, hfogprotect::ErrorCodes::INVALID_PTR);

	EXPECT_FALSE(hfogprotect::instance()->getLastError().has_value());

	const auto memBlockOffLimits{ localSource.getMemory(0_B, 16_B) };
	const auto offset{ localSource.getOffset(memBlockOffLimits.ptr + BUFFER_SIZE + 16_B) };
	EXPECT_EQ(hfogprotect::instance()->getLastError().value().code, hfogprotect::ErrorCodes::NOT_IN_RANGE);

}

TEST(SourcesExternal, tsSimpleMemReceiving)
{

	static byte_t extBuffer[BUFFER_SIZE];
	external_t localSource(prepareExtBuffer(extBuffer));

	static constexpr auto OFFSET_SIZE{ 0_B };
	static constexpr auto EL_SIZE{ 16_B };

	const auto memBlock{ localSource.getMemory(OFFSET_SIZE, EL_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, SET_BYTES));
	EXPECT_EQ(memBlock.ptr, extBuffer);
	EXPECT_EQ(memBlock.size, EL_SIZE);
	localSource.releaseMemory(memBlock);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, CLEAR_BYTES));

}

TEST(SourcesExternal, tsContinuousReceiving)
{

	static byte_t extBuffer[BUFFER_SIZE];
	external_t localSource(prepareExtBuffer(extBuffer));

	static constexpr auto OFFSET_SIZE{ 16_B };
	static constexpr auto EL_SIZE{ 16_B };

	const auto memBlock{ localSource.getMemory(0_B, EL_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, SET_BYTES));
	EXPECT_EQ(memBlock.ptr, extBuffer);
	EXPECT_EQ(memBlock.size, EL_SIZE);

	const auto memBlock2{ localSource.getMemory(OFFSET_SIZE, EL_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, EL_SIZE, SET_BYTES));
	EXPECT_EQ(memBlock2.ptr, extBuffer + OFFSET_SIZE);
	EXPECT_EQ(memBlock2.size, EL_SIZE);

	localSource.releaseMemory(memBlock);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, CLEAR_BYTES));
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, EL_SIZE, SET_BYTES));
	localSource.releaseMemory(memBlock2);
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, EL_SIZE, CLEAR_BYTES));

}

TEST(SourcesExternal, tsOverlapReceiving)
{

	static byte_t extBuffer[BUFFER_SIZE];
	external_t localSource(prepareExtBuffer(extBuffer));

	static constexpr auto OFFSET_SIZE{ 16_B };
	static constexpr auto EL_SIZE{ 64_B };

	const auto memBlock{ localSource.getMemory(OFFSET_SIZE, EL_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, SET_BYTES));
	EXPECT_EQ(memBlock.ptr, extBuffer + OFFSET_SIZE);
	EXPECT_EQ(memBlock.size, EL_SIZE);

	const auto memBlock2{ localSource.getMemory(EL_SIZE, BUFFER_SIZE) };
	EXPECT_EQ(memBlock2.ptr, nullptr);

	localSource.releaseMemory(memBlock);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, CLEAR_BYTES));

	const auto memBlock3{ localSource.getMemory(0_B, BUFFER_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, BUFFER_SIZE, SET_BYTES));
	EXPECT_EQ(memBlock3.ptr, extBuffer);
	EXPECT_EQ(memBlock3.size, BUFFER_SIZE);
	localSource.releaseMemory(memBlock3);
	EXPECT_TRUE(getValuesAre(memBlock3.ptr, 0_B, BUFFER_SIZE, CLEAR_BYTES));

}

TEST(SourcesExternal, tsReverseReceiving)
{

	static byte_t extBuffer[BUFFER_SIZE];
	external_t localSource(prepareExtBuffer(extBuffer));

	static constexpr auto OFFSET_SIZE{ 16_B };
	static constexpr auto EL_SIZE{ 16_B };

	const auto memBlock{ localSource.getMemory(OFFSET_SIZE, EL_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, SET_BYTES));
	EXPECT_EQ(memBlock.ptr, extBuffer + OFFSET_SIZE);
	EXPECT_EQ(memBlock.size, EL_SIZE);
	EXPECT_EQ(localSource.getOffset(memBlock.ptr), OFFSET_SIZE);

	const auto memBlock2{ localSource.getMemory(0_B, EL_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, EL_SIZE, SET_BYTES));
	EXPECT_EQ(memBlock2.ptr, extBuffer);
	EXPECT_EQ(memBlock2.size, EL_SIZE);
	EXPECT_EQ(localSource.getOffset(memBlock2.ptr), 0_B);

	localSource.releaseMemory(memBlock);
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, CLEAR_BYTES));
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, EL_SIZE, SET_BYTES));

	localSource.releaseMemory(memBlock2);
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, EL_SIZE, CLEAR_BYTES));

}

TEST(SourcesExternal, tsReleaseAll)
{

	static byte_t extBuffer[BUFFER_SIZE];
	external_t localSource(prepareExtBuffer(extBuffer));

	static constexpr auto OFFSET_SIZE{ 16_B };
	static constexpr auto EL_SIZE{ 16_B };

	const auto memBlock{ localSource.getMemory(OFFSET_SIZE, EL_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, SET_BYTES));
	EXPECT_EQ(memBlock.ptr, extBuffer + OFFSET_SIZE);
	EXPECT_EQ(memBlock.size, EL_SIZE);

	const auto memBlock2{ localSource.getMemory(0_B, EL_SIZE) };
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, EL_SIZE, SET_BYTES));
	EXPECT_EQ(memBlock2.ptr, extBuffer);
	EXPECT_EQ(memBlock2.size, EL_SIZE);

	localSource.releaseAllMemory();
	EXPECT_TRUE(getValuesAre(memBlock.ptr, 0_B, EL_SIZE, CLEAR_BYTES));
	EXPECT_TRUE(getValuesAre(memBlock2.ptr, 0_B, EL_SIZE, CLEAR_BYTES));

}

TEST(SourcesExternal, tsOutOfMemory)
{

	static byte_t extBuffer[BUFFER_SIZE];
	external_t localSource(prepareExtBuffer(extBuffer));

	const auto memBlock{ localSource.getMemory(0_B, 200_B) };
	EXPECT_EQ(memBlock.ptr, nullptr);

}

TEST(SourcesExternal, tsInvalidNullptr)
{

	static byte_t extBuffer[BUFFER_SIZE];
	external_t localSource(prepareExtBuffer(extBuffer));
	EXPECT_EQ(localSource.getOffset(nullptr), invalidMem_t);

}

TEST(SourcesExternal, tsInvalidOffset)
{

	byte_t invalidVal{ 42 };
	static byte_t extBuffer[BUFFER_SIZE];
	external_t localSource(prepareExtBuffer(extBuffer));
	EXPECT_EQ(localSource.getOffset(&invalidVal), invalidMem_t);

}
