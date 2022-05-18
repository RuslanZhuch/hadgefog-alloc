export module hfog.Algorithms.Islands;

import hfog.MemoryUtils;
import hfog.MemoryBlock;
import hfog.Sources.Common;

import hfog.Protect;

inline constexpr auto invalidEntryId_t{ invalidMem_t };

export namespace hfog::Algorithms
{

	template <Sources::CtSource Source, mem_t alignment, int numOfSegments, int numOfChuncks,
		MemoryUtils::CtAlign alignFunc = MemoryUtils::Align<alignment>>
	class Islands
	{

		static_assert(numOfSegments > 0);
		static_assert(numOfChuncks > 0);

		static constexpr auto maxBlockBytes{ alignment * numOfSegments };

		struct Chunck
		{
			mem_t beginMemoryOffset;
			mem_t freeSpace{ maxBlockBytes };
			mem_t freeSpaceActual{ maxBlockBytes };
			Chunck* nextChunck{ nullptr };
			Chunck* prevChunck{ nullptr };
		};

	public:
		template <typename ... Args>
		Islands(Args ... args) noexcept
			:source(args...)
		{

			mem_t currMemOffset{ 0_B };
			auto currentChunck{ this->chuncks };
			Chunck* prevChunck{ nullptr };
			while(true)
			{
				currentChunck->beginMemoryOffset = currMemOffset;
				currMemOffset += maxBlockBytes;
				currentChunck->prevChunck = prevChunck;
				if (currMemOffset == maxBlockBytes * numOfChuncks)
					break;
				prevChunck = currentChunck;
				auto nextChunck{ currentChunck + 1 };
				currentChunck->nextChunck = nextChunck;
				currentChunck = nextChunck;
			}

			for (size_t id{ 0 }; id < numOfSegments; ++id)
			{
				this->entries[id] = this->chuncks;
			}
			for (size_t id{ 0 }; id < numOfSegments - 1; ++id)
			{
				this->tails[id] = nullptr;
			}
			this->tails[numOfSegments - 1] = &this->chuncks[numOfChuncks - 1];

		}

		Islands(const Islands&) = delete;
		Islands& operator=(const Islands&) = delete;

		Islands(Islands&&) = default;
		Islands& operator=(Islands&&) = default;

		[[nodiscard]] MemoryBlock allocate(mem_t numOfBytes) noexcept
		{
			const auto alignNumOfBytes{ alignFunc::compute(numOfBytes) };

			assertThatLessOrEq(alignNumOfBytes, maxBlockBytes, "(hfog::Algorithms::Islands) Too many bytes to allocate");
			if (alignNumOfBytes > maxBlockBytes)
			{
				return {};
			}

			const auto entryId{ this->computeEntryId(alignNumOfBytes) };
			auto currChunck{ this->entries[entryId] };

			assertThatValidPtr(currChunck, "(hfog::Algorithms::Islands) Failed to allocate new chunck");
			if (currChunck == nullptr)
				return {};

			const auto rootMemOffset{ currChunck->beginMemoryOffset };
			const auto localMemOffset{ maxBlockBytes - currChunck->freeSpace };
			const auto memoryOffset{ rootMemOffset + localMemOffset };

			const auto outMemory{ this->source.getMemory(memoryOffset, alignNumOfBytes) };

			assertThatValidPtr(outMemory.ptr, "(hfog::Algorithms::Islands) Failed to gather memory from memory source");
			if (outMemory.ptr == nullptr)
			{
				return outMemory;
			}

			const auto sourceEntryId{ this->computeEntryId(currChunck->freeSpace) };

			currChunck->freeSpace -= alignNumOfBytes;
			currChunck->freeSpaceActual -= alignNumOfBytes;
			const auto newEntryId{ this->computeEntryId(currChunck->freeSpace) };
			if (newEntryId != sourceEntryId ||
				tails[newEntryId] == nullptr)
			{

				this->entries[sourceEntryId] = currChunck->nextChunck;
				if (this->entries[sourceEntryId] != nullptr)
					this->entries[sourceEntryId]->prevChunck = nullptr;
				else
					this->tails[sourceEntryId] = nullptr;

				currChunck->nextChunck = nullptr;

				if (newEntryId == invalidEntryId_t)
					this->updateEntries();
				else
					this->appendChunck(currChunck, newEntryId);

			}

			return outMemory;

		}

