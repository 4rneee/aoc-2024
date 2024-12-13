#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

#include "../util.h"

void star_1(std::string file_name)
{
    uint64_t result { 0 };

    util::for_n_lines(file_name, 4, [&](const std::vector<std::string>& lines) {
        long a_x, a_y, b_x, b_y, p_x, p_y;
        std::sscanf(lines.at(0).c_str(), "Button A: X+%ld, Y+%ld", &a_x, &a_y);
        std::sscanf(lines.at(1).c_str(), "Button B: X+%ld, Y+%ld", &b_x, &b_y);
        std::sscanf(lines.at(2).c_str(), "Prize: X=%ld, Y=%ld", &p_x, &p_y);

        long used_a { 0 };
        long used_b { 0 };

        while (used_a * a_x + used_b * b_x <= p_x && used_a * a_y + used_b * b_y <= p_y &&
               used_b <= 100) {
            ++used_b;
        }
        --used_b;

        while (used_a * a_x + used_b * b_x != p_x || used_a * a_y + used_b * b_y != p_y) {
            if (used_b < 0 || used_a > 100) {
                used_a = used_b = 0;
                break;
            }
            if ((used_a + 1) * a_x + used_b * b_x > p_x ||
                (used_a + 1) * a_y + used_b * b_y > p_y) {
                --used_b;
            } else {
                ++used_a;
            }
        }
        result += 3 * used_a + used_b;
    });

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

void star_2(std::string file_name)
{
    uint64_t result { 0 };

    util::for_n_lines(file_name, 4, [&](const std::vector<std::string>& lines) {
        long a_x, a_y, b_x, b_y, p_x, p_y;
        std::sscanf(lines.at(0).c_str(), "Button A: X+%ld, Y+%ld", &a_x, &a_y);
        std::sscanf(lines.at(1).c_str(), "Button B: X+%ld, Y+%ld", &b_x, &b_y);
        std::sscanf(lines.at(2).c_str(), "Prize: X=%ld, Y=%ld", &p_x, &p_y);

        p_x += 10000000000000;
        p_y += 10000000000000;

        long used_a { 0 };
        long used_b { 0 };

        long t1 = a_x * p_y - a_y * p_x;
        long t2 = a_x * b_y - a_y * b_x;

        if (t1 % t2 == 0) {
            used_b = t1 / t2;

            t1 = p_x - used_b * b_x;

            if (t1 % a_x == 0) {
                used_a = t1 / a_x;
            } else {
                used_b = 0;
            }
        }

        result += 3 * used_a + used_b;
        std::cout << used_a << ' ' << used_b << '\n';
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
