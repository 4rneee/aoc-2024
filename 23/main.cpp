#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_set>

#include "../util.h"

void star_1(std::string file_name)
{
    uint64_t result { 0 };

    std::unordered_map<std::string, std::unordered_set<std::string>> adj {};

    util::for_each_line(file_name, [&](const std::string& line) {
        auto x = util::split(line, "-");
        if (adj.contains(x.at(0))) {
            adj.at(x.at(0)).insert(x.at(1));
        } else {
            adj.insert({ x.at(0), { x.at(1) } });
        }

        if (adj.contains(x.at(1))) {
            adj.at(x.at(1)).insert(x.at(0));
        } else {
            adj.insert({ x.at(1), { x.at(0) } });
        }
    });

    std::vector<std::unordered_set<std::string>> used {};

    for (auto& [node, ns] : adj) {
        if (node.starts_with('t')) {
            for (auto& node1 : ns) {
                for (auto& node2 : ns) {
                    if (adj.at(node1).contains(node2)) {
                        std::unordered_set<std::string> s { node, node1, node2 };
                        if (std::find(used.begin(), used.end(), s) == used.end()) {
                            used.push_back(s);
                        }
                    }
                }
            }
        }
    }

    result = used.size();

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

void intersect(
    const std::unordered_set<std::string>& a,
    const std::unordered_set<std::string>& b,
    std::unordered_set<std::string>& dst)
{
    for (const auto& x : a) {
        if (b.contains(x)) {
            dst.insert(x);
        }
    }
}

std::vector<std::unordered_set<std::string>> max_cliques {};
void bron_kerbosch(
    const std::unordered_map<std::string, std::unordered_set<std::string>>& adj,
    const std::unordered_set<std::string>& r,
    const std::unordered_set<std::string>& p,
    std::unordered_set<std::string> x)
{
    if (p.empty() && x.empty()) {
        max_cliques.push_back(r);
        return;
    }

    auto p2 = p;
    for (auto& v : p) {
        std::unordered_set<std::string> new_r { r };
        new_r.insert(v);

        std::unordered_set<std::string> new_p {};
        intersect(p2, adj.at(v), new_p);

        std::unordered_set<std::string> new_x {};
        intersect(x, adj.at(v), new_x);

        bron_kerbosch(adj, new_r, new_p, new_x);

        p2.erase(v);
        x.insert(v);
    }
}

void star_2(std::string file_name)
{
    std::unordered_map<std::string, std::unordered_set<std::string>> adj {};

    std::unordered_set<std::string> nodes {};

    util::for_each_line(file_name, [&](const std::string& line) {
        auto x = util::split(line, "-");
        if (adj.contains(x.at(0))) {
            adj.at(x.at(0)).insert(x.at(1));
        } else {
            adj.insert({ x.at(0), { x.at(1) } });
        }

        if (adj.contains(x.at(1))) {
            adj.at(x.at(1)).insert(x.at(0));
        } else {
            adj.insert({ x.at(1), { x.at(0) } });
        }

        nodes.insert(x.at(0));
        nodes.insert(x.at(1));
    });

    bron_kerbosch(adj, {}, nodes, {});

    auto largest_clique =
        *std::max_element(max_cliques.begin(), max_cliques.end(), [](const auto& a, const auto& b) {
            return a.size() < b.size();
        });

    std::vector<std::string> clique_vec {};
    clique_vec.insert(clique_vec.begin(), largest_clique.begin(), largest_clique.end());
    std::sort(clique_vec.begin(), clique_vec.end());

    std::cout << "Solution for Star 2:\n";
    for (size_t i { 0 }; i < clique_vec.size(); ++i) {
        std::cout << clique_vec.at(i);
        if (i < clique_vec.size() - 1) {
            std::cout << ',';
        }
    }

    std::cout << '\n';
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