		void deallocate() noexcept
		{

			this->source.releaseAllMemory();

			mem_t currMemOffset{ 0_B };
			auto currentChunck{ this->chuncks };
			Chunck* prevChunck{ nullptr };
			while (true)
			{
				currentChunck->beginMemoryOffset = currMemOffset;
				currentChunck->freeSpace = maxBlockBytes;
				currentChunck->freeSpaceActual = maxBlockBytes;
				currMemOffset += maxBlockBytes;
				currentChunck->prevChunck = prevChunck;
				if (currMemOffset == maxBlockBytes * numOfChuncks)
					break;
				prevChunck = currentChunck;
				auto nextChunck{ currentChunck + 1 };
				currentChunck->nextChunck = nextChunck;
				currentChunck = nextChunck;
			}

			for (size_t id{ 0 }; id < numOfSegments; ++id)
			{
				this->entries[id] = this->chuncks;
			}
			for (size_t id{ 0 }; id < numOfSegments - 1; ++id)
			{
				this->tails[id] = nullptr;
			}
			this->tails[numOfSegments - 1] = &this->chuncks[numOfSegments - 1];

		}

		void deallocate(const MemoryBlock& block) noexcept
		{
			
			const auto memOffset{ this->source.getOffset(block.ptr) };
			if (memOffset == invalidMem_t)
				return;

			this->source.releaseMemory(block);

			const auto chunckId{ memOffset / maxBlockBytes };

			auto currChunck{ &this->chuncks[chunckId] };

			currChunck->freeSpaceActual += block.size;
			if (currChunck->freeSpaceActual == maxBlockBytes)
			{
				currChunck->freeSpace = maxBlockBytes;
				if (auto prevChunck{ currChunck->prevChunck }; prevChunck != nullptr)
				{
					prevChunck->nextChunck = currChunck->nextChunck;
					if (currChunck->nextChunck != nullptr)
					{
						currChunck->nextChunck->prevChunck = prevChunck;
					}
				}
				this->appendChunck(currChunck, numOfSegments - 1);
			}

		}

		[[nodiscard]] bool getIsOwner(byte_t* ptr) const noexcept
		{

			const auto checkMemoryOffset{ this->source.getOffset(ptr) };
			const auto maxOffset{ alignment * numOfSegments * numOfChuncks };
			if (checkMemoryOffset > maxOffset)
				return false;

			const auto chunckId{ checkMemoryOffset / maxBlockBytes };
			const auto chunck{ &this->chuncks[chunckId] };
			const auto relativeMemOffset{ checkMemoryOffset - chunck->beginMemoryOffset };

			const auto occupiedSize{ maxBlockBytes - chunck->freeSpace };
			const auto bOccupied{ relativeMemOffset < occupiedSize };

			return bOccupied;

		}

	private:

		void updateEntries() noexcept
		{
			for (int id{ numOfSegments - 2 }; id >= 0; --id)
			{
				if (this->tails[id] == nullptr)
				{
					this->entries[id] = this->entries[id + 1];
				}
			}
		}

		void appendChunck(Chunck* currChunck, size_t entryId) noexcept
		{
			if (this->tails[entryId] == nullptr)
			{
				this->entries[entryId] = currChunck;
				this->tails[entryId] = currChunck;
			}
			else
			{
				this->tails[entryId]->nextChunck = currChunck;
				currChunck->prevChunck = this->tails[entryId];
				this->tails[entryId] = currChunck;
			}
			this->updateEntries();
		}

		[[nodiscard]] mem_t computeEntryId(mem_t needBytes) const noexcept
		{
			if (needBytes == 0)
				return invalidEntryId_t;
			return (((needBytes - 1) * numOfSegments) / maxBlockBytes);
		}

	private:
		Source source;

		Chunck chuncks[numOfChuncks];
		Chunck* entries[numOfSegments];
		Chunck* tails[numOfSegments];

	};

};