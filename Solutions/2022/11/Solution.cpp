#include <Common.hpp>
#include <fstream>

namespace AoC {

Solution::Solution() { }

template<typename T = i64> struct Monkey {
    vec<T> items;
    char oper;
    strview oper_arg_str;
    T oper_arg;
    T test_arg;
    T true_val;
    T false_val;

    i64 inspections = 0;
};

std::string Solution::solve(size_t part, std::string_view input) {
    auto monkeys = vec_from_view<Monkey<i64>>(input | split("\n"sv) | fwd_to_strv | chunk(7) | transform([](auto ch) {
        auto lines = vec_from_view<strview>(ch);

        auto oper = lines[2][lines[2].find("old ") + 4];
        auto oper_arg_str = lines[2].substr(lines[2].find(oper) + 2);

        Monkey<i64> monkey {
            .items = vec_from_view<i64>(
              lines[1].substr(lines[1].find(": ") + 2) | split(", "sv) | fwd_to_strv | str_to_t<i64>),
            .oper = oper,
            .oper_arg_str = oper_arg_str,
            .oper_arg = from_chars<i64>(oper_arg_str).value_or(0),
            .test_arg = *from_chars<i64>(lines[3].substr(lines[3].find("by ") + 3)),
            .true_val = *from_chars<i64>(lines[4].substr(lines[4].find("key ") + 4)),
            .false_val = *from_chars<i64>(lines[5].substr(lines[5].find("key ") + 4)),
        };

        return monkey;
    }));

    i64 lcm = 1;
    for (auto const& monkey : monkeys) {
        // lcm = std::lcm(monkey.test_arg, lcm);
        lcm = lcm * monkey.test_arg;
    }

    for (size_t round = 0; round < (part == 1 ? 20 : 10000); round++) {
        for (auto& monkey : monkeys) {
            auto throws = monkey.items | transform([&](auto item) {
                auto res = item;
                i64 arg;
                arg = monkey.oper_arg_str == "old" ? item : monkey.oper_arg;

                switch (monkey.oper) {
                case '*': res *= arg; break;
                case '+': res += arg; break;
                default: std::unreachable();
                }

                if (part == 1)
                    res /= 3;
                res %= lcm;

                auto to = (&monkey.true_val)[res % monkey.test_arg != 0];
                return std::make_pair(to, res);
            });

            for (auto [to, val] : throws) {
                monkeys[to].items.push_back(val);
            }

            monkey.inspections += monkey.items.size();
            monkey.items.clear();
        }
    }

    auto top = find_n_largest<i64, 2>(monkeys | transform([](auto const& v) { return v.inspections; }));

    fmt::print("{}, {} -> {}\n", top[0], top[1], top[0] * top[1]);

    return std::to_string(top[0] * top[1]);
}

void unwashed(bool example, size_t part, std::string_view) {
    /*
     * NOTICE:
     * i thought that mmap broke but i was actually putting my input into the template's input file.
     * i later did not remove this code because it was already ready.
     */

    /*
     * NOTICE AFTER THE ORIGINAL COMMIT:
     * this system is here to stay, having text in code is dumb anyway...
     */

    std::ifstream ifs;
    if (example) {
        ifs.open("Solutions/2022/11/example");
    } else {
        ifs.open("Solutions/2022/11/input");
    }

    if (!ifs) {
        std::unreachable();
    }

    std::string input((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    auto a = input | split("\n"sv) | fwd_to_strv | chunk(7) | transform([](auto ch) {
        auto lines = vec_from_view<strview>(ch);

        auto st_items
          = vec_from_view<i64>(lines[1].substr(lines[1].find(": ") + 2) | split(", "sv) | fwd_to_strv | str_to_t<i64>);
        std::ignore = st_items;

        auto oper = lines[2][lines[2].find("old ") + 4];
        auto oper_arg_str = lines[2].substr(lines[2].find(oper) + 2);
        auto test_arg_str = lines[3].substr(lines[3].find("by ") + 3);
        auto true_val_str = lines[4].substr(lines[4].find("key ") + 4);
        auto false_val_str = lines[5].substr(lines[5].find("key ") + 4);

        i64 oper_arg;
        i64 test_arg;
        i64 true_val;
        i64 false_val;

        std::from_chars(oper_arg_str.data(), oper_arg_str.data() + oper_arg_str.size(), oper_arg);
        std::from_chars(test_arg_str.data(), test_arg_str.data() + test_arg_str.size(), test_arg);
        std::from_chars(true_val_str.data(), true_val_str.data() + true_val_str.size(), true_val);
        std::from_chars(false_val_str.data(), false_val_str.data() + false_val_str.size(), false_val);

        return std::make_tuple(st_items, oper, oper_arg_str, oper_arg, test_arg, true_val, false_val);
    });

    using iii = i64;
    // using iii = i128;

    vec<Monkey<>> monkeys;

    for (auto [st_items, oper, oper_arg_str, oper_arg, test_arg, true_val, false_val] : a) {
        Monkey<> monkey {
            vec<iii>(st_items.begin(), st_items.end()),
            oper,
            oper_arg_str,
            oper_arg,
            test_arg,
            true_val,
            false_val,
        };
        monkeys.push_back(monkey);
    }

    iii lcm = 1;
    for (auto const& monkey : monkeys) {
        // lcm = std::lcm(monkey.test_arg, lcm);
        lcm = lcm * monkey.test_arg;
    }

    for (size_t round = 0; round < (part == 1 ? 20 : 10000); round++) {
        for (size_t i = 0; auto& monkey : monkeys) {
            vec<pair<iii, iii>> throws;
            for (auto item : monkey.items) {
                ++monkey.inspections;
                auto res = item;
                iii arg;
                arg = monkey.oper_arg_str == "old" ? item : monkey.oper_arg;
                switch (monkey.oper) {
                case '*': res *= arg; break;
                case '+': res += arg; break;
                default: std::unreachable();
                }
                if (part == 1)
                    res /= 3;
                res %= lcm;

                auto to = (res % monkey.test_arg) == 0 ? monkey.true_val : monkey.false_val;
                throws.push_back(std::make_pair(to, res));
                // fmt::print("{} is throwing {} ({}) to {}\n", i, item, res, to);
            }
            monkey.items.clear();
            for (auto [to, val] : throws) {
                monkeys[to].items.push_back(val);
            }
            ++i;
        }
    }

    sort(rbegin(monkeys), rend(monkeys),
      [](auto const& lhs, auto const& rhs) { return lhs.inspections < rhs.inspections; });

    iii top[2] { monkeys[0].inspections, monkeys[1].inspections };
    fmt::print("{}, {} -> {}\n", top[0], top[1], top[0] * top[1]);

    std::ignore = lcm;
}

}
