#include "regex/parser.hpp"

namespace regex {

Parser::Parser(std::string pattern) : pattern_(std::move(pattern)), pos_(0) {}

ASTNodePtr Parser::parse() {
    auto node = parseRegex();
    if (!atEnd()) {
        throw std::runtime_error("Unexpected character at position " + std::to_string(pos_));
    }
    return node;
}

ASTNodePtr Parser::parseRegex() {
    ASTNodePtr left = parseTerm();
    while (!atEnd() && peek() == '|') {
        consume();
        ASTNodePtr right = parseTerm();
        left = std::make_unique<BinaryNode>(NodeKind::Alt, std::move(left), std::move(right));
    }
    return left;
}

ASTNodePtr Parser::parseTerm() {
    ASTNodePtr node = parseFactor();
    while (!atEnd() && peek() != ')' && peek() != '|') {
        ASTNodePtr next = parseFactor();
        node = std::make_unique<BinaryNode>(NodeKind::Concat, std::move(node), std::move(next));
    }
    return node;
}

ASTNodePtr Parser::parseFactor() {
    ASTNodePtr node = parseAtom();
    if (!atEnd()) {
        char c = peek();
        if (c == '*' || c == '+' || c == '?') {
            consume();
            NodeKind k = (c == '*') ? NodeKind::Star : (c == '+') ? NodeKind::Plus : NodeKind::Question;
            node = std::make_unique<UnaryNode>(k, std::move(node));
        }
    }
    return node;
}

ASTNodePtr Parser::parseAtom() {
    if (atEnd()) {
        throw std::runtime_error("Unexpected end of pattern");
    }
    char c = peek();
    if (c == '(') {
        consume();
        auto node = parseRegex();
        if (atEnd() || peek() != ')') {
            throw std::runtime_error("Expected closing parenthesis");
        }
        consume();
        return node;
    }
    if (c == ')' || c == '|' || c == '*' || c == '+' || c == '?') {
        throw std::runtime_error(std::string("Unexpected metacharacter: ") + c);
    }
    consume();
    return std::make_unique<CharNode>(c);
}

char Parser::peek() const { return pattern_[pos_]; }
char Parser::consume() { return pattern_[pos_++]; }
bool Parser::atEnd() const { return pos_ >= pattern_.size(); }

}
