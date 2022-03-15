export module SourceStack;

export import MemoryBlock;
export import MemoryUtils;
export import GarbageWriter;

import hfogCore;

export namespace hfog::Sources
{
	
	template <mem_t buffSize, GarbageWriter::CtGarbageWriter<byte_t> garbageWriterOp = GarbageWriter::Default>
	class Stack
	{

	public:
		Stack() = default;
		Stack(const Stack&) = delete;
		Stack& operator=(const Stack&) = delete;

		Stack(Stack&&) = default;
		Stack& operator=(Stack&&) = default;

		[[nodiscard]] MemoryBlock getMemory(mem_t offset, mem_t size) noexcept
		{

			MemoryBlock memBlock;

//			if constexpr (bX64)
//			{
//				const auto bInRange{ offset + size <= buffSize };
//
//				byte_t* path[] = { nullptr, this->memBuffer + offset };
//				memBlock.ptr = path[bInRange];
//				memBlock.size = size * bInRange;
//			}
//			else
//			{

				if (offset + size > buffSize)
					return memBlock;
				
				memBlock.ptr = this->memBuffer + offset;
				memBlock.size = size;
			
//			}

			garbageWriterOp::init(memBlock.ptr, 0, memBlock.size);
			garbageWriterOp::write(memBlock.ptr, 0, memBlock.size);
			
			return memBlock;

		}

		void releaseMemory(const MemoryBlock& memBlock) noexcept
		{
			garbageWriterOp::clear(memBlock.ptr, 0, memBlock.size);
		}

		void releaseAllMemory() noexcept
		{
			garbageWriterOp::clear(this->memBuffer, 0, buffSize);
		}

		[[nodiscard]] mem_t getOffset(byte_t* ptr) const noexcept
		{
			auto bInRange{ ptr != nullptr };
			bInRange &= (ptr >= this->memBuffer);
			bInRange &= ((this->memBuffer + buffSize) - ptr > 0);
			if (!bInRange)
				return invalidMem_t;

			return static_cast<mem_t>(ptr - this->memBuffer);
		}

	private:
		byte_t memBuffer[buffSize];
		
	};

};