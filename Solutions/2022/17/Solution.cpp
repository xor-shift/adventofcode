#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

inline static const arr<vec<pair<i64, i64>>, 5> pieces { {
  { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } },
  { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 }, { 1, 2 } },
  { { 2, 2 }, { 2, 1 }, { 0, 0 }, { 1, 0 }, { 2, 0 } },
  { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } },
  { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } },
} };

enum class Dir { Right, Left, Down };

inline void offset_piece(vec<pair<i64, i64>>& piece, pair<i64, i64> offset) {
    for (auto& [x, y] : piece) {
        x += offset.first;
        y += offset.second;
    }
}

inline bool collides(vec<pair<i64, i64>> piece, numarr<bool, 2>& area) {
    for (auto [x, y] : piece) {
        if (area[y][x])
            return true;
    }
    return false;
}

inline opt<vec<pair<i64, i64>>> bump_peice(vec<pair<i64, i64>> piece, Dir direction, numarr<bool, 2>& area) {
    pair<i64, i64> offset;

    if (direction == Dir::Right) {
        offset = { 1, 0 };
    } else if (direction == Dir::Left) {
        offset = { -1, 0 };
    } else if (direction == Dir::Down) {
        offset = { 0, -1 };
    } else {
        std::unreachable();
    }

    offset_piece(piece, offset);

    if (any_of(begin(piece), end(piece), [](auto c) { return c.second < 0 || c.first < 0 || c.first >= 7; }))
        return std::nullopt;

    if (collides(piece, area))
        return std::nullopt;

    return piece;
}

inline void print_area(numarr<bool, 2>& area, i64 y_lim) {
    for (i64 i = 0; i <= y_lim; i++) {
        auto y = y_lim - i;
        fmt::print("|");
        for (i64 x = 0; x < 7; x++) {
            fmt::print("{}", area[y][x] ? '#' : '.');
        }
        fmt::print("|\n");
    }
}

std::string Solution::solve(size_t part, std::string_view input) {
    i64 piece_index = pieces.size() - 1;
    numarr<bool, 2> play_area;
    i64 high_point = -1;
    arr<i64, 7> high_points = { -1, -1, -1, -1, -1, -1, -1 };

    umap<tuple<arr<i64, 7>, i64, i64>, pair<usize, i64>> state_map {};
    i64 memo_y_sz = 0;

    usize action_ptr = input.size() - 1;

    auto round_ct = part == 1 ? 2022uz : 1000000000000uz;
    for (usize round = 0; round < round_ct; round++) {
        // print_area(play_area, high_point);

        auto cur_piece = pieces[(++piece_index) %= pieces.size()];
        for (auto& v : cur_piece) {
            v.first += 2;
            v.second += high_point + 4;
            // fmt::print("{}, {} | ", v.first, v.second);
        }
        // fmt::print("\n");

        std::ignore = std::fflush(stdout);

        for (;;) {
            bool right_act = input[(++action_ptr) %= input.size()] == '>';
            auto temp_piece = bump_peice(cur_piece, right_act ? Dir::Right : Dir::Left, play_area).value_or(cur_piece);
            auto bd_res = bump_peice(temp_piece, Dir::Down, play_area);
            if (!bd_res) {
                for (auto [x, y] : temp_piece) {
                    play_area[y][x] = true;
                }
                break;
            }
            cur_piece = *bd_res;
        }

        for (auto& [y, row] : play_area) {
            for (auto [x, val] : row) {
                if (val)
                    high_points[x] = std::max(y, high_points[x]);
            }
        }
        high_point = *std::max_element(begin(high_points), end(high_points));

        if (round == 2021uz)
            fmt::print("{}\n", high_point + 1);

        if (round < 2022uz)
             continue;

        arr<i64, 7> high_p_offsets;
        std::transform(begin(high_points), end(high_points), begin(high_p_offsets), [high_point](auto v) { return high_point - v; });
        tuple<arr<i64, 7>, i64, i64> cur_state_key { high_p_offsets, piece_index, action_ptr };
        pair<usize, i64> cur_state_val { round, high_point };

        auto it = state_map.find(cur_state_key);

        if (it == state_map.end()) {
            state_map.insert({cur_state_key, cur_state_val});
            continue;
        }

        fmt::print("okay, this is epic\n");

        auto [round_memo, high_point_memo] = it->second;

        auto single_memo_sz = round - round_memo;
        auto left_rnd = round_ct - round;
        auto memo_ct = left_rnd / single_memo_sz;

        round += memo_ct * single_memo_sz;
        memo_y_sz += memo_ct * (high_point - high_point_memo);

        state_map.clear();
    }

    fmt::print("{}\n", high_point + memo_y_sz + 1);

    return "";
}

}
