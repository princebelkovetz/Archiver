#include "exceptions.h"

FlagDoesNotExistException::FlagDoesNotExistException(const std::string &flag)
    : error_description_("Flag " + flag + " cannot be used in this program, use '-c', '-d' or '-h' :(") {
}

const char *FlagDoesNotExistException::what() const noexcept {
    return error_description_.c_str();
}

FlagNotFoundException::FlagNotFoundException() {
}

const char *FlagNotFoundException::what() const noexcept {
    return "No needed flags found in arguments :(";
}

FileCannotBeOpenedException::FileCannotBeOpenedException(const std::string &file)
    : error_description_("File " + file + " cannot be opened. Make sure the path is correct :(") {
}

const char *FileCannotBeOpenedException::what() const noexcept {
    return error_description_.c_str();
}