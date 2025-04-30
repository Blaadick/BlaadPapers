#pragma once

#include <functional>
#include <map>
#include <set>
#include <string_view>

class OptionExecutor {
public:
    static void execute(int argNumber, char* arguments[]);

private:
    struct Option {
        std::function<void(const std::pmr::set<char>&, int, char*[])> func;
        std::pmr::set<char> allowableSubOptions;
        std::string_view helpMessage;
    };

    static std::pmr::map<char, Option> options;
};
