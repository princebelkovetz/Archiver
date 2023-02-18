#include "trie_node.h"

bool IsLess(const Int9T& lhs, const Int9T& rhs) {
    for (size_t i = 0; i < 9; ++i) {
        if (lhs[8 - i] < rhs[8 - i]) {
            return true;
        }
        if (lhs[8 - i] > rhs[8 - i]) {
            return false;
        }
    }
    return false;
}

TrieNode::TrieNode(Int9T symbol, size_t frequency, bool is_leaf) {
    child_[0] = child_[1] = nullptr;
    priority_ = frequency;
    is_leaf_ = is_leaf;
    min_symbol_ = symbol;
}

TrieNode::TrieNode(TrieNode* lhs, TrieNode* rhs) {
    child_[0] = lhs;
    child_[1] = rhs;
    priority_ = lhs->priority_ + rhs->priority_;
    is_leaf_ = false;
    if (IsLess(lhs->min_symbol_, rhs->min_symbol_)) {
        min_symbol_ = lhs->min_symbol_;
    } else {
        min_symbol_ = rhs->min_symbol_;
    }
}

bool TrieNode::operator<(const TrieNode& other) const {
    if (priority_ != other.priority_) {
        return priority_ < other.priority_;
    }
    return IsLess(min_symbol_, other.min_symbol_);
}

TrieNode::~TrieNode() {
    delete child_[0];
    delete child_[1];
}