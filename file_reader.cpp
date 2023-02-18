#include "file_reader.h"

FileReader::FileReader(std::istream &stream) : stream_(stream), last_byte_(0), bits_left_(0), ended_(false) {
}

bool FileReader::ReadBit() {
    if (bits_left_ == 0) {
        bits_left_ = 8;
        int new_byte = stream_.get();
        if (new_byte == std::char_traits<char>::eof()) {
            ended_ = true;
            new_byte = 0;
        }
        last_byte_ = static_cast<uint8_t>(new_byte);
    }
    --bits_left_;
    return ((last_byte_ >> bits_left_) & 1);
}

std::vector<bool> FileReader::ReadBits(size_t n) {
    std::vector<bool> result(n);
    for (size_t i = 0; i < n; ++i) {
        result[i] = ReadBit();
    }
    return result;
}

uint8_t FileReader::ReadByte() {
    auto bits = ReadBits(8);
    uint8_t result = 0;
    for (size_t i = 0; i < 8; ++i) {
        result = (result << 1) + bits[i];
    }
    return result;
}

Int9T FileReader::ReadInt9T() {
    Int9T result;
    for (size_t i = 0; i < 9; ++i) {
        result[8 - i] = ReadBit();
    }
    return result;
}

size_t FileReader::ReadInt(size_t bits) {
    size_t result = 0;
    for (size_t i = 0; i < bits; ++i) {
        result <<= 1;
        result += ReadBit();
    }
    return result;
}

bool FileReader::Ended() const {
    return ended_;
}
