#include <chrono>
#include <limits>
#include <random>

#include <hex.hpp>

int validate(std::unsigned_integral auto value)
{
    const auto a = ToHexUsingSnprintf(value);
    const auto b = ToHexUsingStringstream(value);
    const auto c = ToHexUsingToChars(value);
    const auto d = ToHexUsingStdFormat(value);
    const auto e = ToHexUsingFmtFormat(value);
    const auto f = ToHexUsingNaive(value);
    const auto g = ToHexUsingLUT1(value);
    const auto h = ToHexUsingLUT2(value);
    const auto i = ToHexUsingLUT3(value);
    const auto j = ToHexUsingSWAR(value);

    if (a != b || b != c || c != d || d != e || e != f || f != g || g != h || h != i || i != j) {
        fmt::print("Failed on value {}\n", value);

        fmt::print("{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n", a, b, c, d, e, f, g, h, i, j);
        return -1;
    }

    return 0;
}

template <std::unsigned_integral Uint>
int test()
{
    const auto start = std::chrono::high_resolution_clock::now();

    if constexpr (sizeof(Uint) < 4) {
        for (Uint i { 0 }; i < std::numeric_limits<Uint>::max(); ++i) {
            if (validate(i) == -1)
                return -1;
        }
    } else {
        static constexpr auto kCount = 1u << 20;
        std::random_device rd;
        std::mt19937 gen { rd() };
        std::uniform_int_distribution<Uint> dist;

        for (auto i { 0u }; i < kCount; ++i) {
            if (validate(dist(gen)) == -1)
                return -1;
        }
    }

    const auto end = std::chrono::high_resolution_clock::now();
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    fmt::print("{} ms\n", ms.count());

    return 0;
}

int main()
{
    fmt::print("Test std::uint8_t\t");

    if (test<std::uint8_t>() == -1)
        return -1;

    fmt::print("Test std::uint16_t\t");

    if (test<std::uint16_t>() == -1)
        return -1;

    fmt::print("Test std::uint32_t\t");

    if (test<std::uint32_t>() == -1)
        return -1;

    fmt::print("Test std::uint64_t\t");

    if (test<std::uint64_t>() == -1)
        return -1;

    return 0;
}