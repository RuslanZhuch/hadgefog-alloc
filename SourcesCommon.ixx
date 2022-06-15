module;
#include <utility>
#include <concepts>

export module hfog.Sources.Common;
import hfog.MemoryUtils;
import hfog.MemoryBlock;

export namespace hfog::Sources
{

	template<typename T>
	concept CtSource = requires(T s, const T cs)
	{
		{s.getMemory(std::declval<mem_t>(), std::declval<mem_t>())} -> std::convertible_to<MemoryBlock>;
		{s.releaseMemory(std::declval<MemoryBlock>())} -> std::convertible_to<void>;
		{s.releaseAllMemory()} -> std::convertible_to<void>;
		{cs.getOffset(std::declval<byte_t*>())} -> std::convertible_to<mem_t>;
		{cs.getMemorySize()} -> std::convertible_to<mem_t>;
		requires std::movable<T>;
		requires not std::copyable<T>;
	};

};