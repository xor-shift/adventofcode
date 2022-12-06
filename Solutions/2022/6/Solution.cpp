#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

void Solution::solution(size_t part, std::string_view input) {
    const usize win_size = part == 1 ? 4 : 14;

    umap<char, usize> m {};
    arr<char, 14> win{};

    std::copy(input.data(), input.data() + win_size, win.data());
    for (char c : input | take(win_size))
        ++m[c];

    for (size_t i = win_size; char c : input | drop(win_size)) {
        if (all_of(begin(m), end(m), [](auto p) { return p.second <= 1; })) {
            fmt::print("{}\n", i);
            break;
        }

        --m[win[0]];
        std::shift_left(begin(win), begin(win) + win_size, 1);
        win[win_size - 1] = c;
        ++m[c];

        ++i;
    }
}

void Solution::solve_example(size_t part) {
    std::string_view example_string = "mjqjpqmgbljsphdztnvjfqwrcgsmlb"sv;
    solution(part, example_string);
}

void Solution::solve(size_t part) {
    const Stf::MMapStringView file(INPUT_FILE, false);
    solution(part, file);
}

}
