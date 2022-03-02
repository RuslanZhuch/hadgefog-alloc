module;
#include <utility>
#include <concepts>

export module SourcesCommon;
import MemoryUtils;
import MemoryBlock;

export namespace hfog::Sources
{

	template<typename T>
	concept CtSource = requires(T s)
	{
		{s.getMemory(std::declval<mem_t>(), std::declval<mem_t>())} -> std::convertible_to<MemoryBlock>;
		{s.releaseMemory(std::declval<MemoryBlock>())} -> std::convertible_to<void>;
		{s.releaseAllMemory()} -> std::convertible_to<void>;
	};

};