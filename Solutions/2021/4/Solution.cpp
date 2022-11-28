#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

void Solution::solution(size_t part, std::string_view input) {
    auto a = input | split("\n"sv) //
      | drop(1)                    //
      | fwd_to_str                 //
      | chunk(6)                   //
      | transform([](auto c) {
            auto a = c  //
              | drop(1) //
              | transform([](auto line) {
                    auto a = line    //
                      | split(" "sv) //
                      | filter([](auto v) { return ranges::distance(v.begin(), v.end()) != 0; }) | fwd_to_str
                      | str_to_t<i32>;
                    return vec<i32>(a.begin(), a.end());
                });
            return vec_from_view<vec<i32>>(a);
        });

    vec<vec<vec<i32>>> boards = vec_from_view<vec<vec<i32>>>(a);

    std::string b = *(input | split("\n"sv) | take(1) | fwd_to_str).begin();
    vec<i32> draws = vec_from_view<i32>(b | split(',') | fwd_to_str | str_to_t<i32>);

    auto check_win = [](vec<vec<i32>>& board) {
        for (auto& row : board) {
            if (std::ranges::all_of(row, [](auto v) { return v == -1; }))
                return true;
        }

        for (size_t c = 0; c < board.front().size(); c++) {
            size_t ct = 0;
            for (size_t r = 0; r < board.size(); r++)
                ct += board[r][c] == -1;
            if (ct == board.size())
                return true;
        }

        /*size_t ct[2] = { 0, 0 };
        for (size_t i = 0; i < board.size(); i++) {
            ct[0] += board[i][i] == -1;
            ct[1] += board[i][board.size() - i - 1] == -1;
        }
        if (ct[0] == board.size() || ct[1] == board.size())
            return true;*/

        return false;
    };

    auto do_draw = [](vec<vec<i32>>& board, i32 d) {
        for (auto& row : board) {
            for (auto& v : row) {
                if (v == d)
                    v = -1;
            }
        }
    };

    auto board_pts = [](vec<vec<i32>>& board) {
        i32 sum = 0;
        for (auto& row : board) {
            for (auto& v : row) {
                if (v != -1)
                    sum += v;
            }
        }
        return sum;
    };

    uset<usize> wins {};
    vec<tuple<usize, i32, usize>> win_log {};
    for (usize round = 0; auto draw : draws) {
        for (usize i = 0; auto& board : boards) {
            ++i;
            do_draw(board, draw);
            if (!check_win(board) || wins.contains(i - 1))
                continue;

            wins.insert(i - 1);
            // fmt::print("{} wins with a score of {} in round {}\n", i, board_pts(board) * draw, round);
            win_log.push_back(std::make_tuple(i, board_pts(board) * draw, round));
        }
        ++round;
    }

    if (part == 1) {
        fmt::print("{}\n", get<1>(win_log.front()));
    } else if (part == 2) {
        fmt::print("{}\n", get<1>(win_log.back()));
    }
}

void Solution::solve_example(size_t part) {
    std::string_view example_string = "7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1\n"
                                      "\n"
                                      "22 13 17 11  0\n"
                                      " 8  2 23  4 24\n"
                                      "21  9 14 16  7\n"
                                      " 6 10  3 18  5\n"
                                      " 1 12 20 15 19\n"
                                      "\n"
                                      " 3 15  0  2 22\n"
                                      " 9 18 13 17  5\n"
                                      "19  8  7 25 23\n"
                                      "20 11 10 24  4\n"
                                      "14 21 16 12  6\n"
                                      "\n"
                                      "14 21 17 24  4\n"
                                      "10 16 15  9 19\n"
                                      "18  8 23 26 20\n"
                                      "22 11 13  6  5\n"
                                      " 2  0 12  3  7"sv;
    solution(part, example_string);
}

void Solution::solve(size_t part) {
    const Stf::MMapStringView file(INPUT_FILE, false);
    solution(part, file);
}

}
