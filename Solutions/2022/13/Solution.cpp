#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

template<typename... Ts> struct ListBase {
    using elem_variant_type = std::variant<ListBase<Ts...>*, Ts...>;

    ListBase() = default;

    template<typename T> ListBase(std::initializer_list<T> list) {
        for (auto v : list)
            push(v);
    }

    ~ListBase() noexcept {
        for (auto& content : contents) {
            if (!std::holds_alternative<ListBase*>(content)) {
                continue;
            }

            auto* p = std::get<ListBase*>(content);
            p->~ListBase();
        }
    }

    constexpr size_t size() const { return contents.size(); }

    constexpr auto& operator[](usize i) { return contents[i]; }
    constexpr auto const& operator[](usize i) const { return contents[i]; }

    void print() const {
        fmt::print("[");

        for (bool first = true; auto const& content : contents) {
            if (!first)
                fmt::print(",");
            first = false;

            auto printer = []<typename T>(T const& v) {
                if constexpr (std::is_same_v<T, i64>) {
                    fmt::print("{}", v);
                } else if constexpr (std::is_same_v<T, ListBase*>) {
                    v->print();
                } else {
                    std::unreachable();
                }
            };

            std::visit(printer, content);
        }

        fmt::print("]");
    }

    auto& push(std::variant<ListBase*, Ts...>&& v) {
        return std::visit(
          [this]<typename T>(T&& v) -> elem_variant_type& { return this->template push<T>(std::forward<T>(v)); },
          std::move(v));
    }

    template<typename U>
    auto push(U&& v) -> decltype(std::declval<vec<elem_variant_type>&>().emplace_back(std::forward<U>(v))) {
        return contents.emplace_back(std::forward<U>(v));
    }

private:
    vec<elem_variant_type> contents;
};

using List = ListBase<i64>;
using ListElem = List::elem_variant_type;

static std::strong_ordering operator<=>(List const& lhs, List const& rhs);
static std::strong_ordering operator<=>(ListElem const& lhs, ListElem const& rhs);

static std::strong_ordering operator<=>(List const& lhs, List const& rhs) {
    for (usize i = 0; i < std::min(lhs.size(), rhs.size()); i++) {
        auto res = lhs[i] <=> rhs[i];
        if (res != std::strong_ordering::equivalent)
            return res;
    }

    return lhs.size() <=> rhs.size();
};

static std::strong_ordering operator<=>(ListElem const& lhs, ListElem const& rhs) {
    if (std::holds_alternative<i64>(lhs) && std::holds_alternative<i64>(rhs)) {
        return std::get<i64>(lhs) <=> std::get<i64>(rhs);
    }

    if (std::holds_alternative<i64>(lhs)) {
        List llhs {};
        llhs.push(std::get<i64>(lhs));
        return llhs <=> *std::get<List*>(rhs);
    }

    if (std::holds_alternative<i64>(rhs)) {
        List lrhs {};
        lrhs.push(std::get<i64>(rhs));
        return *std::get<List*>(lhs) <=> lrhs;
    }

    return *std::get<List*>(lhs) <=> *std::get<List*>(rhs);
}

static tl::expected<pair<i64, strview>, strview> scan_number(strview sv) {
    if (sv.empty() || sv.front() > '9' || '0' > sv.front())
        return tl::unexpected { "empty string or invalid first character" };

    i64 v;
    auto res = std::from_chars(sv.begin(), sv.end(), v);
    if (res.ec != std::errc())
        return tl::unexpected { "invalid integer literal" };

    auto consumed_len = std::distance(sv.begin(), res.ptr);
    sv = sv.substr(consumed_len);

    return std::make_pair(v, sv);
}

static tl::expected<pair<List*, strview>, strview> scan_list(strview sv) {
    if (sv.size() < 2 || sv.front() != '[')
        return tl::unexpected { "empty string or invalid first character" };

    sv = sv.substr(1);

    auto* ret = new List();

    if (sv.front() == ']')
        return std::make_pair(ret, sv.substr(1));

    for (;;) {
        List::elem_variant_type scanned_value;

        if (auto number_res = scan_number(sv); number_res) {
            scanned_value = number_res->first;
            sv = number_res->second;
        } else if (auto list_res = scan_list(sv); list_res) {
            scanned_value = list_res->first;
            sv = list_res->second;
        } else {
            return tl::unexpected { "unexpected character while scanning element" };
        }

        ret->push(std::move(scanned_value));

        auto front = sv.front();
        sv = sv.substr(1);

        if (front != ',' && front != ']')
            return tl::unexpected { "unexpected character while scanning delimiter" };

        if (front == ']')
            break;
    }

    return std::make_pair(ret, sv);
}

std::string Solution::solve(size_t part, std::string_view input) {
    auto input_view = input | split("\n"sv) | fwd_to_strv | filter([](auto s) { return s.front() != '\n'; })
      | transform([](auto line) { return scan_list(line)->first; });

    vec<std::unique_ptr<List>> lists(begin(input_view), end(input_view));

    i64 s = 0;

    if (part == 1) {
        for (size_t i = 0; i < lists.size(); i += 2) {
            auto& first = *lists[i];
            auto& second = *lists[i + 1];

            if ((first <=> second) == std::strong_ordering::less)
                s += i / 2 + 1;
        }
    } else if (part == 2) {
        auto* two = &*lists.emplace_back(new List({ new List({ 2 }) }));
        auto* six = &*lists.emplace_back(new List({ new List({ 6 }) }));

        sort(begin(lists), end(lists),
          [](auto const& lhs, auto const& rhs) { return (*lhs <=> *rhs) == std::strong_ordering::less; });

        auto i = distance(begin(lists), find_if(begin(lists), end(lists), [&two](auto& v) { return &*v == two; }));
        auto j = distance(begin(lists), find_if(begin(lists), end(lists), [&six](auto& v) { return &*v == six; }));

        s = (i + 1) * (j + 1);
    }

    return std::to_string(s);
}

}
