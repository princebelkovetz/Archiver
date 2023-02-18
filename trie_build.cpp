#include "trie_build.h"

void AddSymbolCode(TrieNode* node, const Int9T& symbol, const std::vector<bool>& code) {
    for (size_t i = 0; i < code.size(); ++i) {
        if (node->child_[code[i]] == nullptr) {
            node->child_[code[i]] = new TrieNode(0, 0, false);
        }
        node = node->child_[code[i]];
    }
    node->min_symbol_ = symbol;
    node->is_leaf_ = true;
}