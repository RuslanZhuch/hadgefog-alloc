export module hfogAlloc;

export import MemoryUtils;

export import AlgNull;
export import AlgStack;
export import AlgUnified;

import SourceStack;
import SourceExt;

export namespace hfog::Alloc
{

	//Null aliases
	using Null = Algorithms::Null;

	//Stack aliases
	template <mem_t alignment, mem_t stackSize>
	using Stack = Algorithms::Stack<hfog::Sources::Stack<stackSize, hfog::GarbageWriter::Default>, alignment>;

	template <mem_t alignment>
	using StackExt = Algorithms::Stack<hfog::Sources::External<hfog::GarbageWriter::Default>, alignment>;

//	template <mem_t alignment>
//	using StackHeap = Algorithms::Stack<hfog::Sources::Heap<hfog::GarbageWriter::Default>, alignment>;

	//Unified aliases
	template <mem_t alignment, int numOfSegments, mem_t stackSize>
	using Unified = Algorithms::Unified<hfog::Sources::Stack<stackSize, hfog::GarbageWriter::Default>, 
		alignment, numOfSegments, stackSize / (alignment * numOfSegments)>;

	template <mem_t alignment, int numOfSegments, mem_t stackSize>
	using UnifiedExt = Algorithms::Unified<hfog::Sources::External<hfog::GarbageWriter::Default>,
		alignment, numOfSegments, stackSize / (alignment * numOfSegments)>;

//	template <mem_t alignment, int numOfSegments, mem_t stackSize>
//	using UnifiedHeap = Algorithms::Unified<hfog::Sources::Heap<hfog::GarbageWriter::Default>,
//		alignment, numOfSegments, stackSize / (alignment * numOfSegments)>;

};