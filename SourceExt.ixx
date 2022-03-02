export module SourceExt;

export import MemoryBlock;
export import MemoryUtils;
export import GarbageWriter;

export namespace hfog::Sources
{


	template <GarbageWriter::CtGarbageWriter<char> garbageWriterOp = GarbageWriter::Default>
	class External
	{

	public:
		External(MemoryBlock extMemory)
			:extMemoryBlock(extMemory)
		{}

		MemoryBlock getMemory(mem_t offset, size_t size)
		{

			MemoryBlock memBlock;

			if (this->extMemoryBlock.ptr == nullptr)
				return memBlock;

			if (offset + size > this->extMemoryBlock.size)
				return memBlock;

			memBlock.ptr = extMemoryBlock.ptr + offset;
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
			garbageWriterOp::clear(this->extMemoryBlock.ptr, 0, this->extMemoryBlock.size);
		}

	private:
		MemoryBlock extMemoryBlock;

	};

};