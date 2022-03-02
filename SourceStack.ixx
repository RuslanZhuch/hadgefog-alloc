export module SourceStack;

export import MemoryBlock;
export import MemoryUtils;
export import GarbageWriter;

export namespace hfog::Sources
{
	
	template <mem_t buffSize, GarbageWriter::CtGarbageWriter<char> garbageWriterOp = GarbageWriter::Default>
	class Stack
	{

	public:
		MemoryBlock getMemory(mem_t offset, mem_t size)
		{

			MemoryBlock memBlock;

			if (offset + size > buffSize)
				return memBlock;

			memBlock.ptr = memBuffer + offset;
			if (memBlock.ptr != nullptr)
				memBlock.size = size;

			garbageWriterOp::write(memBlock.ptr, 0, size);

			return memBlock;

		}

		void releaseMemory(MemoryBlock memBlock)
		{
			garbageWriterOp::clear(memBlock.ptr, 0, memBlock.size);
		}

		void releaseAllMemory()
		{
			garbageWriterOp::clear(this->memBuffer, 0, buffSize);
		}

	private:
		byte_t memBuffer[buffSize];

	};

};