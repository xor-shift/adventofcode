#include <Common.hpp>

namespace AoC {

Solution::Solution() { }

struct File {
    strview name;
    bool directory;
    usize size;

    umap<strview, File> children;

    void print(usize depth = 0) {
        for (auto i = 0uz; i < depth; i++) {
            fmt::print(" ");
        }
        fmt::print("- {}{}\n", name, directory ? " (dir)" : "");

        if (directory) {
            for (auto &[n, f] : children) {
                f.print(depth + 1);
            }
        }
    }

    void calc_size() {
        if (!directory)
            return;

        for (auto &[n, f] : children) {
            f.calc_size();
            size += f.size;
        }
    }

    template<typename Fn>
    void traverse(Fn&& fn) {
        std::invoke(fn, this);
        if (!directory)
            return;

        for (auto &[n, f] : children) {
            f.traverse(std::forward<Fn>(fn));
        }
    }
};

void Solution::solve(bool example, size_t part, std::string_view input) {
    if (example)
        input = "$ cd /\n"
                "$ ls\n"
                "dir a\n"
                "14848514 b.txt\n"
                "8504156 c.dat\n"
                "dir d\n"
                "$ cd a\n"
                "$ ls\n"
                "dir e\n"
                "29116 f\n"
                "2557 g\n"
                "62596 h.lst\n"
                "$ cd e\n"
                "$ ls\n"
                "584 i\n"
                "$ cd ..\n"
                "$ cd ..\n"
                "$ cd d\n"
                "$ ls\n"
                "4060174 j\n"
                "8033020 d.log\n"
                "5626152 d.ext\n"
                "7214296 k"sv;

    File root {
        .name = "/",
        .directory = true,
        .size = 0,

        .children = {},
    };

    vec<File*> traversal_stack {};
    traversal_stack.push_back(&root);

    for (auto l : input | split("\n"sv) | fwd_to_strv | drop(1)) {
        auto tokens = vec_from_view<strview>(l | split(" "sv) | fwd_to_strv);

        if (tokens[0] == "$") {
            if (tokens[1] == "ls")
                continue;

            auto to_dir = tokens[2];
            if (to_dir == "..") {
                traversal_stack.pop_back();
                continue;
            }

            auto& children = traversal_stack.back()->children;

            auto it = children.find(to_dir);
            File* to_traverse;
            if (it == children.end()) {
                File child {
                    .name = to_dir,
                    .directory = true,
                    .size = 0,
                    .children = {},
                };
                auto [new_it, _] = children.insert({to_dir, child});
                to_traverse = &(new_it->second);
            } else {
                to_traverse = &(it->second);
            }

            traversal_stack.push_back(to_traverse);

            continue;
        }

        auto filename = tokens[1];
        auto& children = traversal_stack.back()->children;
        File child;

        if (tokens[0] == "dir") {
            child = {
                .name = filename,
                .directory = true,
                .size = 0,
                .children = {},
            };
        } else {
            child = {
                .name = filename,
                .directory = false,
                .size = 0,
                .children = {},
            };
            std::from_chars(tokens[0].data(), tokens[0].data() + tokens[0].size(), child.size);
        }

        std::ignore = children.insert({filename, child});
    }

    //root.print();
    root.calc_size();

    if (part == 1) {
        usize sz = 0;
        root.traverse([&sz](File* f) {
            if (!f->directory || f->size > 100000)
                return;
            sz += f->size;
        });
        fmt::print("{}\n", sz);
    } else if (part == 2) {
        vec<File*> all_dirs;
        root.traverse([&all_dirs](File* f) {
            if (!f->directory)
                return;
            all_dirs.push_back(f);
        });
        std::sort(begin(all_dirs), end(all_dirs), [](File* lhs, File* rhs) {
            return lhs->size < rhs->size;
        });

        const auto unused = 70000000 - root.size;

        //no bsearch because f you
        auto it = std::find_if(begin(all_dirs), end(all_dirs), [&](File* lhs) {
            return lhs->size >= 30000000 - unused;
        });
        fmt::print("{}\n", (*it)->size);
    }
}

}
