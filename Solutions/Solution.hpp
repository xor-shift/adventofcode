#pragma once

#include <string_view>

#include <fmt/format.h>

namespace AoC {

struct Solution {
    Solution();

    std::string solve(size_t part, std::string_view input);
};

}
