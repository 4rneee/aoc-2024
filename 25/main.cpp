#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

#include "../util.h"

void star_1(std::string file_name)
{
    uint64_t result { 0 };

    std::vector<std::array<uint8_t, 5>> locks {};
    std::vector<std::array<uint8_t, 5>> keys {};

    util::for_n_lines(file_name, 8, [&](const std::vector<std::string>& lines) {
        std::array<uint8_t, 5> arr {};
        for (const auto& line : lines) {
            for (size_t i { 0 }; i < line.size(); ++i) {
                arr.at(i) += (line.at(i) == '#');
            }
        }
        if (lines.at(0) == "#####") {
            locks.push_back(arr);
        } else {
            keys.push_back(arr);
        }
    });

    for (auto& lock : locks) {
        for (auto& key : keys) {
            bool fit { true };
            for (size_t i { 0 }; i < 5; ++i) {
                if (lock.at(i) + key.at(i) > 7) {
                    fit = false;
                    break;
                }
            }
            if (fit) {
                ++result;
            }
        }
    }

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "No input file given\n";
        return 1;
    }
    star_1(std::string(argv[1]));
    return 0;
}
