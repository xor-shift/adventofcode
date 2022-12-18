#pragma once

#if !defined(AOC_YEAR) || !defined(AOC_DAY) || !defined(INPUT_FILE)
#    error please define AOC_YEAR, AOC_DAY, and INPUT_FILE
#endif

#include <Stuff/Util/Hacks/Concepts.hpp>
#include <Stuff/Util/Hacks/Expected.hpp>
#include <Stuff/Util/Hacks/Try.hpp>

#include <algorithm>
#include <charconv>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <optional>
#include <set>
#include <span>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <range/v3/range/operations.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/chunk_by.hpp>
#include <range/v3/view/common.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/drop_while.hpp>
#include <range/v3/view/empty.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/for_each.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/sliding.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>

#include <fmt/format.h>

#include <Stuff/Util/Hash.hpp>
#include <Stuff/Util/MMap.hpp>
#include <Stuff/Util/Memo.hpp>
#include <Stuff/Util/Tuple.hpp>

#include <Stuff/Intro/Dump.hpp>
#include <Stuff/Intro/Introspector.hpp>
#include <Stuff/Intro/Introspectors/Array.hpp>
#include <Stuff/Intro/Introspectors/Map.hpp>
#include <Stuff/Intro/Introspectors/Span.hpp>
#include <Stuff/Intro/Introspectors/Tuple.hpp>
#include <Stuff/Intro/Introspectors/UnorderedMap.hpp>
#include <Stuff/Intro/Introspectors/Vector.hpp>

#include <Solution.hpp>

namespace AoC {

namespace Detail {

// N-dimensional Unordered Map Array Helper
template<typename T, size_t N> struct NUMArrHelper;

template<typename T> struct NUMArrHelper<T, 1> {
    using type = std::unordered_map<ptrdiff_t, T>;
};

template<typename T, size_t N> struct NUMArrHelper {
    using type = std::unordered_map<ptrdiff_t, typename NUMArrHelper<T, N - 1>::type>;
};

}

namespace Aliases {

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using i128 = __int128_t;
using isize = ptrdiff_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using u128 = __uint128_t;
using usize = size_t;

template<typename T> using span = std::span<T>;
template<typename... Ts> using umap = std::unordered_map<Ts...>;
template<typename... Ts> using uset = std::unordered_set<Ts...>;
template<typename... Ts> using set = std::set<Ts...>;
template<typename... Ts> using pair = std::pair<Ts...>;
template<typename... Ts> using tuple = std::tuple<Ts...>;
template<typename T> using umarr = umap<isize, T>;
template<typename T, size_t N> using numarr = typename Detail::NUMArrHelper<T, N>::type;
template<typename... Ts> using vec = std::vector<Ts...>;
template<typename T, size_t N> using arr = std::array<T, N>;
template<typename T> using opt = std::optional<T>;
using str = std::string;
using strview = std::string_view;

}

template<typename T, typename Pred> constexpr std::optional<T> opt_if(T v, Pred&& pred) {
    if (std::invoke(std::forward<Pred>(pred), std::forward<T>(v)))
        return std::move(v);
    return std::nullopt;
}

namespace Detail {

template<typename T, typename Fn>
    requires(!requires(Fn && fn, std::span<T> v) { std::ignore = std::invoke(std::forward<Fn>(fn), v); })
constexpr void windowed_foreach(size_t window_size, std::span<T> vals, Fn&& fn) {
    if (window_size == 0)
        return;

    for (size_t i = 0; i < vals.size() + window_size; i++) {
        const auto beg = std::clamp(i, window_size, vals.size() + window_size) - window_size;
        const auto end = std::clamp(i + window_size, window_size, vals.size() + window_size) - window_size;
        if (end == beg)
            continue;
        std::invoke(std::forward<Fn>(fn), std::span(vals.data() + beg, end - beg));
    }
}

template<typename T, typename Fn>
    requires(requires(Fn&& fn, std::span<T> v) { std::ignore = std::invoke(std::forward<Fn>(fn), v); })
constexpr auto windowed_foreach(size_t window_size, std::span<T> vals, Fn&& fn) {
    using Ret = std::invoke_result_t<Fn, std::span<T>>;
    std::vector<Ret> vec {};
    windowed_foreach(window_size, vals,
      [&](auto v) { vec.template emplace_back(std::forward<Ret>(std::invoke(std::forward<Fn>(fn), v))); });
    return vec;
}

}

template<typename T, typename Fn> constexpr auto windowed_foreach(size_t window_size, T&& container, Fn&& fn) {
    return Detail::windowed_foreach(window_size, std::span(container.data(), container.size()), std::forward<Fn>(fn));
}

using namespace Aliases;
using namespace std::string_view_literals;
namespace views = ranges::views;
using ranges::distance;
using views::chunk;
using views::chunk_by;
using views::drop;
using views::drop_while;
using views::empty;
using views::filter;
using views::for_each;
using views::reverse;
using views::sliding;
using views::split;
using views::take;
using views::transform;
using views::zip;

inline static constexpr auto fwd_to_str = views::transform([](auto svd) {
    auto v = svd | views::common;
    return std::string(v.begin(), v.end());
});

inline static constexpr auto fwd_to_strv = views::transform([](auto svd) {
    auto begin = svd.begin();
    auto end = svd.begin();
    for (auto t = end; t != svd.end(); t++) {
        end = t;
    }
    char const& begin_c = *svd.begin();
    char const& end_c = *end;
    return std::string_view(&begin_c, &end_c - &begin_c + 1);
});

template<typename T> constexpr std::vector<T> vec_from_view(auto view) {
    std::vector<T> ret {};
    ret.reserve(ranges::distance(view));
    for (T v : view)
        ret.emplace_back(std::move(v));
    return ret;
}

template<typename T>
inline static constexpr auto str_to_t = views::transform([](auto sv) {
    T v;
    std::from_chars(sv.data(), sv.data() + sv.size(), v);
    return v;
});

template<typename T, typename BinaryOp, typename View>
constexpr auto fold_view(View&& v, T init, BinaryOp&& binary_op) {
    for (T x : v)
        init = std::invoke(std::forward<BinaryOp>(binary_op), init, x);

    return init;
}

template<typename T, typename View> constexpr auto fold_view(View&& v, T init = 0) {
    return fold_view(v, init, std::plus<T> {});
}

template<typename T, typename U, typename View> constexpr pair<T, U> view_to_pair(View&& view) {
    auto it = view.begin();
    T a = *it++;
    U b = *it;
    return std::make_pair(a, b);
}

template<typename T, size_t N, typename View> constexpr arr<T, N> view_to_arr(View&& view) {
    arr<T, N> ret;
    auto it = view.begin();
    for (usize i = 0; i < N; i++)
        ret[i] = *it++;
    return ret;
}

template<typename T, typename View> constexpr std::expected<T, std::errc> from_chars(View&& view) {
    T v;
    auto ret = std::from_chars(begin(view), end(view), v);

    if (ret.ec != std::errc()) {
        return std::unexpected { ret.ec };
    }

    return v;
}

template<typename T, size_t M, typename View, typename Comp = std::less<T>>
    requires std::is_arithmetic_v<T>
constexpr std::array<T, M> find_n_largest(View&& view, Comp&& comp = Comp()) {
    std::array<T, M> ret;
    std::fill(begin(ret), end(ret), std::numeric_limits<T>::min());

    for (auto v : view) {
        auto it = std::lower_bound(begin(ret), end(ret), v, comp);
        if (it == begin(ret))
            continue;

        *std::prev(it) = v;
    }

    return ret;
}

}
