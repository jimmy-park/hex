#include <chrono>
#include <limits>
#include <random>
#include <string_view>

#include <hex.hpp>

using namespace std::literals;

int Validate(std::unsigned_integral auto value)
{
    auto compare = [](auto&& first, auto&&... last) {
        return ((first == last) && ...);
    };

    auto success = compare(
        ToHexSnprintf(value),
        ToHexStringstream(value),
        ToHexToChars(value),
        ToHexStdFormat(value),
        ToHexFmtFormat(value),
        ToHexNaive(value),
        ToHexLUT1(value),
        ToHexLUT2(value),
        ToHexLUT3(value),
        ToHexSWAR(value));

    if (!success) {
        fmt::print("Failed on value {}\n", value);

        return -1;
    }

    return 0;
}

// fmt::print("{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n", a, b, c, d, e, f, g, h, i, j);

// int Register(auto func)
//{
//     return 1;
// }
//
// auto x = Register(ToHexFmtFormat<unsigned int>);

template <std::unsigned_integral Uint>
int Test()
{
    if constexpr (sizeof(Uint) < 4) {
        for (Uint i { 0 }; i < std::numeric_limits<Uint>::max(); ++i) {
            if (Validate(i) == -1)
                return -1;
        }
    } else {
        static constexpr auto kCount = 1u << 20;
        std::random_device rd;
        std::mt19937 gen { rd() };
        std::uniform_int_distribution<Uint> dist;

        for (auto i { 0u }; i < kCount; ++i) {
            if (Validate(dist(gen)) == -1)
                return -1;
        }
    };

    return 0;
}

int main(int argc, char* argv[])
{
    std::array<bool, 4> enable;
    enable.fill(true);

    if (argc == 2) {
        enable[0] = (argv[1] == "1"sv);
        enable[1] = (argv[1] == "2"sv);
        enable[2] = (argv[1] == "3"sv);
        enable[3] = (argv[1] == "4"sv);
    }

    if (enable[0] && (Test<std::uint8_t>() == -1))
        return -1;

    if (enable[1] && (Test<std::uint16_t>() == -1))
        return -1;

    if (enable[2] && (Test<std::uint32_t>() == -1))
        return -1;

    if (enable[3] && (Test<std::uint64_t>() == -1))
        return -1;

    return 0;
}