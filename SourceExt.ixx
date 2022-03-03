export module SourceExt;

export import MemoryBlock;
export import MemoryUtils;
export import GarbageWriter;

export namespace hfog::Sources
{


	template <GarbageWriter::CtGarbageWriter<byte_t> garbageWriterOp = GarbageWriter::Default>
	class External
	{

	public:
		External(MemoryBlock extMemory)
			:extMemoryBlock(extMemory)
		{}

		MemoryBlock getMemory(mem_t offset, size_t size)
		{

			MemoryBlock memBlock;

			if (offset + size > this->extMemoryBlock.size)
				return memBlock;

			memBlock.ptr = extMemoryBlock.ptr + offset;
			memBlock.size = size;

			garbageWriterOp::init(memBlock.ptr, 0, size);
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

		[[nodiscard]] mem_t getOffset(byte_t* ptr)
		{

			auto bInRange{ ptr != nullptr };
			bInRange &= (ptr >= this->extMemoryBlock.ptr);
			bInRange &= ((this->extMemoryBlock.ptr + this->extMemoryBlock.size) - ptr > 0);
			if (!bInRange)
				return invalidMem_t;

			return static_cast<mem_t>(ptr - this->extMemoryBlock.ptr);

		}

	private:
		MemoryBlock extMemoryBlock;

	};

};