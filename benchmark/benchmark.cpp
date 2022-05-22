// benchmark.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include <iostream>

#include "google benchmark/headers/benchmark.h"

import hfog.MemoryUtils;
import hfog.MemoryBlock;
import hfog.Sources.Ext;
import hfog.Sources.Local;

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
    for (auto _ : state)
    {
        cb();
    }

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

    hfog::Sources::Local<128_B * 100> stack;
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
    }
    while (id > useElements / 2)
    {
        s.releaseMemory(blocks[id--]);
    }
    while (id < useElements)
    {
        const auto memBlock{ s.getMemory(allocSize * id, allocSize) };
        blocks[id++] = memBlock;
    }
    while (true)
    {
        s.releaseMemory(blocks[id]);
        if (id-- == 0)
            break;
    }
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

    hfog::Sources::Local<allocSize * numOfElements> src;

    hfog::MemoryBlock blocks[numOfElements];
    measure(state, [&]()
    {
        sources_T_LinearResize<decltype(src), allocSize, useElements>(src, blocks);
    });
}

import hfog.Algorithms.Stack;

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
static void BM_Alg_Stack_Local(benchmark::State& state) {

    hfog::Algorithms::Stack<hfog::Sources::Local<allocSize* buffLen, hfog::GarbageWriter::Default>, 16_B> alg;

    measure(state, [&]()
    {
        alg_T_Stack<allocSize, numOfAllocs>(alg);
    });
    
}

//Stack algorithm
BENCHMARK(BM_Alg_Stack_Ext<16_B, 32, 32>);
BENCHMARK(BM_Alg_Stack_Local<16_B, 32, 32>);

BENCHMARK(BM_Alg_Stack_Ext<16_B, 32, 40 * 512>);
BENCHMARK(BM_Alg_Stack_Local<16_B, 32, 40 * 512>);

BENCHMARK(BM_Alg_Stack_Ext<16_B, 40 * 512, 40 * 512>);
BENCHMARK(BM_Alg_Stack_Local<16_B, 40 * 512, 40 * 512>);

BENCHMARK_MAIN();
