#include <Common.hpp>

#include <queue>

namespace AoC {

Solution::Solution() { }

// see below for my original solution

constexpr auto make_offsets(arr<i64, 3> coords) {
    arr<arr<i64, 3>, 6> offsets_3d { {
      { 0, 1, 0 },
      { -1, 0, 0 },
      { 0, -1, 0 },
      { 1, 0, 0 },
      { 0, 0, 1 },
      { 0, 0, -1 },
    } };

    arr<arr<i64, 3>, 6> ret;
    fill(begin(ret), end(ret), coords);

    for (usize i = 0; i < offsets_3d.size(); i++) {
        ret[i][0] += offsets_3d[i][0];
        ret[i][1] += offsets_3d[i][1];
        ret[i][2] += offsets_3d[i][2];
    }

    return ret;
}

std::string Solution::solve(size_t part, std::string_view input) {
    auto input_view = input | split("\n"sv) | fwd_to_strv | transform([](auto l) { //
        return view_to_arr<i64, 3>(l | split(","sv) | fwd_to_strv | str_to_t<i64>);
    });

    uset<arr<i64, 3>> voxels(begin(input_view), end(input_view));
    uset<arr<i64, 3>> outside {};

    if (part == 2) {
        arr<pair<i64, i64>, 3> bounds;
        fill(begin(bounds), end(bounds), pair<i64, i64> { 9999, 0 });
        for (auto voxel : voxels) {
            for (usize i = 0; i < 3; i++) {
                bounds[i].first = std::min(bounds[i].first, voxel[i] - 1);
                bounds[i].second = std::max(bounds[i].second, voxel[i] + 1);
            }
        }

        auto in_bounds = [](arr<pair<i64, i64>, 3> bounds, arr<i64, 3> coords) {
            for (usize i = 0; i < 3; i++) {
                if (coords[i] > bounds[i].second || coords[i] < bounds[i].first)
                    return false;
            }
            return true;
        };

        for (std::deque<arr<i64, 3>> to_visit(1, { 0, 0, 0 }); !to_visit.empty();) {
            auto cur_coords = to_visit.front();
            to_visit.pop_front();

            if (outside.contains(cur_coords) || !in_bounds(bounds, cur_coords))
                continue;
            outside.insert(cur_coords);

            for (auto o_coords : make_offsets(cur_coords)) {
                auto it = voxels.find(o_coords);
                if (it == voxels.end()) {
                    to_visit.push_back(o_coords);
                }
            }
        }
    }

    i64 s = 0;
    for (auto voxel : voxels) {
        s += std::ranges::count_if(make_offsets(voxel), [&](auto o) { //
            return !voxels.contains(o) && (part != 2 || outside.contains(o));
        });
    }
    return std::to_string(s);
}

std::string unwashed(size_t part, std::string_view input) {
    uset<arr<i64, 3>> grid;

    auto cubes = vec_from_view<arr<i64, 3>>(input | split("\n"sv) | fwd_to_strv | transform([](auto l) { //
        return view_to_arr<i64, 3>(l | split(","sv) | fwd_to_strv | str_to_t<i64>);
    }));

    for (auto [x, y, z] : cubes) {
        grid.insert({ x, y, z });
    }

    uset<arr<i64, 3>> visited;

    arr<arr<i64, 3>, 6> offsets_3d { {
      { 0, 1, 0 },
      { -1, 0, 0 },
      { 0, -1, 0 },
      { 1, 0, 0 },
      { 0, 0, 1 },
      { 0, 0, -1 },
    } };

    auto in_bounds = [](arr<i64, 3> coords) {
        return std::all_of(begin(coords), end(coords), [](auto c) { //
            return c >= -2 && c <= 25;
        });
    };

    uset<arr<i64, 3>> outside;
    {
        std::deque<arr<i64, 3>> to_visit;
        to_visit.push_back({ 0, 0, 0 });
        for (i64 i = 0; !to_visit.empty() && i <= 9000001; i++) {
            auto cur_coords = to_visit.front();
            to_visit.pop_front();

            if (outside.contains(cur_coords) || !in_bounds(cur_coords))
                continue;
            outside.insert(cur_coords);

            for (auto offset : offsets_3d) {
                auto o_coords = cur_coords;
                o_coords[0] += offset[0];
                o_coords[1] += offset[1];
                o_coords[2] += offset[2];

                auto it = grid.find(o_coords);
                if (it == grid.end()) {
                    to_visit.push_back(o_coords);
                }
            }
        }
    }

    auto flood_fill = [&](arr<i64, 3> start_coord, bool part2 = false) {
        std::deque<arr<i64, 3>> to_visit;
        to_visit.push_back(start_coord);

        i64 s_visited = 0;
        i64 s_area = 0;

        while (!to_visit.empty()) {
            auto cur_coords = to_visit.front();
            to_visit.pop_front();

            if (visited.contains(cur_coords))
                continue;
            visited.insert(cur_coords);
            ++s_visited;

            for (auto offset : offsets_3d) {
                auto o_coords = cur_coords;
                o_coords[0] += offset[0];
                o_coords[1] += offset[1];
                o_coords[2] += offset[2];

                auto it = grid.find(o_coords);

                if (it == grid.end()) {
                    if (!part2) {
                        ++s_area;
                        continue;
                    }

                    if (outside.contains(o_coords)) {
                        ++s_area;
                        continue;
                    }
                }

                if (in_bounds(o_coords))
                    to_visit.push_back({ o_coords });
            }
        }

        return std::make_pair(s_area, s_visited);
    };

    i64 s = 0;
    if (part == 1) {
        for (auto& coord : grid) {
            s += flood_fill(coord).first;
        }
    } else if (part == 2) {
        for (auto& coord : grid) {
            s += flood_fill(coord, true).first;
        }
    }
    fmt::print("{}\n", s);

    return "";
}

}
