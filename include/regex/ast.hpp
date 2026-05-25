#pragma once
#include <memory>

namespace regex {

enum class NodeKind { Char, Concat, Alt, Star, Plus, Question };

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual NodeKind kind() const = 0;
};

using ASTNodePtr = std::unique_ptr<ASTNode>;

struct CharNode : ASTNode {
    explicit CharNode(char c) : ch(c) {}
    NodeKind kind() const override { return NodeKind::Char; }
    char ch;
};

struct BinaryNode : ASTNode {
    BinaryNode(NodeKind k, ASTNodePtr l, ASTNodePtr r)
        : k_(k), left(std::move(l)), right(std::move(r)) {}
    NodeKind kind() const override { return k_; }
    NodeKind k_;
    ASTNodePtr left, right;
};

struct UnaryNode : ASTNode {
    UnaryNode(NodeKind k, ASTNodePtr c) : k_(k), child(std::move(c)) {}
    NodeKind kind() const override { return k_; }
    NodeKind k_;
    ASTNodePtr child;
};

}
