export module hfog.Sources.Local;

export import hfog.MemoryBlock;
export import hfog.MemoryUtils;
export import hfog.GarbageWriter;

import hfog.Core;

import hfog.Protect;

export namespace hfog::Sources
{
	
	template <mem_t buffSize, GarbageWriter::CtGarbageWriter garbageWriterOp = GarbageWriter::Default>
	class Local
	{

	public:
		Local() = default;
		Local(const Local&) = delete;
		Local& operator=(const Local&) = delete;

		Local(Local&&) = default;
		Local& operator=(Local&&) = default;

		[[nodiscard]] MemoryBlock getMemory(mem_t offset, mem_t size) noexcept
		{

			MemoryBlock memBlock;

			if (offset + size > buffSize)
				return memBlock;
				
			memBlock.ptr = this->memBuffer + offset;
			memBlock.size = size;
			
			garbageWriterOp::init(memBlock.ptr, 0, memBlock.size);
			garbageWriterOp::write(memBlock.ptr, 0, memBlock.size);
			
			return memBlock;

		}

		void releaseMemory(const MemoryBlock& memBlock) noexcept
		{
			if (!hfog::assertThatValidPtr(memBlock.ptr, "(hfog::Sources::Local::releaseMemory) memoryBlock pointer is invalid") ||
				!hfog::assertThatLessOrEq(memBlock.ptr - this->memBuffer + memBlock.size, sizeof(this->memBuffer),
					"(hfog::Sources::Local::releaseMemory) memoryBlock block is off limits"))
				return;

			garbageWriterOp::clear(memBlock.ptr, 0, memBlock.size);
		}

		void releaseAllMemory() noexcept
		{
			garbageWriterOp::clear(this->memBuffer, 0, buffSize);
		}

		[[nodiscard]] mem_t getOffset(byte_t* ptr) const noexcept
		{

			if (!hfog::assertThatValidPtr(ptr, "(hfog::Sources::Local::getOffset) pointer is invalid") ||
				!hfog::assertThatLessOrEq(static_cast<mem_t>(ptr - this->memBuffer), sizeof(this->memBuffer),
					"(hfog::Sources::Local::getOffset) pointer is off limits"))
				return invalidMem_t;

			auto bInRange{ ptr != nullptr };
			bInRange &= (ptr >= this->memBuffer);
			bInRange &= ((this->memBuffer + buffSize) - ptr > 0);
			if (!bInRange)
				return invalidMem_t;

			return static_cast<mem_t>(ptr - this->memBuffer);
		}

		[[nodiscard]] mem_t getMemorySize() const noexcept
		{
			return buffSize;
		}

	private:
		byte_t memBuffer[buffSize];
		
	};

};