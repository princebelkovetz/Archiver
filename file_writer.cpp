#include "file_writer.h"

FileWriter::FileWriter(std::ostream &stream) : stream_(stream), buffer_(0), bits_used_(0) {
}

void FileWriter::WriteBit(bool bit) {
    buffer_ |= (1 << (7 - bits_used_)) * bit;
    ++bits_used_;
    if (bits_used_ == 8) {
        bits_used_ = 0;
        stream_.put(static_cast<char>(buffer_));
        buffer_ = 0;
    }
}

void FileWriter::WriteBits(std::vector<bool> bits) {
    for (auto bit : bits) {
        WriteBit(bit);
    }
}

void FileWriter::WriteInt9T(Int9T num) {
    for (size_t i = 0; i < 9; ++i) {
        WriteBit(num[8 - i]);
    }
}

void FileWriter::WriteByte(uint8_t byte) {
    for (size_t i = 0; i < 8; ++i) {
        bool bit = (byte >> (7 - i)) & 1;
        WriteBit(bit);
    }
}

void FileWriter::EndWriting() {
    if (bits_used_ > 0) {
        stream_.put(static_cast<char>(buffer_));
    }
    stream_.flush();
}
