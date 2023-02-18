#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>

#include "exceptions.h"

class ArgParser {
public:
    ArgParser(int argc, char** argv, const std::set<std::string>& possible_flags);
    std::vector<std::string> GetArgs(const std::string& flag);
    bool HaveFlag(const std::string& flag);

private:
    std::set<std::string> possible_flags_;
    std::map<std::string, std::vector<std::string> > flag_args_;
};
