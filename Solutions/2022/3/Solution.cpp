#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

void Solution::solution(size_t part, std::string_view input) {
    auto score_of = [](char c) {
        return c - ((c >= 'a') ? 'a' - 1 : 'A' - 27);
    };

    if (part == 1) {
        auto sum = fold_view<i64>(input | split("\n"sv) | fwd_to_strv | transform([&score_of](auto sv) {
            set<char> fs(begin(sv), begin(sv) + sv.size() / 2);
            set<char> ss(begin(sv) + sv.size() / 2, end(sv));
            set<char> res {};

            set_intersection(begin(fs), end(fs), begin(ss), end(ss), inserter(res, begin(res)));

            return score_of(*res.begin());
        }));

        fmt::print("{}\n", sum);
    } else if (part == 2) {
        auto sum = fold_view<i64>(input | split("\n"sv) | fwd_to_strv | chunk(3) | transform([&](auto c) {
            vec<set<char>> sets;
            for (strview s : c) {
                set<char> set(begin(s), end(s));
                sets.emplace_back(std::move(set));
            }

            set<char> res = sets[0];
            for (auto cur : sets | drop(1)) {
                set<char> tmp {};
                set_intersection(begin(cur), end(cur), begin(res), end(res), inserter(tmp, begin(tmp)));
                res = std::move(tmp);
            }

            return score_of(*res.begin());
        }));

        fmt::print("{}\n", sum);
    }
}

void Solution::solve_example(size_t part) {
    std::string_view example_string = "vJrwpWtwJgWrhcsFMMfFFhFp\n"
                                      "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL\n"
                                      "PmmdzqPrVvPwwTWBwg\n"
                                      "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn\n"
                                      "ttgJtRGJQctTZtZT\n"
                                      "CrZsJsPPZsGzwwsLwLmpwMDw"sv;
    solution(part, example_string);
}

void Solution::solve(size_t part) {
    const Stf::MMapStringView file(INPUT_FILE, false);
    solution(part, file);
}

}
