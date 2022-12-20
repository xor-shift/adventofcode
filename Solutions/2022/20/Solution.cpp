#include <Common.hpp>

#include <queue>

namespace AoC {

Solution::Solution() { }

std::string Solution::solve(size_t part, std::string_view input) {
    auto in_it = input | split("\n"sv) | fwd_to_strv | str_to_t<i64>;
    vec<pair<i64, i64>> nums;
    for (i64 i = 0; auto v : in_it) {
        nums.push_back(std::make_pair(i++, v));
    }
    std::deque<pair<i64, i64>> nums_deque(begin(nums), end(nums));

    auto print = []<typename T>(T const& nums) {
        if constexpr (std::is_same_v<T, std::deque<pair<i64, i64>>>) {
            for (auto i : nums)
                fmt::print("{} ", i.second);
        } else if constexpr (std::is_same_v<T, vec<i64>>) {
            for (auto i : nums)
                fmt::print("{} ", i);
        }
        fmt::print("\n");
    };

    auto modulo = []<typename T>(T v, T n) { return (v % n + n) % n; };

    auto rotate_once_between = [modulo](auto& cont, usize from, usize to, bool right) {
        if (from == to)
            return;

        for (i64 i = 1; i < to - from; i++) {
            using std::swap;

            i64 real_i = right ? to - from - i : i;

            auto sw_j = right ? real_i - 1 : real_i + 1;
            sw_j = modulo(sw_j, static_cast<i64>(to - from));

            swap(cont[real_i], cont[sw_j]);
        }
    };

    auto rotate_between = []<typename T>(T& cont, usize from, usize to, i64 amt) {
        if (amt == 0)
            return;

        auto range = static_cast<i64>(to - from);
        auto beg_it = begin(cont) + from;
        auto end_it = beg_it + range;

        auto aamt = std::abs(amt) % cont.size();
        auto right = amt > 0;

        vec<typename T::value_type> temporary;
        temporary.reserve(aamt);

        if (right) {
            copy(end_it - aamt, end_it, back_inserter(temporary));
            copy_backward(beg_it, end_it - aamt, end_it);
            copy(begin(temporary), end(temporary), beg_it);
        } else {
            copy(beg_it, beg_it + aamt, back_inserter(temporary));
            copy(beg_it + aamt, end_it, beg_it);
            copy(begin(temporary), end(temporary), end_it - aamt);
        }
    };

    auto better_rotate = [rotate_between](auto& cont, i64 amt) { rotate_between(cont, 0, cont.size(), amt); };

    std::ignore = better_rotate;

    auto mix = [&] {
        for (auto p : nums) {
            auto [order, val] = p;

            auto it = find(begin(nums_deque), end(nums_deque), p);
            auto idx = distance(begin(nums_deque), it);

            better_rotate(nums_deque, -static_cast<i64>(idx));

            auto aeiou = nums_deque.front();
            nums_deque.pop_front();
            better_rotate(nums_deque, -aeiou.second);
            nums_deque.push_front(aeiou);
        }
    };

    if (part == 1) {
        mix();
    } else if (part == 2) {
        for (usize i = 0; i < nums.size(); i++) {
            static constexpr i64 key = 811589153ul;
            nums[i].second *= key;
            nums_deque[i].second *= key;
        }

        for (int i = 0; i < 10; i++)
            mix();
    }

    auto it = find_if(begin(nums_deque), end(nums_deque), [](auto p) { return p.second == 0; });
    auto it_idx = distance(begin(nums_deque), it);

    auto a = nums_deque[(it_idx + 1000) % nums_deque.size()].second;
    auto b = nums_deque[(it_idx + 2000) % nums_deque.size()].second;
    auto c = nums_deque[(it_idx + 3000) % nums_deque.size()].second;
    return std::to_string(a + b + c);
}

}
