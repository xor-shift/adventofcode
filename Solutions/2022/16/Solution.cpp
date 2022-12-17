#include <Common.hpp>

#include <queue>

namespace AoC {

Solution::Solution() { }

enum class MoveType : int {
    Move,
    Open,
};

template<typename T> struct Move;

template<> struct Move<std::integral_constant<int, static_cast<int>(MoveType::Move)>> {
    strview to;
};

using move_move_t = Move<std::integral_constant<int, static_cast<int>(MoveType::Move)>>;

template<> struct Move<std::integral_constant<int, static_cast<int>(MoveType::Open)>> {
    // total flow volume contribution of the action
    i64 contribution;
};

using open_move_t = Move<std::integral_constant<int, static_cast<int>(MoveType::Open)>>;

using move_variant = std::variant<move_move_t, open_move_t>;

struct Valve {
    strview self;
    i64 flow;
    vec<strview> leads_to;
};

std::string Solution::solve(size_t part, std::string_view input) {

    umap<strview, Valve> valves {};

    for (auto l : input | split("\n"sv) | fwd_to_strv) {
        auto from = l.substr(l.find("Valve ") + 6, 2);
        auto rate = *from_chars<i64>(l.substr(l.find("rate=") + 5));

        auto suff = l.substr(l.find("lead") + 4 + 9);
        auto leads_to = vec_from_view<strview>(suff.substr(suff.find(' ') + 1) | split(", "sv) | fwd_to_strv);

        valves.insert({ from, { from, rate, leads_to } });

        std::ignore = suff;
    }

    if (part == 1) {
        strview cur_valve = "AA";

        auto flow_of = [&](strview valve_name) { return valves.at(valve_name).flow; };

        struct State {
            i64 mins_passed;
            strview cur_at;

            // below is pretty much equivalent to storing the order in which valves were opened

            uset<strview> open_valves; // order is unimportant as we cache the flow volume and flow amount
            i64 flow;
            i64 flow_volume;
        };

        auto get_moves = [&valves](State const& state) -> vec<move_variant> {
            if (state.mins_passed >= 30)
                return {};

            auto& valve = valves.at(state.cur_at);
            auto mins_remaining = 30 - state.mins_passed;

            vec<move_variant> ret {};

            ret.push_back(open_move_t {
              .contribution = mins_remaining * valve.flow,
            });

            for (auto vn : valve.leads_to) {
                if (state.open_valves.contains(vn))
                    continue;

                ret.push_back(move_move_t {
                  .to = vn,
                });
            }

            return ret;
        };

        auto move_to_state = [&](State const& cur_state, move_variant move) {
            struct Lambda {
                umap<strview, Valve> const& valves;
                State const& cur_state;

                State operator()(move_move_t const& move) {
                    State ret = cur_state;
                    ++ret.mins_passed;
                    ret.flow_volume += ret.flow;
                    ret.cur_at = move.to;
                    return ret;
                }

                State operator()(open_move_t const& move) {
                    State ret = cur_state;
                    ++ret.mins_passed;
                    ret.flow_volume += ret.flow;
                    ret.flow = valves.at(cur_state.cur_at).flow;
                    ret.open_valves.insert(cur_state.cur_at);
                    return ret;
                }
            } lambda {
                .valves = valves,
                .cur_state = cur_state,
            };

            return std::visit(lambda, move);
        };

        auto cmp = [](State const& lhs, State const& rhs) { return lhs.flow_volume > rhs.flow_volume; };

        using PQT = State;
        using PQC = vec<PQT>;
        using PQCMP = decltype(cmp);

        std::priority_queue<PQT, PQC, PQCMP> queue;
        queue.push({
          .mins_passed = 0,
          .cur_at = "AA",

          .open_valves {},
          .flow = 0,
          .flow_volume = 0,
        });

        i64 max_flow_vol = 0;
        while (!queue.empty()) {
            State cur_state = queue.top();
            queue.pop();

            if (cur_state.mins_passed >= 30) {
                if (max_flow_vol < cur_state.flow_volume) {
                    max_flow_vol = cur_state.flow_volume;
                    fmt::print("{}\n", max_flow_vol);
                }
                continue;
            }

            auto next_moves = get_moves(cur_state);
            for (auto const& move : next_moves) {
                auto next_state = move_to_state(cur_state, move);
                queue.push(next_state);
            }
        }
        fmt::print("{}\n", max_flow_vol);

    } else if (part == 2) {
    }

    return "";
}

}
