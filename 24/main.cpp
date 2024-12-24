#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "../util.h"

enum class Gate : uint8_t { NONE, AND, OR, XOR };

struct Variable {
    bool value;
    bool set;
    Gate gate;
    std::string p1;
    std::string p2;
};

std::unordered_map<std::string, Variable> vars {};

void set_var(const std::string& var_name)
{
    if (!vars.contains(var_name)) {
        std::cout << "AAAAAAAAAAAAA\n";
        return;
    }

    auto& var = vars.at(var_name);

    if (var.set) {
        return;
    }

    if (var.gate == Gate::NONE) {
        return;
    }

    set_var(var.p1);
    set_var(var.p2);

    bool v1 = vars.at(var.p1).value;
    bool v2 = vars.at(var.p2).value;

    switch (var.gate) {
    case Gate::AND:
        var.value = v1 && v2;
        break;
    case Gate::OR:
        var.value = v1 || v2;
        break;
    case Gate::XOR:
        var.value = v1 ^ v2;
        break;
    case Gate::NONE:
        break;
    }
    var.set = true;
}

void star_1(std::string file_name)
{
    uint64_t result { 0 };

    auto lines = util::get_all_lines(file_name).value();
    size_t i { 0 };

    while (!lines.at(i).empty()) {
        auto split = util::split(lines.at(i), ": ");

        Variable var { split.at(1).at(0) == '1', true, Gate::NONE, "", "" };

        vars.insert({ split.at(0), var });
        ++i;
    }
    ++i;

    while (i < lines.size()) {
        auto split = util::split(lines.at(i), " ");
        Variable var;
        var.set = false;

        if (split.at(1) == "AND") {
            var.gate = Gate::AND;
        } else if (split.at(1) == "OR") {
            var.gate = Gate::OR;
        } else if (split.at(1) == "XOR") {
            var.gate = Gate::XOR;
        } else {
            std::cout << "AAAAAAAAAA\n";
        }

        var.p1 = split.at(0);
        var.p2 = split.at(2);

        vars.insert({ split.at(4), var });
        ++i;
    }

    for (const auto& [name, var] : vars) {
        if (name.starts_with("z")) {
            auto idx = std::stoi(name.substr(1));
            set_var(name);

            if (var.value) {
                result |= (uint64_t)1 << idx;
            }
        }
    }

    std::cout << "Solution for Star 1:\n";
    std::cout << result << '\n';
}

void print_graph()
{
    size_t i { 0 };
    std::cout << "digraph {\n";
    for (const auto& [name, var] : vars) {
        if (var.gate != Gate::NONE) {
            std::string g = "g" + std::to_string(i);
            ++i;
            std::string gate_label;
            switch (var.gate) {
            case Gate::AND:
                gate_label = "AND";
                break;
            case Gate::OR:
                gate_label = "OR";
                break;
            case Gate::XOR:
                gate_label = "XOR";
                break;
            case Gate::NONE:
                break;
            }
            std::cout << g << "[label=" << '"' << gate_label << "\"];\n";
            std::cout << var.p1 << " -> " << g << ";\n";
            std::cout << var.p2 << " -> " << g << ";\n";
            std::cout << g << " -> " << name << ";\n";
        }
    }
    std::cout << "}\n";
}

void star_2(std::string file_name)
{
    // print_graph();

    // Solved it by looking at the first wrong bit in the result, then checking in the graph which
    // gates close to that bit were wrong. Fixed the gate and repeat.
    std::cout << "Solution for Star 2:\n";
    std::cout << "fhc,ggt,hqk,mwh,qhj,z06,z11,z35" << '\n';
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
