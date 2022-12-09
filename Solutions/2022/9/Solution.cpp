#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

void Solution::solve(bool example, size_t part, std::string_view input) {
    if (example) {
        if (part == 1) {
            input = "R 4\n"
                    "U 4\n"
                    "L 3\n"
                    "D 1\n"
                    "R 4\n"
                    "D 1\n"
                    "L 5\n"
                    "R 2"sv;
        } else {
            input = "R 5\n"
                    "U 8\n"
                    "L 8\n"
                    "D 3\n"
                    "R 17\n"
                    "D 10\n"
                    "L 25\n"
                    "U 20"sv;
        }
    }

    if (part == 2)
        return;

    auto pairs = input | split("\n"sv) | fwd_to_strv | transform([](auto l) {
        auto a = l | split(" "sv) | fwd_to_strv;
        auto it = a.begin();
        auto dir = *it++;
        auto s = *it;
        i64 amt;
        std::from_chars(s.data(), s.data() + s.size(), amt);
        return std::make_pair(l[0], amt);
    });

    numarr<bool, 2> visited_first {};
    visited_first[0][0] = true;
    numarr<bool, 2> visited_last {};
    visited_last[0][0] = true;
    arr<pair<i64, i64>, 10> snake {};
    auto& head = snake[0];
    auto& first = snake[1];
    auto& last = snake[snake.size() - 1];

    auto m_sum = [](numarr<bool, 2>& m) {
        i64 s = 0;

        for (auto &[y, row] : m) {
            for (auto [x, v] : row) {
                s += v;
            }
        }

        return s;
    };

    //from p1
    auto snap_tail = [](pair<i64, i64> head, pair<i64, i64> tail) -> pair<i64, i64> {
        auto x_d = head.first - tail.first;
        auto y_d = head.second - tail.second;

        if (std::abs(x_d) <= 1 && std::abs(y_d) <= 1)
            return tail;

        pair<i64, i64> ret(head);

        if (x_d > 1)
            --ret.first;
        else if (x_d < -1)
            ++ret.first;
        if (y_d > 1)
            --ret.second;
        else if (y_d < -1)
            ++ret.second;

        return ret;
    };

    for (auto [dir, amt] : pairs) {
        for (i64 i = 0; i < amt; i++) {
            //fmt::print("({}, {}), ({}, {})\n", head.first, head.second, tail.first, tail.second);
            switch (dir) {
            case 'U': head.second += 1; break;
            case 'D': head.second -= 1; break;
            case 'L': head.first -= 1; break;
            case 'R': head.first += 1; break;
            }

            for (size_t j = 1; j < snake.size(); j++) {
                snake[j] = snap_tail(snake[j - 1], snake[j]);
            }

            visited_first[first.second][first.first] = true;
            visited_last[last.second][last.first] = true;
        }
    }

    fmt::print("{}, {}\n", m_sum(visited_first), m_sum(visited_last));

    //ignore

    if (part == 1111) { //works for p1
        numarr<bool, 2> visited {};

        pair<i64, i64> head { 0, 0 };
        pair<i64, i64> tail { 0, 0 };

        visited[0][0] = true;

        auto snap_tail = [](pair<i64, i64> head, pair<i64, i64> tail) -> pair<i64, i64> {
            auto x_d = head.first - tail.first;
            auto y_d = head.second - tail.second;

            if (std::abs(x_d) <= 1 && std::abs(y_d) <= 1)
                return tail;

            pair<i64, i64> ret(head);

            if (x_d > 1)
                --ret.first;
            else if (x_d < -1)
                ++ret.first;
            else if (y_d > 1)
                --ret.second;
            else if (y_d < -1)
                ++ret.second;

            return ret;
        };

        for (auto [dir, amt] : pairs) {
            for (i64 i = 0; i < amt; i++) {
                //fmt::print("({}, {}), ({}, {})\n", head.first, head.second, tail.first, tail.second);
                switch (dir) {
                case 'U': head.second += 1; break;
                case 'D': head.second -= 1; break;
                case 'L': head.first -= 1; break;
                case 'R': head.first += 1; break;
                }
                tail = snap_tail(head, tail);
                visited[tail.second][tail.first] = true;
            }
        }

        i64 s = 0;
        for (auto &[y, row] : visited) {
            for (auto [x, v] : row) {
                s += v;
            }
        }
        fmt::print("{}\n", s);
        //fmt::print("({}, {}), ({}, {})\n", head.first, head.second, tail.first, tail.second);
    } else if (part == 22222) { // does not work
        numarr<bool, 2> visited {};

        pair<i64, i64> head { 0, 0 };
        pair<i64, i64> tail { 0, 0 };

        auto tail_ok = [](pair<i64, i64> head, pair<i64, i64> tail, i64 dist) {
            auto x_d = head.first - tail.first;
            auto y_d = head.second - tail.second;

            return std::abs(x_d) <= dist && std::abs(y_d) <= dist;
        };

        visited[0][0] = true;
        vec<pair<i64, i64>> head_history;
        head_history.push_back(head);

        auto tail_dist = part == 1 ? 1 : 9;

        for (auto [dir, amt] : pairs) {
            for (i64 i = 0; i < amt; i++) {
                fmt::print("({}, {}), ({}, {})\n", head.first, head.second, tail.first, tail.second);
                switch (dir) {
                case 'U': head.second += 1; break;
                case 'D': head.second -= 1; break;
                case 'L': head.first -= 1; break;
                case 'R': head.first += 1; break;
                }

                size_t j = 0;
                bool tail_selected = false;
                bool tail_candidate_selected = false;
                if (!tail_ok(head, tail, tail_dist)) {
                    for (; j < head_history.size(); j++) {
                        auto k = head_history.size() - j -1;
                        auto candidate = head_history[j];

                        if (tail_ok(head, candidate, tail_dist)) {
                            tail = candidate;
                            break;
                        }

                        /*if (tail_ok(head, candidate) && !tail_selected) {
                            tail = candidate;
                            tail_candidate_selected = true;
                        } else {
                            if (tail_candidate_selected && !tail_selected)
                                tail_selected = true;
                        }*/
                    }
                }
                head_history.push_back(head);
                //head_history.erase(head_history.begin() + j - 1, head_history.end());

                visited[tail.second][tail.first] = true;
            }
        }

        i64 s = 0;
        for (auto &[y, row] : visited) {
            for (auto [x, v] : row) {
                s += v;
            }
        }
        fmt::print("{}\n", s); //529
    }
}

}
