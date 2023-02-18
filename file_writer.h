#pragma once

#include <fstream>
#include <vector>
#include <bitset>

#include "int9t_namespace.h"

class FileWriter {
public:
    explicit FileWriter(std::ostream& stream);
    void WriteBit(bool bit);
    void WriteBits(std::vector<bool> bits);
    void WriteInt9T(Int9T num);
    void WriteByte(uint8_t byte);
    void EndWriting();

private:
    std::ostream& stream_;
    uint8_t buffer_;
    uint8_t bits_used_;
};
