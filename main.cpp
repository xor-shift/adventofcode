#include <Stuff/Util/MMap.hpp>

#include <Solution.hpp>

int main() {
    const Stf::MMapStringView file(INPUT_FILE, false);

    AoC::Solution solution{};

#ifdef AOC_DO_EXAMPLES
    fmt::print("Ex. Pt. 1:\n");
    solution.solve(true, 1);
    fmt::print("Ex. Pt. 2:\n");
    solution.solve(true, 2);
#endif

    auto sv = static_cast<std::string_view>(file);

    fmt::print("Pt. 1:\n");
    solution.solve(false, 1, sv);
    fmt::print("Pt. 2:\n");
    solution.solve(false, 2, sv);
}
