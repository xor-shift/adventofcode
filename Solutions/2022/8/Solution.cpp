#include <Common.hpp>

#include <queue>

namespace AoC {

Solution::Solution() { }

void Solution::solve(bool example, size_t part, std::string_view input) {
    if (example)
        input = "30373\n"
                "25512\n"
                "65332\n"
                "33549\n"
                "35390"sv;

    auto grid = vec_from_view<strview>(input | split("\n"sv) | fwd_to_strv);
    auto ssz = static_cast<isize>(grid.size());

    enum class Dir { N, W, S, E };

    numarr<bool, 2> visibility {};
    auto cast_ray = [&](isize i_c, isize i_r, Dir dir) {
        isize r = i_r, c = i_c;
        isize prev = -1;

        auto inc = [&] {
            switch (dir) {
            case Dir::N: r -= 1; break;
            case Dir::S: r += 1; break;
            case Dir::E: c -= 1; break;
            case Dir::W: c += 1; break;
            }
        };

        const auto initial = grid[i_r][i_c];
        bool blocked = false;
        isize ct = 0;

        for (isize i = 0; i < ssz; i++) {
            if (i == 0) {
                inc();
                continue;
            }

            if (r < 0 || c < 0 || r >= ssz || c >= ssz)
                break;

            auto val = grid[r][c];
            bool visible = val > prev;

            if (!blocked)
                ++ct;
            blocked |= val >= initial;

            if (visible)
                prev = val;

            if (!visibility[r][c] && visible) {
                // fmt::print("{} from {}, {} is visible\n", val, c, r);
            }

            visibility[r][c] |= visible;

            inc();
        }

        return ct;
    };

    if (part == 3) {
        numarr<bool, 2> visibility {};
        numarr<bool, 2> checked {};
        std::queue<pair<isize, isize>> to_search;
        for (auto i = 0uz; i < ssz; i++) {
            auto p0 = std::make_pair(i, 0z);
            auto p1 = std::make_pair(0z, i);
            auto p2 = std::make_pair(i, ssz - 1);
            auto p3 = std::make_pair(ssz - 1, i);

            /*visibility[p0.second][p0.first] = true;
            visibility[p1.second][p1.first] = true;
            visibility[p2.second][p2.first] = true;
            visibility[p3.second][p3.first] = true;*/

            to_search.push(p0);
            to_search.push(p1);
            if (i == grid.size() - 1)
                continue;

            to_search.push(p2);
            to_search.push(p3);
        }

        while (!to_search.empty()) {
            auto [s_c, s_r] = to_search.front();
            to_search.pop();

            if (checked[s_r][s_c])
                continue;

            checked[s_r][s_c] = true;
            visibility[s_r][s_c] = true;
            const auto val = grid[s_r][s_c];

            // if (s_c != 0 && s_r != 0 && s_c != ssz - 1 && s_r != ssz - 1)
            // fmt::print("{} from {}, {} is visible\n", val, s_c, s_r);

            for (isize o_c = -1; o_c <= 1; o_c++) {
                for (isize o_r = -1; o_r <= 1; o_r++) {
                    auto n_c = o_c + s_c;
                    auto n_r = o_r + s_r;

                    if (o_c == o_r || (o_c != 0 && o_r != 0))
                        continue;

                    if (n_c < 0 || n_r < 0 || n_c >= ssz || n_r >= ssz)
                        continue;

                    if (val >= grid[n_r][n_c])
                        continue;

                    if (checked[n_r][n_c])
                        continue;

                    to_search.push(std::make_pair(n_c, n_r));
                }
            }
        }

        i64 s = 0;
        for (auto& [y, row] : visibility) {
            for (auto [x, visible] : row) {
                s += visible;
            }
        }
        fmt::print("{}\n", s);
    } else if (part == 1) {

        auto p = [&] {
            for (isize r = 0; r < ssz; r++) {
                for (isize c = 0; c < ssz; c++) {
                    if (visibility[r][c])
                        fmt::print("#");
                    else
                        fmt::print(".");
                }
                fmt::print("\n");
            }
            fmt::print("\n");
        };

        for (isize i = 0; i < ssz; i++) {
            cast_ray(0, i, Dir::W);
            cast_ray(i, 0, Dir::S);
            cast_ray(ssz - 1, i, Dir::E);
            cast_ray(i, ssz - 1, Dir::N);
        }

        i64 s = 0;
        for (auto& [y, row] : visibility) {
            for (auto [x, visible] : row) {
                s += visible;
            }
        }
        fmt::print("{}\n", s);
    } else if (part == 2) {
        for (isize i = 0; i < ssz; i++) {
            cast_ray(0, i, Dir::W);
            cast_ray(i, 0, Dir::S);
            cast_ray(ssz - 1, i, Dir::E);
            cast_ray(i, ssz - 1, Dir::N);
        }

        i64 s = 0;
        for (auto& [y, row] : visibility) {
            for (auto [x, visible] : row) {
                arr<isize, 4> a;

                a[0] = cast_ray(x, y, Dir::W);
                a[1] = cast_ray(x, y, Dir::S);
                a[2] = cast_ray(x, y, Dir::E);
                a[3] = cast_ray(x, y, Dir::N);

                auto t = std::reduce(begin(a), end(a), 1z, std::multiplies {});
                s = std::max(s, t);
                // fmt::print("{} {} {} {}", a[0], a[1], a[2], a[3]);
            }
        }
        fmt::print("{}\n", s);
    }
}

}
