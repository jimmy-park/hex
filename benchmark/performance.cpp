#include <random>
#include <vector>

#include <benchmark/benchmark.h>
#include <hex.hpp>

inline static const auto vec = []() {
    std::vector<std::uint64_t> ret(1 << 10, 0);

    std::random_device rd;
    std::mt19937 gen { rd() };
    std::uniform_int_distribution<std::uint64_t> dist;

    for (auto& i : ret) {
        i = dist(gen);
    }

    return ret;
}();

static void BM_snprintf(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto i : vec)
            benchmark::DoNotOptimize(ToHexUsingSnprintf(i));
    }
}
BENCHMARK(BM_snprintf);

static void BM_stringstream(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto i : vec)
            benchmark::DoNotOptimize(ToHexUsingStringstream(i));
    }
}
BENCHMARK(BM_stringstream);

static void BM_to_chars(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto i : vec)
            benchmark::DoNotOptimize(ToHexUsingToChars(i));
    }
}
BENCHMARK(BM_to_chars);

static void BM_std_format(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto i : vec)
            benchmark::DoNotOptimize(ToHexUsingStdFormat(i));
    }
}
BENCHMARK(BM_std_format);

static void BM_fmt_format(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto i : vec)
            benchmark::DoNotOptimize(ToHexUsingFmtFormat(i));
    }
}
BENCHMARK(BM_fmt_format);

static void BM_naive(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto i : vec)
            benchmark::DoNotOptimize(ToHexUsingNaive(i));
    }
}
BENCHMARK(BM_naive);

static void BM_LUT1(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto i : vec)
            benchmark::DoNotOptimize(ToHexUsingLUT1(i));
    }
}
BENCHMARK(BM_LUT1);

static void BM_LUT2(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto i : vec)
            benchmark::DoNotOptimize(ToHexUsingLUT2(i));
    }
}
BENCHMARK(BM_LUT2);

static void BM_LUT3(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto i : vec)
            benchmark::DoNotOptimize(ToHexUsingLUT3(i));
    }
}
BENCHMARK(BM_LUT3);

static void BM_SWAR(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto i : vec)
            benchmark::DoNotOptimize(ToHexUsingSWAR(i));
    }
}
BENCHMARK(BM_SWAR);

BENCHMARK_MAIN();