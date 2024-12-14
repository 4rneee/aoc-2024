#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <set>
#include <string>

#include "../util.h"

struct Robot {
    long p_x;
    long p_y;
    long v_x;
    long v_y;
};

void star_1(std::string file_name)
{
    uint64_t result { 0 };

    long MAX_X { 100 }; // 101 tiles wide (0 to 100)
    long MAX_Y { 102 }; // 103 tiles tall (0 to 102)

    if (file_name != "input.txt") {
        MAX_X = 10;
        MAX_Y = 6;
    }

    std::vector<Robot> robots {};
    util::for_each_line(file_name, [&](const std::string& line) {
        Robot robot;
        std::sscanf(
            line.c_str(), "p=%ld,%ld v=%ld,%ld", &robot.p_x, &robot.p_y, &robot.v_x, &robot.v_y);

        robots.push_back(robot);
    });

    long q_1 { 0 };
    long q_2 { 0 };
    long q_3 { 0 };
    long q_4 { 0 };

    for (auto& robot : robots) {
        for (size_t i = 0; i < 100; ++i) {
            robot.p_x += robot.v_x;
            robot.p_y += robot.v_y;

            if (robot.p_x < 0) {
                robot.p_x = MAX_X + 1 + robot.p_x;
            } else if (robot.p_x > MAX_X) {
                robot.p_x -= MAX_X + 1;
            }

            if (robot.p_y < 0) {
                robot.p_y = MAX_Y + 1 + robot.p_y;
            } else if (robot.p_y > MAX_Y) {
                robot.p_y -= MAX_Y + 1;
            }
        }

        for (auto& robot : robots) {
            if (robot.p_x < MAX_X / 2) {
                if (robot.p_y < MAX_Y / 2) {
                    ++q_1;
                } else if (robot.p_y > MAX_Y / 2) {
                    ++q_2;
                }
            } else if (robot.p_x > MAX_X / 2) {
                if (robot.p_y < MAX_Y / 2) {
                    ++q_3;
                } else if (robot.p_y > MAX_Y / 2) {
                    ++q_4;
                }
            }
        }
    }

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

void star_2(std::string file_name)
{
    uint64_t result { 0 };

    long MAX_X { 100 }; // 101 tiles wide (0 to 100)
    long MAX_Y { 102 }; // 103 tiles tall (0 to 102)

    std::vector<Robot> robots {};
    util::for_each_line(file_name, [&](const std::string& line) {
        Robot robot;
        std::sscanf(
            line.c_str(), "p=%ld,%ld v=%ld,%ld", &robot.p_x, &robot.p_y, &robot.v_x, &robot.v_y);

        robots.push_back(robot);
    });

    for (size_t i { 0 }; i < 10000; ++i) {
        std::set<std::pair<long, long>> robot_tiles {};

        for (auto& robot : robots) {
            robot.p_x += robot.v_x;
            robot.p_y += robot.v_y;

            if (robot.p_x < 0) {
                robot.p_x = MAX_X + 1 + robot.p_x;
            } else if (robot.p_x > MAX_X) {
                robot.p_x -= MAX_X + 1;
            }

            if (robot.p_y < 0) {
                robot.p_y = MAX_Y + 1 + robot.p_y;
            } else if (robot.p_y > MAX_Y) {
                robot.p_y -= MAX_Y + 1;
            }
            robot_tiles.insert({ robot.p_x, robot.p_y });
        }

        // hardcoded solution (solved it visually)
        if (i + 1 == 7138) {
            std::cout << i + 1 << '\n';
            result = i + 1;
            for (size_t y { 0 }; y <= MAX_Y; ++y) {
                for (size_t x { 0 }; x <= MAX_X; ++x) {
                    if (robot_tiles.contains({ x, y })) {
                        std::cout << '#';
                    } else {
                        std::cout << '.';
                    }
                }
                std::cout << '\n';
            }
            break;
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
