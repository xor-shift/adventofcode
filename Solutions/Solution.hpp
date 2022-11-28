#pragma once

#include <string_view>

#include <fmt/format.h>

namespace AoC {

struct Solution {
    Solution();

    void solution(size_t part, std::string_view input);

    void solve_example(size_t part);
    void solve(size_t part);
};

}
