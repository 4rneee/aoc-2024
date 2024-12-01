#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>

#include "../util.h"

void star_1(std::string file_name)
{
    int result { 0 };

    std::vector<int> left {};
    std::vector<int> right {};
    util::for_each_line(file_name, [&left, &right](const std::string& line) {
        auto split = util::split(line, "   ");

        int first = std::stoi(split[0]);
        int second = std::stoi(split[split.size() - 1]);
        left.push_back(first);
        right.push_back(second);
    });

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    for (int i { 0 }; i < left.size(); ++i) {
        result += std::abs(left[i] - right[i]);
    }

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

void star_2(std::string file_name)
{
    int result { 0 };

    std::vector<int> left {};
    std::vector<int> right {};
    util::for_each_line(file_name, [&left, &right](const std::string& line) {
        auto split = util::split(line, "   ");

        int first = std::stoi(split[0]);
        int second = std::stoi(split[split.size() - 1]);
        left.push_back(first);
        right.push_back(second);
    });

    for (int i { 0 }; i < left.size(); ++i) {
        result += std::count(right.begin(), right.end(), left[i]) * left[i];
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
