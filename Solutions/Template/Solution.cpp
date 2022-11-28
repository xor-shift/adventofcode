#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

void Solution::solution(size_t part, std::string_view input) {
    if (part == 1) {
    } else if (part == 2) {
    }
}

void Solution::solve_example(size_t part) {
    std::string_view example_string = ""sv;
    solution(part, example_string);
}

void Solution::solve(size_t part) {
    const Stf::MMapStringView file(INPUT_FILE, false);
    solution(part, file);
}

}
