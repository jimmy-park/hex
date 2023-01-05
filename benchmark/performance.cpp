#include <random>
#include <vector>

#include <benchmark/benchmark.h>
#include <hex.hpp>

#define ENABLE_BENCH(func)                                     \
    static void BM_##func(benchmark::State& state)             \
    {                                                          \
        static constexpr auto kCount { 1u << 10 };             \
        std::random_device rd;                                 \
        std::mt19937_64 gen { rd() };                          \
                                                               \
        switch (state.range(0)) {                              \
        case 8: {                                              \
            std::uniform_int_distribution<std::uint16_t> dist; \
            std::vector<std::uint8_t> buffer(kCount, 0);       \
                                                               \
            for (auto& i : buffer)                             \
                i = static_cast<std::uint8_t>(dist(gen));      \
                                                               \
            for (auto _ : state) {                             \
                for (auto i : buffer) {                        \
                    benchmark::DoNotOptimize(hex::func(i));    \
                }                                              \
            }                                                  \
        } break;                                               \
        case 16: {                                             \
            std::uniform_int_distribution<std::uint16_t> dist; \
            std::vector<std::uint16_t> buffer(kCount, 0);      \
                                                               \
            for (auto& i : buffer)                             \
                i = dist(gen);                                 \
                                                               \
            for (auto _ : state) {                             \
                for (auto i : buffer) {                        \
                    benchmark::DoNotOptimize(hex::func(i));    \
                }                                              \
            }                                                  \
        } break;                                               \
        case 32: {                                             \
            std::uniform_int_distribution<std::uint32_t> dist; \
            std::vector<std::uint32_t> buffer(kCount, 0);      \
                                                               \
            for (auto& i : buffer)                             \
                i = dist(gen);                                 \
                                                               \
            for (auto _ : state) {                             \
                for (auto i : buffer) {                        \
                    benchmark::DoNotOptimize(hex::func(i));    \
                }                                              \
            }                                                  \
        } break;                                               \
        case 64: {                                             \
            std::uniform_int_distribution<std::uint64_t> dist; \
            std::vector<std::uint64_t> buffer(kCount, 0);      \
                                                               \
            for (auto& i : buffer)                             \
                i = dist(gen);                                 \
                                                               \
            for (auto _ : state) {                             \
                for (auto i : buffer) {                        \
                    benchmark::DoNotOptimize(hex::func(i));    \
                }                                              \
            }                                                  \
        } break;                                               \
        default:                                               \
            break;                                             \
        }                                                      \
    }                                                          \
    BENCHMARK(BM_##func)->RangeMultiplier(2)->Range(8, 64)

ENABLE_BENCH(snprintf);
ENABLE_BENCH(stringstream);
ENABLE_BENCH(to_chars);
ENABLE_BENCH(format);
ENABLE_BENCH(fmt_format);
ENABLE_BENCH(naive);
ENABLE_BENCH(LUT1);
ENABLE_BENCH(LUT2);
ENABLE_BENCH(LUT3);
ENABLE_BENCH(SWAR);

ENABLE_BENCH(snprintf_LZ);
ENABLE_BENCH(stringstream_LZ);
ENABLE_BENCH(to_chars_LZ);
ENABLE_BENCH(format_LZ);
ENABLE_BENCH(fmt_format_LZ);
ENABLE_BENCH(naive_LZ);
ENABLE_BENCH(LUT1_LZ);
ENABLE_BENCH(LUT2_LZ);
ENABLE_BENCH(LUT3_LZ);
ENABLE_BENCH(SWAR_LZ);

BENCHMARK_MAIN();