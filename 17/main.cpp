#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <set>
#include <string>

#include "../util.h"

enum class Instruction : uint8_t {
    ADV = 0,
    BXL = 1,
    BST = 2,
    JNZ = 3,
    BXC = 4,
    OUT = 5,
    BDV = 6,
    CDV = 7
};

struct State {
    int64_t ip { 0 };
    int64_t ra;
    int64_t rb;
    int64_t rc;
    std::vector<Instruction> intstructions {};

    int64_t get_combo_op(uint8_t operand)
    {
        switch (operand) {
        case 0:
        case 1:
        case 2:
        case 3:
            return operand;
        case 4:
            return ra;
        case 5:
            return rb;
        case 6:
            return rc;
        default:
            std::cout << "AAAAAAAAAAAAAAAA\n";
            return 0;
        }
    }
};

void star_1(std::string file_name)
{
    uint64_t result { 0 };

    auto lines = util::get_all_lines(file_name).value();

    State state;

    std::sscanf(lines.at(0).c_str(), "Register A: %ld", &state.ra);
    std::sscanf(lines.at(1).c_str(), "Register B: %ld", &state.rb);
    std::sscanf(lines.at(2).c_str(), "Register C: %ld", &state.rc);

    for (auto s : util::split(lines.at(lines.size() - 1).substr(9), ",")) {
        state.intstructions.push_back((Instruction)std::stoi(s));
    }

    std::vector<uint8_t> out {};
    while (state.ip >= 0 && state.ip < state.intstructions.size()) {
        Instruction inst = state.intstructions.at(state.ip);
        uint8_t op = static_cast<uint8_t>(state.intstructions.at(state.ip + 1));
        switch (inst) {
        case Instruction::ADV:
            state.ra = state.ra / (0x1 << state.get_combo_op(op));
            break;
        case Instruction::BXL:
            state.rb = state.rb ^ static_cast<int64_t>(op);
            break;
        case Instruction::BST:
            state.rb = state.get_combo_op(op) % 8;
            break;
        case Instruction::JNZ:
            if (state.ra != 0) {
                state.ip = op;
                continue;
            }
            break;
        case Instruction::BXC:
            state.rb = state.rb ^ state.rc;
            break;
        case Instruction::OUT:
            out.push_back(state.get_combo_op(op) % 8);
            break;
        case Instruction::BDV:
            state.rb = state.ra / (0x1 << state.get_combo_op(op));
            break;
        case Instruction::CDV:
            state.rc = state.ra / (0x1 << state.get_combo_op(op));
            break;
        }
        state.ip += 2;
    }

    std::cout << "Solution for Star 1:\n";
    for (size_t i { 0 }; i < out.size(); ++i) {
        std::cout << (int)out.at(i);
        if (i < out.size() - 1) {
            std::cout << ',';
        }
    }
    std::cout << '\n';
}

bool try_ra(State state, uint64_t ra, size_t iteration)
{
    state.ra = ra;
    state.rb = 0;
    state.rc = 0;
    state.ip = 0;
    std::vector<uint8_t> out {};
    while (state.ip >= 0 && state.ip < state.intstructions.size()) {
        Instruction inst = state.intstructions.at(state.ip);
        uint8_t op = static_cast<uint8_t>(state.intstructions.at(state.ip + 1));
        switch (inst) {
        case Instruction::ADV:
            state.ra = state.ra / (0x1 << state.get_combo_op(op));
            break;
        case Instruction::BXL:
            state.rb = state.rb ^ static_cast<int64_t>(op);
            break;
        case Instruction::BST:
            state.rb = state.get_combo_op(op) % 8;
            break;
        case Instruction::JNZ:
            if (state.ra != 0) {
                state.ip = op;
                continue;
            }
            break;
        case Instruction::BXC:
            state.rb = state.rb ^ state.rc;
            break;
        case Instruction::BDV:
            state.rb = state.ra / (0x1 << state.get_combo_op(op));
            break;
        case Instruction::CDV:
            state.rc = state.ra / (0x1 << state.get_combo_op(op));
            break;
        case Instruction::OUT:
            out.push_back(state.get_combo_op(op) % 8);
            break;
        }
        state.ip += 2;
    }

    // if (out.size() < iteration || out.size() > state.intstructions.size()) {
    if (out.size() != state.intstructions.size()) {
        return false;
    }
    for (size_t i { 0 }; i < iteration; ++i) {
        size_t idx { state.intstructions.size() - 1 - i };
        // if ((uint8_t)state.intstructions.at(i) != out.at(i)) {
        if ((uint8_t)state.intstructions.at(idx) != out.at(out.size() - 1 - i)) {
            return false;
        }
    }

    return true;
}

