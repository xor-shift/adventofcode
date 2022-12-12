#include <Common.hpp>

#include <queue>

namespace AoC {

Solution::Solution() { }

std::string Solution::solve(size_t part, std::string_view input) {
    numarr<int, 2> grid {};
    pair<isize, isize> start;
    pair<isize, isize> end;

    auto grid_width = 0;

    for (isize y = 0; auto line : input | split("\n"sv) | fwd_to_strv) {
        grid_width = line.size();
        for (isize x = 0; char c : line) {
            if (c == 'S') {
                start = std::make_pair(x, y);
                c = 'a';
            } else if (c == 'E') {
                end = std::make_pair(x, y);
                c = 'z';
            }

            grid[y][x] = c - 'a' + 1;

            ++x;
        }
        ++y;
    }

    if (part == 2) {
        std::ignore = part;
    }

    const auto grid_height = grid.size();

    using PQT = pair<pair<isize, isize>, isize>;
    using PQC = vec<PQT>;

    auto cmp = [](PQT const& lhs, PQT const& rhs) { return lhs.second < rhs.second; };
    std::ignore = cmp;

    using PQCMP = decltype(cmp);

    // std::priority_queue<PQT, PQC, PQCMP> pq {};
    std::queue<pair<isize, isize>> pq {};
    numarr<pair<isize, isize>, 2> came_from {};

    auto started_on = part == 1 ? start : end;
    pair<isize, isize> ended_up_on;

    pq.push(started_on);

    while (!pq.empty()) {
        auto cur = pq.front();
        auto cur_height = grid[cur.second][cur.first];
        pq.pop();

        if (part == 1) {
            if (cur == end) {
                ended_up_on = cur;
                break;
            }
        } else {
            if (cur_height == 1) {
                ended_up_on = cur;
                break;
            }
        }

        static constexpr arr<pair<isize, isize>, 4> offsets { {
          { -1, 0 },
          { 1, 0 },
          { 0, -1 },
          { 0, 1 },
        } };

        for (auto [o_x, o_y] : offsets) {
            auto next_x = o_x + cur.first;
            auto next_y = o_y + cur.second;
            auto next = std::make_pair(next_x, next_y);

            if (next_x < 0 || next_y < 0 || next_x >= grid_width || next_y >= grid_height)
                continue;

            auto new_height = grid[next_y][next_x];

            if (part == 1) {
                if (new_height - 1 > cur_height)
                    continue;
            } else {
                if (cur_height > new_height + 1)
                    continue;
            }

            auto& row = came_from[next_y];
            if (row.find(next_x) != row.end())
                continue;
            row[next_x] = cur;

            pq.push(next);
        }
    }

    auto cur = ended_up_on;
    i64 s = 0;
    for (;;) {
        auto& cur_row = came_from[cur.second];
        auto it = cur_row.find(cur.first);
        if (it == cur_row.end()) {
            fmt::print("what\n");
            break;
        }

        auto cf = it->second;

        ++s;
        cur = cf;

        if (cur == started_on)
            break;
    }

    fmt::print("{}\n", s); // not 896 p1

    return "";
}

}
