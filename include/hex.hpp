#ifndef HEX_HPP_
#define HEX_HPP_

#include <cstdint>
#include <cstdio>

#include <algorithm>
#include <array>
#include <bit>
#include <charconv>
#include <concepts>
#include <format>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>

#include <fmt/format.h>

namespace hex::detail {

inline constexpr char kHexTable1[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};

inline constexpr char kHexTable2[] = {
    '0', '0', '0', '1', '0', '2', '0', '3', '0', '4', '0', '5', '0', '6', '0', '7', '0', '8', '0', '9', '0', 'a', '0', 'b', '0', 'c', '0', 'd', '0', 'e', '0', 'f',
    '1', '0', '1', '1', '1', '2', '1', '3', '1', '4', '1', '5', '1', '6', '1', '7', '1', '8', '1', '9', '1', 'a', '1', 'b', '1', 'c', '1', 'd', '1', 'e', '1', 'f',
    '2', '0', '2', '1', '2', '2', '2', '3', '2', '4', '2', '5', '2', '6', '2', '7', '2', '8', '2', '9', '2', 'a', '2', 'b', '2', 'c', '2', 'd', '2', 'e', '2', 'f',
    '3', '0', '3', '1', '3', '2', '3', '3', '3', '4', '3', '5', '3', '6', '3', '7', '3', '8', '3', '9', '3', 'a', '3', 'b', '3', 'c', '3', 'd', '3', 'e', '3', 'f',
    '4', '0', '4', '1', '4', '2', '4', '3', '4', '4', '4', '5', '4', '6', '4', '7', '4', '8', '4', '9', '4', 'a', '4', 'b', '4', 'c', '4', 'd', '4', 'e', '4', 'f',
    '5', '0', '5', '1', '5', '2', '5', '3', '5', '4', '5', '5', '5', '6', '5', '7', '5', '8', '5', '9', '5', 'a', '5', 'b', '5', 'c', '5', 'd', '5', 'e', '5', 'f',
    '6', '0', '6', '1', '6', '2', '6', '3', '6', '4', '6', '5', '6', '6', '6', '7', '6', '8', '6', '9', '6', 'a', '6', 'b', '6', 'c', '6', 'd', '6', 'e', '6', 'f',
    '7', '0', '7', '1', '7', '2', '7', '3', '7', '4', '7', '5', '7', '6', '7', '7', '7', '8', '7', '9', '7', 'a', '7', 'b', '7', 'c', '7', 'd', '7', 'e', '7', 'f',
    '8', '0', '8', '1', '8', '2', '8', '3', '8', '4', '8', '5', '8', '6', '8', '7', '8', '8', '8', '9', '8', 'a', '8', 'b', '8', 'c', '8', 'd', '8', 'e', '8', 'f',
    '9', '0', '9', '1', '9', '2', '9', '3', '9', '4', '9', '5', '9', '6', '9', '7', '9', '8', '9', '9', '9', 'a', '9', 'b', '9', 'c', '9', 'd', '9', 'e', '9', 'f',
    'a', '0', 'a', '1', 'a', '2', 'a', '3', 'a', '4', 'a', '5', 'a', '6', 'a', '7', 'a', '8', 'a', '9', 'a', 'a', 'a', 'b', 'a', 'c', 'a', 'd', 'a', 'e', 'a', 'f',
    'b', '0', 'b', '1', 'b', '2', 'b', '3', 'b', '4', 'b', '5', 'b', '6', 'b', '7', 'b', '8', 'b', '9', 'b', 'a', 'b', 'b', 'b', 'c', 'b', 'd', 'b', 'e', 'b', 'f',
    'c', '0', 'c', '1', 'c', '2', 'c', '3', 'c', '4', 'c', '5', 'c', '6', 'c', '7', 'c', '8', 'c', '9', 'c', 'a', 'c', 'b', 'c', 'c', 'c', 'd', 'c', 'e', 'c', 'f',
    'd', '0', 'd', '1', 'd', '2', 'd', '3', 'd', '4', 'd', '5', 'd', '6', 'd', '7', 'd', '8', 'd', '9', 'd', 'a', 'd', 'b', 'd', 'c', 'd', 'd', 'd', 'e', 'd', 'f',
    'e', '0', 'e', '1', 'e', '2', 'e', '3', 'e', '4', 'e', '5', 'e', '6', 'e', '7', 'e', '8', 'e', '9', 'e', 'a', 'e', 'b', 'e', 'c', 'e', 'd', 'e', 'e', 'e', 'f',
    'f', '0', 'f', '1', 'f', '2', 'f', '3', 'f', '4', 'f', '5', 'f', '6', 'f', '7', 'f', '8', 'f', '9', 'f', 'a', 'f', 'b', 'f', 'c', 'f', 'd', 'f', 'e', 'f', 'f'
};

template <bool leading_zero>
std::string snprintf(std::unsigned_integral auto value)
{
    static constexpr auto size = sizeof(value) * 2 + 1;
    std::array<char, size> buffer {};
    auto* first = buffer.data();
    auto pos { 0u };

    if constexpr (leading_zero) {
        if constexpr (sizeof(value) == 1) {
            pos = std::snprintf(first, size, "%02hhx", value);
        } else if constexpr (sizeof(value) == 2) {
            pos = std::snprintf(first, size, "%04hx", value);
        } else if constexpr (sizeof(value) == 4) {
            pos = std::snprintf(first, size, "%08x", value);
        } else {
            pos = std::snprintf(first, size, "%016llx", value);
        }
    } else {
        if constexpr (sizeof(value) == 1) {
            pos = std::snprintf(first, size, "%hhx", value);
        } else if constexpr (sizeof(value) == 2) {
            pos = std::snprintf(first, size, "%hx", value);
        } else if constexpr (sizeof(value) == 4) {
            pos = std::snprintf(first, size, "%x", value);
        } else {
            pos = std::snprintf(first, size, "%llx", value);
        }
    }

    return { first, first + pos };
}

template <bool leading_zero>
std::string stringstream(std::unsigned_integral auto value)
{
    std::ostringstream buffer;
    buffer << std::hex;

    if constexpr (leading_zero) {
        buffer << std::setw(sizeof(value) * 2) << std::setfill('0');
    }

    if constexpr (sizeof(value) == 1) {
        buffer << static_cast<std::uint32_t>(value);
    } else {
        buffer << value;
    }

    return buffer.str();
}

template <bool leading_zero>
std::string to_chars(std::unsigned_integral auto value)
{
    static constexpr auto size = sizeof(value) * 2;
    std::array<char, size> buffer {};
    auto* first = buffer.data();
    auto* last = first + size;
    auto length { 0u };

    if constexpr (sizeof(value) == 1) {
        auto [ptr, ec] = std::to_chars(first, last, static_cast<std::uint32_t>(value), 16);
        length = ptr - first;
    } else {
        auto [ptr, ec] = std::to_chars(first, last, value, 16);
        length = ptr - first;
    }

    if constexpr (leading_zero) {
        std::memmove(last - length, first, length);
        std::fill_n(first, size - length, '0');

        return { first, last };
    } else {
        return { first, first + length };
    }
}

template <bool leading_zero>
std::string format(std::unsigned_integral auto value)
{
    if constexpr (leading_zero) {
        return std::format("{0:0{1}x}", value, sizeof(value) * 2);
    } else {
        return std::format("{:x}", value);
    }
}

template <bool leading_zero>
std::string fmt_format(std::unsigned_integral auto value)
{
    if constexpr (leading_zero) {
        return fmt::format("{0:0{1}x}", value, sizeof(value) * 2);
    } else {
        return fmt::format("{:x}", value);
    }
}

template <bool leading_zero>
std::string naive(std::unsigned_integral auto value)
{
    static constexpr auto size = sizeof(value) * 2;
    std::array<char, size> buffer {};
    const auto* first = buffer.data();
    const auto* last = first + size;
    auto pos = size;

    if constexpr (leading_zero) {
        buffer.fill('0');
    }

    do {
        const auto nibble = value & 0xf;

        buffer[--pos] = static_cast<char>('0' + nibble);

        if (nibble > 9)
            buffer[pos] += 'a' - '0' - 10;

        value >>= 4;
    } while (value > 0);

    if constexpr (leading_zero) {
        return { first, last };
    } else {
        return { first + pos, last };
    }
}

template <bool leading_zero>
std::string LUT1(std::unsigned_integral auto value)
{
    static constexpr auto size = sizeof(value) * 2;
    std::array<char, size> buffer {};
    const auto* first = buffer.data();
    const auto* last = first + size;
    auto pos = size - 1;

    if constexpr (leading_zero) {
        buffer.fill('0');
    }

    while (value >= 0x10u) {
        buffer[pos] = kHexTable1[value % 0x10u];
        value /= 0x10u;
        --pos;
    }

    buffer[pos] = kHexTable1[value];

    if constexpr (leading_zero) {
        return { first, last };
    } else {
        return { first + pos, last };
    }
}

template <bool leading_zero>
std::string LUT2(std::unsigned_integral auto value)
{
    static constexpr auto size = sizeof(value) * 2;
    std::array<char, size> buffer {};
    const auto* first = buffer.data();
    const auto* last = first + size;
    auto pos = size - 1;

    if constexpr (leading_zero) {
        buffer.fill('0');
    }

    while (value >= 0x100u) {
        buffer[pos] = kHexTable1[value % 0x10u];
        value /= 0x10u;
        buffer[pos - 1] = kHexTable1[value % 0x10u];
        value /= 0x10u;
        pos -= 2;
    }

    if (value >= 0x10u) {
        buffer[pos] = kHexTable1[value % 0x10u];
        value /= 0x10u;
        buffer[pos - 1] = kHexTable1[value];
        --pos;
    } else {
        buffer[pos] = kHexTable1[value];
    }

    if constexpr (leading_zero) {
        return { first, last };
    } else {
        return { first + pos, last };
    }
}

template <bool leading_zero>
std::string LUT3(std::unsigned_integral auto value)
{
    static constexpr auto size = sizeof(value) * 2;
    std::array<char, size> buffer {};
    const auto* first = buffer.data();
    const auto* last = first + size;
    auto pos = size - 1;

    if constexpr (leading_zero) {
        buffer.fill('0');
    }

    while (value >= 0x100u) {
        const auto num = (value % 0x100u) * 2;
        buffer[pos - 1] = kHexTable2[num];
        buffer[pos] = kHexTable2[num + 1];
        value /= 0x100u;
        pos -= 2;
    }

    if (value >= 0x10u) {
        const auto num = value * 2;
        buffer[pos - 1] = kHexTable2[num];
        buffer[pos] = kHexTable2[num + 1];
        --pos;
    } else {
        buffer[pos] = kHexTable2[value * 2 + 1];
    }

    if constexpr (leading_zero) {
        return { first, last };
    } else {
        return { first + pos, last };
    }
}

template <bool leading_zero>
std::string SWAR(std::unsigned_integral auto value)
{
    static constexpr auto size = sizeof(value) * 2;
    std::array<char, size> buffer {};
    auto* first = buffer.data();
    const auto last = first + size;

    auto expand = [](auto x) {
        static_assert(sizeof(x) < 8);

        std::uint64_t nibbles { 0 };

        for (auto i { 0u }; i < 2 * sizeof(x); ++i) {
            if constexpr (std::endian::native == std::endian::little) {
                // 0x1234 -> 0x04030201
                nibbles |= ((x & (0xfull << (4 * i))) >> (4 * i)) << (8 * (2 * sizeof(x) - 1 - i));
            } else {
                // 0x1234 -> 0x01020304
                nibbles |= (x & (0xfull << (4 * i))) << (4 * i);
            }
        }

        if constexpr (sizeof(x) == 1) {
            return static_cast<std::uint16_t>(nibbles);
        } else if constexpr (sizeof(x) == 2) {
            return static_cast<std::uint32_t>(nibbles);
        } else {
            return static_cast<std::uint64_t>(nibbles);
        }
    };

    auto convert = [](auto nibbles) {
        auto pack = [](std::uint8_t byte) {
            static_assert(sizeof(nibbles) > 1);

            if constexpr (sizeof(nibbles) == 2) {
                return static_cast<std::uint16_t>(byte * 0x0101u);
            } else if constexpr (sizeof(nibbles) == 4) {
                return static_cast<std::uint32_t>(byte * 0x01010101u);
            } else {
                return static_cast<std::uint64_t>(byte * 0x0101010101010101ull);
            }
        };

        static constexpr auto correction = pack('a' - '0' - 10);
        const auto ascii09 = pack('0') + nibbles;
        const auto tmp = pack(0x80 - 10) + nibbles;
        const auto msb = pack(0x80) & tmp;
        const auto mask = msb - (msb >> 7);

        return ascii09 + (mask & correction);
    };

    if constexpr (sizeof(value) < 8) {
        const auto packed = convert(expand(value));
        const auto* ptr = reinterpret_cast<const std::uint8_t*>(&packed);

        std::copy_n(ptr, size, first);
    } else {
        const auto packed_hi = convert(expand(static_cast<std::uint32_t>(value >> 32)));
        const auto packed_lo = convert(expand(static_cast<std::uint32_t>(value)));
        auto* ptr_hi = reinterpret_cast<std::uint64_t*>(first);
        auto* ptr_lo = ptr_hi + 1;

        *ptr_hi = packed_hi;
        *ptr_lo = packed_lo;
    }

    if constexpr (leading_zero) {
        return { first, last };
    } else {
        const auto pos = std::countl_zero(value) / 4 - static_cast<int>(value == 0);

        return { first + pos, last };
    }
}

} // namespace hex::detail

