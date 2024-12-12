#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <set>
#include <string>
#include <unordered_set>

#include "../util.h"

using coord_t = std::pair<int, int>;

struct Hash {
    std::size_t operator()(const coord_t& c) const { return ((int64_t)c.first << 32) | c.second; }
};

struct Plot {
    std::unordered_set<coord_t, Hash> plants {};
    int perimiter(std::vector<std::string>& lines)
    {
        int fences { 0 };

        for (auto c : plants) {
            const std::vector<coord_t> neighbour_diffs { { 1, 0 }, { 0, 1 }, { 0, -1 }, { -1, 0 } };
            for (auto [d1, d2] : neighbour_diffs) {
                coord_t n = { c.first + d1, c.second + d2 };
                if (n.first < 0 || n.first >= lines.size() || n.second < 0 ||
                    n.second >= lines.at(n.first).size() ||
                    lines.at(n.first).at(n.second) != lines.at(c.first).at(c.second)) {
                    ++fences;
                }
            }
        }

        return fences;
    }

    int sides(std::vector<std::string>& lines)
    {
        std::set<std::pair<coord_t, int>> fences {};

        for (auto c : plants) {
            const std::vector<coord_t> neighbour_diffs { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
            for (int i { 0 }; i < neighbour_diffs.size(); ++i) {
                auto [d1, d2] = neighbour_diffs.at(i);
                coord_t n = { c.first + d1, c.second + d2 };
                if (n.first < 0 || n.first >= lines.size() || n.second < 0 ||
                    n.second >= lines.at(n.first).size() ||
                    lines.at(n.first).at(n.second) != lines.at(c.first).at(c.second)) {
                    fences.insert({ n, i });
                }
            }
        }

        int sides { 0 };
        std::set<std::pair<coord_t, int>> used {};

        for (auto [c, dir] : fences) {
            if (used.contains({ c, dir })) {
                continue;
            }
            ++sides;
            used.insert({ { c.first, c.second }, dir });
            if (dir < 2) {
                for (int i { 1 }; fences.contains({ { c.first, c.second + i }, dir }); ++i) {
                    used.insert({ { c.first, c.second + i }, dir });
                }
                for (int i { 1 }; fences.contains({ { c.first, c.second - i }, dir }); ++i) {
                    used.insert({ { c.first, c.second - i }, dir });
                }
            } else {
                for (int i { 1 }; fences.contains({ { c.first + i, c.second }, dir }); ++i) {
                    used.insert({ { c.first + i, c.second }, dir });
                }
                for (int i { 1 }; fences.contains({ { c.first - i, c.second }, dir }); ++i) {
                    used.insert({ { c.first - i, c.second }, dir });
                }
            }
        }

        return sides;
    }

    int area() { return plants.size(); }
};

void expand_plot(
    Plot& p,
    coord_t start,
    std::unordered_set<coord_t, Hash>& used,
    std::vector<std::string>& lines)
{
    if (used.contains(start)) {
        return;
    }
    p.plants.insert(start);
    used.insert(start);

    const std::vector<coord_t> neighbour_diffs { { 1, 0 }, { 0, 1 }, { 0, -1 }, { -1, 0 } };

    for (auto [d1, d2] : neighbour_diffs) {
        coord_t n = { start.first + d1, start.second + d2 };

        if (n.first >= 0 && n.first < lines.size() && n.second >= 0 &&
            n.second < lines.at(n.first).size()) {
            if (lines.at(n.first).at(n.second) == lines.at(start.first).at(start.second)) {
                expand_plot(p, n, used, lines);
            }
        }
    }
}

void star_1(std::string file_name)
{
    uint64_t result { 0 };

    auto lines = util::get_all_lines(file_name).value();

    std::unordered_set<coord_t, Hash> used_plants {};

    std::unordered_map<char, std::vector<Plot>> plots {};

    for (size_t y { 0 }; y < lines.size(); ++y) {
        for (size_t x { 0 }; x < lines.at(y).size(); ++x) {
            coord_t coord = { y, x };

            if (!used_plants.contains(coord)) {
                Plot p {};
                expand_plot(p, coord, used_plants, lines);

                char type = lines.at(y).at(x);
                if (!plots.contains(type)) {
                    plots.insert({ type, {} });
                }
                plots.at(type).push_back(p);
            }
        }
    }

    for (auto& [type, plots_v] : plots) {
        for (auto& plot : plots_v) {
            result += plot.area() * plot.perimiter(lines);
        }
    }

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

void star_2(std::string file_name)
{
    uint64_t result { 0 };

    auto lines = util::get_all_lines(file_name).value();

    std::unordered_set<coord_t, Hash> used_plants {};

    std::unordered_map<char, std::vector<Plot>> plots {};

    for (size_t y { 0 }; y < lines.size(); ++y) {
        for (size_t x { 0 }; x < lines.at(y).size(); ++x) {
            coord_t coord = { y, x };
            if (!used_plants.contains(coord)) {
                Plot p {};
                expand_plot(p, coord, used_plants, lines);

                char type = lines.at(y).at(x);
                if (!plots.contains(type)) {
                    plots.insert({ type, {} });
                }
                plots.at(type).push_back(p);
            }
        }
    }

    for (auto& [type, plots_v] : plots) {
        for (auto& plot : plots_v) {
            result += plot.area() * plot.sides(lines);
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
