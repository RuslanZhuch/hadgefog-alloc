export module hfogAlloc;

export import MemoryUtils;
export import AlgLinear;
import SourceStack;
import SourceExt;
import SourceHeap;

export namespace hfog::Alloc
{

	template <mem_t alignment, mem_t stackSize>
	using LinearStack = Algorithms::Linear<hfog::Sources::Stack<stackSize, hfog::GarbageWriter::Default>, alignment>;

	template <mem_t alignment>
	using LinearExt = Algorithms::Linear<hfog::Sources::External<hfog::GarbageWriter::Default>, alignment>;

	template <mem_t alignment>
	using LinearHeap = Algorithms::Linear<hfog::Sources::Heap<hfog::GarbageWriter::Default>, alignment>;


};