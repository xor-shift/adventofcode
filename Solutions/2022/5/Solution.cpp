#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

static vec<vec<char>> parse_crates(strview input) {
    vec<vec<char>> ret{};

    auto a = input | split("\n"sv) | fwd_to_strv | transform([](auto line) {
        return line | chunk(4) | fwd_to_strv | transform([](auto c) -> char {
            if ('1' <= c[1] && c[1] <= '9')
                return c[1] - '0';

            if (c[0] == ' ')
                return '\0';

            return c[1];
        });
    });

    for (auto line : a) {
        ret.reserve(distance(line));
        for (auto i = ret.size(); i < distance(line); i++, ret.emplace_back());
        for (auto i = 0uz;char c : line) {
            i++;

            if (c <= '9')
                continue;

            ret[i-1].push_back(c);
        }
    }

    for (auto& v : ret) {
        std::reverse(begin(v), end(v));
    }

    return ret;
}

void Solution::solution(size_t part, std::string_view input) {
    auto input_sections = view_to_pair<strview, strview>(input | split("\n\n"sv) | fwd_to_strv);
    vec<vec<char>> crates = parse_crates(input_sections.first);

    vec<arr<long, 3>> actions {};
    for (strview sv : input_sections.second | split("\n"sv) | fwd_to_strv) {
        str s(sv);
        auto& x = actions.emplace_back();
        std::sscanf(s.c_str(), "move %ld from %ld to %ld", &x[0], &x[1], &x[2]);
    }

    for (auto [amt, ifrom, ito] : actions) {
        auto& from = crates[ifrom - 1];
        auto& to = crates[ito - 1];

        if (part == 1)
            copy(from.rend() - amt - 1, from.rend() - 1, back_inserter(to));
        else
            copy(from.end() - amt, from.end(), back_inserter(to));

        from.erase(from.end() - amt, from.end());
    }

    for (auto& stack : crates) {
        if (stack.empty())
            continue;
        fmt::print("{}", stack.back());
    }
    fmt::print("\n");
}

void Solution::solve_example(size_t part) {
    std::string_view example_string = "    [D]\n"
                                      "[N] [C]\n"
                                      "[Z] [M] [P]\n"
                                      " 1   2   3 \n"
                                      "\n"
                                      "move 1 from 2 to 1\n"
                                      "move 3 from 1 to 3\n"
                                      "move 2 from 2 to 1\n"
                                      "move 1 from 1 to 2"sv;

    solution(part, example_string);
}

void Solution::solve(size_t part) {
    const Stf::MMapStringView file(INPUT_FILE, false);
    solution(part, file);
}
}
