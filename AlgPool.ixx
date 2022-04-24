export module hfog.Algorithms.Pool;

import hfog.MemoryUtils;
import hfog.MemoryBlock;
import hfog.Sources.Common;

static constexpr auto invalidEntryId_t{ invalidMem_t };

export namespace hfog::Algorithms
{

	template <Sources::CtSource Source, mem_t alignment, int numOfChuncks, 
		MemoryUtils::CtAlign alignFunc = MemoryUtils::Align<alignment>>
		class Pool
	{
		static_assert(numOfChuncks > 0);

		static constexpr auto maxBlockBytes{ alignment * numOfChuncks };

		struct Chunck {
			bool isEmpty{ true };
			mem_t memOffset;
			Chunck* nextChunck{ nullptr };
		};

	public:
		//Pool() = default;
		Pool(const Pool&) = delete;
		Pool& operator=(const Pool&) = delete;

		Pool(Pool&&) = default;
		Pool& operator=(Pool&&) = default;

		template <typename ... Args>
		Pool(Args ... args) noexcept
			:source(args...)
		{

			mem_t currMemOffset{ 0_B };
			auto currentChunck{ this->chuncks };
			currentChunck->memOffset = currMemOffset;
			firstEmptyChunk = currentChunck;
			for (mem_t i = 1; i < numOfChuncks; ++i) {
				chuncks[i - 1].nextChunck = &chuncks[i];
				chuncks[i].memOffset = currMemOffset + i * alignment;
			}
			lastEmptyChunk = &chuncks[numOfChuncks - 1];

		}

		[[nodiscard]] MemoryBlock allocate(mem_t numOfBytes) noexcept
		{
			const auto alignNumOfBytes{ alignFunc::compute(numOfBytes) };

			if (alignNumOfBytes > alignment)
				return {};

			if (firstEmptyChunk == nullptr) {
				return MemoryBlock{};
			}

			const auto outMemory{ this->source.getMemory(firstEmptyChunk->memOffset, alignNumOfBytes) };
			if (outMemory.ptr == nullptr) {
				return outMemory;
			}
			
			auto currChunck = firstEmptyChunk;
			firstEmptyChunk->isEmpty = false;
			firstEmptyChunk = firstEmptyChunk->nextChunck;
			currChunck->nextChunck = nullptr;

			if (firstEmptyChunk == nullptr) {
				lastEmptyChunk = nullptr;
			}

			return outMemory;

		}

		void deallocate() noexcept
		{

			this->source.releaseAllMemory();
			
			mem_t currMemOffset{ 0_B };
			auto currentChunck{ this->chuncks };
			currentChunck->isEmpty = true;
			currentChunck->memOffset = currMemOffset;
			firstEmptyChunk = currentChunck;
			for (mem_t i = 1; i < numOfChuncks; ++i) {
				chuncks[i - 1].nextChunck = &chuncks[i];
				chuncks[i].memOffset = currMemOffset + i * alignment;
				chuncks[i].isEmpty = true;
				chuncks[i].nextChunck = nullptr;
			}
			lastEmptyChunk = &chuncks[numOfChuncks - 1];

		}

		void deallocate(const MemoryBlock& block) noexcept
		{

			const auto memOffset{ this->source.getOffset(block.ptr) };
			this->source.releaseMemory(block);

			const auto chunckId{ memOffset / alignment };

			if (chunckId >= numOfChuncks) {
				return;
			}

			auto currChunck{ &this->chuncks[chunckId] };

			if (currChunck->isEmpty) {
				return;
			}
			if (lastEmptyChunk != nullptr) {
				lastEmptyChunk->nextChunck = currChunck;
			}
			lastEmptyChunk = currChunck;
			lastEmptyChunk->isEmpty = true;

			if (firstEmptyChunk == nullptr) {
				firstEmptyChunk = lastEmptyChunk;
			}
		}

		[[nodiscard]] bool getIsOwner(byte_t* ptr) const
		{

			const auto checkMemoryOffset{ this->source.getOffset(ptr) };
			const auto maxOffset{ alignment * numOfChuncks };
			if (checkMemoryOffset > maxOffset)
				return false;

			const auto chunckId{ checkMemoryOffset / alignment };

			return !this->chuncks[chunckId].isEmpty;
		}

	private:
		Source source;

		Chunck chuncks[numOfChuncks];
		Chunck* firstEmptyChunk;
		Chunck* lastEmptyChunk;

	};

};