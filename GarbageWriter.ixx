module;
#include <cstddef>
#include <type_traits>
#include <concepts>
#include <cstring>

export module hfog.GarbageWriter;
import hfog.MemoryUtils;

template<typename T>
concept CtSizeIsOne = requires(T t) { requires sizeof(T) == 1; };

export namespace hfog::GarbageWriter
{

	template<typename T, typename TCallBuffer>
	concept CtGarbageWriter = requires(T, TCallBuffer)
	{
		{T::init(std::declval<TCallBuffer*>(), std::declval<mem_t>(), std::declval<mem_t>())} -> std::convertible_to<void>;
		{T::write(std::declval<TCallBuffer*>(), std::declval<mem_t>(), std::declval<mem_t>())} -> std::convertible_to<void>;
		{T::clear(std::declval<TCallBuffer*>(), std::declval<mem_t>(), std::declval<mem_t>())} -> std::convertible_to<void>;
		
	};

	class Default
	{
	public:
		static void init([[maybe_unused]] auto* dest, [[maybe_unused]] mem_t beginId, [[maybe_unused]] mem_t endId)
		{	}
		static void write([[maybe_unused]] auto* dest, [[maybe_unused]] mem_t beginId, [[maybe_unused]] mem_t endId)
		{	}
		static void clear([[maybe_unused]] auto* dest, [[maybe_unused]] mem_t beginId, [[maybe_unused]] mem_t endId)
		{	}
	};

	template <int writeByte, int clearByte = writeByte> requires requires 
	{
		requires writeByte <= 0xff;
		requires clearByte <= 0xff;
	}
	class ByteWriter
	{
	public:
		static void init([[maybe_unused]] auto* dest, [[maybe_unused]] mem_t beginId, [[maybe_unused]] mem_t endId)
		{	}

		template<typename T>
		static void write(T* dest, mem_t beginId, mem_t endId) requires CtSizeIsOne<T>
		{
			fill(dest, beginId, endId, writeByte);
		}

		template<typename T>
		static void clear(T* dest, mem_t beginId, mem_t endId) requires CtSizeIsOne<T>
		{
			fill(dest, beginId, endId, clearByte);
		}
	private:
		static void fill(auto* dest, mem_t beginId, mem_t endId, int byteToWrite)
		{
			auto currId{ beginId };
			while (currId != endId)
			{
				dest[currId++] = static_cast<std::remove_pointer_t<decltype(dest)>>(byteToWrite);
			}
		}
	};


	class IncDecWriter
	{
	public:
		template<typename T>
		static void init(T* dest, mem_t beginId, mem_t endId)
		{
			std::memset(dest + beginId, 0, endId - beginId);
		}

		template<typename T>
		static void write(T* dest, mem_t beginId, mem_t endId) requires CtSizeIsOne<T>
		{
			fill(dest, beginId, endId, 1);
		}

		template<typename T>
		static void clear(T* dest, mem_t beginId, mem_t endId) requires CtSizeIsOne<T>
		{
			fill(dest, beginId, endId, -1);
		}
	private:
		static void fill(auto* dest, mem_t beginId, mem_t endId, int incVal)
		{
			auto currId{ beginId };
			while (currId != endId)
			{
				dest[currId++] += static_cast<std::remove_pointer_t<decltype(dest)>>(incVal);
			}
		}
	};

};