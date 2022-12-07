#pragma once

#include <string_view>

#include <fmt/format.h>

namespace AoC {

struct Solution {
    Solution();

    void solve(bool example, size_t part, std::string_view input = "");
};

}
