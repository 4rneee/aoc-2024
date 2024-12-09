#include <cstdlib>
#include <iostream>
#include <list>
#include <string>

#include "../util.h"

struct fragment {
    bool free;
    long len;
    long file_id;
};

void star_1(std::string file_name)
{
    unsigned long result { 0 };

    auto input = util::get_all_lines(file_name).value().front();

    std::vector<fragment> fragments {};

    bool free = false;
    long file_id = 0;
    for (char c : input) {
        long len = c - '0';
        fragment f;
        f.free = free;
        f.len = len;
        if (!f.free) {
            f.file_id = file_id;
            ++file_id;
        }
        if (f.len > 0) {
            fragments.push_back(f);
        }

        free = !free;
    }

    auto free_it = fragments.begin();
    auto file_it = fragments.end() - 1;

    while (free_it < file_it) {
        while (!free_it->free) {
            ++free_it;
        }
        while (file_it->free) {
            --file_it;
        }

        if (free_it->len < file_it->len) {
            free_it->file_id = file_it->file_id;
            free_it->free = false;

            file_it->len = file_it->len - free_it->len;
        } else if (free_it->len == file_it->len) {
            free_it->file_id = file_it->file_id;
            free_it->len = file_it->len;
            free_it->free = false;

            file_it->free = true;
        } else { // free_it->len > file_it->len
            free_it->len = free_it->len - file_it->len;
            free_it = fragments.insert(free_it, *file_it);

            file_it = file_it + 1;

            file_it->free = true;
        }
    }

    size_t position { 0 };
    for (auto& f : fragments) {
        if (!f.free) {
            for (size_t i { 0 }; i < f.len; ++i) {
                result += f.file_id * position;
                ++position;
            }
        } else {
            position += f.len;
        }
    }

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

void star_2(std::string file_name)
{
    unsigned long result { 0 };

    auto input = util::get_all_lines(file_name).value().front();

    std::vector<fragment> fragments {};

    bool free = false;
    long file_id = 0;
    for (char c : input) {
        long len = c - '0';
        fragment f;
        f.free = free;
        f.len = len;
        if (!f.free) {
            f.file_id = file_id;
            ++file_id;
        }
        if (f.len > 0) {
            fragments.push_back(f);
        }

        free = !free;
    }

    auto file_it = fragments.end() - 1;

    while (file_it >= fragments.begin()) {

        while (file_it->free) {
            --file_it;
        }
        auto free_it = fragments.begin();
        while (free_it < file_it && (!free_it->free || free_it->len < file_it->len)) {
            ++free_it;
        }
        if (free_it >= file_it) {
            --file_it;
            continue;
        }

        if (free_it->len == file_it->len) {
            free_it->file_id = file_it->file_id;
            free_it->len = file_it->len;
            free_it->free = false;

            file_it->free = true;
        } else if (free_it->len > file_it->len) {
            free_it->len = free_it->len - file_it->len;
            free_it = fragments.insert(free_it, *file_it);

            file_it = file_it + 1;

            file_it->free = true;
        }
    }

    size_t position { 0 };
    for (auto& f : fragments) {
        if (!f.free) {
            for (size_t i { 0 }; i < f.len; ++i) {
                result += f.file_id * position;
                ++position;
            }
        } else {
            position += f.len;
        }
    }

    std::cout << "Solution for Star 2:\n";
    std::cout << result << '\n';
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "No input file given\n";
        return 1;
    }
    star_1(std::string(argv[1]));
    star_2(std::string(argv[1]));
    return 0;
}
