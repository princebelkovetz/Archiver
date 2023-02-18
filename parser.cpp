#include "parser.h"

ArgParser::ArgParser(int argc, char **argv, const std::set<std::string> &possible_flags)
    : possible_flags_(possible_flags) {
    std::string cur_flag;
    for (size_t i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            cur_flag = argv[i];
            flag_args_[argv[i]] = {};
            if (possible_flags_.find(cur_flag) == possible_flags_.end()) {
                throw FlagDoesNotExistException(cur_flag);
            }
        } else {
            std::string new_arg = static_cast<std::string>(argv[i]);
            flag_args_[cur_flag].push_back(new_arg);
        }
    }
}

std::vector<std::string> ArgParser::GetArgs(const std::string &flag) {
    return flag_args_[flag];
}

bool ArgParser::HaveFlag(const std::string &flag) {
    return flag_args_.find(flag) != flag_args_.end();
}