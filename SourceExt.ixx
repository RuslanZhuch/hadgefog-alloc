export module hfog.Sources.Ext;

export import hfog.MemoryBlock;
export import hfog.MemoryUtils;
export import hfog.GarbageWriter;

import hfog.Core;

import hfog.Protect;

export namespace hfog::Sources
{

	template <GarbageWriter::CtGarbageWriter garbageWriterOp = GarbageWriter::Default>
	class External
	{

	public:
		External() = default;
		External(const External&) = delete;
		External& operator=(const External&) = delete;

		External(External&&) = default;
		External& operator=(External&&) = default;

		External(MemoryBlock extMemory) noexcept
			//:extMemoryBlock(extMemory)
			:begin(extMemory.ptr),
			end(extMemory.ptr + extMemory.size)
		{}

		[[nodiscard]] constexpr MemoryBlock getMemory(mem_t offset, mem_t size) noexcept
		{

			MemoryBlock memBlock;

			const auto bCanProvide{ this->begin + offset + size < this->end };
			if (!bCanProvide)
				return memBlock;
				
			//byte_t* outputMem[2]{ nullptr, this->extMemoryBlock.ptr + offset };
			memBlock.ptr = this->begin + offset; //outputMem[static_cast<size_t>(bCanProvide)];
			memBlock.size = size;

//			garbageWriterOp::init(memBlock.ptr, 0, memBlock.size);
//			garbageWriterOp::write(memBlock.ptr, 0, memBlock.size);
			
			return memBlock;

		}

		void releaseMemory(const MemoryBlock& memBlock) noexcept
		{
//			if (!hfog::assertThatValidPtr(memBlock.ptr, "(hfog::Sources::External::releaseMemory) memoryBlock pointer is invalid") ||
//				!hfog::assertThatLessOrEq(memBlock.ptr - this->extMemoryBlock.ptr + memBlock.size, this->extMemoryBlock.size,
//					"(hfog::Sources::External::releaseMemory) memoryBlock block is off limits"))
//				return;
//			garbageWriterOp::clear(memBlock.ptr, 0, memBlock.size);
		}

		void releaseAllMemory() noexcept
		{
//			garbageWriterOp::clear(this->extMemoryBlock.ptr, 0, this->extMemoryBlock.size);
		}

		[[nodiscard]] mem_t getOffset(byte_t* ptr) const noexcept
		{

//			if (!hfog::assertThatValidPtr(ptr, "(hfog::Sources::External::getOffset) pointer is invalid") ||
//				!hfog::assertThatLessOrEq(static_cast<mem_t>(ptr - this->extMemoryBlock.ptr), this->extMemoryBlock.size,
//					"(hfog::Sources::External::getOffset) pointer is off limits"))
//				return invalidMem_t;

			auto bInRange{ ptr != nullptr };
			bInRange &= (ptr >= this->begin);
			bInRange &= (this->end - ptr > 0);
			if (!bInRange)
				return invalidMem_t;

			return static_cast<mem_t>(ptr - this->begin);

		}

		[[nodiscard]] mem_t getMemorySize() const noexcept
		{
			return this->end - this->begin;
		}

	private:
		//MemoryBlock extMemoryBlock;
		byte_t* begin{ nullptr };
		byte_t* end{ nullptr };

	};

};