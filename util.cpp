#include "util.h"
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace util {

void for_each_line(const std::string& file_name, const std::function<void(const std::string&)>& f)
{
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cout << "Unable to open file\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        f(line);
    }

    file.close();
}

void for_n_lines(
    const std::string& file_name,
    size_t n,
    const std::function<void(const std::vector<std::string>&)>& f)
{
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cout << "Unable to open file\n";
        return;
    }

    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line)) {
        lines.push_back(line);
        if (lines.size() == n) {
            f(lines);
            lines.clear();
        }
    }
    if (!lines.empty()) {
        f(lines);
    }

    file.close();
}

std::optional<std::vector<std::string>> get_all_lines(const std::string& file_name)
{
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cout << "Unable to open file\n";
        return {};
    }

    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();
    return lines;
}

std::vector<std::string> split(const std::string& string, const std::string& delimiter) {
    std::vector<std::string> res {};
    size_t from {0};
    size_t to {0};

    while (from < string.size()) {
        to = std::min(string.size(), string.find(delimiter, from));
        res.push_back(string.substr(from, to-from));
        from = to + delimiter.size();
    }

    return res;
}
}
