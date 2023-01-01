#include <random>

#include <hex.hpp>

int main()
{
    static constexpr auto kCount = 1u << 16;
    std::random_device rd;
    std::mt19937 gen { rd() };
    std::uniform_int_distribution<std::uint64_t> dist;

    for (auto i = 0u; i < kCount; ++i) {
        const auto value = dist(gen);
        const auto a = ToHexUsingSnprintf(value);
        const auto b = ToHexUsingStringstream(value);
        const auto c = ToHexUsingToChars(value);
        const auto d = ToHexUsingStdFormat(value);
        const auto e = ToHexUsingFmtFormat(value);
        const auto f = ToHexUsingTable1_1(value);
        const auto g = ToHexUsingTable1_2(value);
        const auto h = ToHexUsingTable2(value);

        if (a != b || b != c || c != d || d != e || e != f || f != g || g != h) {
            std::cout << std::format("Failed on value {}\n", value);
            return -1;
        }
    }

    return 0;
}