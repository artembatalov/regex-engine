#pragma once
#include "nfa.hpp"
#include <map>
#include <set>
#include <string>

namespace regex {

struct DFA {
    static constexpr int DEAD = -1;

    int start = 0;
    std::set<int> accepting;
    std::map<int, std::map<char, int>> transitions;
    std::set<char> alphabet;

    bool accepts(const std::string& input) const;
};

DFA buildDFA(const NFA& nfa, const std::set<char>& alphabet);

}
