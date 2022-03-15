export module SourceExt;

export import MemoryBlock;
export import MemoryUtils;
export import GarbageWriter;

import hfogCore;

export namespace hfog::Sources
{


	template <GarbageWriter::CtGarbageWriter<byte_t> garbageWriterOp = GarbageWriter::Default>
	class External
	{

	public:
		External() = default;
		External(const External&) = delete;
		External& operator=(const External&) = delete;

		External(External&&) = default;
		External& operator=(External&&) = default;

		External(MemoryBlock extMemory) noexcept
			:extMemoryBlock(extMemory)
		{}

		[[nodiscard]] constexpr MemoryBlock getMemory(mem_t offset, mem_t size) noexcept
		{

			MemoryBlock memBlock;

//			if constexpr (bX64)
//			{
//				const auto bInRange{ offset + size <= this->extMemoryBlock.size };
//
//				byte_t* path[] = { nullptr, this->extMemoryBlock.ptr + offset };
//				memBlock.ptr = path[bInRange];
//				memBlock.size = size * bInRange;
//			}
//			else
//			{
				if (offset + size > this->extMemoryBlock.size)
					return memBlock;

				memBlock.ptr = this->extMemoryBlock.ptr + offset;
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
			garbageWriterOp::clear(this->extMemoryBlock.ptr, 0, this->extMemoryBlock.size);
		}

		[[nodiscard]] mem_t getOffset(byte_t* ptr) const noexcept
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