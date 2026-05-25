#include <gtest/gtest.h>
#include "regex/parser.hpp"

using namespace regex;

TEST(ParserTest, SingleChar) {
    Parser p("a");
    auto ast = p.parse();
    ASSERT_NE(ast, nullptr);
    EXPECT_EQ(ast->kind(), NodeKind::Char);
    EXPECT_EQ(static_cast<CharNode&>(*ast).ch, 'a');
}

TEST(ParserTest, Concatenation) {
    Parser p("ab");
    auto ast = p.parse();
    ASSERT_NE(ast, nullptr);
    EXPECT_EQ(ast->kind(), NodeKind::Concat);
    const auto& bn = static_cast<BinaryNode&>(*ast);
    EXPECT_EQ(bn.left->kind(), NodeKind::Char);
    EXPECT_EQ(bn.right->kind(), NodeKind::Char);
}

TEST(ParserTest, Alternation) {
    Parser p("a|b");
    auto ast = p.parse();
    ASSERT_NE(ast, nullptr);
    EXPECT_EQ(ast->kind(), NodeKind::Alt);
}

TEST(ParserTest, StarQuantifier) {
    Parser p("a*");
    auto ast = p.parse();
    ASSERT_NE(ast, nullptr);
    EXPECT_EQ(ast->kind(), NodeKind::Star);
    EXPECT_EQ(static_cast<UnaryNode&>(*ast).child->kind(), NodeKind::Char);
}

TEST(ParserTest, PlusQuantifier) {
    Parser p("a+");
    auto ast = p.parse();
    EXPECT_EQ(ast->kind(), NodeKind::Plus);
}

TEST(ParserTest, QuestionQuantifier) {
    Parser p("a?");
    auto ast = p.parse();
    EXPECT_EQ(ast->kind(), NodeKind::Question);
}

TEST(ParserTest, Group) {
    Parser p("(a|b)");
    auto ast = p.parse();
    EXPECT_EQ(ast->kind(), NodeKind::Alt);
}

TEST(ParserTest, QuantifierOnGroup) {
    Parser p("(a|b)*");
    auto ast = p.parse();
    ASSERT_EQ(ast->kind(), NodeKind::Star);
    EXPECT_EQ(static_cast<UnaryNode&>(*ast).child->kind(), NodeKind::Alt);
}

TEST(ParserTest, ComplexPatternAbb) {
    Parser p("(a|b)*abb");
    EXPECT_NO_THROW(p.parse());
}

TEST(ParserTest, ComplexPatternBinaryEven) {
    Parser p("0*(10*10*)*");
    EXPECT_NO_THROW(p.parse());
}

TEST(ParserTest, NestedGroups) {
    Parser p("(a(b|c)*)");
    EXPECT_NO_THROW(p.parse());
}

TEST(ParserTest, MultipleAlternation) {
    Parser p("a|b|c");
    auto ast = p.parse();
    EXPECT_EQ(ast->kind(), NodeKind::Alt);
}

TEST(ParserTest, UnmatchedOpenParen) {
    Parser p("(a|b");
    EXPECT_THROW(p.parse(), std::runtime_error);
}

TEST(ParserTest, UnmatchedCloseParen) {
    Parser p("a|b)");
    EXPECT_THROW(p.parse(), std::runtime_error);
}

TEST(ParserTest, LeadingQuantifier) {
    Parser p("*a");
    EXPECT_THROW(p.parse(), std::runtime_error);
}
