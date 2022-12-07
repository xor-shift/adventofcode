#include <atomic>

#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

void unwashed(size_t part, std::string_view input) {
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

void Solution::solve(bool example, size_t part, std::string_view input) {
    if (example)
        input = "mjqjpqmgbljsphdztnvjfqwrcgsmlb";

    static constexpr std::array<size_t, 2> window_sizes{4, 14};
    static constexpr auto max_window_size = *std::max_element(begin(window_sizes), end(window_sizes));
    const auto window_size = window_sizes[part - 1];

    std::array<size_t, 24> out;
    std::fill(begin(out), end(out), std::numeric_limits<size_t>::max());
    std::atomic_size_t res_idx = 0;

#pragma omp parallel for
    for (size_t i = 0; i < input.size() - window_size; i++) {
        auto window = input.substr(i, window_size);

        uint32_t s = 0;
        for (char c : window) {
            s |= 1 << static_cast<uint32_t>(c - 'a');
        }

        if (std::popcount(s) == window_size) [[unlikely]] {
            fmt::print("{}\n", i);
            out[res_idx++] = i + window_size;
        }
    }

    fmt::print("{}\n", *std::min_element(begin(out), end(out)));
}

}
