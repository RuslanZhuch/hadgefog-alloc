#include "pch.h"

#include <type_traits>

import hfog.Alloc;

using namespace hfog::MemoryUtils::Literals;

TEST(Predefines, tsAlgStack)
{

	static_assert(std::is_same_v<
		hfog::Alloc::Stack<16_B, 128_B>,
		hfog::Algorithms::Stack<hfog::Sources::Stack<128_B, hfog::GarbageWriter::Default>, 16_B>
	>);

	static_assert(std::is_same_v<
		hfog::Alloc::StackExt<16_B>,
		hfog::Algorithms::Stack<hfog::Sources::External<hfog::GarbageWriter::Default>, 16_B>
	>);

}

TEST(Predefines, tsAlgNull)
{

	static_assert(std::is_same_v<
		hfog::Alloc::Null,
		hfog::Algorithms::Null
	>);

}

TEST(Predefines, tsAlgPool)
{

	static_assert(std::is_same_v<
		hfog::Alloc::Pool<16_B, 4>,
		hfog::Algorithms::Pool<hfog::Sources::Stack<64_B, hfog::GarbageWriter::Default>, 16_B, 4>
	>);

	static_assert(std::is_same_v<
		hfog::Alloc::Pool<16_B, 4, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>,
		hfog::Algorithms::Pool<hfog::Sources::Stack<64_B, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>, 16_B, 4>
	>);

	static_assert(std::is_same_v<
		hfog::Alloc::PoolExt<16_B, 4>,
		hfog::Algorithms::Pool<hfog::Sources::External<hfog::GarbageWriter::Default>, 16_B, 4>
	>);

	static_assert(std::is_same_v<
		hfog::Alloc::PoolExt<16_B, 4, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>,
		hfog::Algorithms::Pool<hfog::Sources::External<hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>, 16_B, 4>
	>);

}

TEST(Predefines, tsAlgIslands)
{

	static_assert(std::is_same_v<
		hfog::Alloc::Islands<16_B, 64_B, 8>,
		hfog::Algorithms::Islands<hfog::Sources::Stack<512_B, hfog::GarbageWriter::Default>, 16_B, 4, 8>
	>);

	static_assert(std::is_same_v<
		hfog::Alloc::Islands<16_B, 64_B, 8, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>,
		hfog::Algorithms::Islands<hfog::Sources::Stack<512_B, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>, 16_B, 4, 8>
	>);

	static_assert(std::is_same_v<
		hfog::Alloc::IslandsExt<16_B, 64_B, 8>,
		hfog::Algorithms::Islands<hfog::Sources::External<hfog::GarbageWriter::Default>, 16_B, 4, 8>
	>);

	static_assert(std::is_same_v<
		hfog::Alloc::IslandsExt<16_B, 64_B, 8, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>,
		hfog::Algorithms::Islands<hfog::Sources::External<hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>, 16_B, 4, 8>
	>);

}

TEST(Predefines, tsAlgUnified)
{

	static_assert(std::is_same_v<
		hfog::Alloc::Unified<128_B, 1_kB>,
		hfog::Algorithms::Unified<hfog::Sources::Stack<1152_B, hfog::GarbageWriter::Default>, 128_B, 1_kB>
	>);

	static_assert(std::is_same_v<
		hfog::Alloc::Unified<128_B, 1_kB, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>,
		hfog::Algorithms::Unified<hfog::Sources::Stack<1152_B, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>, 128_B, 1_kB>
	>);

	static_assert(std::is_same_v<
		hfog::Alloc::UnifiedExt<128_B, 1_kB>,
		hfog::Algorithms::Unified<hfog::Sources::External<hfog::GarbageWriter::Default>, 128_B, 1_kB>
	>);

	static_assert(std::is_same_v<
		hfog::Alloc::UnifiedExt<128_B, 1_kB, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>,
		hfog::Algorithms::Unified<hfog::Sources::External<hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>, 128_B, 1_kB>
	>);

}