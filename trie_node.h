#pragma once

#include <memory>
#include <cstddef>
#include <algorithm>
#include <bitset>

#include "int9t_namespace.h"

bool IsLess(const Int9T& lhs, const Int9T& rhs);

class TrieNode {
public:
    TrieNode(Int9T symbol, size_t frequency, bool is_leaf);
    TrieNode(TrieNode* lhs, TrieNode* rhs);

    bool operator<(const TrieNode& other) const;

    ~TrieNode();

    TrieNode* child_[2];
    size_t priority_;
    Int9T min_symbol_;
    bool is_leaf_;
};
