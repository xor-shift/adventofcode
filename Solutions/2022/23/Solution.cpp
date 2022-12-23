#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

std::string Solution::solve(size_t part, std::string_view input) {
    auto raw_grid = vec_from_view<vec<bool>>(input | split("\n"sv) | fwd_to_strv
      | transform([](auto l) { return vec_from_view<bool>(l | transform([](char c) { return c == '#'; })); }));

    pair<i64, i64> bounds { raw_grid.back().size(), raw_grid.size() };

    uset<pair<i64, i64>> elves;
    for (i64 y = 0; auto& row : raw_grid) {
        for (i64 x = 0; auto v : row) {
            if (v)
                elves.insert({ x, y });
            ++x;
        }
        ++y;
    }

    static const arr<arr<pair<i64, i64>, 3>, 4> offsets_2d_dir { {
      { { { -1, -1 }, { 0, -1 }, { 1, -1 } } }, //n
      { { { -1, 1 }, { 0, 1 }, { 1, 1 } } }, //s
      { { { -1, -1 }, { -1, 0 }, { -1, 1 } } }, //w
      { { { 1, -1 }, { 1, 0 }, { 1, 1 } } }, //e
    } };

    auto make_offsets_dir = [](pair<i64, i64> c, i64 dir, i64 round) {
        dir = (dir + round) % 4;

        arr<pair<i64, i64>, 3> ret;
        for (size_t i = 0; auto& [o_x, o_y] : offsets_2d_dir[dir]) {
            auto t = c;
            t.first += o_x;
            t.second += o_y;
            ret[i++] = t;
        }
        return ret;
    };

    /*auto in_bounds = [bounds](pair<i64, i64> c) {
        return !(c.first < 0 || c.second < 0 || c.first >= bounds.first || c.second >= bounds.second);
    };*/

    static const arr<pair<i64, i64>, 8> offsets_2d { {
      { 1, 0 },
      { 1, -1 },
      { 0, -1 },
      { -1, -1 },
      { -1, 0 },
      { -1, 1 },
      { 0, 1 },
      { 1, 1 },
    } };

    auto has_nbr = [&](pair<i64, i64> c) {
        for (auto [o_x, o_y] : offsets_2d) {
            auto t = c;
            t.first += o_x;
            t.second += o_y;

            if (/*in_bounds(t) && */elves.contains(t))
                return true;
        }
        return false;
    };

    auto iterate = [&](i64 round) {
        i64 s = 0;

        uset<pair<i64, i64>> new_elves;
        umap<pair<i64, i64>, pair<i64, i64>> came_from;

        for (auto elf : elves) {
            if (!has_nbr(elf)) {
                new_elves.insert(elf);
                continue;
            }

            bool did_a_thing = false;

            for (i64 d = 0; d < 4; d++) {
                auto offsets = make_offsets_dir(elf, d, round);

                bool viable = true; //in_bounds(offsets[1]);
                for (auto o_elf : offsets) {
                    /*if (!in_bounds(o_elf))
                        continue;*/
                    viable = viable && !elves.contains(o_elf);
                }

                if (!viable)
                    continue;

                if (auto it_new = new_elves.find(offsets[1]); it_new != new_elves.end()) {
                    auto it = came_from.find(offsets[1]);
                    if (it == came_from.end())
                        continue;

                    new_elves.erase(it_new);
                    new_elves.insert(it->second);
                    new_elves.insert(elf);
                    came_from.erase(it);

                    s -= 1;

                    did_a_thing = true;

                    break;
                }

                //nobody else wants the spot

                came_from[offsets[1]] = elf;
                new_elves.insert(offsets[1]);

                s += 1;

                did_a_thing = true;
                break;
            }

            if (!did_a_thing) {
                new_elves.insert(elf);
            }
        }

        elves = new_elves;

        return s;
    };

    auto print = [&] {
        for (i64 i = 0; i < bounds.second; i++) {
            for (i64 j = 0; j < bounds.first; j++) {
                fmt::print("{}", elves.contains({j, i}) ? '#' : '.');
            }
            fmt::print("\n");
        }
        fmt::print("\n");
    };

    auto elf_bounds = [&] {
        pair<pair<i64, i64>, pair<i64, i64>> b {{99999, -1}, {99999, -1}};

        auto &min_x = b.first.first;
        auto &max_x = b.first.second;

        auto &min_y = b.second.first;
        auto &max_y = b.second.second;

        for (auto [x, y] : elves) {
            min_x = std::min(min_x, x);
            max_x = std::max(max_x, x);

            min_y = std::min(min_y, y);
            max_y = std::max(max_y, y);
        }

        return b;
    };

    if (part == 1) {
        for (i64 r = 0; r < 10; r++) {
            iterate(r);
            //fmt::print("end of round {}\n", r + 1);
            //print();
        }

        auto b = elf_bounds();

        auto &min_x = b.first.first;
        auto &max_x = b.first.second;

        auto &min_y = b.second.first;
        auto &max_y = b.second.second;

        i64 s = 0;
        for (i64 y = min_y; y <= max_y; y++) {
            for (i64 x = min_x; x <= max_x; x++) {
                s += elves.contains({x, y});
            }
        }
        return std::to_string((max_x - min_x + 1) * (max_y - min_y + 1) - s);
    }

    if (part == 2) {
        for (i64 r = 0;; r++) {
            if (iterate(r) == 0) {
                return std::to_string(r + 1);
            }
        }
    }

    std::unreachable();
}

}
