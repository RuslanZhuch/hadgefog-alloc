// benchmark.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include <iostream>

#include "google benchmark/headers/benchmark.h"

import hfog.MemoryUtils;
import hfog.MemoryBlock;
import hfog.Source.Heap;
import hfog.Source.Ext;
import hfog.Source.Stack;

using namespace hfog::MemoryUtils::Literals;

template <typename Source>
void sources_T_ReleaseAll(Source& s)
{
    mem_t offset{ 0_B };
    for (size_t numOfItersLeft = 100; numOfItersLeft > 0; --numOfItersLeft)
    {
        const auto memBlock{ s.getMemory(offset, 128_B) };
        offset += 128_B;
        benchmark::DoNotOptimize((*memBlock.ptr));
    }
    s.releaseAllMemory();
}

void measure(auto& state, auto cb)
{
//    int microseconds = state.range(0);
//    std::chrono::duration<double, std::nano> sleep_duration{
//      static_cast<double>(microseconds)
//    };

    for (auto _ : state)
    {

//        auto start = std::chrono::high_resolution_clock::now();

        cb();

//        auto end = std::chrono::high_resolution_clock::now();
//        auto elapsed_seconds =
//            std::chrono::duration_cast<std::chrono::duration<double>>(
//                end - start);

//        state.SetIterationTime(elapsed_seconds.count());
    }

}

static void BM_Sources_Heap_ReleaseAll(benchmark::State& state) {
    hfog::Sources::Heap heap;
    measure(state, [&]()
    {
        sources_T_ReleaseAll(heap);
    });
}

static void BM_Sources_External_ReleaseAll(benchmark::State& state) {
    byte_t* extBuffer = new byte_t[128_B * 100];
    hfog::MemoryBlock extMemBlock;
    extMemBlock.ptr = extBuffer;
    extMemBlock.size = 128_B * 100;
    hfog::Sources::External external(extMemBlock);
    measure(state, [&]()
    {
        sources_T_ReleaseAll(external);
    });
}

static void BM_Sources_Stack_ReleaseAll(benchmark::State& state) {

    hfog::Sources::Stack<128_B * 100> stack;
    measure(state, [&]()
    {
        sources_T_ReleaseAll(stack);
    });
}

template <typename Source, mem_t allocSize, mem_t useElements>
void sources_T_LinearResize(Source& s, auto& blocks)
{
    size_t id{0};
    while (id <  useElements)
    {
        const auto memBlock{ s.getMemory(allocSize * id, allocSize) };
        blocks[id++] = memBlock;
        //benchmark::DoNotOptimize((*memBlock.ptr));
    }
    while (id > useElements / 2)
    {
        s.releaseMemory(blocks[id--]);
    }
    while (id < useElements)
    {
        const auto memBlock{ s.getMemory(allocSize * id, allocSize) };
        blocks[id++] = memBlock;
        //benchmark::DoNotOptimize((*memBlock.ptr));
    }
    while (true)
    {
        s.releaseMemory(blocks[id]);
        if (id-- == 0)
            break;
    }
}

template <mem_t allocSize, mem_t numOfElements, mem_t useElements>
static void BM_Sources_Heap_LinearResize(benchmark::State& state) {
    static_assert(numOfElements >= useElements);

    hfog::Sources::Heap src;
    hfog::MemoryBlock blocks[numOfElements];

    measure(state, [&]()
    {
        sources_T_LinearResize<decltype(src), allocSize, useElements>(src, blocks);
    });
}

template <mem_t allocSize, mem_t numOfElements, mem_t useElements>
static void BM_Sources_External_LinearResize(benchmark::State& state) {
    static_assert(numOfElements >= useElements);

    byte_t* extBuffer = new byte_t[allocSize * numOfElements];
    hfog::MemoryBlock extMemBlock;
    extMemBlock.ptr = extBuffer;
    extMemBlock.size = allocSize * numOfElements;
    hfog::Sources::External src(extMemBlock);

    hfog::MemoryBlock blocks[numOfElements];
    measure(state, [&]()
    {
        sources_T_LinearResize<decltype(src), allocSize, useElements>(src, blocks);
    });
}

