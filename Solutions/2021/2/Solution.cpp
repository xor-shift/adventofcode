#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

void Solution::solution(size_t part, std::string_view input) {
    auto a = input | split("\n"sv) | fwd_to_str | transform([](auto line) {
                 auto a = line | split(" "sv) | fwd_to_str;
                 std::vector<std::string> a_v{a.begin(), a.end()};
                 i64 v;
                 std::from_chars(a_v[1].data(), a_v[1].data() + a_v[1].size(), v);

                 return std::pair<char, i64>{a_v[0][0], v};
             });

    if (part == 1) {
        i64 coords[2] = {0, 0};
        for (auto [dir, amt] : a) {
            switch (dir) {
            case 'f': coords[0] += amt; break;
            case 'u': coords[1] -= amt; break;
            case 'd': coords[1] += amt; break;
            default: std::unreachable();
            }
        }

        fmt::print("{}*{}={}\n", coords[0], coords[1], coords[0] * coords[1]);
    } else if (part == 2) {
        i64 aim = 0;
        i64 coords[2] = {0, 0};
        for (auto [dir, amt] : a) {
            switch (dir) {
            case 'f': coords[0] += amt; coords[1] += aim * amt; break;
            case 'u': aim -= amt; break;
            case 'd': aim += amt; break;
            default: std::unreachable();
            }
        }
        fmt::print("{}*{}={}\n", coords[0], coords[1], coords[0] * coords[1]);
    }
}

void Solution::solve_example(size_t part) {
    std::string_view example_string = "forward 5\n"
                                      "down 5\n"
                                      "forward 8\n"
                                      "up 3\n"
                                      "down 8\n"
                                      "forward 2"sv;
    solution(part, example_string);
}

void Solution::solve(size_t part) {
    const Stf::MMapStringView file(INPUT_FILE, false);
    solution(part, file);
}

}
