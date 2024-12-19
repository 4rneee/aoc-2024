#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>

#include "../util.h"

bool is_possible(const std::vector<std::string>& towels, std::string design)
{
    std::set<std::string> subdesigns { design };

    while (!subdesigns.empty()) {
        std::set<std::string> new_subdesigns {};
        for (auto d : subdesigns) {
            if (d.empty()) {
                return true;
            }
            for (const auto& t : towels) {
                if (d.starts_with(t)) {
                    new_subdesigns.insert(d.substr(t.size()));
                }
            }
        }

        subdesigns = std::move(new_subdesigns);
    }

    return false;
}

void star_1(std::string file_name)
{
    uint64_t result { 0 };

    auto lines = util::get_all_lines(file_name).value();

    auto towels = util::split(lines.at(0), ", ");

    for (size_t i { 2 }; i < lines.size(); ++i) {
        if (is_possible(towels, lines.at(i))) {
            ++result;
        }
    }

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

uint64_t possibilities(
    std::unordered_map<std::string, uint64_t>& mem,
    const std::vector<std::string>& towels,
    std::string design)
{
    if (mem.contains(design)) {
        return mem.at(design);
    }

    uint64_t p { 0 };

    for (const auto& t : towels) {
        if (design.starts_with(t)) {
            p += possibilities(mem, towels, design.substr(t.size()));
        }
    }

    mem.insert({ design, p });
    return p;
}

void star_2(std::string file_name)
{
    uint64_t result { 0 };

    auto lines = util::get_all_lines(file_name).value();

    auto towels = util::split(lines.at(0), ", ");

    std::unordered_map<std::string, uint64_t> mem {};
    mem.insert_or_assign("", 1);

    for (size_t i { 2 }; i < lines.size(); ++i) {
        result += possibilities(mem, towels, lines.at(i));
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
