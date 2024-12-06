#include <cstdlib>
#include <iostream>
#include <set>
#include <string>

#include "../util.h"

enum class Direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3,
};
using coord_t = std::pair<int, int>;

coord_t get_next(coord_t pos, int direction)
{
    coord_t next;
    switch (direction) {
    case 0:
        next.first = pos.first - 1;
        next.second = pos.second;
        break;
    case 1:
        next.first = pos.first;
        next.second = pos.second + 1;
        break;
    case 2:
        next.first = pos.first + 1;
        next.second = pos.second;
        break;
    case 3:
        next.first = pos.first;
        next.second = pos.second - 1;
        break;
    }
    return next;
}

void star_1(std::string file_name)
{
    int result { 0 };

    auto lines = util::get_all_lines(file_name).value();

    coord_t pos = std::make_pair(94, 73);
    int direction = 0;

    std::set<coord_t> visited {};

    while (pos.first >= 0 && pos.second >= 0 && pos.first < lines.size() &&
           pos.second < lines.at(0).size()) {
        visited.insert(pos);
        coord_t next = get_next(pos, direction);
        if (next.first < 0 || next.second < 0 || next.first >= lines.size() ||
            next.second >= lines.at(0).size()) {
            break;
        }

        if (lines.at(next.first).at(next.second) == '#' ||
            lines.at(next.first).at(next.second) == 'O') {
            direction = (direction + 1) % 4;
            pos = get_next(pos, direction);
        } else {
            pos = next;
        }
    }

    result = visited.size();

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

// y, x, direction
using pos_t = std::tuple<int, int, int>;

bool has_loop(const std::vector<std::string>& lines)
{
    std::set<pos_t> visited {};

    coord_t start = std::make_pair(6, 4);
    start = std::make_pair(94, 73);
    coord_t pos = start;
    int direction = 0;

    while (pos.first >= 0 && pos.second >= 0 && pos.first < lines.size() &&
           pos.second < lines.at(0).size()) {
        if (visited.contains(std::make_tuple(pos.first, pos.second, direction))) {
            return true;
        }
        visited.insert(std::make_tuple(pos.first, pos.second, direction));

        coord_t next = get_next(pos, direction);
        if (next.first < 0 || next.second < 0 || next.first >= lines.size() ||
            next.second >= lines.at(0).size()) {
            return false;
        }

        if (lines.at(next.first).at(next.second) == '#') {
            direction = (direction + 1) % 4;
            next = get_next(pos, direction);
            if (lines.at(next.first).at(next.second) == '#') {
                visited.insert(std::make_tuple(pos.first, pos.second, direction));
                direction = (direction + 1) % 4;
                next = get_next(pos, direction);
                if (lines.at(next.first).at(next.second) == '#') {
                    visited.insert(std::make_tuple(pos.first, pos.second, direction));
                    direction = (direction + 1) % 4;
                    next = get_next(pos, direction);
                }
            }
        }
        pos = next;
    }
    return false;
}

void star_2(std::string file_name)
{
    int result { 0 };

    auto lines = util::get_all_lines(file_name).value();

    coord_t start = std::make_pair(6, 4);
    start = std::make_pair(94, 73);
    coord_t pos = start;
    int direction = 0;

    std::set<coord_t> visited {};

    while (pos.first >= 0 && pos.second >= 0 && pos.first < lines.size() &&
           pos.second < lines.at(0).size()) {
        visited.insert(pos);
        coord_t next = get_next(pos, direction);
        if (next.first < 0 || next.second < 0 || next.first >= lines.size() ||
            next.second >= lines.at(0).size()) {
            break;
        }

        if (lines.at(next.first).at(next.second) == '#') {
            direction = (direction + 1) % 4;
            pos = get_next(pos, direction);
        } else {
            pos = next;
        }
    }

    visited.erase(start);
    size_t i { 0 };
    for (const auto& candidate : visited) {
        //std::cout << ++i << '/' << visited.size() << '\n';

        lines.at(candidate.first).at(candidate.second) = '#';
        if (has_loop(lines)) {
            ++result;
        }
        lines.at(candidate.first).at(candidate.second) = '.';
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
