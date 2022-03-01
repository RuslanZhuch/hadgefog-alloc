export module MemoryUtils;

export using mem_t = size_t;

export namespace hfog::MemoryUtils::Literals
{

	consteval mem_t operator ""_B(size_t in) noexcept
	{
		return in;
	}
	consteval mem_t operator ""_kB(size_t in) noexcept
	{
		return in * 1024;
	}
	consteval mem_t operator ""_MB(size_t in) noexcept
	{
		return in * 1024 * 1024;
	}
	consteval mem_t operator ""_GB(size_t in) noexcept
	{
		return in * 1024 * 1024 * 1024;
	}
	consteval mem_t operator ""_TB(size_t in) noexcept
	{
		return in * 1024 * 1024 * 1024 * 1024;
	}
};

export namespace hfog::MemoryUtils
{

	template <size_t alignment>
	class align
	{
	public:
		static_assert(alignment > 0, "Alignment must be > 0");
		[[nodiscard]] constexpr size_t operator()(size_t in) const noexcept
		{
			return ((in + alignment - 1) / alignment) * alignment;
		}
	};

};