export module hfogAlloc;

export import MemoryUtils;

export import AlgNull;
export import AlgLinear;
export import AlgStack;

import SourceStack;
import SourceExt;
import SourceHeap;

export namespace hfog::Alloc
{

	//Null aliases
	using Null = Algorithms::Null;

	//Linear aliases
	template <mem_t alignment, mem_t stackSize>
	using Linear = Algorithms::Linear<hfog::Sources::Stack<stackSize, hfog::GarbageWriter::Default>, alignment>;

	template <mem_t alignment>
	using LinearExt = Algorithms::Linear<hfog::Sources::External<hfog::GarbageWriter::Default>, alignment>;

	template <mem_t alignment>
	using LinearHeap = Algorithms::Linear<hfog::Sources::Heap<hfog::GarbageWriter::Default>, alignment>;

	//Stack aliases
	template <mem_t alignment, mem_t stackSize>
	using Stack = Algorithms::Stack<hfog::Sources::Stack<stackSize, hfog::GarbageWriter::Default>, alignment>;

	template <mem_t alignment>
	using StackExt = Algorithms::Stack<hfog::Sources::External<hfog::GarbageWriter::Default>, alignment>;

	template <mem_t alignment>
	using StackHeap = Algorithms::Stack<hfog::Sources::Heap<hfog::GarbageWriter::Default>, alignment>;


};