#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

std::string Solution::solve(size_t part, std::string_view input) {
    static bool s_example = true;
    bool is_example = s_example;
    s_example ^= s_example && part == 2;

    if (is_example && part == 2) {
        return "";
    }

    const auto quadrant_dim = is_example ? 4 : 50;

    uset<pair<i64, i64>> bounds {};

    pair<pair<i64, i64>, pair<i64, i64>> bounds_span {};
    umap<i64, pair<i64, i64>> row_spans {};
    umap<i64, pair<i64, i64>> col_spans {};

    uset<pair<i64, i64>> walls {};

    auto lines = vec_from_view<strview>(input | split("\n"sv) | fwd_to_strv | transform([](auto l) { //
        return l == "\n" ? "" : l;
    }));

    for (auto y = 0z; auto l : lines) {
        if (l.empty())
            break;

        for (auto x = 0z; auto c : l) {
            if (c == ' ') {
                ++x;
                continue;
            }

            if (c == '#')
                walls.insert({ x, y });
            bounds.insert({ x, y });

            bounds_span.first.first = std::min(x, bounds_span.first.first);
            bounds_span.first.second = std::max(x, bounds_span.first.second);
            bounds_span.second.first = std::min(y, bounds_span.second.first);
            bounds_span.second.second = std::max(y, bounds_span.second.second);

            if (auto it = row_spans.find(y); it != row_spans.end()) {
                it->second.first = std::min(x, it->second.first);
                it->second.second = std::max(x, it->second.second);
            } else {
                row_spans.insert({ y, { x, x } });
            }

            if (auto it = col_spans.find(x); it != col_spans.end()) {
                it->second.first = std::min(y, it->second.first);
                it->second.second = std::max(y, it->second.second);
            } else {
                col_spans.insert({ x, { y, y } });
            }

            ++x;
        }
        ++y;
    }

    vec<pair<i64, char>> actions;

    /*
     * NOTICE: ADD A SINGLE 'R' TO YOUR INPUT'S LAST LINE
     */

    for (auto *ptr = lines.back().data(), *end = lines.back().data() + lines.back().size(); ptr != end;) {
        i64 amt;
        auto res = std::from_chars(ptr, end, amt);
        if (res.ec != std::errc())
            std::unreachable();
        ptr = res.ptr;
        char dir = *ptr++;

        actions.emplace_back(amt, dir);
    }

    auto modulo = []<typename T>(T v, T n) { return (v % n + n) % n; };

    auto hdg_char = [modulo](i64 hdg) {
        switch (modulo(hdg, 4z)) {
        case 0: return '>';
        case 1: return 'v';
        case 2: return '<';
        case 3: return '^';
        default: std::unreachable();
        }
    };

    /* ccw -> -1, cw -> +1
     * 0 -> right
     * 1 -> down
     * 2 -> left
     * 3 -> up
     */
    static const arr<pair<i64, i64>, 4> hdg_offsets { {
      { 1, 0 },
      { 0, 1 },
      { -1, 0 },
      { 0, -1 },
    } };

    pair<i64, i64> pos { row_spans.at(0).first, 0 };
    i64 heading = 0;

    auto wrap_p1 = [&bounds, &row_spans, &col_spans](pair<i64, i64> coord, i64 heading) {
        auto in_bounds = bounds.contains(coord);

        if (in_bounds)
            return coord;

        switch (heading) {
        case 0: coord.first = row_spans.at(coord.second).first; break;
        case 1: coord.second = col_spans.at(coord.first).first; break;
        case 2: coord.first = row_spans.at(coord.second).second; break;
        case 3: coord.second = col_spans.at(coord.first).second; break;
        default: std::unreachable();
        }

        return coord;
    };

    /*
     *  ##
     *  #
     * ##
     * #
     *
     */

    auto wrap_p2 = [is_example, quadrant_dim](pair<i64, i64> coord, i64 heading) -> pair<pair<i64, i64>, i64> {
        auto quadrant_x = coord.first / quadrant_dim;
        if (coord.first < 0)
            quadrant_x -= 1;

        auto quadrant_y = coord.second / quadrant_dim;
        if (coord.second < 0)
            quadrant_y -= 1;

        arr<arr<pair<i64, pair<pair<i64, i64>, i64>>, 2>, 3> ver_wr { {
          { { { 2, { { 50, coord.first + 50 }, 0 } }, { 3, { { coord.first + 100, 0 }, 1 } } } },
          { { { 0, { { 0, coord.first + 100 }, 0 } }, { 2, { { 49, coord.first + 100 }, 2 } } } },
          { { { 0, { { coord.first - 100, 199 }, 3 } }, { 0, { { 99, coord.first - 50 }, 2 } } } },
        } };

        if ((heading == 1 || heading == 3) && (quadrant_x >= 0 && quadrant_x <= 2)) {
            auto [min, max] = ver_wr[quadrant_x];
            if (quadrant_y < min.first)
                return min.second;
            if (quadrant_y > max.first)
                return max.second;
            return { coord, heading };
        }

        arr<arr<pair<i64, pair<pair<i64, i64>, i64>>, 2>, 4> hor_wr { {
          { { { 1, { { 0, 149 - coord.second }, 0 } }, { 2, { { 99, 149 - coord.second }, 2 } } } },
          { { { 1, { { coord.second - 50, 100 }, 1 } }, { 1, { { coord.second + 50, 49 }, 3 } } } },
          { { { 0, { { 50, 149 - coord.second }, 0 } }, { 1, { { 149, 149 - coord.second }, 2 } } } },
          { { { 0, { { coord.second - 100, 0 }, 1 } }, { 0, { { coord.second - 100, 149 }, 3 } } } },
        } };

        if ((heading == 0 || heading == 2) && (quadrant_y >= 0 && quadrant_y <= 3)) {
            auto [min, max] = hor_wr[quadrant_y];
            if (quadrant_x < min.first)
                return min.second;
            if (quadrant_x > max.first)
                return max.second;
            return { coord, heading };
        }

    };

    auto wrap = [part, wrap_p1, wrap_p2](pair<i64, i64> coord, i64 heading) -> pair<pair<i64, i64>, i64> {
        if (part == 1) {
            return { wrap_p1(coord, heading), heading };
        }

        if (part == 2) {
            return wrap_p2(coord, heading);
        }

        std::unreachable();
    };

    // fmt::print("{}, {}\n", pos.first, pos.second);
    for (auto [go_amt, rot_dir] : actions) {
        for (i64 i = 0; i < go_amt; i++) {
            auto [o_x, o_y] = hdg_offsets[heading];
            auto candidate_pos = pos;
            candidate_pos.first += o_x;
            candidate_pos.second += o_y;
            auto wrap_res = wrap(candidate_pos, heading);
            candidate_pos = wrap_res.first;
            auto candidate_hdg = wrap_res.second;

            // fmt::print("{}: {}, {} ({}, {})\n", hdg_char(heading), pos.first, pos.second, candidate_pos.first,
            //   candidate_pos.second);

            if (walls.contains(candidate_pos))
                break;

            pos = candidate_pos;
            heading = candidate_hdg;
        }
        heading = modulo(heading + (rot_dir == 'R' ? 1z : -1z), 4z);
    }
    heading = modulo(heading - 1, 4z);
    // fmt::print("{}, {}\n", pos.first, pos.second);

    // std::exit(0);

    return std::to_string(1000 * (pos.second + 1) + 4 * (pos.first + 1) + heading);
}

}
