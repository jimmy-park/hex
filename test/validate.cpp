#include <chrono>
#include <limits>
#include <random>
#include <string_view>

#include <hex.hpp>

using namespace std::literals;

bool Validate(std::unsigned_integral auto value)
{
    auto compare = [](auto&& first, auto&&... last) {
        return ((first == last) && ...);
    };

    auto success
        = compare(
              hex::snprintf(value),
              hex::stringstream(value),
              hex::to_chars(value),
              hex::format(value),
              hex::fmt_format(value),
              hex::naive(value),
              hex::LUT1(value),
              hex::LUT2(value),
              hex::LUT3(value),
              hex::SWAR(value))
        && compare(
            hex::snprintf_LZ(value),
            hex::stringstream_LZ(value),
            hex::to_chars_LZ(value),
            hex::format_LZ(value),
            hex::fmt_format_LZ(value),
            hex::naive_LZ(value),
            hex::LUT1_LZ(value),
            hex::LUT2_LZ(value),
            hex::LUT3_LZ(value),
            hex::SWAR_LZ(value));

    if (!success) {
        fmt::print("Failed on value {}\n", value);
    }

    return success;
}

template <std::unsigned_integral Uint>
bool Test()
{
    if constexpr (sizeof(Uint) < 4) {
        Uint i { 0 };

        do {
            if (!Validate(i))
                return false;
        } while (++i != 0);
    } else {
        static constexpr auto kCount = 1u << 20;
        std::random_device rd;
        std::mt19937 gen { rd() };
        std::uniform_int_distribution<Uint> dist;

        for (auto i { 0u }; i < kCount; ++i) {
            if (!Validate(dist(gen)))
                return false;
        }
    };

    return true;
}

int main(int argc, char* argv[])
{
    std::array<bool, 4> enable {};
    enable.fill(true);

    if (argc == 2) {
        enable[0] = (argv[1] == "1"sv);
        enable[1] = (argv[1] == "2"sv);
        enable[2] = (argv[1] == "3"sv);
        enable[3] = (argv[1] == "4"sv);
    }

    if (enable[0] && !Test<std::uint8_t>())
        return -1;

    if (enable[1] && !Test<std::uint16_t>())
        return -1;

    if (enable[2] && !Test<std::uint32_t>())
        return -1;

    if (enable[3] && !Test<std::uint64_t>())
        return -1;

    return 0;
}