template <mem_t allocSize, mem_t numOfElements, mem_t useElements>
static void BM_Sources_Stack_LinearResize(benchmark::State& state) {
    static_assert(numOfElements >= useElements);

    hfog::Sources::Stack<allocSize * numOfElements> src;

    hfog::MemoryBlock blocks[numOfElements];
    measure(state, [&]()
    {
        sources_T_LinearResize<decltype(src), allocSize, useElements>(src, blocks);
    });
}

import AlgLinear;

template <mem_t allocSize, int numOfAllocs>
void alg_T_Linear(auto& alg)
{
    int numOfAllocsLeft{ numOfAllocs };
    while (numOfAllocsLeft-- > 0)
    {
        const auto memBlock{ alg.allocate(allocSize) };
        benchmark::DoNotOptimize(memBlock);
    }
    alg.deallocate({});
}

template <mem_t allocSize, int numOfAllocs, size_t buffLen>
static void BM_Alg_Linear_Heap(benchmark::State& state) {
    
    hfog::Algorithms::Linear<hfog::Sources::Heap<hfog::GarbageWriter::Default>, 16_B> alg;

    measure(state, [&]()
    {
        alg_T_Linear<allocSize, numOfAllocs>(alg);
    });
}

template <mem_t allocSize, int numOfAllocs, size_t buffLen>
static void BM_Alg_Linear_Ext(benchmark::State& state) {

    byte_t* extBuffer = new byte_t[allocSize * buffLen];
    hfog::MemoryBlock extMemBlock;
    extMemBlock.ptr = extBuffer;
    extMemBlock.size = sizeof(extBuffer);

    hfog::Algorithms::Linear<hfog::Sources::External<hfog::GarbageWriter::Default>, allocSize> alg(extMemBlock);
    
    measure(state, [&]()
    {
        alg_T_Linear<allocSize, numOfAllocs>(alg);
    });
}

template <mem_t allocSize, int numOfAllocs, size_t buffLen>
static void BM_Alg_Linear_Stack(benchmark::State& state) {

    hfog::Algorithms::Linear<hfog::Sources::Stack<allocSize * buffLen, hfog::GarbageWriter::Default>, 16_B> alg;
    
    measure(state, [&]()
    {
        alg_T_Linear<allocSize, numOfAllocs>(alg);
    });
}

import AlgStack;

template <mem_t allocSize, int numOfAllocs>
void alg_T_Stack(auto& alg)
{
    int allocId{ 0 };
    auto beginPtr{ alg.allocate(allocSize) };
    while (allocId < numOfAllocs - 1)
    {
        const auto memBlock{ alg.allocate(allocSize) };
        benchmark::DoNotOptimize(memBlock);
        ++allocId;
    }
    beginPtr.ptr += allocSize * allocId;
    while (allocId-- >= 0)
    {
        alg.deallocate(beginPtr);
        beginPtr.ptr -= allocSize;
    }
}

template <mem_t allocSize, int numOfAllocs, size_t buffLen>
static void BM_Alg_Stack_Heap(benchmark::State& state) {

    hfog::Algorithms::Stack<hfog::Sources::Heap<hfog::GarbageWriter::Default>, 16_B> alg;
    byte_t* blocks[numOfAllocs];

    
    measure(state, [&]()
    {
        int allocId{ 0 };
        auto beginPtr{ alg.allocate(allocSize) };
        while (allocId < numOfAllocs - 1)
        {
            const auto memBlock{ alg.allocate(allocSize) };
            blocks[allocId++] = memBlock.ptr;
        }
        while (allocId != 0)
        {
            alg.deallocate({ blocks[--allocId], allocSize });
        }
    });

}

template <mem_t allocSize, int numOfAllocs, size_t buffLen>
static void BM_Alg_Stack_Ext(benchmark::State& state) {

    byte_t* extBuffer = new byte_t[allocSize * buffLen];
    hfog::MemoryBlock extMemBlock;
    extMemBlock.ptr = extBuffer;
    extMemBlock.size = allocSize * buffLen;

    hfog::Algorithms::Stack<hfog::Sources::External<hfog::GarbageWriter::Default>, 16_B> alg(extMemBlock);

    measure(state, [&]()
    {
        alg_T_Stack<allocSize, numOfAllocs>(alg);
    });
}

