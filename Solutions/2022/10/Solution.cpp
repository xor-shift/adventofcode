#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

void Solution::solve(bool example, size_t part, std::string_view input) {
    if (example) {
        input = "addx 15\n"
                "addx -11\n"
                "addx 6\n"
                "addx -3\n"
                "addx 5\n"
                "addx -1\n"
                "addx -8\n"
                "addx 13\n"
                "addx 4\n"
                "noop\n"
                "addx -1\n"
                "addx 5\n"
                "addx -1\n"
                "addx 5\n"
                "addx -1\n"
                "addx 5\n"
                "addx -1\n"
                "addx 5\n"
                "addx -1\n"
                "addx -35\n"
                "addx 1\n"
                "addx 24\n"
                "addx -19\n"
                "addx 1\n"
                "addx 16\n"
                "addx -11\n"
                "noop\n"
                "noop\n"
                "addx 21\n"
                "addx -15\n"
                "noop\n"
                "noop\n"
                "addx -3\n"
                "addx 9\n"
                "addx 1\n"
                "addx -3\n"
                "addx 8\n"
                "addx 1\n"
                "addx 5\n"
                "noop\n"
                "noop\n"
                "noop\n"
                "noop\n"
                "noop\n"
                "addx -36\n"
                "noop\n"
                "addx 1\n"
                "addx 7\n"
                "noop\n"
                "noop\n"
                "noop\n"
                "addx 2\n"
                "addx 6\n"
                "noop\n"
                "noop\n"
                "noop\n"
                "noop\n"
                "noop\n"
                "addx 1\n"
                "noop\n"
                "noop\n"
                "addx 7\n"
                "addx 1\n"
                "noop\n"
                "addx -13\n"
                "addx 13\n"
                "addx 7\n"
                "noop\n"
                "addx 1\n"
                "addx -33\n"
                "noop\n"
                "noop\n"
                "noop\n"
                "addx 2\n"
                "noop\n"
                "noop\n"
                "noop\n"
                "addx 8\n"
                "noop\n"
                "addx -1\n"
                "addx 2\n"
                "addx 1\n"
                "noop\n"
                "addx 17\n"
                "addx -9\n"
                "addx 1\n"
                "addx 1\n"
                "addx -3\n"
                "addx 11\n"
                "noop\n"
                "noop\n"
                "addx 1\n"
                "noop\n"
                "addx 1\n"
                "noop\n"
                "noop\n"
                "addx -13\n"
                "addx -19\n"
                "addx 1\n"
                "addx 3\n"
                "addx 26\n"
                "addx -30\n"
                "addx 12\n"
                "addx -1\n"
                "addx 3\n"
                "addx 1\n"
                "noop\n"
                "noop\n"
                "noop\n"
                "addx -9\n"
                "addx 18\n"
                "addx 1\n"
                "addx 2\n"
                "noop\n"
                "noop\n"
                "addx 9\n"
                "noop\n"
                "noop\n"
                "noop\n"
                "addx -1\n"
                "addx 2\n"
                "addx -37\n"
                "addx 1\n"
                "addx 3\n"
                "noop\n"
                "addx 15\n"
                "addx -21\n"
                "addx 22\n"
                "addx -6\n"
                "addx 1\n"
                "noop\n"
                "addx 2\n"
                "addx 1\n"
                "noop\n"
                "addx -10\n"
                "noop\n"
                "noop\n"
                "addx 20\n"
                "addx 1\n"
                "addx 2\n"
                "addx 2\n"
                "addx -6\n"
                "addx -11\n"
                "noop\n"
                "noop\n"
                "noop"sv;
    }

    auto program = input | split("\n"sv) | fwd_to_strv | transform([](auto sv) {
        auto a = vec_from_view<strview>(sv | split(" "sv) | fwd_to_strv);
        i64 b = 0;
        if (a[0] == "addx") {
            std::from_chars(a[1].data(), a[1].data() + a[1].size(), b);
        }
        return std::make_pair(a[0], b);
    });

    if (part == 1) {
        vec<i64> cycs {20, 60, 100, 140, 180, 220};
        vec<i64> ss;

        i64 cyc = 1;
        i64 x = 1;
        i64 sum = 0;

        auto cycbegin = [&] {
            if (find(begin(cycs), end(cycs), cyc) != end(cycs)) {
                //fmt::print("{}: {}\n", cyc, x);
                sum += x * cyc;
            }
            ++cyc;
        };

        for (auto [opcode, arg] : program) {
            cycbegin();
            if (opcode == "addx") {
                cycbegin();
                x += arg;
            }
        }
        cycbegin();
        fmt::print("{}\n", sum);
    } else if (part == 2) {
        bool skipping = false;

        struct Executor {
            i64 cyc = 1;
            i64 x = 1;
            i64 sum = 0;
            bool skip = false;

            void exec(strview opcode, i64 arg) {
                during();
                ++cyc;

                if (opcode == "addx") {
                    during();
                    ++cyc;
                    x += arg;
                }
            }

            void during() {
                const vec<i64> cycs {20, 60, 100, 140, 180, 220};
                if (find(begin(cycs), end(cycs), cyc) != end(cycs)) {
                    sum += x * cyc;
                }

                auto col = cyc % 40 - 1;

                if (col == 1) {
                    fmt::print("\n");
                }


                auto d = x - col;

                //fmt::print("pos @ {}: {}, {}\n", cyc, x, d);
                fmt::print("{}", std::abs(d) <= 1 ? '#' : '.');
            }

        };

        Executor exec {};

        for (auto [opcode, arg] : program) {
            exec.exec(opcode, arg);
        }

        fmt::print("\nsum: {}\n", exec.sum);
    }
}

}
