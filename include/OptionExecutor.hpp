#pragma once

#include <functional>
#include <map>
#include <set>
#include <string_view>

class OptionExecutor {
    struct Option {
        std::function<void(const std::pmr::set<char>&, int, char*[])> func;
        std::pmr::set<char> allowableSubOptions;
        std::string_view helpMessage;
    };

    static std::pmr::map<char, Option> options;

    static void help(const std::pmr::set<char>& subOptions, int argNumber, char* arguments[]);

    static void version(const std::pmr::set<char>& subOptions, int argNumber, char* arguments[]);

    static void set(const std::pmr::set<char>& subOptions, int argNumber, char* arguments[]);

    static void random(const std::pmr::set<char>& subOptions, int argNumber, char* arguments[]);

    static void list(const std::pmr::set<char>& subOptions, int argNumber, char* arguments[]);

public:
    static void execute(int argNumber, char* arguments[]);
};
