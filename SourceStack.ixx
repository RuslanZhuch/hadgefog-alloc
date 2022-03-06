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
		MemoryBlock getMemory(mem_t offset, mem_t size)
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

		void releaseMemory(const MemoryBlock& memBlock)
		{
			garbageWriterOp::clear(memBlock.ptr, 0, memBlock.size);
		}

		void releaseAllMemory()
		{
			garbageWriterOp::clear(this->memBuffer, 0, buffSize);
		}

		[[nodiscard]] mem_t getOffset(byte_t* ptr)
		{
			auto bInRange{ ptr != nullptr };
			auto memPtr{ reinterpret_cast<byte_t*>(&this->memBuffer) };
			bInRange &= (ptr >= memPtr);
			bInRange &= ((memPtr + buffSize) - ptr > 0);
			if (!bInRange)
				return invalidMem_t;

			return static_cast<mem_t>(ptr - memPtr);
		}

	private:
		byte_t memBuffer[buffSize];
		

	};

};