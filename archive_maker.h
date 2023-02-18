#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <utility>
#include <bitset>
#include <unordered_map>
#include <algorithm>
#include <queue>

#include "file_reader.h"
#include "file_writer.h"
#include "trie_node.h"
#include "priority_queue.h"
#include "int9t_namespace.h"

class ArchiveMaker {
public:
    explicit ArchiveMaker(const std::vector<std::string>& files);
    void ProcessArchiving(const std::string& archive_name);

    ~ArchiveMaker();

private:
    std::vector<std::unordered_map<Int9T, size_t>> char_frequency_;
    std::vector<TrieNode*> trie_;
    std::vector<std::string> files_;

    std::vector<std::pair<Int9T, size_t>> code_lengths_;
    std::unordered_map<Int9T, std::vector<bool>> canonical_code_;

    void CountCharFrequency();
    void BuildTrie();

    void GetCodeLengths(size_t index);
    void BuildCanonicalCode();

    void ArchiveFile(const std::string& file_path, FileWriter& writer);
    void ArchiveAlphabet(FileWriter& writer);
    void ArchiveCodeLengths(FileWriter& writer);
};
