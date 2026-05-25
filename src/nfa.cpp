#include "regex/nfa.hpp"
#include <stdexcept>

namespace regex {

int NFA::newState() {
    adj.emplace_back();
    return num_states++;
}

void NFA::addTransition(int from, std::optional<char> symbol, int to) {
    adj[static_cast<size_t>(from)].push_back({symbol, to});
}

namespace {

struct Fragment {
    int start;
    int accept;
};

Fragment buildFragment(NFA& nfa, const ASTNode& node) {
    switch (node.kind()) {
        case NodeKind::Char: {
            const auto& cn = static_cast<const CharNode&>(node);
            int s0 = nfa.newState();
            int s1 = nfa.newState();
            nfa.addTransition(s0, cn.ch, s1);
            return {s0, s1};
        }
        case NodeKind::Concat: {
            const auto& bn = static_cast<const BinaryNode&>(node);
            auto f1 = buildFragment(nfa, *bn.left);
            auto f2 = buildFragment(nfa, *bn.right);
            nfa.addTransition(f1.accept, std::nullopt, f2.start);
            return {f1.start, f2.accept};
        }
        case NodeKind::Alt: {
            const auto& bn = static_cast<const BinaryNode&>(node);
            int q0 = nfa.newState();
            int qa = nfa.newState();
            auto f1 = buildFragment(nfa, *bn.left);
            auto f2 = buildFragment(nfa, *bn.right);
            nfa.addTransition(q0, std::nullopt, f1.start);
            nfa.addTransition(q0, std::nullopt, f2.start);
            nfa.addTransition(f1.accept, std::nullopt, qa);
            nfa.addTransition(f2.accept, std::nullopt, qa);
            return {q0, qa};
        }
        case NodeKind::Star: {
            const auto& un = static_cast<const UnaryNode&>(node);
            int q0 = nfa.newState();
            int qa = nfa.newState();
            auto f = buildFragment(nfa, *un.child);
            nfa.addTransition(q0, std::nullopt, f.start);
            nfa.addTransition(q0, std::nullopt, qa);
            nfa.addTransition(f.accept, std::nullopt, f.start);
            nfa.addTransition(f.accept, std::nullopt, qa);
            return {q0, qa};
        }
        case NodeKind::Plus: {
            const auto& un = static_cast<const UnaryNode&>(node);
            int qa = nfa.newState();
            auto f = buildFragment(nfa, *un.child);
            nfa.addTransition(f.accept, std::nullopt, f.start);
            nfa.addTransition(f.accept, std::nullopt, qa);
            return {f.start, qa};
        }
        case NodeKind::Question: {
            const auto& un = static_cast<const UnaryNode&>(node);
            int q0 = nfa.newState();
            int qa = nfa.newState();
            auto f = buildFragment(nfa, *un.child);
            nfa.addTransition(q0, std::nullopt, f.start);
            nfa.addTransition(q0, std::nullopt, qa);
            nfa.addTransition(f.accept, std::nullopt, qa);
            return {q0, qa};
        }
        default:
            throw std::runtime_error("Unknown node kind");
    }
}

}

NFA buildNFA(const ASTNodePtr& node) {
    NFA nfa;
    auto [start, accept] = buildFragment(nfa, *node);
    nfa.start = start;
    nfa.accept = accept;
    return nfa;
}

}