namespace hex {

auto snprintf(auto value) { return detail::snprintf<false>(value); }
auto stringstream(auto value) { return detail::stringstream<false>(value); }
auto to_chars(auto value) { return detail::to_chars<false>(value); }
auto format(auto value) { return detail::format<false>(value); }
auto fmt_format(auto value) { return detail::fmt_format<false>(value); }
auto naive(auto value) { return detail::naive<false>(value); }
auto LUT1(auto value) { return detail::LUT1<false>(value); }
auto LUT2(auto value) { return detail::LUT2<false>(value); }
auto LUT3(auto value) { return detail::LUT3<false>(value); }
auto SWAR(auto value) { return detail::SWAR<false>(value); }

auto snprintf_LZ(auto value) { return detail::snprintf<true>(value); }
auto stringstream_LZ(auto value) { return detail::stringstream<true>(value); }
auto to_chars_LZ(auto value) { return detail::to_chars<true>(value); }
auto format_LZ(auto value) { return detail::format<true>(value); }
auto fmt_format_LZ(auto value) { return detail::fmt_format<true>(value); }
auto naive_LZ(auto value) { return detail::naive<true>(value); }
auto LUT1_LZ(auto value) { return detail::LUT1<true>(value); }
auto LUT2_LZ(auto value) { return detail::LUT2<true>(value); }
auto LUT3_LZ(auto value) { return detail::LUT3<true>(value); }
auto SWAR_LZ(auto value) { return detail::SWAR<true>(value); }

} // namespace hex

#endif // HEX_HPP_