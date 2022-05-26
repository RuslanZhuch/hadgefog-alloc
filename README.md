# hadgefog-alloc
A composable C++20 allocators library for game engines and more!
## Features
- composable - combine multiple algorithms to create an allocator suitable in your case
- template based - we try to make it compile-time friendly as much as we can
- two types of memory sources: __local__ and __external__
- five types of allocation algorithms: __Null__, __Stack__, __Pool__, __Islands__, __Unified__
- two structured allocator algorithms: __Fallback__ and __Segregator__
- flexible errors detecting system (WIP)

## Getting started

### Requirements
You need Visual Studio 2022 to build this project. The library requires c++20 to build and uses modules experimental support. 
Unfortunately, Intellisense doesn't work with modules and concepts quite well, so you would get a lot of errors in editor. 

### Installation
Just load and build the solution in Visual Studio. 
We prefer to use x64 as our target platform, but you can use x86 as well.

## Usage
### Quick start

```c++
import hfog.Alloc;

//Include this header once to provide default error catching systems (asserts)
#include <hfogProtectionHook.h>

#include <iostream>

//For _B, _kB, _MB, _GB and _TB
using namespace hfog::MemoryUtils::Literals;

int main()
{

    //Create a stack allocator with local source
    constexpr auto ALIGNMENT{ 8_B };
    constexpr auto MAX_BYTE{ 64_B };
    hfog::Alloc::Stack<ALIGNMENT, MAX_BYTE> stackAlloc;

    //Allocate 32 bytes of memory
    hfog::MemoryBlock mem1{ stackAlloc.allocate(32_B) };

    //Allocator returns a struct with a pointer to allocated data and size of this data
    std::cout << "Allocated 1 ptr: " << std::hex << mem_t(mem1.ptr) << "\n";
    std::cout << "Allocated 1 size: " << std::dec << mem1.size << "\n";

    //Allocate another 16 bytes (12 bytes would be alignment to 8 bytes)
    hfog::MemoryBlock mem2{ stackAlloc.allocate(12_B) };
    std::cout << "Allocated 2 ptr: " << std::hex << mem_t(mem2.ptr) << "\n";
    std::cout << "Allocated 2 size: " << std::dec << mem2.size << "\n";

    //If you try to allocate more bytes then allocator can provide, it will return MemoryBlock with nullptr and size 0
    hfog::MemoryBlock memInvalid{ stackAlloc.allocate(32_B) };
    std::cout << "Allocated invalid ptr: " << std::hex << mem_t(memInvalid.ptr) << "\n";
    std::cout << "Allocated invalid size: " << std::dec << memInvalid.size << "\n";

    return 0;

}
```
_Result_:
```
Allocated 1 ptr: 43ff9cf580
Allocated 1 size: 32
Allocated 2 ptr: 43ff9cf5a0
Allocated 2 size: 16
Allocated invalid ptr: 0
Allocated invalid size: 0
```

Module __hfog.Alloc__ contains prefabricated allocators and exports all vital modules you need to start using hadgefog-alloc.

First we create a stack allocator with local memory source. 
```
hfog::Alloc::Stack<ALIGNMENT, MAX_BYTE> stackAlloc;
```
Under the hoot it creates an array of type __byte_t__ and size __MAX_BYTE__. So its data will be automatically destroyed after your program leaves the scope where the allocator was created. 
Method __allocate__(mem_t numOfBytes) allotates __n__ bytes. Where __n__ is equal to __numOfBytes__ alignment to __ALIGNMENT__ parameter provided to the allocator's constructor.
```
hfog::MemoryBlock mem1{ stackAlloc.allocate(32_B) };
```
To return an allocated memory, all allocators use __MemoryBlock__ struct. It contains a pointer to data __byte_t* ptr__ and a size __mem_t size__.
If an allocator doesn't have enough memory to allocate, it returns a __MemoryBlock__ structure with __ptr__ equals to __nullptr__.

Let's see another example with __stackExt__ allocator:
```c++
byte_t* externalBuffer = new byte_t[64_B];

hfog::MemoryBlock extBlock(externalBuffer, 64_B);

//Create stack allocator with external source
constexpr auto ALIGNMENT{ 8_B };
hfog::Alloc::StackExt<ALIGNMENT> stackAlloc(extBlock);
```
Every allocator with an external source receives a __MemoryBlock__ with a user-provided buffer. You don't have to specify buffer size in a second template parameter. Therefore, you can use a dynamically allocated buffer as a source for your allocator.

