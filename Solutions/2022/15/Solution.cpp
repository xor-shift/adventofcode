#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

std::string Solution::solve(size_t part, std::string_view input) {
    enum class Tile {
        Sensor, Beacon
    };

    vec<pair<pair<i64, i64>, pair<i64, i64>>> raw_in;
    numarr<Tile, 2> grid;

    for (auto s : input | split("\n"sv) | fwd_to_str) {
        long long a, b, c, d;
        std::sscanf(s.c_str(), "Sensor at x=%lld, y=%lld: closest beacon is at x=%lld, y=%lld", //
          &a, &b, &c, &d);

        grid[b][a] = Tile::Sensor;
        grid[d][c] = Tile::Beacon;

        raw_in.push_back({{a, b}, {c, d}});
    }

    static bool example = true;

    if (part == 1) {
        i64 tgt_y = example ? 10 : 2'000'000;
        numarr<bool, 1> cover_map{};

        for (auto [sensor, closest_b] : raw_in) {
            auto ad_y = std::abs(closest_b.second - sensor.second);
            auto ad_x = std::abs(closest_b.first - sensor.first);

            auto diamond_dist = ad_x + ad_y;

            auto cover_at_y = [=, sensor = sensor](i64 y) -> i64 {
                auto d = std::abs(y - sensor.second);

                if (d > diamond_dist)
                    return 0;

                return 2 * (diamond_dist - d) + 1;
            };

            //fmt::print("{} {} -> {}\n", sensor.first, sensor.second, cover_at_y(sensor.second));
            auto cover = cover_at_y(tgt_y);
            auto hc = cover / 2;

            for (i64 i = -hc; i <= hc; i++) {
                i64 x = sensor.first + i;
                if (auto it = grid[tgt_y].find(x); it != grid[tgt_y].end()) {
                    if (it->second == Tile::Beacon)
                        continue;
                }
                cover_map[x] = true;
            }
        }

        fmt::print("{}\n", cover_map.size());

    } else if (part == 2) {
        i64 search_range = example ? 20 : 4000000;

        //vec<pair<i64, i64>> to_check;

        auto check = [&](pair<i64, i64> coords) {
            if (coords.first < 0 || coords.second < 0 || coords.first > search_range || coords.second > search_range)
                return false;

            for (auto [sensor, closest_b] : raw_in) {
                auto i = std::abs(sensor.first - closest_b.first) + std::abs(sensor.second - closest_b.second);
                auto j = std::abs(sensor.first - coords.first) + std::abs(sensor.second - coords.second);

                if (j < i)
                    return false;
            }

            fmt::print("!!!: {} {}\n", coords.first, coords.second);

            return true;
        };

        for (auto [sensor, closest_b] : raw_in) {
            auto ad_y = std::abs(closest_b.second - sensor.second);
            auto ad_x = std::abs(closest_b.first - sensor.first);

            auto diamond_dist = ad_x + ad_y;

            auto cover_at_y = [=, sensor = sensor](i64 y) -> i64 {
                auto d = std::abs(y - sensor.second);

                if (d > diamond_dist)
                    return 0;

                return 2 * (diamond_dist - d) + 1;
            };

            auto cover = cover_at_y(sensor.second);
            auto hc = cover / 2;

            check({sensor.first, sensor.second - hc - 1});
            check({sensor.first, sensor.second + hc + 1});

            for (i64 i = -hc; i <= hc; i++) {
                auto y = sensor.second + i;

                auto curcov = cover_at_y(y);
                auto curhc = curcov / 2;

                auto x_0 = sensor.first - curhc - 1;
                auto x_1 = sensor.first + curhc + 1;

                check({x_0, y});
                check({x_1, y});
            }
        }

        example = false;
    }

    return "";
}

}
