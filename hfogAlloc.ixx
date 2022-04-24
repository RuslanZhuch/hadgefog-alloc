export module hfog.Alloc;

export import hfog.MemoryUtils;

export import hfog.Algorithms.Null;
export import hfog.Algorithms.Stack;
export import hfog.Algorithms.Pool;
export import hfog.Algorithms.Islands;
export import hfog.Algorithms.Unified;

import hfog.Sources.Stack;
import hfog.Sources.Ext;

export namespace hfog::Alloc
{

	//Null aliases
	using Null = Algorithms::Null;

	//Stack aliases
	template <mem_t alignment, mem_t stackSize,
		hfog::GarbageWriter::CtGarbageWriter<byte_t> GWriter = hfog::GarbageWriter::Default>
	using Stack = Algorithms::Stack<hfog::Sources::Stack<stackSize, GWriter>, alignment>;

	template <mem_t alignment,
		hfog::GarbageWriter::CtGarbageWriter<byte_t> GWriter = hfog::GarbageWriter::Default>
	using StackExt = Algorithms::Stack<hfog::Sources::External<GWriter>, alignment, MemoryUtils::Align<alignment>>;

	//Pool aliases
	template <mem_t alignment, size_t numOfChunks,
		hfog::GarbageWriter::CtGarbageWriter<byte_t> GWriter = hfog::GarbageWriter::Default>
		using Pool = Algorithms::Pool<hfog::Sources::Stack<alignment * numOfChunks, GWriter>,
		alignment, numOfChunks>;

	template <mem_t alignment, size_t numOfChunks,
		hfog::GarbageWriter::CtGarbageWriter<byte_t> GWriter = hfog::GarbageWriter::Default>
		using PoolExt = Algorithms::Pool<hfog::Sources::External<GWriter>,
		alignment, numOfChunks>;

	//Islands aliases
	template <mem_t alignment, mem_t maxAllocateBytes, size_t numOfChunks,
		hfog::GarbageWriter::CtGarbageWriter<byte_t> GWriter = hfog::GarbageWriter::Default>
	using Islands = Algorithms::Islands<hfog::Sources::Stack<maxAllocateBytes * numOfChunks, GWriter>,
		alignment, maxAllocateBytes / alignment, numOfChunks>;

	template <mem_t alignment, mem_t maxAllocateBytes, size_t numOfChunks,
		hfog::GarbageWriter::CtGarbageWriter<byte_t> GWriter = hfog::GarbageWriter::Default>
	using IslandsExt = Algorithms::Islands<hfog::Sources::External<GWriter>,
		alignment, maxAllocateBytes / alignment, numOfChunks>;

	//Unified aliases
	template <mem_t alignment, mem_t maxAllocateBytes,
		hfog::GarbageWriter::CtGarbageWriter<byte_t> GWriter = hfog::GarbageWriter::Default>
	using Unified = Algorithms::Unified<hfog::Sources::Stack<
		maxAllocateBytes + hfog::Algorithms::computeFreelistMemorySize<maxAllocateBytes, alignment>(), GWriter>,
		alignment, maxAllocateBytes>;

	template <mem_t alignment, mem_t maxAllocateBytes,
		hfog::GarbageWriter::CtGarbageWriter<byte_t> GWriter = hfog::GarbageWriter::Default>
	using UnifiedExt = Algorithms::Unified<hfog::Sources::External<GWriter>,
		alignment, maxAllocateBytes>;
};