Deallocation is common for all allocators. There are two methods:
```c++
void deallocate(const MemoryBlock&)
void deallocate()
```
The first one takes a pointer and a size of memory you want to deallocate. 
The second method deallocates all memory that belongs to an allocator.
```c++
int main()
{
    //Create a stack allocator with local source
    constexpr auto ALIGNMENT{ 8_B };
    constexpr auto MAX_BYTE{ 64_B };
    hfog::Alloc::Stack<ALIGNMENT, MAX_BYTE> stackAlloc;

    hfog::MemoryBlock mem1{ stackAlloc.allocate(32_B) };
    hfog::MemoryBlock mem2{ stackAlloc.allocate(16_B) };
    hfog::MemoryBlock mem3{ stackAlloc.allocate(8_B) };
    hfog::MemoryBlock mem4{ stackAlloc.allocate(8_B) };
    
    stackAlloc.deallocate(mem4);
    stackAlloc.deallocate(mem3);
    stackAlloc.deallocate();

    return 0;
}
```
With __Stack__ allocator you can only deallocate from the top. If, for example, you deallocate __mem3__ befor __mem4__, nothing will happen. 

### Prefabricated allocators

All prefabricated allocators can be found under the hfog::Alloc namespace:
- Null
- Stack
- StackExt
- Pool
- PoolExt
- Islands
- IslandsExt
- Unified
- UnifiedExt

__*Ext__ postfix means that an allocator is using an external buffer as a source. 
A __Null__ allocator doesn't have a __NullExt__ analog.

##### Null prefabricated allocator
The __Null__ allocator is the most simple algorithm in the __hadgefog-alloc__ library. It doesn't allocate or deallocate any memory and always returns nullptr. What's the point? The point is to use a __Null__ allocator in more complex structured algorithms. For example: you want to create an allocator that can allocate only chunks of data not greater than 2MB. You can create a __Segregator__ allocator, which uses some allocator if desired memory size is less or equal than 2MB and a __Null__ otherwise. Therefore, if you ask the __Segregator__ allocator to allocate you 2.5MB, it will return nullptr.

You don't really have to directly use a __Null__ allocator. Just pass its type to a structure that accepts allocators as a template parameter.

	using workingAllocator_t = hfog::Alloc::Stack<128_B, 64_MB>;
	using terminatingAllocator_t = hfog::Alloc::Null;

	constexpr auto THRESHOLD{ 2_MB };

	hfog::Structures::Segregator<workingAllocator_t, terminatingAllocator_t, THRESHOLD> segregator;

##### Stack & StackExt prefabricated allocators

__Stack__ allocator works basically as a regular stack. It allocates a new memory block right one after another, simply incrementing its pointer until it is out of memory. Deallocation is the same but in reverse: it checks if the provided memory block fits with its pointer and decrements it on the size of this memory. If the provided memory block doesn't fit, it does nothing. 
Complexity of allocation and deallocation is O(1).
So, it is a very simple, fast, lightweight and continuous allocator.

##### Pool & PoolExt prefabricated allocators

__Pool__ allocator provides chunks of memory with equal size. It can deallocate memory in random order with O(1) complexity. Allocation complexity is also O(1). Under the hood it uses a free-list to quickly find the first empty chunk of memory to allocate, so the memory layout is not continuous but the probability of fragmentation is minimal.

    int main()
    {
    
    	//Create a pool allocator with local source
    	constexpr auto ALIGNMENT{ 8_B };
    	constexpr auto NUM_OF_CHUNKS{ 4 };
    	hfog::Alloc::Pool<ALIGNMENT, NUM_OF_CHUNKS> pool;
    
    	const auto mem1{ pool.allocate(8_B) };
    	std::cout << "Allocated 1 ptr: " << std::hex << mem_t(mem1.ptr) << "\n";
    	std::cout << "Allocated 1 size: " << std::dec << mem1.size << "\n";
    
    	const auto mem2{ pool.allocate(4_B) };
    	std::cout << "Allocated 2 ptr: " << std::hex << mem_t(mem2.ptr) << "\n";
    	std::cout << "Allocated 2 size: " << std::dec << mem2.size << "\n";
    
    	const auto mem3{ pool.allocate(12_B) };
    	std::cout << "Allocated 3 ptr: " << std::hex << mem_t(mem3.ptr) << "\n";
    	std::cout << "Allocated 3 size: " << std::dec << mem3.size << "\n";
    
    	pool.deallocate(mem1);
    	pool.deallocate(mem2);
    
    }

