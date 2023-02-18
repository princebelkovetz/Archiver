#include "archive_unpacker.h"

ArchiveUnpacker::ArchiveUnpacker(const std::string& archive_file) : archive_file_(archive_file) {
}

void ArchiveUnpacker::BuildCanonical() {
    canonical_code_.clear();
    std::vector<bool> code;
    for (size_t i = 0; i < alphabet_.size(); ++i) {
        if (code.empty()) {
            code.assign(code_lengths_[i], false);
        } else {
            size_t last_zero = code.size() - 1;
            while (code[last_zero] == 1) {
                if (last_zero == 0) {
                    throw std::runtime_error("could not build canonical code :(");
                }
                --last_zero;
            }
            code[last_zero] = true;
            for (size_t j = last_zero + 1; j < code.size(); ++j) {
                code[j] = false;
            }
            while (code.size() < code_lengths_[i]) {
                code.push_back(false);
            }
        }
        canonical_code_[alphabet_[i]] = code;
    }
}

void ArchiveUnpacker::BuildTrie() {
    trie_ = new TrieNode(0, 0, false);
    for (const auto& [symbol, code] : canonical_code_) {
        AddSymbolCode(trie_, symbol, code);
    }
}

Int9T ArchiveUnpacker::UnpackSymbol(FileReader& reader) {
    TrieNode* node = trie_;
    while (!node->is_leaf_) {
        bool bit = reader.ReadBit();
        node = node->child_[bit];
    }
    return node->min_symbol_;
}

char GetChar(Int9T symbol) {
    size_t result = 0;
    for (size_t i = 0; i < 9; ++i) {
        result <<= 1;
        result += symbol[8 - i];
    }
    return static_cast<char>(result);
}

void ArchiveUnpacker::UnpackAlphabet(FileReader& reader) {
    alphabet_.clear();
    size_t symbols_count = reader.ReadInt(9);
    for (size_t i = 0; i < symbols_count; ++i) {
        alphabet_.push_back(reader.ReadInt9T());
    }
}

void ArchiveUnpacker::UnpackCodeLengths(FileReader& reader, size_t symbols_count) {
    code_lengths_.clear();
    size_t cur_length = 1;
    while (symbols_count > 0) {
        size_t length_cnt = reader.ReadInt(9);
        symbols_count -= length_cnt;
        for (size_t i = 0; i < length_cnt; ++i) {
            code_lengths_.push_back(cur_length);
        }
        ++cur_length;
    }
}

std::string ArchiveUnpacker::UnpackFileName(FileReader& reader) {
    Int9T symbol_i9 = UnpackSymbol(reader);
    std::string file_name;
    while (symbol_i9 != FILENAME_END) {
        file_name += GetChar(symbol_i9);
        symbol_i9 = UnpackSymbol(reader);
    }
    return file_name;
}

Int9T ArchiveUnpacker::UnpackFile(FileReader& reader) {
    UnpackAlphabet(reader);
    UnpackCodeLengths(reader, alphabet_.size());

    BuildCanonical();
    BuildTrie();

    std::string file_name = UnpackFileName(reader);
    std::ofstream file;
    file.open(file_name);

    if (file.is_open()) {
        FileWriter writer(file);
        Int9T symbol_i9 = UnpackSymbol(reader);
        while (symbol_i9 != ARCHIVE_END && symbol_i9 != ONE_MORE_FILE) {
            writer.WriteByte(GetChar(symbol_i9));
            symbol_i9 = UnpackSymbol(reader);
        }
        writer.EndWriting();
        file.close();
        delete trie_;
        return symbol_i9;
    } else {
        delete trie_;
        throw FileCannotBeOpenedException(file_name);
    }
}

void ArchiveUnpacker::ProcessUnpacking() {
    try {
        std::ifstream archive;
        archive.open(archive_file_);
        if (archive.is_open()) {
            FileReader reader(archive);

            Int9T last_symbol = UnpackFile(reader);

            while (last_symbol == ONE_MORE_FILE) {
                last_symbol = UnpackFile(reader);
            }

            archive.close();
        } else {
            throw FileCannotBeOpenedException(archive_file_);
        }
    } catch (...) {
        throw;
    }
}
