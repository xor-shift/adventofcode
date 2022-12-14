#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

std::string Solution::solve(size_t part, std::string_view input) {
    auto a = input | split("\n"sv) | fwd_to_strv | transform([](auto line) {
        return vec_from_view<pair<i64, i64>>(line | split(" -> "sv) | fwd_to_strv
          | transform([](auto p) { return view_to_pair<i64, i64>(p | split(","sv) | fwd_to_strv | str_to_t<i64>); }));
    });

    numarr<bool, 2> solids {};
    isize height = 0;

    auto draw_line = [](numarr<bool, 2>& grid, pair<i64, i64> start, pair<i64, i64> end) {
        auto [start_x, start_y] = start;
        auto [end_x, end_y] = end;

        auto d_y = end_y - start_y;
        auto ad_y = std::abs(d_y);
        auto d_x = end_x - start_x;
        auto ad_x = std::abs(d_x);

        if (start_x == end_x) {
            for (isize i = 0; i <= ad_y; i++) {
                auto o = (d_y > 0 ? 1 : -1) * i;
                grid[o + start_y][start_x] = true;
            }
        } else {
            for (isize i = 0; i <= ad_x; i++) {
                auto o = (d_x > 0 ? 1 : -1) * i;
                grid[start_y][o + start_x] = true;
            }
        }
    };

    for (auto rock : a) {
        auto prev = rock[0];
        height = std::max(height, prev.second);
        for (auto segment : rock | drop(1)) {
            auto next = segment;
            draw_line(solids, prev, next);
            prev = next;
            height = std::max(height, prev.second);
        }
    }

    auto try_move = [](numarr<bool, 2>& grid, pair<i64, i64> coords) -> pair<isize, isize> {
        auto it_row = grid.find(coords.second + 1);
        if (it_row == grid.end()) {
            return std::make_pair(coords.first, coords.second + 1);
        }

        auto& row = it_row->second;

        auto it_l = row.find(coords.first - 1);
        auto it_m = row.find(coords.first);
        auto it_r = row.find(coords.first + 1);

        auto l_free = it_l == row.end() || !it_l->second;
        auto m_free = it_m == row.end() || !it_m->second;
        auto r_free = it_r == row.end() || !it_r->second;

        if (m_free)
            return std::make_pair(coords.first, coords.second + 1);

        if (l_free)
            return std::make_pair(coords.first - 1, coords.second + 1);

        if (r_free)
            return std::make_pair(coords.first + 1, coords.second + 1);

        return coords;
    };

    auto staring_at_the_abyss = [](numarr<bool, 2>& grid, pair<i64, i64> coords) {
        for (auto& [y, row] : grid) {
            if (y <= coords.second)
                continue;

            auto it = row.find(coords.first);
            if (it == row.end() || !it->second)
                continue;

            if (it->second)
                return false;
        }
        return true;
    };

    auto try_place_sand = [try_move, staring_at_the_abyss](numarr<bool, 2>& grid, pair<i64, i64> coords = {500, 0}) {
        for (;;) {
            auto new_coords = try_move(grid, coords);

            if (auto it_r = grid.find(new_coords.second); it_r != grid.end()) {
                auto& row = it_r->second;
                if (auto it_c = row.find(new_coords.first); it_c != row.end()) {
                    if (it_c->second)
                        break;
                }
            }

            //fmt::print("moving from {}, {} to {}, {}\n", coords.first, coords.second, new_coords.first, new_coords.second);

            if (new_coords == coords)
                break;

            if (staring_at_the_abyss(grid, new_coords))
                return false;

            coords = new_coords;
        }

        grid[coords.second][coords.first] = true;
        return true;
    };

    auto draw = [](numarr<bool, 2>& grid, pair<i64, i64> coords, pair<i64, i64> range) {
        for (isize i = 0; i < range.second; i++) {
            for (isize j = 0; j < range.first; j++) {
                auto b = grid[i + coords.second][j + coords.first];
                if (b)
                    fmt::print("#");
                else
                    fmt::print(".");
            }
            fmt::print("\n");
        }
    };

    //fmt::print("{}\n", try_place_sand(solids));

    if (part == 2) {
        draw_line(solids, {-1000, height + 2}, {1000, height + 2});
    }

    i64 s = 0;
    //draw(solids, {490, 0}, {15, 15});
    while(try_place_sand(solids)) {
        ++s;
        if (solids[0][500])
            break;
        //draw(solids, {490, 0}, {15, 15});
        //fmt::print("\n");
    }
    fmt::print("{}\n", s);

    if (part == 1) {
        //std::exit(0);
    } else if (part == 2) {
    }

    return "";
}

}
