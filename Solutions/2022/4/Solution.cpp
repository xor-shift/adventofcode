#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

void unwashed(size_t part, std::string_view input) {
    auto in = input | split("\n"sv) | fwd_to_strv | transform([](auto v) {
        auto a = v | split(","sv) | fwd_to_strv | transform([](auto r) {
            auto a = r | split("-"sv) | fwd_to_strv;
            auto it = a.begin();
            i64 beg, end;
            std::from_chars((*it).data(), (*it).data() + (*it).size(), beg);
            ++it;
            std::from_chars((*it).data(), (*it).data() + (*it).size(), end);
            return std::make_pair(beg, end);
        });
        auto it = a.begin();
        pair<i64, i64> b = *it++;
        pair<i64, i64> c = *it;
        return std::make_pair(b, c);
    });

    auto contains = [](pair<i64, i64> larger, pair<i64, i64> smaller) -> bool {
        if (larger.first > smaller.first)
            return false;
        if (larger.second < smaller.second)
            return false;
        return true;
    };

    auto overlap = [](pair<i64, i64> a, pair<i64, i64> b) -> i64 {
        if (b.first > a.second || a.first > b.second)
            return 0;

        auto t0 = std::max(a.first, b.first);
        auto t1 = std::min(a.second, b.second);

        return t1 - t0 + 1;
    };

    i64 s = 0;
    if (part == 1) {
        s = fold_view<i64>(in | transform([&](auto p) {
            auto [a, b] = p;
            return static_cast<i64>(contains(a, b) || contains(b, a));
        }));
    } else if (part == 2) {
        s = fold_view<i64>(in | transform([&](auto p) { return static_cast<i64>(overlap(p.first, p.second) > 0); }));
    }
    fmt::print("{}\n", s);
}

void Solution::solution(size_t part, std::string_view input) {
    auto a = input | split("\n"sv) | fwd_to_str | transform([part](auto s) {
        long in[4];
        std::sscanf(s.c_str(), "%ld-%ld,%ld-%ld", in + 0, in + 1, in + 2, in + 3);

        bool ok = false;

        if (part == 1) {
            ok |= !(in[0] > in[2]) && !(in[1] < in[3]);
            ok |= !(in[2] > in[0]) && !(in[3] < in[1]);
        } else {
            ok = !(in[2] > in[1] || in[0] > in[3]);
        }

        return static_cast<i64>(ok);
    });

    fmt::print("{}\n", fold_view<i64>(a));
}

void Solution::solve_example(size_t part) {
    std::string_view example_string = "2-4,6-8\n"
                                      "2-3,4-5\n"
                                      "5-7,7-9\n"
                                      "2-8,3-7\n"
                                      "6-6,4-6\n"
                                      "2-6,4-8"sv;
    solution(part, example_string);
}

void Solution::solve(size_t part) {
    const Stf::MMapStringView file(INPUT_FILE, false);
    solution(part, file);
}

}
