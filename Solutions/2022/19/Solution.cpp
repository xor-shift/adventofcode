#include <Common.hpp>

#include <queue>

namespace AoC {

Solution::Solution() { }

inline static constexpr size_t ore_index = 0;
inline static constexpr size_t clay_index = 1;
inline static constexpr size_t obsidian_index = 2;
inline static constexpr size_t geode_index = 3;

struct Cost {
    arr<i64, 4> costs;

    constexpr auto& ore() { return costs[0]; }
    constexpr auto const& ore() const { return costs[0]; }
    constexpr auto& clay() { return costs[1]; }
    constexpr auto const& clay() const { return costs[1]; }
    constexpr auto& obsidian() { return costs[2]; }
    constexpr auto const& obsidian() const { return costs[2]; }
};

struct Blueprint {
    arr<Cost, 4> costs;

    constexpr auto& ore() { return costs[0]; }
    constexpr auto const& ore() const { return costs[0]; }
    constexpr auto& clay() { return costs[1]; }
    constexpr auto const& clay() const { return costs[1]; }
    constexpr auto& obsidian() { return costs[2]; }
    constexpr auto const& obsidian() const { return costs[2]; }
    constexpr auto& geode() { return costs[3]; }
    constexpr auto const& geode() const { return costs[3]; }
};

struct State {
    i64 minute = 0;
    arr<i64, 4> robots {};
    arr<i64, 4> resources {};
    arr<i64, 4> total_resources {};

    i64 val() const {
        i64 r = 0;
        for (i64 i = 0, m = 1; i < 4; i++, m *= 10) {
            r += total_resources[i] * m;
        }
        return r;
    }

    friend bool operator<(State const& lhs, State const& rhs) { return lhs.val() < rhs.val(); }
};

static i64 fn(Blueprint bp, size_t total_time, size_t max_depth = 1000) {
    std::deque<State> pq;
    pq.push_back({
      .robots{1, 0, 0, 0},
    });

    i64 ret = 0;

    for (i64 max_min = 0; !pq.empty();) {
        auto cur_state = pq.front();
        pq.pop_front();

        if (cur_state.minute > max_min) {
            sort(begin(pq), end(pq));
            while (pq.size() > max_depth)
                pq.pop_front();
            pq.shrink_to_fit();
            max_min = cur_state.minute;
        }

        if (cur_state.minute == total_time) {
            ret = std::max(ret, cur_state.total_resources[geode_index]);
            continue;
        }

        State next_state_base {
            .minute = cur_state.minute + 1,
            .robots = cur_state.robots,
            .resources = cur_state.resources,
            .total_resources = cur_state.total_resources,
        };

        for (usize i = 0; i <= geode_index; i++) {
            next_state_base.resources[i] += cur_state.robots[i];
            next_state_base.total_resources[i] += cur_state.robots[i];
        }

        pq.push_back(next_state_base);

        for (usize robot_idx = 0; robot_idx <= geode_index; robot_idx++) {
            const auto robot_costs = bp.costs[robot_idx];

            State next_state = next_state_base;
            next_state.robots[robot_idx]++;

            bool can_afford = true;
            for (usize resource_idx = 0; resource_idx <= geode_index; resource_idx++) {
                const auto cost = robot_costs.costs[resource_idx];
                can_afford &= cur_state.resources[resource_idx] >= cost;
                next_state.resources[resource_idx] -= cost;
            }

            if (!can_afford)
                continue;

            pq.push_back(next_state);
        }
    }

    return ret;
}

std::string Solution::solve(size_t part, std::string_view input) {
    auto bps = vec_from_view<Blueprint>(input | split("\n"sv) | fwd_to_str | transform([](auto line) {
        Blueprint bp {};
        std::sscanf(line.c_str(), "%ld %ld %ld %ld %ld %ld", //
          &bp.ore().ore(),                                   //
          &bp.clay().ore(),                                  //
          &bp.obsidian().ore(),                              //
          &bp.obsidian().clay(),                             //
          &bp.geode().ore(),                                 //
          &bp.geode().obsidian());
        return bp;
    }));

    i64 s = 0;
    if (part == 1) {
        for (i64 i = 0; i < bps.size(); i++) {
            auto t = fn(bps[i], 24, 2500);
            //fmt::print("\n{}: {}", i + 1, t);
            s += (i + 1) * t;
        }
        //fmt::print("\n");
    } else if (part == 2) {
        s = 1;
        for (i64 i = 0; i < std::min(bps.size(), 3uz); i++) {
            auto t = fn(bps[i], 32, 10000);
            s *= t;
        }
    }
    return std::to_string(s);
}

}
