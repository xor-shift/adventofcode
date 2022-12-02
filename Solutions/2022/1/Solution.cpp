#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

static void unwashed(size_t part, std::string_view input) {
    /*auto ni = input | split("\n"sv) | transform([](auto v) { //
        return v | drop_while([](auto c) { return c == '+'; });
    }) | fwd_to_str;
    auto ci = input | split(","sv) | transform([](auto v) { //
        return v | drop_while([](auto c) { return c == '+'; });
    }) | fwd_to_str;

    std::ignore = ni;
    std::ignore = ci;

    auto nums = vec_from_view<i64>(ni | str_to_t<i64>);
    auto cnums = vec_from_view<i64>(ci | str_to_t<i64>);*/

    vec<vec<i64>> in {};
    vec<i64> staging {};
    for (str s : input | split("\n"sv) | fwd_to_str) {
        if (s.empty()) {
            in.emplace_back(std::move(staging));
            staging.clear();
            continue;
        }

        i64 a;
        std::from_chars(s.data(), s.data() + s.size(), a);
        staging.push_back(a);
    }
    in.emplace_back(std::move(staging));

    vec<i64> sums;
    for (auto v : in) {
        i64 s = 0;
        for (auto n : v)
            s += n;
        sums.push_back(s);
    }

    if (part == 1) {
        fmt::print("{}\n", *std::max_element(sums.cbegin(), sums.cend()));
    } else if (part == 2) {
        std::sort(sums.begin(), sums.end());
        auto it = sums.end();
        auto a = *(--it);
        auto b = *(--it);
        auto c = *(--it);
        fmt::print("{}\n", a + b + c);
    }
}

void Solution::solution(size_t part, std::string_view input) {
    auto groups = vec_from_view<i64>(input | split("\n\n"sv) | transform([](auto block) { //
        return fold_view<i64>(block | split("\n"sv) | fwd_to_strv | str_to_t<i64>);
    }));
    sort(begin(groups), end(groups));
    fmt::print("{}\n", fold_view<i64>(groups | reverse | take(part == 1 ? 1 : 3)));
}

void Solution::solve_example(size_t part) {
    std::string_view example_string = "1000\n"
                                      "2000\n"
                                      "3000\n"
                                      "\n"
                                      "4000\n"
                                      "\n"
                                      "5000\n"
                                      "6000\n"
                                      "\n"
                                      "7000\n"
                                      "8000\n"
                                      "9000\n"
                                      "\n"
                                      "10000"sv;
    solution(part, example_string);
}

void Solution::solve(size_t part) {
    const Stf::MMapStringView file(INPUT_FILE, false);
    solution(part, file);
}

}
