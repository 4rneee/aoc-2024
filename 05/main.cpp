#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <set>
#include <string>

#include "../util.h"

void star_1(std::string file_name)
{
    int result { 0 };
    long page_idx { INTMAX_MAX };
    int idx { 0 };
    std::vector<std::vector<int>> pages {};

    std::set<std::pair<unsigned, unsigned>> rules {};
    util::for_each_line(
        file_name, [&result, &rules, &idx, &page_idx, &pages](const std::string line) {
            unsigned x, y;
            if (std::sscanf(line.c_str(), "%u|%u", &x, &y) == 2) {
                rules.insert(std::make_pair(x, y));
            }

            if (idx >= page_idx) {
                pages.push_back({});

                for (const auto& s : util::split(line, ",")) {
                    pages.back().push_back(std::stoi(s));
                }
            }

            if (line.size() == 0) {
                page_idx = idx + 1;
            }
            ++idx;
        });

    for (const auto& page : pages) {
        bool correct { true };
        for (size_t i { 0 }; i < page.size(); ++i) {
            for (size_t j { i + 1 }; j < page.size(); ++j) {
                if (rules.contains(std::make_pair(page.at(j), page.at(i)))) {
                    correct = false;
                    break;
                }
            }
            if (!correct) {
                break;
            }
        }
        if (correct) {
            result += page.at(page.size() / 2);
        }
    }

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

std::tuple<bool, size_t, size_t> crct(
    std::vector<int>& page, const std::set<std::pair<unsigned, unsigned>>& rules)
{
    bool correct { true };
    for (size_t i { 0 }; i < page.size(); ++i) {
        for (size_t j { i + 1 }; j < page.size(); ++j) {
            if (rules.contains(std::make_pair(page.at(j), page.at(i)))) {
                return std::make_tuple(false, i, j);
            }
        }
    }

    return std::make_tuple(correct, -1, -1);
}

void star_2(std::string file_name)
{
    int result { 0 };
    long page_idx { INTMAX_MAX };
    int idx { 0 };
    std::vector<std::vector<int>> pages {};

    std::set<std::pair<unsigned, unsigned>> rules {};
    util::for_each_line(
        file_name, [&result, &rules, &idx, &page_idx, &pages](const std::string line) {
            unsigned x, y;
            if (std::sscanf(line.c_str(), "%u|%u", &x, &y) == 2) {
                rules.insert(std::make_pair(x, y));
            }

            if (idx >= page_idx) {
                pages.push_back({});

                for (const auto& s : util::split(line, ",")) {
                    pages.back().push_back(std::stoi(s));
                }
            }

            if (line.size() == 0) {
                page_idx = idx + 1;
            }
            ++idx;
        });

    std::vector<std::vector<int>> incorect {};
    for (const auto& page : pages) {
        bool correct { true };
        for (size_t i { 0 }; i < page.size(); ++i) {
            for (size_t j { i + 1 }; j < page.size(); ++j) {
                if (rules.contains(std::make_pair(page.at(j), page.at(i)))) {
                    correct = false;
                    break;
                }
            }
            if (!correct) {
                break;
            }
        }
        if (!correct) {
            incorect.push_back(page);
        }
    }

    for (auto& page : incorect) {
        bool correct { false };
        do {
            auto [c, i, j] = crct(page, rules);
            correct = c;
            if (!correct) {
                std::swap(page.at(i), page.at(j));
            }

        } while (!correct);
        result += page.at(page.size() / 2);
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
