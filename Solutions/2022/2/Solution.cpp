#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

enum class Selection : i64 { Rock = 1, Paper = 2, Scissors = 3 };

inline Selection sel_from_char(char c) {
    switch (c) {
    case 'X': [[fallthrough]];
    case 'A': return Selection::Rock;
    case 'Y': [[fallthrough]];
    case 'B': return Selection::Paper;
    case 'Z': [[fallthrough]];
    case 'C': return Selection::Scissors;
    }

    std::unreachable();
}

enum class Status : i64 { Lost = 0, Draw = 3, Won = 6 };

inline Status status_from_char(char c) {
    switch (c) {
    case 'X': return Status::Lost;
    case 'Y': return Status::Draw;
    case 'Z': return Status::Won;
    }

    std::unreachable();
}

inline Status decide_status(Selection played, Selection got) {
    Status statuses[3][3] {
        { Status::Draw, Status::Lost, Status::Won },
        { Status::Won, Status::Draw, Status::Lost },
        { Status::Lost, Status::Won, Status::Draw },
    };
    return statuses[static_cast<i64>(played) - 1][static_cast<i64>(got) - 1];
}

void unwashed(size_t part, std::string_view input) {
    i64 s = 0;

    auto in = input | split("\n"sv) | fwd_to_strv | transform([](auto sv) { return pair<char, char>(sv[0], sv[2]); });

    if (part == 1) {
        auto strategies = in | transform([](auto p) {
            return pair<Selection, Selection>(sel_from_char(p.first), sel_from_char(p.second));
        });

        for (auto [got, played] : strategies) {
            auto status = decide_status(played, got);
            s += static_cast<i64>(status);
            s += static_cast<i64>(played);
        }
    } else if (part == 2) {
        auto results = in | transform([](auto p) {
            return pair<Selection, Status>(sel_from_char(p.first), status_from_char(p.second));
        });

        for (auto [got, result] : results) {
            Selection sels[3] { Selection::Rock, Selection::Paper, Selection::Scissors };
            auto a = sels
              | transform([got = got](auto s) { return std::pair<Selection, Status>(s, decide_status(s, got)); });
            auto played = (*std::ranges::find_if(a, [result = result](auto p) {
                auto [_, r] = p;
                return r == result;
            })).first;
            s += static_cast<i64>(result) + static_cast<i64>(played);
        }
    }

    fmt::print("{}\n", s);
}

void Solution::solution(size_t part, std::string_view input) {
    umap<strview, arr<i64, 2>> values {
        { "A X", { 4, 3 } },
        { "A Y", { 8, 4 } },
        { "A Z", { 3, 8 } },
        { "B X", { 1, 1 } },
        { "B Y", { 5, 5 } },
        { "B Z", { 9, 9 } },
        { "C X", { 7, 2 } },
        { "C Y", { 2, 6 } },
        { "C Z", { 6, 7 } },
    };

    fmt::print("{}\n", fold_view<i64>(input | split("\n"sv) | fwd_to_strv | transform([&](auto sv) { //
        return values.at(sv)[part-1];
    })));
}

void Solution::solve_example(size_t part) {
    std::string_view example_string = "A Y\n"
                                      "B X\n"
                                      "C Z"sv;
    solution(part, example_string);
}

void Solution::solve(size_t part) {
    const Stf::MMapStringView file(INPUT_FILE, false);
    solution(part, file);
}

}
