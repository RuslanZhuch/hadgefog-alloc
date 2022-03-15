module;
#include <utility>
#include <concepts>

export module hfogCore;

import MemoryUtils;
import MemoryBlock;

export namespace hfog
{

#ifdef _WIN64
	constexpr bool bX64{ true };
#else
	constexpr bool bX64{ false };
#endif

	template<typename T>
	concept CtAllocator = requires(T t, const T ct)
	{
		{t.allocate(std::declval<mem_t>())} -> std::convertible_to<MemoryBlock>;
		{t.deallocate(std::declval<const MemoryBlock&>())} -> std::convertible_to<void>;
		{ct.getIsOwner(std::declval<byte_t*>())} -> std::convertible_to<bool>;
		requires std::movable<T>;
		requires not std::copyable<T>;
	};

};