#include <cstdlib>
#include <iostream>
#include <string>

#include "../util.h"

bool safe_line(std::vector<std::string> split, std::optional<size_t> skip)
{
    if (skip) {
        split.erase(split.begin() + *skip);
    }

    int last_level { std::stoi(split[0]) };
    bool increasing { last_level < std::stoi(split[1]) };
    bool safe { true };

    for (size_t i { 1 }; i < split.size(); ++i) {
        auto level = std::stoi(split[i]);
        int diff = level - last_level;

        last_level = level;

        if (std::abs(diff) == 0 || std::abs(diff) > 3) {
            safe = false;
            break;
        }
        if (increasing && diff < 0) {
            safe = false;
            break;
        }
        if (!increasing && diff > 0) {
            safe = false;
            break;
        }
    }

    return safe;
}

void star_1(std::string file_name)
{
    int result { 0 };

    util::for_each_line(file_name, [&result](const std::string& line) {
        auto split = util::split(line, " ");

        if (safe_line(split, {})) {
            ++result;
        }
    });

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

void star_2(std::string file_name)
{
    int result { 0 };

    util::for_each_line(file_name, [&result](const std::string& line) {
        //
        auto split = util::split(line, " ");
        int last_level { std::stoi(split[0]) };
        bool increasing { last_level < std::stoi(split[1]) };

        bool safe { false };

        for (size_t i { 0 }; i < split.size(); ++i) {
            safe = safe_line(split, i);
            if (safe) {
                break;
            }
        }

        if (safe) {
            ++result;
        }
    });

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
