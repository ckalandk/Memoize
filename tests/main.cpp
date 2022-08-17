#include <iostream>
#include "../src/memoize.hpp"
#include <benchmark/benchmark.h>

size_t fibo(size_t n)
{
    if(n <= 1) {
        return n;
    }
    return fibo(n-1) + fibo(n-2);
}

static void Fibo(benchmark::State& state)
{
    for(auto _ : state) {
        benchmark::DoNotOptimize(fibo(state.range(0)));
    }
}

static void FiboMemoize(benchmark::State& state)
{
    auto memfibo = mem::memoize(fibo);
    for(auto _ : state) {
        benchmark::DoNotOptimize(memfibo(state.range(0)));
    }
}

BENCHMARK(Fibo)->Arg(8)->Arg(16)->Arg(24)->Arg(32)->Arg(40);
BENCHMARK(FiboMemoize)->Arg(8)->Arg(16)->Arg(24)->Arg(32)->Arg(40);
BENCHMARK_MAIN();