_Result_:

    Allocated 1 ptr: 90d6cff9d0
    Allocated 1 size: 8
    Allocated 2 ptr: 90d6cff9d8
    Allocated 2 size: 8
    Allocated 3 ptr: 0
    Allocated 3 size: 0

__Pool__ allocator will return __nullptr__, if you provide more bytes to allocate then its chunk size. __ALAGNMENT__ parameter is used as a chunk size. So, the __Pool__ allocator will round up any number of bytes you ask it to allocate.

##### Islands & IslandsExt prefabricated allocators
The idea behind an __Islands__ allocator is to provide memory blocks with different sizes and minimum fragmentation, while maintaining O(1) complexity of allocation and deallocation.

	constexpr auto ALIGNMENT{ 8_B };
	constexpr auto BUFFER_SIZE{ 512_B };
	constexpr auto NUM_OF_CHUNKS{ 4 };
	hfog::Alloc::Islands<ALIGNMENT, BUFFER_SIZE, NUM_OF_CHUNKS> islands;

Usage of __Islands__ allocator is very similar to __Pool__ allocator. You can deallocate memory in any order, but you can  allocate memory with a different size. But you cannot allocate a chunk with size greater than __BUFFER_SIZE__ / __NUM_OF_CHUNKS__! __Island__ allocator tries to keep memory local. Each chunk works as a stack: it continuously allocates a new memory block if a chunk has enough free memory. If there is not non-empty chunk where the allocator can fit new memory block, it will use a new one. Therefore, memory layout is partially continuous, and fragmentation depends on the allocator's parameters and required memory blocks. 
Another important property is that a chunk can only deallocate its memory all at once. If you deallocate three of the four memory blocks it will appear as all these deallocated blocks are still exit. So, the chunk will be deallocated, only if __all__ its memory has been freed.
Despite the fact, allocation and deallocation complexity is O(1), allocator's size depends on __Alignment__ and __BufferSize__. Internally __Islands__ allocator uses a data structure, that binds every chunk to amount of free memory it has. The amount of entries that bind chunks is equal to __BUFFER_SIZE__ / __ALIGNMENT__. Memory fragmentation, on the other hand, highly depends on __NUM_OF_CHUNKS__ parameter: more chunks - less fragmentation. So, to achieve optimal size and performance, you have to carefully tune all three input parameters to your situation. 

##### Unified & UnifiedExt prefabricated allocators
__Unified__ allocator uses the most unconstrained algorithm is this library. You must specify alignment and maximum buffer size and after that you can allocate a memory block of any size (until the allocator is out of memory). Complexity of allocation and deallocation linearly depends on a number of allocations and memory fragmentation. It is better to allocate memory blocks with the same size, as an algorithm always tries to find the best chunk to fit new memory data. This way memory fragmentation tends to be minimal.

	constexpr auto ALIGNMENT{ 8_B };
	constexpr auto BUFFER_SIZE{ 512_B };
	hfog::Alloc::Unified<ALIGNMENT, BUFFER_SIZE> unified;

You can deallocate a memory block in any order, and the memory footprint is usually less than in __Islands__ allocator. Therefore, in some cases __Unified__ allocator can be more optimal than __Islands__ allocator.

#### What about *Ext?
Prefabricated allocators with external memory source works identically to allocators with local source. The only difference is creation. As in the case with __stackExt__ allocator, you have to provide a __MemoryBlock__ structure to an allocator's constructor. In some cases you have to explicitly specify maximum buffer size in a template parameter. If your allocator's buffer size doesn't match an external buffer's size, the allocator uses the minimum of both.

### Alignment
Every allocator is provided with an Alignment class.  