struct PossibleRA {
    PossibleRA()
        : val(64, 0)
    {
    }
    std::vector<int> val {};
    std::set<uint8_t> locked_bits {};

    bool try_set_bits(std::vector<int> idxs, std::vector<int> bits)
    {
        if (idxs.size() != bits.size()) {
            return false;
        }
        for (size_t i { 0 }; i < idxs.size(); ++i) {
            if (locked_bits.contains(idxs.at(i)) && val.at(idxs.at(i)) != bits.at(i)) {
                return false;
            }
            locked_bits.insert(idxs.at(i));
            val.at(idxs.at(i)) = bits.at(i);
        }
        return true;
    }

    uint64_t int_val() const
    {
        uint64_t v = 0;
        for (size_t i { 0 }; i < val.size(); ++i) {
            v = v | ((uint64_t)val.at(i) << i);
        }
        return v;
    }
};
void star_2(std::string file_name)
{
    uint64_t result { 0 };

    auto lines = util::get_all_lines(file_name).value();

    State state;

    std::sscanf(lines.at(0).c_str(), "Register A: %ld", &state.ra);
    std::sscanf(lines.at(1).c_str(), "Register B: %ld", &state.rb);
    std::sscanf(lines.at(2).c_str(), "Register C: %ld", &state.rc);

    for (auto s : util::split(lines.at(lines.size() - 1).substr(9), ",")) {
        state.intstructions.push_back((Instruction)std::stoi(s));
    }
    std::vector<PossibleRA> possible(1, PossibleRA {});
    std::vector<uint8_t> desired_output { 2, 4, 1, 1, 7, 5, 1, 5, 4, 0, 5, 5, 0, 3, 3, 0 };

    for (size_t i { 0 }; i < desired_output.size(); ++i) {
        // uint8_t desired = desired_output.at(i);
        uint8_t desired = desired_output.at(desired_output.size() - 1 - i);
        std::vector<PossibleRA> still_possible {};
        // uint8_t start_rb = i * 3;
        uint8_t start_rb = 48 - 3 - i * 3;

        for (uint8_t x { 0 }; x < 8; ++x) {
            uint8_t rb = x;
            for (auto pra : possible) {
                if (!pra.try_set_bits(
                        { start_rb, start_rb + 1, start_rb + 2 },
                        { rb & 1, (rb >> 1) & 1, (rb >> 2) & 1 })) {
                    continue;
                }
                rb = rb ^ 1;
                uint8_t start_rc = start_rb + rb;
                rb = rb ^ 5;
                uint8_t rc = desired ^ rb;
                if (!pra.try_set_bits(
                        { start_rc, start_rc + 1, start_rc + 2 },
                        { rc & 1, (rc >> 1) & 1, (rc >> 2) & 1 })

                ) {
                    continue;
                }
                if (try_ra(state, pra.int_val(), i + 1)) {
                    still_possible.push_back(pra);
                }
            }
        }

        possible = std::move(still_possible);
        // std::cout << i << ' ' << possible.size() << '\n';
        // std::cout << i << ":\n";

        // for (auto& pra : possible) {
        //     std::cout << pra.int_val() << '\n';
        // }
        // std::cout << '\n';
    }

    if (!possible.empty()) {
        result = std::min_element(possible.begin(), possible.end(), [](auto a, auto b) {
                     return a.int_val() < b.int_val();
                 })->int_val();
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