template <mem_t allocSize, int numOfAllocs, size_t buffLen>
static void BM_Alg_Stack_Stack(benchmark::State& state) {

    hfog::Algorithms::Stack<hfog::Sources::Stack<allocSize* buffLen, hfog::GarbageWriter::Default>, 16_B> alg;

    measure(state, [&]()
    {
        alg_T_Stack<allocSize, numOfAllocs>(alg);
    });
}

////Sources
//BENCHMARK(BM_Sources_Heap_ReleaseAll);
//BENCHMARK(BM_Sources_External_ReleaseAll);
//BENCHMARK(BM_Sources_Stack_ReleaseAll);
//
//BENCHMARK(BM_Sources_Heap_LinearResize<128_B, 100, 100>);
//BENCHMARK(BM_Sources_External_LinearResize<128_B, 100, 100>);
//BENCHMARK(BM_Sources_Stack_LinearResize<128_B, 100, 100>);
//
//BENCHMARK(BM_Sources_Heap_LinearResize<16_B, 100, 100>);
//BENCHMARK(BM_Sources_External_LinearResize<16_B, 100, 100>);
//BENCHMARK(BM_Sources_Stack_LinearResize<16_B, 100, 100>);
//
//BENCHMARK(BM_Sources_Heap_LinearResize<16_B, 100, 32>);
//BENCHMARK(BM_Sources_External_LinearResize<16_B, 100, 32>);
//BENCHMARK(BM_Sources_Stack_LinearResize<16_B, 100, 32>);
//
//BENCHMARK(BM_Sources_Heap_LinearResize<16_B, 32, 32>);
//BENCHMARK(BM_Sources_External_LinearResize<16_B, 32, 32>);
//BENCHMARK(BM_Sources_Stack_LinearResize<16_B, 32, 32>);
// 
//
////Linear algorithm
//BENCHMARK(BM_Alg_Linear_Heap<16_B, 32, 32>);
//BENCHMARK(BM_Alg_Linear_Ext<16_B, 32, 32>);
//BENCHMARK(BM_Alg_Linear_Stack<16_B, 32, 32>);
//
//BENCHMARK(BM_Alg_Linear_Heap<16_B, 32, 40 * 1024>);
//BENCHMARK(BM_Alg_Linear_Ext<16_B, 32, 40 * 1024>);
//BENCHMARK(BM_Alg_Linear_Stack<16_B, 32, 40 * 1024>);
//
//BENCHMARK(BM_Alg_Linear_Heap<16_B, 40 * 1024, 40 * 1024>);
//BENCHMARK(BM_Alg_Linear_Ext<16_B, 40 * 1024, 40 * 1024>);
//BENCHMARK(BM_Alg_Linear_Stack<16_B, 40 * 1024, 40 * 1024>);

//Stack algorithm
//BENCHMARK(BM_Alg_Stack_Heap<16_B, 32, 32>);
BENCHMARK(BM_Alg_Stack_Ext<16_B, 32, 32>);
BENCHMARK(BM_Alg_Stack_Stack<16_B, 32, 32>);// ->Iterations(1000000);

//BENCHMARK(BM_Alg_Stack_Heap<16_B, 32, 40 * 512>);
BENCHMARK(BM_Alg_Stack_Ext<16_B, 32, 40 * 512>);
BENCHMARK(BM_Alg_Stack_Stack<16_B, 32, 40 * 512>);// ->Iterations(1000000);

//BENCHMARK(BM_Alg_Stack_Heap<16_B, 40 * 512, 40 * 512>);
BENCHMARK(BM_Alg_Stack_Ext<16_B, 40 * 512, 40 * 512>);
BENCHMARK(BM_Alg_Stack_Stack<16_B, 40 * 512, 40 * 512>);// ->Iterations(1000000);

BENCHMARK_MAIN();
