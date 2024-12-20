#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../util.h"

using coord_t = std::pair<int64_t, int64_t>;

struct HashC {
    std::size_t operator()(const coord_t& c) const { return ((int64_t)c.first << 32) | c.second; }
};

std::unordered_map<coord_t, uint64_t, HashC> dists_without_cheat {};

void init_dists(const std::vector<std::string>& lines, coord_t start, coord_t end)
{
    if (!dists_without_cheat.empty()) {
        return;
    }

    coord_t pos { start };
    uint64_t dist { 0 };

    while (pos != end) {
        dists_without_cheat.insert({ pos, dist });

        const std::vector<coord_t> neighbour_diffs { { 1, 0 }, { 0, 1 }, { 0, -1 }, { -1, 0 } };
        for (auto [d1, d2] : neighbour_diffs) {
            coord_t n { pos.first + d1, pos.second + d2 };
            if (!dists_without_cheat.contains(n) && lines.at(n.second).at(n.first) != '#') {
                pos = n;
                ++dist;
                break;
            }
        }
    }
    dists_without_cheat.insert({ end, dist });
}

struct Cheat {
    int64_t start_x;
    int64_t start_y;
    int64_t end_x;
    int64_t end_y;
    bool operator==(const Cheat& o) const
    {
        return start_x == o.start_x && start_y == o.start_y && end_x == o.end_x && end_y == o.end_y;
    }
};
struct HashCh {
    std::size_t operator()(const Cheat& c) const
    {
        return (c.start_x << 48) | (c.start_y << 32) | (c.end_x << 16) | c.end_y;
    }
};

void star_1(std::string file_name)
{
    uint64_t result { 0 };

    auto lines = util::get_all_lines(file_name).value();

    coord_t start, end;

    for (size_t y { 0 }; y < lines.size(); ++y) {
        for (size_t x { 0 }; x < lines.size(); ++x) {
            if (lines.at(y).at(x) == 'S') {
                start = { x, y };
            }
            if (lines.at(y).at(x) == 'E') {
                end = { x, y };
            }
        }
    }

    init_dists(lines, end, start);

    std::unordered_set<Cheat, HashCh> good_cheats {};

    for (auto [coord, dist] : dists_without_cheat) {
        auto [x, y] = coord;
        for (long dx { -2 }; dx <= 2; ++dx) {
            for (long dy { std::abs(dx) - 2 }; dy <= 2 - std::abs(dx); ++dy) {
                auto it = dists_without_cheat.find({ x + dx, y + dy });
                if (it != dists_without_cheat.end()) {
                    auto [coord2, dist2] = *it;
                    long diff = dist - dist2 - (std::abs(dx) + std::abs(dy));
                    if (diff >= 100) {
                        good_cheats.insert(
                            { coord.first, coord.second, coord2.first, coord2.second });
                    }
                }
            }
        }
    }

    result = good_cheats.size();

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

void star_2(std::string file_name)
{
    uint64_t result { 0 };

    auto lines = util::get_all_lines(file_name).value();

    coord_t start, end;

    for (size_t y { 0 }; y < lines.size(); ++y) {
        for (size_t x { 0 }; x < lines.size(); ++x) {
            if (lines.at(y).at(x) == 'S') {
                start = { x, y };
            }
            if (lines.at(y).at(x) == 'E') {
                end = { x, y };
            }
        }
    }

    init_dists(lines, end, start);

    std::unordered_set<Cheat, HashCh> good_cheats {};

    for (auto [coord, dist] : dists_without_cheat) {
        auto [x, y] = coord;
        for (long dx { -20 }; dx <= 20; ++dx) {
            for (long dy { std::abs(dx) - 20 }; dy <= 20 - std::abs(dx); ++dy) {
                auto it = dists_without_cheat.find({ x + dx, y + dy });
                if (it != dists_without_cheat.end()) {
                    auto [coord2, dist2] = *it;
                    long diff = dist - dist2 - (std::abs(dx) + std::abs(dy));
                    if (diff >= 100) {
                        good_cheats.insert(
                            { coord.first, coord.second, coord2.first, coord2.second });
                    }
                }
            }
        }
    }

    result = good_cheats.size();

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
