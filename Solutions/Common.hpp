#pragma once

#if !defined(AOC_YEAR) || !defined(AOC_DAY) || !defined(INPUT_FILE)
#    error please define AOC_YEAR, AOC_DAY, and INPUT_FILE
#endif

#include <Stuff/Competition/Memo.hpp>

#include <Solution.hpp>
#include <Stuff/Competition/Comp.hpp>
#include <Stuff/Util/MMap.hpp>

#include <range/v3/range/operations.hpp>
#include <range/v3/view/common.hpp>
#include <range/v3/view/sliding.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/for_each.hpp>

namespace AoC {

using namespace Stf::Comp::Aliases;
using namespace Stf::Comp;
using namespace std::string_view_literals;
namespace views = ranges::views;
using views::sliding;
using views::split;
using views::transform;
using views::for_each;

inline static constexpr auto fwd_to_str = views::transform([](auto svd) {
    auto v = svd | views::common;
    return std::string(v.begin(), v.end());
});

template<typename T>
inline static constexpr auto str_to_t = views::transform([](auto sv) {
    T v;
    std::from_chars(sv.data(), sv.data() + sv.size(), v);
    return v;
});

}