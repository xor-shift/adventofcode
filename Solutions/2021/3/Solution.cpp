#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

void Solution::solution(size_t part, std::string_view input) {
    if (part == 1) {
        auto a = input | split("\n"sv) | fwd_to_str;

        arr<arr<usize, 2>, 12> cts = { 0 };

        for (auto fw : a) {
            for (size_t b = 0; auto c : fw) {
                cts[b++][c - '0']++;
            }
        }

        auto b = cts | transform([](auto arr) { return arr[1] > arr[0] ? '1' : '0'; });
        auto c = b | transform([](auto c) { return c == '1' ? '0' : '1'; });

        fmt::print("{}\n", std::string(b.begin(), b.end()));
        fmt::print("{}\n", std::string(c.begin(), c.end()));
    } else if (part == 2) {
        auto a = input | split("\n"sv) | fwd_to_str;

        using usets = std::unordered_set<std::string>;

        auto find_xcommon = [](usets& set, bool common, size_t index) -> char {
            usize ct[2] = { 0, 0 };
            for (auto& s : set)
                ct[s[index] - '0']++;

            return (ct[1] >= ct[0] != common) ? '1' : '0';
        };

        auto keep_xcommon = [&find_xcommon](usets& set, bool common, size_t index, char required = '\0') {
            if (required == '\0')
                required = find_xcommon(set, !common, index);
            std::vector<std::string> to_remove {};
            to_remove.reserve(set.size());
            for (auto const& s : set) {
                if (s[index] != required)
                    to_remove.push_back(s);
            }

            std::ignore = to_remove;

            for (auto const& s : to_remove) {
                set.erase(s);
            }
        };

        usets o2_scrubber { a.begin(), a.end() };
        usets co2_scrubber { a.begin(), a.end() };

        for (size_t i = 0; o2_scrubber.size() > 1;) {
            keep_xcommon(o2_scrubber, true, i++);
        }

        for (size_t i = 0; co2_scrubber.size() > 1;) {
            keep_xcommon(co2_scrubber, false, i++);
        }

        fmt::print("{}, {}\n", *o2_scrubber.begin(), *co2_scrubber.begin());
    }
}

void Solution::solve_example(size_t part) {
    std::string_view example_string = "00100\n"
                                      "11110\n"
                                      "10110\n"
                                      "10111\n"
                                      "10101\n"
                                      "01111\n"
                                      "00111\n"
                                      "11100\n"
                                      "10000\n"
                                      "11001\n"
                                      "00010\n"
                                      "01010"sv;
    solution(part, example_string);
}

void Solution::solve(size_t part) {
    const Stf::MMapStringView file(INPUT_FILE, false);
    solution(part, file);
}

}
