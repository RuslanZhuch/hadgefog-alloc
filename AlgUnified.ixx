export module AlgUnified;

import MemoryUtils;
import MemoryBlock;
import SourcesCommon;

static constexpr auto invalidEntryId_t{ invalidMem_t };

export namespace hfog::Algorithms
{

	template <Sources::CtSource Source, mem_t alignment, int numOfSegments, int numOfChuncks,
		MemoryUtils::CtAlign alignFunc = MemoryUtils::Align<alignment>>
	class Unified
	{

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
		Unified(Args ... args) noexcept
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
			this->tails[numOfSegments - 1] = &this->chuncks[numOfSegments - 1];

		}

		[[nodiscard]] MemoryBlock allocate(mem_t numOfBytes) noexcept
		{
			const auto alignNumOfBytes{ alignFunc::compute(numOfBytes) };

			if (alignNumOfBytes > maxBlockBytes)
				return {};

			const auto entryId{ this->computeEntryId(alignNumOfBytes) };
			auto currChunck{ this->entries[entryId] };
			if (currChunck == nullptr)
				return {};

			const auto rootMemOffset{ currChunck->beginMemoryOffset };
			const auto localMemOffset{ maxBlockBytes - currChunck->freeSpace };
			const auto memoryOffset{ rootMemOffset + localMemOffset };

			const auto outMemory{ this->source.getMemory(memoryOffset, alignNumOfBytes) };
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

		void deallocate(const MemoryBlock& block) noexcept
		{
			
			const auto memOffset{ this->source.getOffset(block.ptr) };
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

		[[nodiscard]] bool getIsOwner(byte_t* ptr)
		{
			const auto offset{ this->source.getOffset(ptr) };
			return offset < this->currMemPoint;
		}

	private:

		void updateEntries()
		{
			for (int id{ numOfSegments - 2 }; id >= 0; --id)
			{
				if (this->tails[id] == nullptr)
				{
					this->entries[id] = this->entries[id + 1];
				}
			}
		}

		void appendChunck(Chunck* currChunck, size_t entryId)
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

		[[nodiscard]] mem_t computeEntryId(mem_t needBytes)
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