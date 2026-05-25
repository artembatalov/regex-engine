#include <gtest/gtest.h>
#include "regex/dfa.hpp"
#include "regex/nfa.hpp"
#include "regex/parser.hpp"

using namespace regex;

namespace {
DFA build(const std::string& pattern, const std::set<char>& alphabet) {
    Parser p(pattern);
    return buildDFA(buildNFA(p.parse()), alphabet);
}
}

TEST(DFATest, SingleCharAccept) {
    auto dfa = build("a", {'a', 'b'});
    EXPECT_TRUE(dfa.accepts("a"));
    EXPECT_FALSE(dfa.accepts("b"));
    EXPECT_FALSE(dfa.accepts(""));
    EXPECT_FALSE(dfa.accepts("aa"));
    EXPECT_FALSE(dfa.accepts("ab"));
}

TEST(DFATest, ConcatAccept) {
    auto dfa = build("ab", {'a', 'b'});
    EXPECT_TRUE(dfa.accepts("ab"));
    EXPECT_FALSE(dfa.accepts("a"));
    EXPECT_FALSE(dfa.accepts("b"));
    EXPECT_FALSE(dfa.accepts("ba"));
    EXPECT_FALSE(dfa.accepts("abb"));
    EXPECT_FALSE(dfa.accepts(""));
}

TEST(DFATest, AltAccept) {
    auto dfa = build("a|b", {'a', 'b'});
    EXPECT_TRUE(dfa.accepts("a"));
    EXPECT_TRUE(dfa.accepts("b"));
    EXPECT_FALSE(dfa.accepts("ab"));
    EXPECT_FALSE(dfa.accepts("ba"));
    EXPECT_FALSE(dfa.accepts(""));
}

TEST(DFATest, StarAccept) {
    auto dfa = build("a*", {'a', 'b'});
    EXPECT_TRUE(dfa.accepts(""));
    EXPECT_TRUE(dfa.accepts("a"));
    EXPECT_TRUE(dfa.accepts("aa"));
    EXPECT_TRUE(dfa.accepts("aaa"));
    EXPECT_FALSE(dfa.accepts("b"));
    EXPECT_FALSE(dfa.accepts("ab"));
    EXPECT_FALSE(dfa.accepts("ba"));
}

TEST(DFATest, PlusAccept) {
    auto dfa = build("a+", {'a', 'b'});
    EXPECT_FALSE(dfa.accepts(""));
    EXPECT_TRUE(dfa.accepts("a"));
    EXPECT_TRUE(dfa.accepts("aa"));
    EXPECT_TRUE(dfa.accepts("aaa"));
    EXPECT_FALSE(dfa.accepts("b"));
    EXPECT_FALSE(dfa.accepts("ab"));
}

TEST(DFATest, QuestionAccept) {
    auto dfa = build("a?", {'a', 'b'});
    EXPECT_TRUE(dfa.accepts(""));
    EXPECT_TRUE(dfa.accepts("a"));
    EXPECT_FALSE(dfa.accepts("aa"));
    EXPECT_FALSE(dfa.accepts("b"));
    EXPECT_FALSE(dfa.accepts("ab"));
}

TEST(DFATest, AltStarAccept) {
    auto dfa = build("(a|b)*", {'a', 'b'});
    EXPECT_TRUE(dfa.accepts(""));
    EXPECT_TRUE(dfa.accepts("a"));
    EXPECT_TRUE(dfa.accepts("b"));
    EXPECT_TRUE(dfa.accepts("ab"));
    EXPECT_TRUE(dfa.accepts("ba"));
    EXPECT_TRUE(dfa.accepts("aabb"));
    EXPECT_TRUE(dfa.accepts("bbba"));
}

TEST(DFATest, AbbPatternBasic) {
    auto dfa = build("(a|b)*abb", {'a', 'b'});
    EXPECT_TRUE(dfa.accepts("abb"));
    EXPECT_TRUE(dfa.accepts("aabb"));
    EXPECT_TRUE(dfa.accepts("babb"));
    EXPECT_TRUE(dfa.accepts("ababb"));
    EXPECT_FALSE(dfa.accepts("ab"));
    EXPECT_FALSE(dfa.accepts("bb"));
    EXPECT_FALSE(dfa.accepts(""));
    EXPECT_FALSE(dfa.accepts("abba"));
}

TEST(DFATest, BinaryEvenOnesBasic) {
    auto dfa = build("0*(10*10*)*", {'0', '1'});
    EXPECT_TRUE(dfa.accepts(""));
    EXPECT_TRUE(dfa.accepts("0"));
    EXPECT_TRUE(dfa.accepts("00"));
    EXPECT_TRUE(dfa.accepts("11"));
    EXPECT_TRUE(dfa.accepts("110"));
    EXPECT_TRUE(dfa.accepts("101"));
    EXPECT_TRUE(dfa.accepts("00110"));
    EXPECT_FALSE(dfa.accepts("1"));
    EXPECT_FALSE(dfa.accepts("01"));
    EXPECT_FALSE(dfa.accepts("10"));
    EXPECT_FALSE(dfa.accepts("111"));
}
