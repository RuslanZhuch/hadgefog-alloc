module;
#include <cstddef>
#include <type_traits>
#include <concepts>
#include <cstring>

export module hfog.GarbageWriter;
import hfog.MemoryUtils;

export namespace hfog::GarbageWriter
{

	template<typename T>
	concept CtGarbageWriter = requires(T)
	{
		{T::init(std::declval<byte_t*>(), std::declval<mem_t>(), std::declval<mem_t>())} -> std::convertible_to<void>;
		{T::write(std::declval<byte_t*>(), std::declval<mem_t>(), std::declval<mem_t>())} -> std::convertible_to<void>;
		{T::clear(std::declval<byte_t*>(), std::declval<mem_t>(), std::declval<mem_t>())} -> std::convertible_to<void>;
		
	};

	class Default
	{
	public:
		static void init([[maybe_unused]] byte_t* dest, [[maybe_unused]] mem_t beginId, [[maybe_unused]] mem_t endId)
		{	}
		static void write([[maybe_unused]] byte_t* dest, [[maybe_unused]] mem_t beginId, [[maybe_unused]] mem_t endId)
		{	}
		static void clear([[maybe_unused]] byte_t* dest, [[maybe_unused]] mem_t beginId, [[maybe_unused]] mem_t endId)
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
		static void init([[maybe_unused]] byte_t* dest, [[maybe_unused]] mem_t beginId, [[maybe_unused]] mem_t endId)
		{	}

		static void write(byte_t* dest, mem_t beginId, mem_t endId)
		{
			fill(dest, beginId, endId, writeByte);
		}

		static void clear(byte_t* dest, mem_t beginId, mem_t endId)
		{
			fill(dest, beginId, endId, clearByte);
		}
	private:
		static void fill(byte_t* dest, mem_t beginId, mem_t endId, int byteToWrite)
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
		static void init(byte_t* dest, mem_t beginId, mem_t endId)
		{
			std::memset(dest + beginId, 0, endId - beginId);
		}

		static void write(byte_t* dest, mem_t beginId, mem_t endId)
		{
			fill(dest, beginId, endId, 1);
		}

		static void clear(byte_t* dest, mem_t beginId, mem_t endId)
		{
			fill(dest, beginId, endId, -1);
		}
	private:
		static void fill(byte_t* dest, mem_t beginId, mem_t endId, int incVal)
		{
			auto currId{ beginId };
			while (currId != endId)
			{
				dest[currId++] += static_cast<std::remove_pointer_t<decltype(dest)>>(incVal);
			}
		}
	};

};