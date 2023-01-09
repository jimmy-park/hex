#include <chrono>
#include <limits>
#include <random>
#include <string_view>

#include <catch2/catch_all.hpp>
#include <hex.hpp>

using namespace std::literals;
using namespace Catch::Matchers;

#define COMPARE(value)                                            \
    do {                                                          \
        auto expected = hex::snprintf(value);                     \
        REQUIRE_THAT(expected, Equals(hex::stringstream(value))); \
        REQUIRE_THAT(expected, Equals(hex::to_chars(value)));     \
        REQUIRE_THAT(expected, Equals(hex::format(value)));       \
        REQUIRE_THAT(expected, Equals(hex::fmt_format(value)));   \
        REQUIRE_THAT(expected, Equals(hex::naive(value)));        \
        REQUIRE_THAT(expected, Equals(hex::LUT1(value)));         \
        REQUIRE_THAT(expected, Equals(hex::LUT2(value)));         \
        REQUIRE_THAT(expected, Equals(hex::LUT3(value)));         \
        REQUIRE_THAT(expected, Equals(hex::SWAR(value)));         \
    } while (false)

#define COMPARE_LZ(value)                                            \
    do {                                                             \
        auto expected = hex::snprintf_LZ(value);                     \
        REQUIRE_THAT(expected, Equals(hex::stringstream_LZ(value))); \
        REQUIRE_THAT(expected, Equals(hex::to_chars_LZ(value)));     \
        REQUIRE_THAT(expected, Equals(hex::format_LZ(value)));       \
        REQUIRE_THAT(expected, Equals(hex::fmt_format_LZ(value)));   \
        REQUIRE_THAT(expected, Equals(hex::naive_LZ(value)));        \
        REQUIRE_THAT(expected, Equals(hex::LUT1_LZ(value)));         \
        REQUIRE_THAT(expected, Equals(hex::LUT2_LZ(value)));         \
        REQUIRE_THAT(expected, Equals(hex::LUT3_LZ(value)));         \
        REQUIRE_THAT(expected, Equals(hex::SWAR_LZ(value)));         \
    } while (false)

inline constexpr auto kCount = 1u << 20;

TEMPLATE_TEST_CASE("Unit test for non leading zero", "[validate]", std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t)
{
    if constexpr (sizeof(TestType) < 4) {
        TestType i { 0 };

        do {
            COMPARE(i);
        } while (++i != 0);
    } else {
        std::random_device rd;
        std::mt19937_64 gen { rd() };
        std::uniform_int_distribution<TestType> dist;

        for (auto i { 0u }; i < kCount; ++i) {
            auto value = dist(gen);
            COMPARE(value);
        }
    }
}

TEMPLATE_TEST_CASE("Unit test for leading zero", "[validate]", std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t)
{
    if constexpr (sizeof(TestType) < 4) {
        TestType i { 0 };

        do {
            COMPARE_LZ(i);
        } while (++i != 0);
    } else {
        std::random_device rd;
        std::mt19937_64 gen { rd() };
        std::uniform_int_distribution<TestType> dist;

        for (auto i { 0u }; i < kCount; ++i) {
            auto value = dist(gen);
            COMPARE_LZ(value);
        }
    }
}
