#pragma once

#include <string>
#include <exception>

class FlagDoesNotExistException : public std::exception {
public:
    explicit FlagDoesNotExistException(const std::string& flag);
    const char* what() const noexcept override;

private:
    std::string error_description_;
};

class FlagNotFoundException : public std::exception {
public:
    explicit FlagNotFoundException();
    const char* what() const noexcept override;
};

class FileCannotBeOpenedException : public std::exception {
public:
    explicit FileCannotBeOpenedException(const std::string& file);
    const char* what() const noexcept override;

private:
    std::string error_description_;
};
