#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

struct List {
    i64 value = 0;
    vec<std::unique_ptr<List>> values;
};

static opt<pair<i64, strview>> scan_number(strview sv) {}

static opt<pair<std::unique_ptr<List>, strview>> scan_list(strview sv) {

}

std::string Solution::solve(size_t part, std::string_view input) {
    input | split("\n\n"sv) | fwd_to_strv | transform([](auto group) {
        auto [first, second] = view_to_pair<strview, strview>(group | split("\n"sv) | fwd_to_strv);



        return 1;
    });

    if (part == 1) {
    } else if (part == 2) {
    }

    return "";
}

}
