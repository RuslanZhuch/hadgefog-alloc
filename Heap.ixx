module;

#include <algorithm>
#include <optional>

export module Heap;

export namespace hfog::Supports
{

	template <typename T, size_t maxNodes>
	class Heap
	{

		struct Node
		{
			T value;
		};

		void correctBottom()
		{
			if (this->numOfNodes == 0)
				return;
			auto currNodeId{ this->numOfNodes - 1 };

			while (currNodeId != 0)
			{
				const auto bIsRight{ (currNodeId & 1) == 0 };
				const auto offset{ 1 * bIsRight };
				const auto parentId{ (currNodeId - (1 + offset)) / 2 };
				const auto bSmallerThanParent{ this->buffer[parentId].value > this->buffer[currNodeId].value };
				if (bSmallerThanParent)
				{
					std::swap(this->buffer[parentId], this->buffer[currNodeId]);
					currNodeId = parentId;
				}
				else
					return;
			}
		}


		void correctTop()
		{

			if (this->numOfNodes == 0)
				return;

			size_t currNodeId{ 0 };

			while (currNodeId != this->numOfNodes - 1)
			{

				const auto currValue{ this->buffer[currNodeId].value };
				
				const auto findMinChild = [&]()
				{
					T minValue{ currValue };
					size_t minId{ currNodeId };
					if(const auto leftChildId{ 2 * currNodeId + 1 };
						leftChildId < this->numOfNodes && 
						minValue > this->buffer[leftChildId].value)
					{
						minValue = this->buffer[leftChildId].value;
						minId = leftChildId;
					}
					if (const auto rightChildId{ 2 * currNodeId + 2 };
						rightChildId < this->numOfNodes &&
						minValue > this->buffer[rightChildId].value)
					{
						minValue = this->buffer[rightChildId].value;
						minId = rightChildId;
					}
					return minId;
				};

				const auto minId{ findMinChild() };
				if (minId == currNodeId)
					return;
				std::swap(this->buffer[currNodeId], this->buffer[minId]);
				currNodeId = minId;

			}

		}

	public:
		[[nodiscard]] bool push(T val)
		{

			if (this->numOfNodes == maxNodes)
				return false;

			Node newNode;
			newNode.value = val;
			this->buffer[this->numOfNodes++] = newNode;
			this->correctBottom();
			return true;

		}

		[[nodiscard]] std::optional<T> pick()
		{
			if (this->numOfNodes == 0)
				return std::nullopt;
			return this->buffer[0].value;
		}

		void pop()
		{

			if (this->numOfNodes == 0)
				return;
			
			const auto lastIndex{ this->numOfNodes - 1 };
			std::swap(this->buffer[0], this->buffer[lastIndex]);
			this->numOfNodes--;

			this->correctTop();

		}

	private:
		size_t numOfNodes{ 0 };
		Node buffer[maxNodes];

	};

};