#include <Stuff/Util/MMap.hpp>

#include <Solution.hpp>

inline static AoC::Solution solution{}; // NOLINT(cert-err58-cpp)

inline void run_for_file(bool single_part, const char* filename) {
    const Stf::MMapStringView file(filename, true);
    auto sv = static_cast<std::string_view>(file);

    fmt::print("Running for file \"{}\":\n", filename);
    if (single_part) {
        solution.solve(1, sv);
        return;
    }

    fmt::print("Pt. 1:\n");
    solution.solve(1, sv);
    fmt::print("Pt. 2:\n");
    solution.solve(2, sv);
}

int main() {
#ifdef AOC_SINGLE_PART
    static constexpr bool single_part = true;
#else
    static constexpr bool single_part = false;
#endif

#ifdef AOC_DO_EXAMPLES
    run_for_file(single_part, EXAMPLE_FILE);
#endif

    run_for_file(single_part, INPUT_FILE);
}
