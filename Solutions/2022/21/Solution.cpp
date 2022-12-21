#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

enum class Oper {
    Add,
    Sub,
    Mul,
    Div,
};

struct Monkey {
    strview name;
    opt<i64> val;
    pair<strview, strview> args;
    Oper oper;
};

static auto  fn(umap<strview, Monkey>& monkey_map, bool leave_human_alone = false) {
    for (;;) {
        i64 c = 0;

        for (auto& [name, monkey] : monkey_map) {
            if (monkey.val.has_value())
                continue;

            if (monkey.name == "humn" && leave_human_alone)
                continue;

            auto lhsopt = monkey_map.at(monkey.args.first).val;
            auto rhsopt = monkey_map.at(monkey.args.second).val;

            if (!lhsopt || !rhsopt)
                continue;

            auto lhs = *lhsopt;
            auto rhs = *rhsopt;

            switch (monkey.oper) {
            case Oper::Add: monkey.val = lhs + rhs; break;
            case Oper::Sub: monkey.val = lhs - rhs; break;
            case Oper::Mul: monkey.val = lhs * rhs; break;
            case Oper::Div: monkey.val = lhs / rhs; break;
            default: std::unreachable();
            }

            ++c;
        }

        if (c == 0)
            break;
    }
}

static void print_expr(umap<strview, Monkey> const& monkey_map, strview of_what) {
    auto const& monke = monkey_map.at(of_what);

    fmt::print("(");
    if (monke.name == "humn") {
        fmt::print("x");
    } else if (monke.val.has_value()) {
        fmt::print("{}", *monke.val);
    } else {
        print_expr(monkey_map, monke.args.first);
        if (monke.name == "root") {
            fmt::print("=");
        } else {
            switch (monke.oper) {
            case Oper::Add: fmt::print("+"); break;
            case Oper::Sub: fmt::print("-"); break;
            case Oper::Mul: fmt::print("*"); break;
            case Oper::Div: fmt::print("/"); break;
            default: std::unreachable();
            }
        }
        print_expr(monkey_map, monke.args.second);
    }
    fmt::print(")");
}

std::string Solution::solve(size_t part, std::string_view input) {
    auto in_iter = input | split("\n"sv) | fwd_to_strv | transform([](auto l) {
        Monkey monkey {
            .name = l.substr(0, 4),
        };

        auto expr = l.substr(6);

        if (expr[0] >= '0' && expr[0] <= '9') {
            i64 v;
            std::from_chars(expr.data(), expr.data() + expr.size(), v);
            monkey.val = v;
        } else {
            monkey.args.first = expr.substr(0, 4);
            monkey.args.second = expr.substr(7, 4);

            switch (expr[5]) {
            case '+': monkey.oper = Oper::Add; break;
            case '-': monkey.oper = Oper::Sub; break;
            case '*': monkey.oper = Oper::Mul; break;
            case '/': monkey.oper = Oper::Div; break;
            default: std::unreachable();
            }
        }

        return monkey;
    });

    umap<strview, Monkey> monkey_map;
    for (auto m : in_iter)
        monkey_map[m.name] = m;

    if (part == 1) {
        fn(monkey_map);
        return std::to_string(*monkey_map["root"].val);
    }
    if (part == 2) {
        monkey_map["humn"].val = std::nullopt;
        fn(monkey_map, true);
        print_expr(monkey_map, "root");

        //((521+(2*(130833695252697-(((((((((3*((((4*((((((((((((((((371+(((((433+((((((120+((((((((475+(414+(((((((((((531+((378+(((((700+(x))/3)-913)*77)+673))+792))/2)-178)*2)-69)/5)+363)*2)+559)*3)-748)))/6)-808)/7)+175)*53)-851)/3))*2)+826)/9)-156)*15))+511)/2)-704)*2))*2)-984)/10)+521)/2)-309)*6)-626)*2)+680)/2)-625)+12)/5)+161))-895)/3)+315))+258)/9)-77)*18)-601)*2)+17)/3))))/3)=31343426392931
        //feed this to sympy

        /*for (i64 v = 0;; v++) {
            auto copy = monkey_map;

            copy["humn"].val = v;
            fn(copy);

            if (copy[copy["root"].args.first].val == copy[copy["root"].args.second].val) {
                return std::to_string(v);
            }
        }*/
    }

    return "";
}

}
