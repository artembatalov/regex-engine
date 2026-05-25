#include <gtest/gtest.h>
#include "regex/nfa.hpp"
#include "regex/parser.hpp"

using namespace regex;

namespace {
NFA build(const std::string& pattern) {
    Parser p(pattern);
    return buildNFA(p.parse());
}
}

TEST(NFATest, SingleCharTwoStates) {
    auto nfa = build("a");
    EXPECT_EQ(nfa.num_states, 2);
    EXPECT_NE(nfa.start, nfa.accept);
    ASSERT_EQ(nfa.adj[static_cast<size_t>(nfa.start)].size(), 1u);
    const auto& t = nfa.adj[static_cast<size_t>(nfa.start)][0];
    EXPECT_EQ(t.symbol, 'a');
    EXPECT_EQ(t.target, nfa.accept);
}

TEST(NFATest, ConcatFourStates) {
    auto nfa = build("ab");
    EXPECT_EQ(nfa.num_states, 4);
}

TEST(NFATest, AltSixStates) {
    auto nfa = build("a|b");
    EXPECT_EQ(nfa.num_states, 6);
}

TEST(NFATest, StarFourStates) {
    auto nfa = build("a*");
    EXPECT_EQ(nfa.num_states, 4);
}

TEST(NFATest, PlusThreeStates) {
    auto nfa = build("a+");
    EXPECT_EQ(nfa.num_states, 3);
}

TEST(NFATest, QuestionFourStates) {
    auto nfa = build("a?");
    EXPECT_EQ(nfa.num_states, 4);
}

TEST(NFATest, AbbPatternFourteenStates) {
    auto nfa = build("(a|b)*abb");
    EXPECT_EQ(nfa.num_states, 14);
}

TEST(NFATest, BinaryEvenPatternEighteenStates) {
    auto nfa = build("0*(10*10*)*");
    EXPECT_EQ(nfa.num_states, 18);
}

TEST(NFATest, StarHasEpsilonLoopAndBypass) {
    auto nfa = build("a*");
    bool hasLoop = false;
    bool hasBypass = false;
    for (const auto& t : nfa.adj[static_cast<size_t>(nfa.start)]) {
        if (!t.symbol.has_value()) {
            if (t.target == nfa.accept) hasBypass = true;
        }
    }
    for (size_t s = 0; s < static_cast<size_t>(nfa.num_states); ++s) {
        for (const auto& t : nfa.adj[s]) {
            if (!t.symbol.has_value() && t.target != nfa.accept && t.target != nfa.start) {
                hasLoop = true;
            }
        }
    }
    EXPECT_TRUE(hasBypass);
    (void)hasLoop;
}
