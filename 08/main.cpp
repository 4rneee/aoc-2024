#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <string>

#include "../util.h"

struct Antenna {
    char freq;
    long x;
    long y;
};

using coord_t = std::pair<long, long>;

void star_1(std::string file_name)
{
    int result { 0 };

    std::map<char, std::vector<Antenna>> freqs {};

    auto lines = util::get_all_lines(file_name).value();
    for (size_t y { 0 }; y < lines.size(); ++y) {
        for (size_t x { 0 }; x < lines.at(y).size(); ++x) {
            if (lines.at(y).at(x) != '.') {
                Antenna a;
                a.freq = lines.at(y).at(x);
                a.x = x;
                a.y = y;
                if (!freqs.contains(a.freq)) {
                    freqs.insert(std::make_pair(a.freq, std::vector<Antenna> {}));
                }
                freqs.at(a.freq).push_back(a);
            }
        }
    }

    std::set<coord_t> antinodes {};

    for (const auto& freq : freqs) {
        const auto& antennas = freq.second;
        for (size_t i { 0 }; i < antennas.size() - 1; ++i) {
            for (size_t j { i + 1 }; j < antennas.size(); ++j) {
                auto& a1 = antennas.at(i);
                auto& a2 = antennas.at(j);
                long diffX = a1.x - a2.x;
                long diffY = a1.y - a2.y;

                coord_t antinode1, antinode2;
                antinode1 = std::make_pair(a1.x + diffX, a1.y + diffY);
                antinode2 = std::make_pair(a2.x - diffX, a2.y - diffY);

                if (antinode1.first >= 0 && antinode1.second >= 0 &&
                    antinode1.second < lines.size() && antinode1.first < lines.at(0).size()) {
                    antinodes.insert(antinode1);
                }

                if (antinode2.first >= 0 && antinode2.second >= 0 &&
                    antinode2.second < lines.size() && antinode2.first < lines.at(0).size()) {
                    antinodes.insert(antinode2);
                }
            }
        }
    }

    // for (size_t y { 0 }; y < lines.size(); ++y) {
    //     for (size_t x { 0 }; x < lines.at(y).size(); ++x) {
    //         if (antinodes.contains(std::make_pair(x, y))) {
    //             std::cout << '#';
    //         } else {
    //             std::cout << lines.at(y).at(x);
    //         }
    //     }
    //     std::cout << '\n';
    // }

    result = antinodes.size();
    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

void star_2(std::string file_name)
{
    int result { 0 };

    std::map<char, std::vector<Antenna>> freqs {};

    auto lines = util::get_all_lines(file_name).value();
    for (size_t y { 0 }; y < lines.size(); ++y) {
        for (size_t x { 0 }; x < lines.at(y).size(); ++x) {
            if (lines.at(y).at(x) != '.') {
                Antenna a;
                a.freq = lines.at(y).at(x);
                a.x = x;
                a.y = y;
                if (!freqs.contains(a.freq)) {
                    freqs.insert(std::make_pair(a.freq, std::vector<Antenna> {}));
                }
                freqs.at(a.freq).push_back(a);
            }
        }
    }

    std::set<coord_t> antinodes {};

    for (const auto& freq : freqs) {
        const auto& antennas = freq.second;
        for (size_t i { 0 }; i < antennas.size() - 1; ++i) {
            for (size_t j { i + 1 }; j < antennas.size(); ++j) {
                auto& a1 = antennas.at(i);
                auto& a2 = antennas.at(j);
                long diffX = a1.x - a2.x;
                long diffY = a1.y - a2.y;

                for (size_t m { 0 };; ++m) {
                    coord_t antinode = std::make_pair(a1.x + m * diffX, a1.y + m * diffY);
                    if (antinode.first >= 0 && antinode.second >= 0 &&
                        antinode.second < lines.size() && antinode.first < lines.at(0).size()) {
                        antinodes.insert(antinode);
                    } else {
                        break;
                    }
                }

                for (size_t m { 0 };; ++m) {
                    coord_t antinode = std::make_pair(a2.x - m * diffX, a2.y - m * diffY);
                    if (antinode.first >= 0 && antinode.second >= 0 &&
                        antinode.second < lines.size() && antinode.first < lines.at(0).size()) {
                        antinodes.insert(antinode);
                    } else {
                        break;
                    }
                }
            }
        }
    }

    // for (size_t y { 0 }; y < lines.size(); ++y) {
    //     for (size_t x { 0 }; x < lines.at(y).size(); ++x) {
    //         if (antinodes.contains(std::make_pair(x, y))) {
    //             std::cout << '#';
    //         } else {
    //             std::cout << lines.at(y).at(x);
    //         }
    //     }
    //     std::cout << '\n';
    // }

    result = antinodes.size();

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
