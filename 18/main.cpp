#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>

#include "../util.h"

using coord_t = std::pair<long, long>;

struct HashC {
    std::size_t operator()(const coord_t& c) const { return ((int64_t)c.first << 32) | c.second; }
};

struct Position {
    size_t x;
    size_t y;
    unsigned long distance;

    bool operator==(const Position& other) const { return x == other.x && y == other.y; }
    bool operator<(const Position& other) const { return distance > other.distance; }
};
struct HashP {
    std::size_t operator()(const Position& p) const { return ((int64_t)p.x << 32) | p.y; }
};

void star_1(std::string file_name)
{
    uint64_t result { 0 };

    auto lines = util::get_all_lines(file_name).value();

    std::unordered_set<coord_t, HashC> bytes {};

    for (size_t i { 0 }; i < 1024; ++i) {
        long x, y;
        std::sscanf(lines.at(i).c_str(), "%ld,%ld", &x, &y);

        if (!bytes.contains({ x, y })) {
            bytes.insert({ x, y });
        }
    }

    Position start { 0, 0, 0 };

    long MAX_X { 70 };
    long MAX_Y { 70 };
    coord_t end { MAX_X, MAX_Y };

    std::unordered_set<Position, HashP> used {};
    std::priority_queue<Position> pq {};
    pq.push({ 0, 0, 0 });

    while (!pq.empty()) {
        Position cur = pq.top();
        pq.pop();

        if (cur.x == end.first && cur.y == end.second) {
            result = cur.distance;
            break;
        }

        const std::vector<coord_t> neighbour_diffs { { 1, 0 }, { 0, 1 }, { 0, -1 }, { -1, 0 } };
        for (auto [d1, d2] : neighbour_diffs) {
            Position n = { cur.x + d1, cur.y + d2, cur.distance + 1 };
            if (n.x >= 0 && n.x <= MAX_X && n.y >= 0 && n.y <= MAX_Y) {
                if (!used.contains(n)) {
                    if (!bytes.contains({ n.x, n.y })) {
                        pq.push(n);
                        used.insert(n);
                    }
                }
            }
        }
    }

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

bool has_path(const std::vector<std::string>& lines, size_t steps_to_simulate)
{
    std::unordered_set<coord_t, HashC> bytes {};

    for (size_t i { 0 }; i < steps_to_simulate; ++i) {
        long x, y;
        std::sscanf(lines.at(i).c_str(), "%ld,%ld", &x, &y);

        if (!bytes.contains({ x, y })) {
            bytes.insert({ x, y });
        }
    }

    Position start { 0, 0, 0 };

    long MAX_X { 70 };
    long MAX_Y { 70 };
    coord_t end { MAX_X, MAX_Y };

    std::unordered_set<Position, HashP> used {};
    std::priority_queue<Position> pq {};
    pq.push({ 0, 0, 0 });

    while (!pq.empty()) {
        Position cur = pq.top();
        pq.pop();

        if (cur.x == end.first && cur.y == end.second) {
            return true;
        }

        const std::vector<coord_t> neighbour_diffs { { 1, 0 }, { 0, 1 }, { 0, -1 }, { -1, 0 } };
        for (auto [d1, d2] : neighbour_diffs) {
            Position n = { cur.x + d1, cur.y + d2, cur.distance + 1 };
            if (n.x >= 0 && n.x <= MAX_X && n.y >= 0 && n.y <= MAX_Y) {
                if (!used.contains(n)) {
                    if (!bytes.contains({ n.x, n.y })) {
                        pq.push(n);
                        used.insert(n);
                    }
                }
            }
        }
    }

    return false;
}

void star_2(std::string file_name)
{
    std::string result {};

    auto lines = util::get_all_lines(file_name).value();

    for (size_t i { 0 }; i < lines.size(); ++i) {
        if (!has_path(lines, i+1)) {
            result = lines.at(i);
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
