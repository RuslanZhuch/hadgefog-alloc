#include "pch.h"

import hfog.GarbageWriter;

static_assert(hfog::GarbageWriter::CtGarbageWriter<hfog::GarbageWriter::Default, char>);
static_assert(hfog::GarbageWriter::CtGarbageWriter<hfog::GarbageWriter::ByteWriter<0xAF>, char>);

TEST(GarbageWriter, tsDefaultWriter)
{

	hfog::GarbageWriter::Default gWriter;

	char bytes[10];
	memset(bytes, 0xAF, sizeof(bytes));
	char expected[]{
		static_cast<char>(0xAF),
		static_cast<char>(0xAF),
		static_cast<char>(0xAF),
		static_cast<char>(0xAF),
		static_cast<char>(0xAF),
		static_cast<char>(0xAF),
		static_cast<char>(0xAF),
		static_cast<char>(0xAF),
		static_cast<char>(0xAF),
		static_cast<char>(0xAF)
	};

	gWriter.write(bytes, 2, 7);
	EXPECT_EQ(std::memcmp(bytes, expected, sizeof(expected)), 0);

	gWriter.clear(bytes, 2, 7);
	EXPECT_EQ(std::memcmp(bytes, expected, sizeof(expected)), 0);

}

TEST(GarbageWriter, tsByteWriter)
{
	
	hfog::GarbageWriter::ByteWriter<0xAF, 0xFA> gWriter;

	char bytes[10];
	memset(bytes, 0, sizeof(bytes));
	char expected[]{ 
		static_cast<char>(0x00), 
		static_cast<char>(0x00), 
		static_cast<char>(0xAF), 
		static_cast<char>(0xAF), 
		static_cast<char>(0xAF), 
		static_cast<char>(0xAF), 
		static_cast<char>(0xAF),
		static_cast<char>(0x00),
		static_cast<char>(0x00),
		static_cast<char>(0x00)	
	};

	gWriter.write(bytes, 2, 7);
	EXPECT_EQ(std::memcmp(bytes, expected, sizeof(expected)), 0);

	char expectedClear[]{
	static_cast<char>(0x00),
	static_cast<char>(0x00),
	static_cast<char>(0xFA),
	static_cast<char>(0xFA),
	static_cast<char>(0xFA),
	static_cast<char>(0xFA),
	static_cast<char>(0xFA),
	static_cast<char>(0x00),
	static_cast<char>(0x00),
	static_cast<char>(0x00)
	};

	gWriter.clear(bytes, 2, 7);
	EXPECT_EQ(std::memcmp(bytes, expectedClear, sizeof(expectedClear)), 0);

}

TEST(GarbageWriter, tsIncDecWriter)
{

	hfog::GarbageWriter::IncDecWriter gWriter;

	char bytes[4];
	memset(bytes, 0, sizeof(bytes));
	char expected[]{
		static_cast<char>(0x00),
		static_cast<char>(0x00),
		static_cast<char>(0x01),
		static_cast<char>(0x01)
	};

	gWriter.write(bytes, 2, 4);
	EXPECT_EQ(std::memcmp(bytes, expected, sizeof(expected)), 0);

	char expected2[]{
		static_cast<char>(0x00),
		static_cast<char>(0x01),
		static_cast<char>(0x02),
		static_cast<char>(0x01)
	}; 

	gWriter.write(bytes, 1, 3);
	EXPECT_EQ(std::memcmp(bytes, expected2, sizeof(expected2)), 0);

	char expectedClear[]{
		static_cast<char>(0x00),
		static_cast<char>(0x01),
		static_cast<char>(0x01),
		static_cast<char>(0x00)
	};

	gWriter.clear(bytes, 2, 4);
	EXPECT_EQ(std::memcmp(bytes, expectedClear, sizeof(expectedClear)), 0);

	bytes[0] = 1;
	bytes[1] = 2;
	bytes[2] = 3;
	bytes[3] = 4;

	char expectedInit[]{
		static_cast<char>(0x01),
		static_cast<char>(0x02),
		static_cast<char>(0x00),
		static_cast<char>(0x00)
	};

	gWriter.init(bytes, 2, 4);
	EXPECT_EQ(std::memcmp(bytes, expectedInit, sizeof(expectedInit)), 0);

}