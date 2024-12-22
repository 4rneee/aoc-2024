#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_set>

#include "../util.h"

uint64_t next_num(uint64_t x)
{
    x = ((x * 64) ^ x) % 16777216;
    x = ((x / 32) ^ x) % 16777216;
    x = ((x * 2048) ^ x) % 16777216;

    return x;
}

void star_1(std::string file_name)
{
    uint64_t result { 0 };

    util::for_each_line(file_name, [&](const std::string& line) {
        uint64_t secret = std::stoi(line);

        for (size_t i { 0 }; i < 2000; ++i) {
            secret = next_num(secret);
        }

        result += secret;
    });

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

struct Sequence {
    int n1;
    int n2;
    int n3;
    int n4;
    bool operator==(const Sequence& o) const
    {
        return n1 == o.n1 && n2 == o.n2 && n3 == o.n3 && n4 == o.n4;
    }
};
struct Hash {
    std::size_t operator()(const Sequence& s) const
    {
        return ((int64_t)s.n1 << 48) | ((int64_t)s.n2 << 32) | ((int64_t)s.n3 << 16) |
               ((int64_t)s.n4);
    }
};

void star_2(std::string file_name)
{
    uint64_t result { 0 };

    std::unordered_map<Sequence, uint64_t, Hash> m {};

    util::for_each_line(file_name, [&](const std::string& line) {
        uint64_t secret = std::stoi(line);
        Sequence seq;
        std::unordered_set<Sequence, Hash> seqs {};

        auto bananas = secret % 10;

        for (size_t i { 0 }; i < 2000; ++i) {
            secret = next_num(secret);
            int diff = bananas - (secret % 10);
            bananas = secret % 10;

            seq.n1 = seq.n2;
            seq.n2 = seq.n3;
            seq.n3 = seq.n4;
            seq.n4 = diff;

            if (i > 2) {
                if (!seqs.contains(seq)) {
                    seqs.insert(seq);
                    if (m.contains(seq)) {
                        m.at(seq) += bananas;
                    } else {
                        m.insert({ seq, bananas });
                    }
                }
            }
        }
    });

    result = std::max_element(m.begin(), m.end(), [](const auto& a, const auto& b) {
                 return a.second < b.second;
             })->second;

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
