#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../util.h"

using coord_t = std::pair<int64_t, int64_t>;

struct HashC {
    std::size_t operator()(const coord_t& c) const { return ((int64_t)c.first << 32) | c.second; }
};

const std::unordered_map<char, coord_t> pad_1 {
    { '7', { 0, 3 } }, { '8', { 1, 3 } }, { '9', { 2, 3 } }, { '4', { 0, 2 } },
    { '5', { 1, 2 } }, { '6', { 2, 2 } }, { '1', { 0, 1 } }, { '2', { 1, 1 } },
    { '3', { 2, 1 } }, { '0', { 1, 0 } }, { 'A', { 2, 0 } }
};
const std::unordered_set<coord_t, HashC> pad_1_vals { { 0, 3 }, { 1, 3 }, { 2, 3 }, { 0, 2 },
                                                      { 1, 2 }, { 2, 2 }, { 0, 1 }, { 1, 1 },
                                                      { 2, 1 }, { 1, 0 }, { 2, 0 } };

const std::unordered_map<char, coord_t> pad_2 {
    { '^', { 1, 1 } }, { 'A', { 2, 1 } }, { '<', { 0, 0 } }, { 'v', { 1, 0 } }, { '>', { 2, 0 } }
};
const std::unordered_set<coord_t, HashC> pad_2_vals {
    { 1, 1 }, { 2, 1 }, { 0, 0 }, { 1, 0 }, { 2, 0 }
};

std::unordered_set<std::string> possibles(
    const std::unordered_map<char, coord_t>& pad,
    const std::unordered_set<coord_t, HashC>& pad_vals,
    coord_t pos,
    coord_t dest)
{
    if (!pad_vals.contains(pos)) {
        return {};
    }

    if (pos == dest) {
        return { "A" };
    }

    std::unordered_set<std::string> res {};

    if (pos.first < dest.first) {
        for (auto& s : possibles(pad, pad_vals, { pos.first + 1, pos.second }, dest)) {
            res.insert('>' + s);
        }
    }
    if (pos.first > dest.first) {
        for (auto& s : possibles(pad, pad_vals, { pos.first - 1, pos.second }, dest)) {
            res.insert('<' + s);
        }
    }

    if (pos.second < dest.second) {
        for (auto& s : possibles(pad, pad_vals, { pos.first, pos.second + 1 }, dest)) {
            res.insert('^' + s);
        }
    }
    if (pos.second > dest.second) {
        for (auto& s : possibles(pad, pad_vals, { pos.first, pos.second - 1 }, dest)) {
            res.insert('v' + s);
        }
    }

    return res;
}

std::unordered_map<std::string, std::unordered_set<std::string>> pad2_cache {};

std::unordered_set<std::string> pad2_get_shortests(const std::string& seq)
{
    if (pad2_cache.contains(seq)) {
        return pad2_cache.at(seq);
    }
    std::unordered_set<std::string> res { "" };

    coord_t pos = pad_2.at('A');

    for (auto c : seq) {
        coord_t dest = pad_2.at(c);
        std::unordered_set<std::string> new_res {};
        for (auto& s1 : res) {
            for (auto& s2 : possibles(pad_2, pad_2_vals, pos, dest)) {
                new_res.insert(s1 + s2);
            }
        }

        pos = dest;
        res = std::move(new_res);
        auto min_it = std::min_element(res.begin(), res.end(), [](const auto& a, const auto& b) {
            return a.size() < b.size();
        });

        std::erase_if(res, [&min_it](const std::string& x) { return x.size() > min_it->size(); });
    }

    pad2_cache.insert({ seq, res });
    return res;
}

std::unordered_set<std::string> get_shortests(
    const std::unordered_map<char, coord_t>& pad,
    const std::unordered_set<coord_t, HashC>& pad_vals,
    coord_t pos,
    const std::unordered_set<std::string>& seqs)
{
    std::unordered_set<std::string> res { "" };

    for (auto& seq : seqs) {
        for (auto c : seq) {
            coord_t dest = pad.at(c);
            std::unordered_set<std::string> new_res {};
            for (auto& s1 : res) {
                for (auto& s2 : possibles(pad, pad_vals, pos, dest)) {
                    new_res.insert(s1 + s2);
                }
            }

            pos = dest;
            res = std::move(new_res);
            auto min_it =
                std::min_element(res.begin(), res.end(), [](const auto& a, const auto& b) {
                    return a.size() < b.size();
                });

            std::erase_if(res, [&min_it](const std::string& x) {
                return x.size() > min_it->size();
            });
        }
    }

    return res;
}

