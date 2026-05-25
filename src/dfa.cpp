#include "regex/dfa.hpp"
#include <map>
#include <queue>
#include <set>
#include <stack>

namespace regex {

namespace {

std::set<int> epsilonClosure(const NFA& nfa, const std::set<int>& states) {
    std::set<int> closure = states;
    std::stack<int> worklist;
    for (int s : states) {
        worklist.push(s);
    }
    while (!worklist.empty()) {
        int s = worklist.top();
        worklist.pop();
        for (const auto& t : nfa.adj[static_cast<size_t>(s)]) {
            if (!t.symbol.has_value() && closure.insert(t.target).second) {
                worklist.push(t.target);
            }
        }
    }
    return closure;
}

std::set<int> moveSet(const NFA& nfa, const std::set<int>& states, char c) {
    std::set<int> result;
    for (int s : states) {
        for (const auto& t : nfa.adj[static_cast<size_t>(s)]) {
            if (t.symbol.has_value() && t.symbol.value() == c) {
                result.insert(t.target);
            }
        }
    }
    return result;
}

}

bool DFA::accepts(const std::string& input) const {
    int current = start;
    for (char c : input) {
        if (current == DEAD) {
            return false;
        }
        auto stateIt = transitions.find(current);
        if (stateIt == transitions.end()) {
            return false;
        }
        auto symIt = stateIt->second.find(c);
        if (symIt == stateIt->second.end() || symIt->second == DEAD) {
            return false;
        }
        current = symIt->second;
    }
    return accepting.count(current) > 0;
}

DFA buildDFA(const NFA& nfa, const std::set<char>& alpha) {
    DFA dfa;
    dfa.alphabet = alpha;

    std::map<std::set<int>, int> stateMap;
    std::queue<std::set<int>> worklist;
    int nextId = 0;

    auto getOrCreate = [&](const std::set<int>& nfaStates) -> int {
        auto [it, inserted] = stateMap.emplace(nfaStates, nextId);
        if (inserted) {
            ++nextId;
            worklist.push(nfaStates);
            if (nfaStates.count(nfa.accept)) {
                dfa.accepting.insert(it->second);
            }
        }
        return it->second;
    };

    auto startSet = epsilonClosure(nfa, {nfa.start});
    dfa.start = getOrCreate(startSet);

    while (!worklist.empty()) {
        auto current = worklist.front();
        worklist.pop();
        int currentId = stateMap[current];

        for (char c : alpha) {
            auto moved = moveSet(nfa, current, c);
            auto closure = epsilonClosure(nfa, moved);
            if (closure.empty()) {
                dfa.transitions[currentId][c] = DFA::DEAD;
            } else {
                dfa.transitions[currentId][c] = getOrCreate(closure);
            }
        }
    }

    return dfa;
}

}
