#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <string>

#include "../util.h"

struct Equation {
    long result { 0 };
    std::vector<long> nums {};
};

bool can_be_true(const Equation& eq, unsigned long long acc = 0, bool star2 = false)
{
    if (eq.nums.size() == 0) {
        if (eq.result == acc) {
            return true;
        } else {
            return false;
        }
    }
    if (eq.result < acc) {
        return false;
    }

    Equation new_eq;
    new_eq.result = eq.result;

    for (size_t j { 1 }; j < eq.nums.size(); ++j) {
        new_eq.nums.push_back(eq.nums.at(j));
    }
    if (can_be_true(new_eq, acc + eq.nums.front(), star2)) {
        return true;
    }
    if (can_be_true(new_eq, std::max((unsigned long long)1, acc) * eq.nums.front(), star2)) {
        return true;
    }

    if (star2) {
        if (can_be_true(
                new_eq,
                std::stoull(std::to_string(acc) + std::to_string(eq.nums.front())),
                star2)) {
            return true;
        }
    }
    return false;
}


void star_1(std::string file_name)
{
    unsigned long result { 0 };

    util::for_each_line(file_name, [&result](const std::string& line) {
        Equation eq;
        eq.result = std::stol(line.substr(0, line.find_first_of(':')));
        for (auto& n_str : util::split(util::split(line, ": ").back(), " ")) {
            eq.nums.push_back(std::stoi(n_str));
        }

        if (can_be_true(eq)) {
            result += eq.result;
        }
    });

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

void star_2(std::string file_name)
{
    unsigned long result { 0 };

    util::for_each_line(file_name, [&result](const std::string& line) {
        Equation eq;
        eq.result = std::stol(line.substr(0, line.find_first_of(':')));
        for (auto& n_str : util::split(util::split(line, ": ").back(), " ")) {
            eq.nums.push_back(std::stoi(n_str));
        }

        if (can_be_true(eq, 0, true)) {
            result += eq.result;
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
