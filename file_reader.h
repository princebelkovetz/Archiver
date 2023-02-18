#pragma once

#include <fstream>
#include <vector>
#include <bitset>

#include "int9t_namespace.h"
#include "exceptions.h"

class FileReader {
public:
    explicit FileReader(std::istream& stream);
    bool ReadBit();
    std::vector<bool> ReadBits(size_t n);
    uint8_t ReadByte();
    Int9T ReadInt9T();
    size_t ReadInt(size_t bits);
    bool Ended() const;

private:
    std::istream& stream_;
    uint8_t last_byte_;
    size_t bits_left_;
    bool ended_;
};
