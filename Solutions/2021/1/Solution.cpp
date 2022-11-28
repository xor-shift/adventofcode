#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

void Solution::solution(size_t part, std::string_view input) {
    if (part == 1) {
        auto a = input                      //
               | split("\n"sv) | fwd_to_str //
               | str_to_t<int>              //
               | sliding(2)                 //
               | transform([](auto w) {
                     auto it = w.begin();
                     auto a = *it++;
                     auto b = *it;
                     return a < b;
                 });

        fmt::print("{}\n", std::reduce(begin(a), end(a), 0));
    } else if (part == 2) {
        auto a = input                      //
               | split("\n"sv) | fwd_to_str //
               | str_to_t<int>              //
               | sliding(3)                 //
               | transform([](auto w) {
                     auto s = 0;
                     for (auto v : w)
                         s += v;
                     return s;
                 })         //
               | sliding(2) //
               | transform([](auto w) {
                     auto it = w.begin();
                     auto a = *it++;
                     auto b = *it;
                     return a < b;
                 });

        fmt::print("{}\n", std::reduce(begin(a), end(a), 0));
    }
}

void Solution::solve_example(size_t part) {
    std::string_view example_string = "199\n"
                                      "200\n"
                                      "208\n"
                                      "210\n"
                                      "200\n"
                                      "207\n"
                                      "240\n"
                                      "269\n"
                                      "260\n"
                                      "263"sv;
    solution(part, example_string);
}

void Solution::solve(size_t part) {
    const Stf::MMapStringView file(INPUT_FILE, false);
    solution(part, file);
}

}
