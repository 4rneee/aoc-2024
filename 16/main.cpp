#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <unordered_set>

#include "../util.h"

enum class Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
};

struct Position {
    size_t x;
    size_t y;
    unsigned long distance;
    Direction direction;

    bool operator==(const Position& other) const
    {
        return x == other.x && y == other.y && direction == other.direction;
    }
    bool operator<(const Position& other) const { return distance > other.distance; }
};

struct HashPos {
    std::size_t operator()(const Position& p) const { return (p.x << 32) + p.y + (int)p.direction; }
};

void star_1(std::string file_name)
{
    uint64_t result { 0 };

    auto lines = util::get_all_lines(file_name).value();

    Position start = { 1, lines.size() - 2, 0, Direction::RIGHT };

    std::priority_queue<Position> pq {};
    pq.push(start);

    std::unordered_set<Position, HashPos> used {};
    used.insert(start);

    while (!pq.empty()) {
        Position cur = pq.top();
        pq.pop();

        if (lines.at(cur.y).at(cur.x) == 'E') {
            result = cur.distance;
            break;
        }

        std::vector<Position> to_push {};

        size_t next_x = cur.x;
        size_t next_y = cur.y;
        switch (cur.direction) {
        case Direction::RIGHT:
            next_x += 1;
            to_push.push_back({ cur.x, cur.y, cur.distance + 1000, Direction::DOWN });
            to_push.push_back({ cur.x, cur.y, cur.distance + 1000, Direction::UP });
            break;
        case Direction::LEFT:
            next_x -= 1;
            to_push.push_back({ cur.x, cur.y, cur.distance + 1000, Direction::DOWN });
            to_push.push_back({ cur.x, cur.y, cur.distance + 1000, Direction::UP });
            break;
        case Direction::UP:
            next_y -= 1;
            to_push.push_back({ cur.x, cur.y, cur.distance + 1000, Direction::RIGHT });
            to_push.push_back({ cur.x, cur.y, cur.distance + 1000, Direction::LEFT });
            break;
        case Direction::DOWN:
            next_y += 1;
            to_push.push_back({ cur.x, cur.y, cur.distance + 1000, Direction::RIGHT });
            to_push.push_back({ cur.x, cur.y, cur.distance + 1000, Direction::LEFT });
            break;
        }

        if (lines.at(next_y).at(next_x) != '#') {
            Position next { next_x, next_y, cur.distance + 1, cur.direction };
            to_push.push_back(next);
        }

        for (auto p : to_push) {
            if (!used.contains(p)) {
                pq.push(p);
                used.insert(p);
            }
        }
    }

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

void star_2(std::string file_name)
{
    uint64_t result { 0 };

    auto lines = util::get_all_lines(file_name).value();

    Position start = { 1, lines.size() - 2, 0, Direction::RIGHT };

    std::priority_queue<Position> pq {};
    pq.push(start);

    std::unordered_map<Position, std::unordered_set<Position, HashPos>, HashPos> parents_of_used {};
    parents_of_used.insert_or_assign(start, std::unordered_set<Position, HashPos> {});

    bool found { false };
    size_t shortes_len { 0 };

    while (!pq.empty()) {
        Position cur = pq.top();
        pq.pop();

        if (lines.at(cur.y).at(cur.x) == 'E') {
            if (!found) {
                found = true;
                shortes_len = cur.distance;
            }
            continue;
        }

        std::vector<Position> to_push {};

        size_t next_x = cur.x;
        size_t next_y = cur.y;
        switch (cur.direction) {
        case Direction::RIGHT:
            next_x += 1;
            to_push.push_back({ cur.x, cur.y, cur.distance + 1000, Direction::DOWN });
            to_push.push_back({ cur.x, cur.y, cur.distance + 1000, Direction::UP });
            break;
        case Direction::LEFT:
            next_x -= 1;
            to_push.push_back({ cur.x, cur.y, cur.distance + 1000, Direction::DOWN });
            to_push.push_back({ cur.x, cur.y, cur.distance + 1000, Direction::UP });
            break;
        case Direction::UP:
            next_y -= 1;
            to_push.push_back({ cur.x, cur.y, cur.distance + 1000, Direction::RIGHT });
            to_push.push_back({ cur.x, cur.y, cur.distance + 1000, Direction::LEFT });
            break;
        case Direction::DOWN:
            next_y += 1;
            to_push.push_back({ cur.x, cur.y, cur.distance + 1000, Direction::RIGHT });
            to_push.push_back({ cur.x, cur.y, cur.distance + 1000, Direction::LEFT });
            break;
        }

        if (lines.at(next_y).at(next_x) != '#') {
            Position next { next_x, next_y, cur.distance + 1, cur.direction };
            to_push.push_back(next);
        }

        for (auto p : to_push) {
            if (found && p.distance > shortes_len) {
                continue;
            }
            if (!parents_of_used.contains(p)) {
                pq.push(p);
                parents_of_used.insert_or_assign(p, std::unordered_set<Position, HashPos> { cur });
            } else {
                auto& [used, parents] = *parents_of_used.find(p);
                if (used.distance == p.distance) {
                    parents.insert(cur);
                }
            }
        }
    }

    std::set<std::pair<size_t, size_t>> tiles {};

    std::queue<Position> to_check {};
    to_check.push({ lines.at(0).size() - 2, 1, shortes_len, Direction::RIGHT });
    to_check.push({ lines.at(0).size() - 2, 1, shortes_len, Direction::UP });
    to_check.push({ lines.at(0).size() - 2, 1, shortes_len, Direction::LEFT });
    to_check.push({ lines.at(0).size() - 2, 1, shortes_len, Direction::DOWN });

    while (!to_check.empty()) {
        Position p = to_check.front();
        to_check.pop();

        if (parents_of_used.contains(p)) {
            tiles.insert({ p.x, p.y });
            for (auto parent : parents_of_used.at(p)) {
                to_check.push(parent);
            }
        }
    }

    result = tiles.size();

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
