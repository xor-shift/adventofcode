#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

void Solution::solution(size_t part, std::string_view input) {
    auto scanner_config = vec_from_view<pair<i64, i64>>(input | split("\n"sv) | fwd_to_str | transform([](auto v) {
        auto a = v | split(": "sv) | fwd_to_str | str_to_t<i64>;
        auto a_it = a.begin();
        return pair<i64, i64>(*a_it++, *a_it++);
    }));

    const auto layers = scanner_config.back().first;

    struct Scanner {
        i64 depth;
        bool going_down;
    };

    umarr<pair<i64, Scanner>> state {};

    for (auto [layer, depth] : scanner_config) {
        state[layer] = { depth, { 0, true } };
    }

    auto tick_state = [](umarr<pair<i64, Scanner>>& state) {
        for (auto& [layer, s] : state) {
            auto& [max_depth, scanner] = s;
            if (scanner.going_down) {
                scanner.going_down = scanner.depth != max_depth - 1;
            } else {
                scanner.going_down = scanner.depth == 0;
            }

            scanner.depth += scanner.going_down ? 1 : -1;
        }
    };

    auto print_state = [layers](umarr<pair<i64, Scanner>>& state) {
        for (i64 i = 0; i < layers; i++) {
            auto it = state.find(i);
            fmt::print("{}: ", i);
            if (it == state.end()) {
                fmt::print("\n");
                continue;
            }

            auto& [layer, s] = *it;
            auto& [max_depth, scanner] = s;

            for (i64 j = 0; j < max_depth; j++) {
                if (scanner.depth == j)
                    fmt::print("[S] ");
                else
                    fmt::print("[ ] ");
            }
            fmt::print("\n");
        }
    };

    std::ignore = print_state;
    std::ignore = tick_state;

    /*for (auto i = 0uz; i < 8; i++) {
        print_state(state);
        tick_state(state);
    }
    print_state(state);*/

    auto get_severity = [&tick_state, layers](umarr<pair<i64, Scanner>> state, i64 delay = 0) {
        for (i64 i = 0; i < delay; i++)
            tick_state(state);

        i64 a = 0;
        for (i64 i = 0; i <= layers; i++) {
            auto it = state.find(i);
            if (it != state.end()) {
                auto& [layer, s] = *it;
                auto& [max_depth, scanner] = s;

                if (scanner.depth == 0) {
                    a += max_depth * layer;
                }
            }
            tick_state(state);
        }
        return a;
    };

    auto is_suitable_delay = [](i64 layer, i64 delay, i64 range) {
        const auto mod = (range - 1) * 2;
        return ((delay + layer) % mod) != 0;
    };

    if (part == 1) {
        fmt::print("{}\n", get_severity(state));
    } else if (part == 2) {
        // #pragma omp parallel for
        for (i64 i = 1; i < 99999999; i++) {
            bool ok = true;
            for (auto& [layer, s] : state) {
                auto& [range, scanner] = s;
                ok &= is_suitable_delay(layer, i, range);
                if (!ok)
                    break;
            }
            if (ok) {
                fmt::print("{}\n", i);
                break;
            }
        }
    }
}

void Solution::solve_example(size_t part) {
    std::string_view example_string = "0: 3\n"
                                      "1: 2\n"
                                      "4: 4\n"
                                      "6: 4"sv;
    solution(part, example_string);
}

void Solution::solve(size_t part) {
    const Stf::MMapStringView file(INPUT_FILE, false);
    solution(part, file);
}

}
