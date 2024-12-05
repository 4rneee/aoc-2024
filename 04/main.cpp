#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/types.h>

#include "../util.h"

void star_1(std::string file_name)
{
    int result { 0 };

    auto lines = util::get_all_lines(file_name).value();

    for (size_t y { 0 }; y < lines.size(); ++y) {
        for (size_t x { 0 }; x < lines.at(y).size(); ++x) {
            if (lines.at(y).at(x) == 'X') {
                if (lines.at(y).substr(x, 4) == "XMAS") {
                    ++result;
                }
                if (y < lines.size() - 3) {
                    if (lines.at(y + 1).at(x) == 'M' && lines.at(y + 2).at(x) == 'A' &&
                        lines.at(y + 3).at(x) == 'S') {
                        ++result;
                    }
                }
                if (y < lines.size() - 3 && x < lines.at(y).size() - 3) {
                    if (lines.at(y + 1).at(x + 1) == 'M' && lines.at(y + 2).at(x + 2) == 'A' &&
                        lines.at(y + 3).at(x + 3) == 'S') {
                        ++result;
                    }
                }
                if (y >= 3 && x < lines.at(y).size() - 3) {
                    if (lines.at(y - 1).at(x + 1) == 'M' && lines.at(y - 2).at(x + 2) == 'A' &&
                        lines.at(y - 3).at(x + 3) == 'S') {
                        ++result;
                    }
                }

            } else if (lines.at(y).at(x) == 'S') {
                if (lines.at(y).substr(x, 4) == "SAMX") {
                    ++result;
                }
                if (y < lines.size() - 3) {
                    if (lines.at(y + 1).at(x) == 'A' && lines.at(y + 2).at(x) == 'M' &&
                        lines.at(y + 3).at(x) == 'X') {
                        ++result;
                    }
                }
                if (y < lines.size() - 3 && x < lines.at(y).size() - 3) {
                    if (lines.at(y + 1).at(x + 1) == 'A' && lines.at(y + 2).at(x + 2) == 'M' &&
                        lines.at(y + 3).at(x + 3) == 'X') {
                        ++result;
                    }
                }

                if (y >= 3 && x < lines.at(y).size() - 3) {
                    if (lines.at(y - 1).at(x + 1) == 'A' && lines.at(y - 2).at(x + 2) == 'M' &&
                        lines.at(y - 3).at(x + 3) == 'X') {
                        ++result;
                    }
                }
            }
        }
    }

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

void star_2(std::string file_name)
{
    int result { 0 };

    auto lines = util::get_all_lines(file_name).value();

    for (size_t y { 0 }; y < lines.size() - 2; ++y) {
        for (size_t x { 0 }; x < lines.at(y).size() - 2; ++x) {
            if (lines.at(y).at(x) == 'M' && lines.at(y + 1).at(x + 1) == 'A' &&
                lines.at(y + 2).at(x + 2) == 'S') {
                if (lines.at(y + 2).at(x) == 'M' && lines.at(y).at(x + 2) == 'S' ||
                    lines.at(y + 2).at(x) == 'S' && lines.at(y).at(x + 2) == 'M') {
                    ++result;
                }
            }
            if (lines.at(y).at(x) == 'S' && lines.at(y + 1).at(x + 1) == 'A' &&
                lines.at(y + 2).at(x + 2) == 'M') {
                if (lines.at(y + 2).at(x) == 'M' && lines.at(y).at(x + 2) == 'S' ||
                    lines.at(y + 2).at(x) == 'S' && lines.at(y).at(x + 2) == 'M') {
                    ++result;
                }
            }
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
