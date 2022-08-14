module;
//#include <utility>
#include <concepts>
#include <limits>

export module hfog.MemoryUtils;

export using mem_t = unsigned int;
export using byte_t = unsigned char;
export inline constexpr auto invalidMem_t = std::numeric_limits<mem_t>::max();

export namespace hfog::MemoryUtils::Literals
{

	using litInput_t = unsigned long long;

	consteval mem_t operator ""_B(litInput_t in) noexcept
	{
		return static_cast<mem_t>(in);
	}
	consteval mem_t operator ""_kB(litInput_t in) noexcept
	{
		return static_cast<mem_t>(in) * 1024;
	}
	consteval mem_t operator ""_MB(litInput_t in) noexcept
	{
		return static_cast<mem_t>(in) * 1024 * 1024;
	}
	consteval mem_t operator ""_GB(litInput_t in) noexcept
	{
		return static_cast<mem_t>(in) * 1024 * 1024 * 1024;
	}
	consteval mem_t operator ""_TB(litInput_t in) noexcept
	{
		return static_cast<mem_t>(in) * 1024 * 1024 * 1024 * 1024;
	}
};

export namespace hfog::MemoryUtils
{

	template<typename T>
	concept CtAlign = requires(T)
	{
		{T::compute(std::declval<mem_t>())} -> std::convertible_to<mem_t>;
	};

	template <mem_t alignment>
	class Align
	{
	public:
		static_assert(alignment > 0, "Alignment must be > 0");
		[[nodiscard]] static constexpr mem_t compute(mem_t in) noexcept
		{
			return ((in + alignment - 1) / alignment) * alignment;
		}
	};

};