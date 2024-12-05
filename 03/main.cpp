#include <cstdlib>
#include <iostream>
#include <regex>
#include <string>

#include "../util.h"

void star_1(std::string file_name)
{
    uint64_t result { 0 };

    std::regex exp { "mul\\([0-9]+,[0-9]+\\)" };

    util::for_each_line(file_name, [&result, &exp](const std::string& line) {
        auto begin = std::sregex_iterator { line.begin(), line.end(), exp };
        auto end = std::sregex_iterator {};

        for (auto i = begin; i != end; ++i) {
            std::string match { i->str() };
            //auto idx = match.find(',');
            //uint64_t x = std::stoi(match.substr(4, idx));
            //uint64_t y = std::stoi(match.substr(idx + 1, match.size() - 1));
            uint64_t x;
            uint64_t y;
            std::sscanf(match.c_str(), "mul(%ld,%ld)", &x, &y);
            result += x * y;
        }
    });

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

void star_2(std::string file_name)
{
    uint64_t result { 0 };

    std::regex exp { "mul\\([0-9]+,[0-9]+\\)|do\\(\\)|don't\\(\\)" };

    bool do_ { true };
    util::for_each_line(file_name, [&result, &exp, &do_](const std::string& line) {
        auto begin = std::sregex_iterator { line.begin(), line.end(), exp };
        auto end = std::sregex_iterator {};

        for (auto i = begin; i != end; ++i) {
            std::string match { i->str() };
            if (match == "do()") {
                do_ = true;
                continue;
            } else if (match == "don't()") {
                do_ = false;
                continue;
            }
            auto idx = match.find(',');
            uint64_t x = std::stoi(match.substr(4, idx));
            uint64_t y = std::stoi(match.substr(idx + 1, match.size() - 1));
            if (do_) {
                result += x * y;
            }
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
