#include <cstdlib>
#include <iostream>
#include <string>

#include "../util.h"

void star_1(std::string file_name)
{
    size_t result { 0 };

    auto line = util::get_all_lines(file_name).value().at(0);

    std::vector<size_t> stones {};

    for (auto& s : util::split(line, " ")) {
        stones.push_back(std::stoi(s));
    }

    for (size_t i { 0 }; i < 25; ++i) {
        std::vector<size_t> new_stones {};
        new_stones.reserve(stones.size());

        for (auto stone : stones) {
            if (stone == 0) {
                new_stones.push_back(1);
                continue;
            }
            auto stone_str = std::to_string(stone);
            if (stone_str.size() % 2 == 0) {
                new_stones.push_back(std::stoi(stone_str.substr(0, stone_str.size() / 2)));
                new_stones.push_back(std::stoi(stone_str.substr(stone_str.size() / 2)));
                continue;
            }
            new_stones.push_back(stone * 2024);
        }

        stones = std::move(new_stones);
    }

    result = stones.size();

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

void insert_or_add(std::unordered_map<size_t, size_t>& map, size_t key, size_t amount)
{
    if (map.contains(key)) {
        map.at(key) += amount;
    } else {
        map.insert({ key, amount });
    }
}

void star_2(std::string file_name)
{
    size_t result { 0 };

    auto line = util::get_all_lines(file_name).value().at(0);

    std::unordered_map<size_t, size_t> stones {};
    for (auto& s : util::split(line, " ")) {
        insert_or_add(stones, std::stoi(s), 1);
    }

    for (size_t i { 0 }; i < 75; ++i) {
        std::unordered_map<size_t, size_t> new_stones {};
        new_stones.reserve(stones.size());

        for (auto [stone, amount] : stones) {
            if (stone == 0) {
                insert_or_add(new_stones, 1, amount);
                continue;
            }
            auto stone_str = std::to_string(stone);
            if (stone_str.size() % 2 == 0) {
                insert_or_add(
                    new_stones, std::stoi(stone_str.substr(0, stone_str.size() / 2)), amount);
                insert_or_add(
                    new_stones, std::stoi(stone_str.substr(stone_str.size() / 2)), amount);
                continue;
            }
            insert_or_add(new_stones, stone * 2024, amount);
        }

        stones = std::move(new_stones);
    }

    for (auto [stone, amount] : stones) {
        result += amount;
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
