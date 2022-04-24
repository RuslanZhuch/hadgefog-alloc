module;

#include <cassert>
#include <memory>

export module hfog.Algorithms.Unified;

import hfog.MemoryUtils;
import hfog.MemoryBlock;
import hfog.Sources.Common;

inline constexpr auto invalidEntryId_t{ invalidMem_t };


export namespace hfog::Algorithms
{

	template <mem_t totalMemory, mem_t alignment>
	consteval mem_t computeFreelistMemorySize()
	{
		return static_cast<mem_t>(totalMemory / alignment / 2.f + 0.5f) * 32;
	}

	template <Sources::CtSource Source, mem_t alignment, mem_t MAX_BYTES,
		MemoryUtils::CtAlign alignFunc = MemoryUtils::Align<alignment>>
		class Unified
	{

		static_assert(MAX_BYTES > 0);

		struct Chunck
		{
			mem_t memOffset{};
			mem_t numOfBytes{};
			Chunck* next{ nullptr };
			Chunck* prev{ nullptr };
		};

		static constexpr auto CHUNCK_SIZE{ sizeof(Chunck) };

		static constexpr auto FREELIST_MAX_BYTES{ computeFreelistMemorySize<MAX_BYTES, alignment>() };
		static constexpr auto MAX_FRAGMENTS{ FREELIST_MAX_BYTES / CHUNCK_SIZE };

		static_assert(FREELIST_MAX_BYTES <= MAX_BYTES);

	public:
		template <typename ... Args>
		Unified(Args ... args) noexcept
			:source(args...)
		{

			const auto memBlock{ this->source.getMemory(MAX_BYTES, FREELIST_MAX_BYTES) };
			assert(memBlock.ptr != nullptr);
			assert(memBlock.size == FREELIST_MAX_BYTES);

			this->freelistEntry = reinterpret_cast<Chunck*>(memBlock.ptr);

			this->addChunck(0_B, MAX_BYTES);

		}

		Unified(const Unified&) = delete;
		Unified& operator=(const Unified&) = delete;

		Unified(Unified&&) = default;
		Unified& operator=(Unified&&) = default;

		[[nodiscard]] MemoryBlock allocate(mem_t numOfBytes) noexcept
		{

			if (this->freeBlockHead == nullptr)
				return {};

			const auto alignNumOfBytes{ alignFunc::compute(numOfBytes) };

			Chunck* bestChunck{ this->freeBlockHead };
			mem_t bestSize{ bestChunck->numOfBytes };
			auto currChunck{ this->freeBlockHead->next };
			while (currChunck != nullptr)
			{
				const auto currSize{ currChunck->numOfBytes };
				if (currSize >= alignNumOfBytes &&
					(currSize < bestSize) || (bestSize < alignNumOfBytes))
				{
					bestSize = currSize;
					bestChunck = currChunck;
					if (bestSize == alignNumOfBytes)
						break;
				}
				currChunck = currChunck->next;
			}

			if (bestSize < alignNumOfBytes)
				return {};

			const auto memBlock{ this->source.getMemory(bestChunck->memOffset, alignNumOfBytes) };

			if (bestSize == alignNumOfBytes)
			{
				this->removeChunck(bestChunck);
			}
			else
			{
				bestChunck->memOffset += alignNumOfBytes;
				bestChunck->numOfBytes -= alignNumOfBytes;
			}

			return memBlock;

		}

		void deallocate() noexcept
		{

			this->freeBlockHead = nullptr;
			this->freeBlockTail = nullptr;
			this->numOfFreeListChuncks = 0;
			this->addChunck(0_B, MAX_BYTES);

			this->source.releaseAllMemory();
		
		}

		void deallocate(const MemoryBlock& block) noexcept
		{

			this->addFreeListChunck(block);
			this->source.releaseMemory(block);

		}

		[[nodiscard]] bool getIsOwner(byte_t* ptr) const noexcept
		{

			const auto memOffset{ this->source.getOffset(ptr) };
			if (memOffset == invalidMem_t)
				return false;

			auto currChunck{ this->freeBlockHead };
			while (currChunck != nullptr)
			{
				if (currChunck->memOffset <= memOffset &&
					memOffset < currChunck->memOffset + currChunck->numOfBytes)
					return false;
				currChunck = currChunck->next;
			}

			return true;

		}

	private:
		void addFreeListChunck(const MemoryBlock& block)
		{

			auto memOffset = this->source.getOffset(block.ptr);
			auto numOfBytes = block.size;

			Chunck* adjPrev{ nullptr };
			Chunck* adjNext{ nullptr };

			auto currChunck{ this->freeBlockHead };
			while (currChunck != nullptr)
			{

				if (adjPrev == nullptr &&
					currChunck->memOffset + currChunck->numOfBytes == memOffset)
				{
					adjPrev = currChunck;
					if (adjNext != nullptr)
					{
						adjPrev->numOfBytes += adjNext->numOfBytes;
						this->removeChunck(adjNext);
					}
					else
					{
						adjPrev->numOfBytes += numOfBytes;
					}
				}
				if (adjNext == nullptr &&
					currChunck->memOffset == memOffset + numOfBytes)
				{
					adjNext = currChunck;
					if (adjPrev != nullptr)
					{
						adjPrev->numOfBytes += adjNext->numOfBytes;
						this->removeChunck(adjNext);
					}
					else
					{
						adjNext->numOfBytes += numOfBytes;
						adjNext->memOffset = memOffset;
					}
				}
				if ((adjNext != nullptr) && (adjPrev != nullptr))
					break;
				currChunck = currChunck->next;
			}

			if ((adjNext == nullptr) && (adjPrev == nullptr))
			{
				this->addChunck(memOffset, numOfBytes);
			}

		}

		void removeChunck(Chunck* chunckToRemove)
		{

			chunckToRemove->memOffset = this->freeBlockTail->memOffset;
			chunckToRemove->numOfBytes = this->freeBlockTail->numOfBytes;

			this->freeBlockTail = this->freeBlockTail->prev;
			if (this->freeBlockTail != nullptr)
			{
				this->freeBlockTail->next = nullptr;
			}
			else
			{
				this->freeBlockHead = nullptr;
			}

			--this->numOfFreeListChuncks;

		}

		void addChunck(mem_t memOffset, mem_t numOfBytes)
		{

			assert(this->numOfFreeListChuncks < MAX_FRAGMENTS);

			if (this->freeBlockHead == nullptr)
			{
				this->freeBlockHead = this->freelistEntry;
				this->freeBlockTail = this->freeBlockHead;
				this->freeBlockTail->prev = nullptr;
			}
			else
			{
				auto prevTail{ this->freeBlockTail };
				this->freeBlockTail = this->freelistEntry + this->numOfFreeListChuncks;
				prevTail->next = this->freeBlockTail;
				this->freeBlockTail->prev = prevTail;
				this->freeBlockTail->prev = this->freeBlockTail->prev;
			}
			this->freeBlockTail->next = nullptr;

			this->freeBlockTail->memOffset = memOffset;
			this->freeBlockTail->numOfBytes = numOfBytes;

			++this->numOfFreeListChuncks;

		}

	private:
		Source source;

		Chunck* freelistEntry{ nullptr };

		Chunck* freeBlockHead{ nullptr };
		Chunck* freeBlockTail{ nullptr };
		size_t numOfFreeListChuncks{ 0 };
		size_t chuncksCapacity{ MAX_FRAGMENTS };

	};

};