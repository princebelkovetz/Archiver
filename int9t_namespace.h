#pragma once

#include <bitset>

namespace {
using Int9T = std::bitset<9>;
const Int9T FILENAME_END = static_cast<Int9T>(256);
const Int9T ONE_MORE_FILE = static_cast<Int9T>(257);
const Int9T ARCHIVE_END = static_cast<Int9T>(258);
}  // namespace
