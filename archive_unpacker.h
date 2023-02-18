#pragma once

#include <string>
#include <fstream>
#include <bitset>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "file_writer.h"
#include "file_reader.h"
#include "trie_node.h"
#include "int9t_namespace.h"
#include "exceptions.h"
#include "trie_build.h"

class ArchiveUnpacker {
public:
    explicit ArchiveUnpacker(const std::string& archive_file);
    void ProcessUnpacking();

private:
    std::string archive_file_;
    std::vector<Int9T> alphabet_;
    std::vector<size_t> code_lengths_;
    std::unordered_map<Int9T, std::vector<bool>> canonical_code_;
    TrieNode* trie_;

    void BuildCanonical();
    void BuildTrie();

    Int9T UnpackSymbol(FileReader& reader);
    Int9T UnpackFile(FileReader& reader);  // return next symbol after file

    void UnpackAlphabet(FileReader& reader);
    void UnpackCodeLengths(FileReader& reader, size_t symbols_count);
    std::string UnpackFileName(FileReader& reader);
};
