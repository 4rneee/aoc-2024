#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <set>
#include <string>

#include "../util.h"

using coord_t = std::pair<long, long>;

void move_robot(std::vector<std::string>& grid, coord_t& robot, char direction)
{
    coord_t dir;
    switch (direction) {
    case '<':
        dir = { 0, -1 };
        break;
    case '>':
        dir = { 0, 1 };
        break;
    case '^':
        dir = { -1, 0 };
        break;
    case 'v':
        dir = { 1, 0 };
        break;
    default:
        return;
    }

    size_t i { 1 };
    while (grid.at(robot.first + i * dir.first).at(robot.second + i * dir.second) == 'O') {
        ++i;
    }

    if (grid.at(robot.first + i * dir.first).at(robot.second + i * dir.second) == '#') {
        return;
    }

    grid.at(robot.first).at(robot.second) = '.';
    grid.at(robot.first + dir.first).at(robot.second + dir.second) = '@';

    if (i > 1) {
        grid.at(robot.first + i * dir.first).at(robot.second + i * dir.second) = 'O';
    }

    robot.first += dir.first;
    robot.second += dir.second;
}

void star_1(std::string file_name)
{
    uint64_t result { 0 };

    auto lines = util::get_all_lines(file_name).value();

    size_t grid_height = 0;
    while (!lines.at(grid_height).empty()) {
        ++grid_height;
    }

    std::vector<std::string> grid {};
    grid.reserve(grid_height);

    grid.insert(grid.begin(), lines.begin(), lines.begin() + grid_height);

    coord_t robot;
    for (size_t y { 0 }; y < grid.size(); ++y) {
        for (size_t x { 0 }; x < grid.at(y).size(); ++x) {
            if (grid.at(y).at(x) == '@') {
                robot = { y, x };
            }
        }
    }

    for (size_t i { grid_height + 1 }; i < lines.size(); ++i) {
        for (char dir : lines.at(i)) {
            move_robot(grid, robot, dir);
        }
    }

    for (size_t y { 0 }; y < grid.size(); ++y) {
        for (size_t x { 0 }; x < grid.at(y).size(); ++x) {
            if (grid.at(y).at(x) == 'O') {
                result += 100 * y + x;
            }
        }
    }

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

void move_robot2(std::vector<std::string>& grid, coord_t& robot, char direction)
{
    coord_t dir;
    switch (direction) {
    case '<':
        dir = { 0, -1 };
        break;
    case '>':
        dir = { 0, 1 };
        break;
    case '^':
        dir = { -1, 0 };
        break;
    case 'v':
        dir = { 1, 0 };
        break;
    default:
        return;
    }

    if (direction == '<' || direction == '>') {
        size_t i { 1 };
        while (grid.at(robot.first + i * dir.first).at(robot.second + i * dir.second) == '[' ||
               grid.at(robot.first + i * dir.first).at(robot.second + i * dir.second) == ']') {
            ++i;
        }

        if (grid.at(robot.first + i * dir.first).at(robot.second + i * dir.second) == '#') {
            return;
        }

        while (i > 0) {
            grid.at(robot.first + i * dir.first).at(robot.second + i * dir.second) =
                grid.at(robot.first + (i - 1) * dir.first).at(robot.second + (i - 1) * dir.second);
            --i;
        }
        grid.at(robot.first).at(robot.second) = '.';
        grid.at(robot.first + dir.first).at(robot.second + dir.second) = '@';

        robot.first += dir.first;
        robot.second += dir.second;

        return;
    }

    std::set<coord_t> moving { { robot.first, robot.second } };
    std::vector<std::pair<long, long>> x_ranges { { robot.second, robot.second + 1 } };
    size_t i { 1 };
    while (true) {
        long x_from { x_ranges.back().first };
        long x_to { x_ranges.back().second };

        bool br { true };
        for (long x { x_ranges.back().first }; x < x_ranges.back().second; ++x) {
            if (grid.at(robot.first + i * dir.first).at(x) == '#') {
                if (moving.contains({ robot.first + (i - 1) * dir.first, x })) {
                    return;
                }
            }

            if (grid.at(robot.first + i * dir.first).at(x) == '[' &&
                (moving.contains({ robot.first + (i - 1) * dir.first, x }) ||
                 moving.contains({ robot.first + (i - 1) * dir.first, x + 1 }))) {
                br = false;
                x_to = std::max(x_to, x + 2);
                moving.insert({ robot.first + i * dir.first, x });
                moving.insert({ robot.first + i * dir.first, x + 1 });
            } else if (
                grid.at(robot.first + i * dir.first).at(x) == ']' &&
                (moving.contains({ robot.first + (i - 1) * dir.first, x }) ||
                 moving.contains({ robot.first + (i - 1) * dir.first, x - 1 }))) {
                br = false;
                x_from = std::min(x_from, x - 1);
                moving.insert({ robot.first + i * dir.first, x });
                moving.insert({ robot.first + i * dir.first, x - 1 });
            }
        }
        x_ranges.push_back({ x_from, x_to });

        if (br) {
            break;
        }
        ++i;
    }

    while (i > 0) {
        for (long x { x_ranges.at(i - 1).first }; x < x_ranges.at(i - 1).second; ++x) {
            if (moving.contains({ robot.first + (i - 1) * dir.first, x })) {
                grid.at(robot.first + i * dir.first).at(x) =
                    grid.at(robot.first + (i - 1) * dir.first).at(x);
                grid.at(robot.first + (i - 1) * dir.first).at(x) = '.';
            }
        }
        --i;
    }

    grid.at(robot.first).at(robot.second) = '.';
    robot.first += dir.first;
    robot.second += dir.second;
}

void star_2(std::string file_name)
{
    uint64_t result { 0 };

    auto lines = util::get_all_lines(file_name).value();

    size_t grid_height = 0;
    while (!lines.at(grid_height).empty()) {
        ++grid_height;
    }

    std::vector<std::string> small_grid {};
    small_grid.reserve(grid_height);

    small_grid.insert(small_grid.begin(), lines.begin(), lines.begin() + grid_height);

    std::vector<std::string> grid {};
    grid.reserve(grid_height);

    for (auto& line : small_grid) {
        grid.push_back("");
        for (auto c : line) {
            if (c == '#') {
                grid.back().append("##");
            } else if (c == 'O') {
                grid.back().append("[]");
            } else if (c == '.') {
                grid.back().append("..");
            } else if (c == '@') {
                grid.back().append("@.");
            }
        }
    }

    coord_t robot;
    for (size_t y { 0 }; y < grid.size(); ++y) {
        for (size_t x { 0 }; x < grid.at(y).size(); ++x) {
            if (grid.at(y).at(x) == '@') {
                robot = { y, x };
            }
        }
    }

    for (size_t i { grid_height + 1 }; i < lines.size(); ++i) {
        for (char dir : lines.at(i)) {
            move_robot2(grid, robot, dir);
        }
    }

    for (size_t y { 0 }; y < grid.size(); ++y) {
        for (size_t x { 0 }; x < grid.at(y).size(); ++x) {
            if (grid.at(y).at(x) == '[') {
                result += 100 * y + x;
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
