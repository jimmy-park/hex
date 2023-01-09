#include <chrono>
#include <limits>
#include <random>
#include <string_view>

#include <doctest/doctest.h>
#include <hex.hpp>

#define COMPARE(value)                                  \
    do {                                                \
        auto expected = hex::snprintf(value);           \
        REQUIRE_EQ(expected, hex::stringstream(value)); \
        REQUIRE_EQ(expected, hex::to_chars(value));     \
        REQUIRE_EQ(expected, hex::format(value));       \
        REQUIRE_EQ(expected, hex::fmt_format(value));   \
        REQUIRE_EQ(expected, hex::naive(value));        \
        REQUIRE_EQ(expected, hex::LUT1(value));         \
        REQUIRE_EQ(expected, hex::LUT2(value));         \
        REQUIRE_EQ(expected, hex::LUT3(value));         \
        REQUIRE_EQ(expected, hex::SWAR(value));         \
    } while (false)

#define COMPARE_LZ(value)                                  \
    do {                                                   \
        auto expected = hex::snprintf_LZ(value);           \
        REQUIRE_EQ(expected, hex::stringstream_LZ(value)); \
        REQUIRE_EQ(expected, hex::to_chars_LZ(value));     \
        REQUIRE_EQ(expected, hex::format_LZ(value));       \
        REQUIRE_EQ(expected, hex::fmt_format_LZ(value));   \
        REQUIRE_EQ(expected, hex::naive_LZ(value));        \
        REQUIRE_EQ(expected, hex::LUT1_LZ(value));         \
        REQUIRE_EQ(expected, hex::LUT2_LZ(value));         \
        REQUIRE_EQ(expected, hex::LUT3_LZ(value));         \
        REQUIRE_EQ(expected, hex::SWAR_LZ(value));         \
    } while (false)

inline constexpr auto kCount = 1u << 20;

TEST_CASE_TEMPLATE("Unit test for non leading zero", Uint, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t)
{
    if constexpr (sizeof(Uint) < 4) {
        Uint i { 0 };

        do {
            COMPARE(i);
        } while (++i != 0);
    } else {
        std::random_device rd;
        std::mt19937_64 gen { rd() };
        std::uniform_int_distribution<Uint> dist;

        for (auto i { 0u }; i < kCount; ++i) {
            auto value = dist(gen);
            COMPARE(value);
        }
    }
}

TEST_CASE_TEMPLATE("Unit test for leading zero", Uint, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t)
{
    if constexpr (sizeof(Uint) < 4) {
        Uint i { 0 };

        do {
            COMPARE_LZ(i);
        } while (++i != 0);
    } else {
        std::random_device rd;
        std::mt19937_64 gen { rd() };
        std::uniform_int_distribution<Uint> dist;

        for (auto i { 0u }; i < kCount; ++i) {
            auto value = dist(gen);
            COMPARE_LZ(value);
        }
    }
}
