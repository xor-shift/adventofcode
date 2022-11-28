#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

enum class OpCode { INP, ADD, MUL, DIV, MOD, EQL };

template<OpCode OP> using op_code_tag = std::integral_constant<OpCode, OP>;

struct IArgs {
    char a;
    std::variant<char, i64> b;
};

template<OpCode OP> using instruction_type = std::pair<op_code_tag<OP>, IArgs>;

using instructions_variant = std::variant<
  instruction_type<OpCode::INP>, //
  instruction_type<OpCode::ADD>, //
  instruction_type<OpCode::MUL>, //
  instruction_type<OpCode::DIV>, //
  instruction_type<OpCode::MOD>, //
  instruction_type<OpCode::EQL>  //
  >;

// second one is IP
using vm_state_type = std::pair<std::array<i64, 4>, size_t>;

size_t register_index(char c) {
    switch (c) {
    case 'w': return 0uz;
    case 'x': return 1uz;
    case 'y': return 2uz;
    case 'z': return 3uz;
    }

    std::unreachable();
}

i64 register_or_value(vm_state_type& vm, std::variant<char, i64> const& arg) {
    return std::visit(
      [&vm]<typename T>(T v) {
          if constexpr (std::is_same_v<T, i64>)
              return v;
          else
              return vm.first[register_index(v)];
      },
      arg
    );
}

void vm_advance(vm_state_type& vm, instruction_type<OpCode::INP> ins, char input) {
    vm.first[register_index(ins.second.a)] = static_cast<i64>(input - '0');
}

template<typename BinaryOp> void vm_arith(vm_state_type& vm, IArgs args, BinaryOp&& op = BinaryOp()) {
    i64& target = vm.first[register_index(args.a)];
    target = std::invoke(std::forward<BinaryOp>(op), target, register_or_value(vm, args.b));
}

void vm_advance(vm_state_type& vm, instruction_type<OpCode::ADD> ins, char) { vm_arith(vm, ins.second, std::plus {}); }

void vm_advance(vm_state_type& vm, instruction_type<OpCode::MUL> ins, char) {
    vm_arith(vm, ins.second, std::multiplies {});
}

void vm_advance(vm_state_type& vm, instruction_type<OpCode::DIV> ins, char) {
    vm_arith(vm, ins.second, std::divides {});
}

void vm_advance(vm_state_type& vm, instruction_type<OpCode::MOD> ins, char) {
    vm_arith(vm, ins.second, [](auto a, auto b) { return a % b; });
}

void vm_advance(vm_state_type& vm, instruction_type<OpCode::EQL> ins, char) {
    vm_arith(vm, ins.second, [](auto a, auto b) { return a == b; });
}

template<bool Part1>
struct Memo : Stf::Comp::MemoizerBase<opt<i64>, i64, i64, char> {
    std::span<instructions_variant> instructions;

    Memo()
        : MemoizerBase(1024uz * 1024uz * 24uz) { }

    virtual ~Memo() = default;

protected:
    opt<i64> impl(i64 depth, i64 z, char next_input) override {
        const auto z_next = next_z(depth, z, next_input);

        if (depth == 13) {
            if (z_next == 0)
                return static_cast<i64>(next_input - '0');
            return std::nullopt;
        }

        for (char c = Part1 ? '9' : '1'; Part1 ? (c >= '1') : (c <= '9'); Part1 ? c-- : c++) {
            const auto mul = static_cast<i64>(std::pow(10, 13 - depth));

            auto res = call(depth + 1, z_next, c);
            if (!res)
                continue;

            return static_cast<i64>(next_input - '0') * mul + *res;
        }

        return std::nullopt;
    }

private:
    i64 next_z(i64 depth, i64 z, char next_input) {
        vm_state_type vm { { 0, 0, 0, z }, depth * 18 };
        auto input_instr = get<instruction_type<OpCode::INP>>(instructions[vm.second++]);
        vm_advance(vm, input_instr, next_input);

        for (auto i = 0uz; i < 17uz; i++) {
            std::ignore = vm;
            std::visit([&vm, next_input](auto i) { vm_advance(vm, i, next_input); }, instructions[vm.second++]);
        }

        return vm.first[3];
    }
};

void Solution::solution(size_t part, std::string_view input) {
    auto ir
      = input         //
      | split("\n"sv) //
      | fwd_to_str    //
      | transform([](auto line) -> instructions_variant {
            auto lr = line | split(" "sv) | fwd_to_str;
            auto it = lr.begin();
            auto opcode = *it++;
            auto arg1_str = *it++;
            auto arg1 = arg1_str[0];
            auto arg2_str = *it;
            std::variant<char, i64> arg2 = 0;
            if (std::from_chars(arg2_str.data(), arg2_str.data() + arg2_str.size(), std::get<i64>(arg2)).ec != std::errc()) {
                arg2 = arg2_str[0];
            }

            // clang-format off
                 if(opcode == "inp") return instruction_type<OpCode::INP>{{}, {arg1, arg2}};
                 if(opcode == "add") return instruction_type<OpCode::ADD>{{}, {arg1, arg2}};
                 if(opcode == "mul") return instruction_type<OpCode::MUL>{{}, {arg1, arg2}};
                 if(opcode == "div") return instruction_type<OpCode::DIV>{{}, {arg1, arg2}};
                 if(opcode == "mod") return instruction_type<OpCode::MOD>{{}, {arg1, arg2}};
                 if(opcode == "eql") return instruction_type<OpCode::EQL>{{}, {arg1, arg2}};
            // clang-format on
            std::unreachable();
        });

    std::vector<instructions_variant> vec { ir.begin()++, ir.end() };
    const auto initial = part == 1 ? std::numeric_limits<i64>::min() : std::numeric_limits<i64>::max();
    i64 solution = initial;

    if (part == 1) {
        Memo<true> memo {};
        memo.instructions = { vec };

        for (char c = '9'; c >= '1'; c--) {
            auto res = memo(0, 0, c);
            if (!res)
                continue;
            solution = std::max(solution, *res);
            break;
        }
    } else if (part == 2) {
        Memo<false> memo {};
        memo.instructions = { vec };

        for (char c = '1'; c <= '9'; c++) {
            auto res = memo(0, 0, c);
            if (!res)
                continue;
            solution = std::min(solution, *res);
            break;
        }
    }

    fmt::print("{}\n", solution);
}

void Solution::solve_example(size_t part) {
    std::string_view example_string = ""sv;
    // solution(part, example_string);
}

void Solution::solve(size_t part) {
    const Stf::MMapStringView file(INPUT_FILE, false);
    solution(part, file);
}

}
