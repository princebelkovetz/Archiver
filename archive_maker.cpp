#include "archive_maker.h"

std::string GetFilename(const std::string& path) {
    size_t filename_begin = path.rfind('/');
    if (filename_begin == std::string::npos) {
        filename_begin = 0;
    } else {
        ++filename_begin;
    }
    return path.substr(filename_begin);
}

struct CompareTriePtr {
    bool operator()(TrieNode* lhs, TrieNode* rhs) const {
        return *lhs < *rhs;
    }
};

bool CompareCodeLength(const std::pair<Int9T, size_t>& lhs, const std::pair<Int9T, size_t>& rhs) {
    if (lhs.second != rhs.second) {
        return lhs.second < rhs.second;
    }
    return IsLess(lhs.first, rhs.first);
}

void ArchiveMaker::CountCharFrequency() {
    for (size_t i = 0; i < files_.size(); ++i) {
        std::ifstream cur_file;
        cur_file.open(files_[i]);
        if (cur_file.is_open()) {
            FileReader reader(cur_file);
            Int9T symbol_i9 = static_cast<Int9T>(reader.ReadByte());
            while (!reader.Ended()) {
                ++char_frequency_[i][symbol_i9];
                symbol_i9 = static_cast<Int9T>(reader.ReadByte());
            }
            for (const auto& symbol : GetFilename(files_[i])) {
                symbol_i9 = static_cast<Int9T>(symbol);
                ++char_frequency_[i][symbol_i9];
            }
            cur_file.close();

            char_frequency_[i][FILENAME_END] = 1;
            char_frequency_[i][ONE_MORE_FILE] = 1;
            char_frequency_[i][ARCHIVE_END] = 1;
        } else {
            throw FileCannotBeOpenedException(files_[i]);
        }
    }
}

void ArchiveMaker::BuildTrie() {
    for (size_t i = 0; i < files_.size(); ++i) {
        PriorityQueue<TrieNode*, std::vector<TrieNode*>, CompareTriePtr> que;

        for (const auto& [symbol, freq] : char_frequency_[i]) {
            TrieNode* new_node = new TrieNode(symbol, freq, true);
            que.Push(new_node);
        }

        while (que.Size() > 1) {
            auto lhs = que.Top();
            que.Pop();
            auto rhs = que.Top();
            que.Pop();
            TrieNode* new_node = new TrieNode(lhs, rhs);
            que.Push(new_node);
        }

        trie_[i] = que.Top();
    }
}

ArchiveMaker::ArchiveMaker(const std::vector<std::string>& files) : files_(files) {
    char_frequency_.resize(files_.size());
    trie_.resize(files_.size());

    CountCharFrequency();
    BuildTrie();
}

void DepthFirstSearch(TrieNode* cur_node, size_t cur_depth, std::vector<std::pair<Int9T, size_t>>& result) {
    if (cur_node->is_leaf_) {
        result.emplace_back(cur_node->min_symbol_, cur_depth);
    } else {
        DepthFirstSearch(cur_node->child_[0], cur_depth + 1, result);
        DepthFirstSearch(cur_node->child_[1], cur_depth + 1, result);
    }
}

void ArchiveMaker::GetCodeLengths(size_t index) {
    code_lengths_.clear();  // pair { char, len }
    DepthFirstSearch(trie_[index], 0, code_lengths_);
    std::sort(code_lengths_.begin(), code_lengths_.end(), CompareCodeLength);
}

void ArchiveMaker::BuildCanonicalCode() {
    canonical_code_.clear();
    std::vector<bool> code;
    for (const auto& [symbol, length] : code_lengths_) {
        if (code.empty()) {
            code.assign(length, false);
        } else {
            size_t last_zero = code.size() - 1;
            while (code[last_zero] == 1) {
                if (last_zero == 0) {
                    throw std::runtime_error("could not build canonical code :(");
                }
                --last_zero;
            }
            code[last_zero] = true;
            for (size_t i = last_zero + 1; i < code.size(); ++i) {
                code[i] = false;
            }
            while (code.size() < length) {
                code.push_back(false);
            }
        }
        canonical_code_[symbol] = code;
    }
}

void ArchiveMaker::ArchiveAlphabet(FileWriter& writer) {
    writer.WriteInt9T(code_lengths_.size());  // SYMBOLS_COUNT
    for (const auto& [symbol, length] : code_lengths_) {
        writer.WriteInt9T(symbol);
    }
}

void ArchiveMaker::ArchiveCodeLengths(FileWriter& writer) {
    size_t max_symbol_code_size = code_lengths_.back().second;

    std::vector<int> cnt_length(max_symbol_code_size + 1);
    for (const auto& [symbol, length] : code_lengths_) {
        ++cnt_length[length];
    }

    for (size_t len = 1; len <= max_symbol_code_size; ++len) {
        Int9T value = static_cast<Int9T>(cnt_length[len]);
        writer.WriteInt9T(value);
    }
}

void ArchiveMaker::ArchiveFile(const std::string& file_path, FileWriter& writer) {
    std::ifstream file;
    file.open(file_path);
    if (file.is_open()) {
        FileReader reader(file);
        for (const auto& symbol : GetFilename(file_path)) {
            Int9T symbol_i9 = static_cast<Int9T>(symbol);
            writer.WriteBits(canonical_code_[symbol_i9]);
        }
        writer.WriteBits(canonical_code_[FILENAME_END]);
        Int9T new_symbol = static_cast<Int9T>(reader.ReadByte());
        while (!reader.Ended()) {
            writer.WriteBits(canonical_code_[new_symbol]);
            new_symbol = static_cast<Int9T>(reader.ReadByte());
        }
        file.close();
    } else {
        throw FileCannotBeOpenedException(file_path);
    }
}

void ArchiveMaker::ProcessArchiving(const std::string& archive_name) {
    try {
        std::ofstream archive;
        archive.open(archive_name);
        if (archive.is_open()) {
            FileWriter archiver(archive);
            for (size_t i = 0; i < files_.size(); ++i) {
                GetCodeLengths(i);  // pair <symbol, code length>
                BuildCanonicalCode();

                ArchiveAlphabet(archiver);
                ArchiveCodeLengths(archiver);
                ArchiveFile(files_[i], archiver);

                if (i + 1 == files_.size()) {
                    archiver.WriteBits(canonical_code_[ARCHIVE_END]);
                } else {
                    archiver.WriteBits(canonical_code_[ONE_MORE_FILE]);
                }
            }
            archiver.EndWriting();
            archive.close();
        } else {
            throw FileCannotBeOpenedException(archive_name);
        }
    } catch (...) {
        throw;
    }
}

ArchiveMaker::~ArchiveMaker() {
    for (TrieNode* node : trie_) {
        delete node;
    }
}