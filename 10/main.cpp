#include <cstdlib>
#include <iostream>
#include <set>
#include <string>

#include "../util.h"

using coord_t = std::pair<int, int>;

struct Hash {
    std::size_t operator()(const coord_t& c) const { return c.first + c.second * 150; }
};
struct Equal {
    bool operator()(const coord_t& lhs, const coord_t& rhs) const
    {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }
};

void reachable(
    coord_t start,
    const std::vector<std::string>& lines,
    const std::unordered_map<coord_t, std::set<coord_t>, Hash, Equal>& edges,
    std::set<coord_t>& r_acc)
{
    auto [x, y] = start;
    if (lines.at(y).at(x) == '9') {
        r_acc.insert(start);
        return;
    }

    for (auto& n : edges.at(start)) {
        reachable(n, lines, edges, r_acc);
    }
}

void star_1(std::string file_name)
{
    int result { 0 };

    std::set<coord_t> tailheads {};

    auto lines = util::get_all_lines(file_name).value();

    std::unordered_map<coord_t, std::set<coord_t>, Hash, Equal> edges {};

    for (size_t y { 0 }; y < lines.size(); ++y) {
        for (size_t x { 0 }; x < lines.at(y).size(); ++x) {
            coord_t coord = { x, y };
            std::set<coord_t> neighbours {};
            if (x > 0 && lines.at(y).at(x - 1) == lines.at(y).at(x) + 1) {
                neighbours.insert({ x - 1, y });
            }
            if (x < lines.at(y).size() - 1 && lines.at(y).at(x + 1) == lines.at(y).at(x) + 1) {
                neighbours.insert({ x + 1, y });
            }
            if (y > 0 && lines.at(y - 1).at(x) == lines.at(y).at(x) + 1) {
                neighbours.insert({ x, y - 1 });
            }
            if (y < lines.size() - 1 && lines.at(y + 1).at(x) == lines.at(y).at(x) + 1) {
                neighbours.insert({ x, y + 1 });
            }
            edges.insert({ coord, neighbours });

            if (lines.at(y).at(x) == '0') {
                tailheads.insert(coord);
            }
        }
    }

    for (auto coord : tailheads) {
        std::set<coord_t> reach {};
        reachable(coord, lines, edges, reach);
        result += reach.size();
    }

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

int paths(
    coord_t start,
    const std::vector<std::string>& lines,
    const std::unordered_map<coord_t, std::set<coord_t>, Hash, Equal>& edges)
{
    auto [x, y] = start;
    if (lines.at(y).at(x) == '9') {
        return 1;
    }

    int sum { 0 };
    for (auto& n : edges.at(start)) {
        sum += paths(n, lines, edges);
    }

    return sum;
}

void star_2(std::string file_name)
{
    int result { 0 };

    std::set<coord_t> tailheads {};

    auto lines = util::get_all_lines(file_name).value();

    std::unordered_map<coord_t, std::set<coord_t>, Hash, Equal> edges {};

    for (size_t y { 0 }; y < lines.size(); ++y) {
        for (size_t x { 0 }; x < lines.at(y).size(); ++x) {
            coord_t coord = { x, y };
            std::set<coord_t> neighbours {};
            if (x > 0 && lines.at(y).at(x - 1) == lines.at(y).at(x) + 1) {
                neighbours.insert({ x - 1, y });
            }
            if (x < lines.at(y).size() - 1 && lines.at(y).at(x + 1) == lines.at(y).at(x) + 1) {
                neighbours.insert({ x + 1, y });
            }
            if (y > 0 && lines.at(y - 1).at(x) == lines.at(y).at(x) + 1) {
                neighbours.insert({ x, y - 1 });
            }
            if (y < lines.size() - 1 && lines.at(y + 1).at(x) == lines.at(y).at(x) + 1) {
                neighbours.insert({ x, y + 1 });
            }
            edges.insert({ coord, neighbours });

            if (lines.at(y).at(x) == '0') {
                tailheads.insert(coord);
            }
        }
    }

    for (auto coord : tailheads) {
        result += paths(coord, lines, edges);
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
