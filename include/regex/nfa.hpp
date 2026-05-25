#pragma once
#include "ast.hpp"
#include <optional>
#include <vector>

namespace regex {

struct NFA {
    struct Transition {
        std::optional<char> symbol;
        int target;
    };

    int num_states = 0;
    int start = 0;
    int accept = 0;
    std::vector<std::vector<Transition>> adj;

    int newState();
    void addTransition(int from, std::optional<char> symbol, int to);
};

NFA buildNFA(const ASTNodePtr& node);

}
