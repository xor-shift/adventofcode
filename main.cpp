#include <Solution.hpp>

int main() {
    AoC::Solution solution{};
    fmt::print("Running Ex. Pt. 1:\n");
    solution.solve_example(1);
    fmt::print("Running Ex. Pt. 2:\n");
    solution.solve_example(2);
    fmt::print("Running Pt. 1:\n");
    solution.solve(1);
    fmt::print("Running Pt. 2:\n");
    solution.solve(2);
}