If no user-defined Alignment class was provided, an allocator will use the default one.

	template<typename T>
	concept CtAlign = requires(T)
	{
		{T::compute(std::declval<mem_t>())} -> std::convertible_to<mem_t>;
	};

	template <mem_t alignment>
	class Align
	{
	public:
		static_assert(alignment > 0, "Alignment must be > 0");
		[[nodiscard]] static constexpr mem_t compute(mem_t in) noexcept
		{
			return ((in + alignment - 1) / alignment) * alignment;
		}
	};

User-defined Alignment class has to satisfy the __CtAlign__ concept. 
All prefabricated classes use the default __Align__ class described above.

### GarbageWriter
__GarbageWriter__ is an instrument to debug situations where a source initializes, acquires and releases its memory. For example, you would like to write __OxFA__ to all bytes that were acquired, and __OxAF__ - to all bytes that were released. To solve this problem __hadgefog-alloc__ provides prefabricated __GarbageWriter__ '__hfog::GarbageWriter::ByteWriter__'. 
If a user doesn't explicitly specify a __GarbageWriter__, the default one is used. You can provide a __GarbageWriter__ to any prefabricated allocator or directly to a memory source.

    void printMemory(const auto& mem)
    {
        for (mem_t id{ 0 }; id < mem.size; ++id)
            std::cout << mem_t(*(mem.ptr + id));
        std::cout << '\n';
    }
    
    int main()
    {
    
        byte_t* externalBuffer = new byte_t[64_B];
        hfog::MemoryBlock extBlock(externalBuffer, 64_B);
        
        constexpr auto ALIGNMENT{ 8_B };
        hfog::Alloc::StackExt<ALIGNMENT, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>> stackAlloc(extBlock);
    
        hfog::MemoryBlock mem{ stackAlloc.allocate(32_B) };
        std::cout << "Memory allocated\n" << std::hex;
        printMemory(mem);
    
        stackAlloc.deallocate();
        std::cout << "Memory deallocated\n" << std::hex;
        printMemory(mem);
    
        return 0;
    
    }
__Result__

    Memory allocated
    fafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafa
    Memory deallocated
    afafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafaf

User-defined __GarbageWrited__ must satisfy the __CtGarbageWriter__ concept.

	template<typename T, typename TCallBuffer>
	concept CtGarbageWriter = requires(T, TCallBuffer)
	{
		{T::init(std::declval<TCallBuffer*>(), std::declval<mem_t>(), std::declval<mem_t>())} 
		-> std::convertible_to<void>;
		{T::write(std::declval<TCallBuffer*>(), std::declval<mem_t>(), std::declval<mem_t>())} 
		-> std::convertible_to<void>;
		{T::clear(std::declval<TCallBuffer*>(), std::declval<mem_t>(), std::declval<mem_t>())} 
		-> std::convertible_to<void>;
	};

### Trampa-key protection (WIP)
By default, most of the errors are captured via standard asserts. But __hadgefog-alloc__ allows you to provide your own error capture system. __hadgefog-alloc__ use the __Trampa-key__ api. For the library to compile, you have to include a __hfogProtectionHook.h__ header file in your project. 

If you want to implement, for example, error catching system via exceptions, implement __HfogHook::onAssertTriggered(const trampa::Assert::Cause& assertCause, std::string_view msg)__ in your code. Do __not__ include __hfogProtectionHook.h__ header file in your project!
This way you can provide custom logging, exception, classic get-last-error system and more.

    #include <string_view>
    import trampa.key;
    import hfog.Protect;
    
    [[nodiscard]] bool HfogHook::onAssertTriggered(const trampa::Assert::Cause& assertCause, std::string_view msg)
    {
        //Throw exception
    	return false;
    }
__trampa::Assert::Cause__ is a variant type that contains information about triggered assert. __msg__ provides a commentary about an assert. __onAssertTriggered__ usually must return __false__. But you can override a value that triggered assertion will return.

### Low-level library structure 
__TODO:__

### Pre-release version
This is a pre-release version of __hadgefog-alloc__ library. It is currently in heavy development with the __fovere-containers__ libriry for the __Universal-Augmented-Core game Engine__.  
Some of its features are field tested, some of them - not. But the first release will be very soon.
  