void star_1(std::string file_name)
{
    uint64_t result { 0 };

    auto lines = util::get_all_lines(file_name).value();

    util::for_each_line(file_name, [&](const std::string& line) {
        coord_t door_pos = pad_1.at('A');

        const auto door_seqs = get_shortests(pad_1, pad_1_vals, door_pos, { line });

        std::unordered_set<std::string> r1_seqs {};
        for (auto seq : door_seqs) {
            auto steps = util::split(seq, "A");
            std::vector<std::unordered_set<std::string>> next {};
            for (auto step : steps) {
                next.push_back(pad2_get_shortests(step + 'A'));
            }

            std::unordered_set<std::string> seqs { "" };
            for (auto& n : next) {
                std::unordered_set<std::string> new_seqs {};
                for (auto& s : seqs) {
                    for (auto& s2 : n) {
                        new_seqs.insert(s + s2);
                    }
                }
                seqs = std::move(new_seqs);
            }
            r1_seqs.merge(seqs);
            auto min_it =
                std::min_element(r1_seqs.begin(), r1_seqs.end(), [](const auto& a, const auto& b) {
                    return a.size() < b.size();
                });

            std::erase_if(r1_seqs, [&min_it](const std::string& x) {
                return x.size() > min_it->size();
            });
        }

        size_t len { 0 };

        std::unordered_set<std::string> r2_seqs {};
        for (auto seq : r1_seqs) {
            auto steps = util::split(seq, "A");
            std::vector<std::unordered_set<std::string>> next {};
            for (auto step : steps) {
                next.push_back(pad2_get_shortests(step + 'A'));
            }

            std::unordered_set<std::string> seqs { "" };
            for (auto& n : next) {
                std::unordered_set<std::string> new_seqs {};
                for (auto& s : seqs) {
                    for (auto& s2 : n) {
                        new_seqs.insert(s + s2);
                    }
                }
                seqs = std::move(new_seqs);
            }
            r2_seqs.merge(seqs);
            auto min_it =
                std::min_element(r2_seqs.begin(), r2_seqs.end(), [](const auto& a, const auto& b) {
                    return a.size() < b.size();
                });

            std::erase_if(r2_seqs, [&min_it](const std::string& x) {
                return x.size() > min_it->size();
            });
        }

        int val;
        std::sscanf(line.c_str(), "%dA", &val);

        auto shortest_it =
            std::min_element(r2_seqs.begin(), r2_seqs.end(), [](const auto& a, const auto& b) {
                return a.size() < b.size();
            });
        len = shortest_it->size();

        result += val * len;
    });

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

struct Hash {
    std::size_t operator()(const std::pair<std::string, uint64_t>& p) const
    {
        auto h1 = std::hash<std::string> {}(p.first);
        auto h2 = std::hash<uint64_t> {}(p.second);
        return h1 ^ (h2 << 1);
    }
};
std::unordered_map<std::pair<std::string, uint64_t>, uint64_t, Hash> seq_cache {};
uint64_t seq_len(const std::string& seq, uint64_t depth)
{
    if (depth == 0) {
        return seq.size();
    }

    if (seq_cache.contains({ seq, depth })) {
        return seq_cache.at({ seq, depth });
    }

    uint64_t len { 0 };

    auto steps = util::split(seq, "A");

    for (size_t i { 0 }; i < steps.size(); ++i) {
        steps.at(i) += 'A';
        auto sols = pad2_get_shortests(steps.at(i));

        std::unordered_set<uint64_t> pos_lens {};
        for (auto& s : sols) {
            pos_lens.insert(seq_len(s, depth - 1));
        }
        len += *std::min_element(pos_lens.begin(), pos_lens.end());
    }

    seq_cache.insert({ { seq, depth }, len });

    return len;
}

void star_2(std::string file_name)
{
    uint64_t result { 0 };

    auto lines = util::get_all_lines(file_name).value();

    util::for_each_line(file_name, [&](const std::string& line) {
        coord_t door_pos = pad_1.at('A');

        auto cur_seqs = get_shortests(pad_1, pad_1_vals, door_pos, { line });

        std::vector<uint64_t> lens {};

        for (auto& seq : cur_seqs) {
            lens.push_back(seq_len(seq, 25));
        }
        uint64_t len = *std::min_element(lens.begin(), lens.end());

        int val;
        std::sscanf(line.c_str(), "%dA", &val);

        result += val * len;
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
