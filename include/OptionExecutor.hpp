#pragma once

#include <functional>
#include <map>
#include <set>
#include <string_view>

class OptionExecutor {
    struct Option {
        std::function<void(const std::pmr::set<char> &, int, const char **)> func;
        std::pmr::set<char> allowableSubOptions;
        std::string_view helpMessage;
    };

    std::pmr::map<char, Option> options;

    OptionExecutor();

    static void help(const std::pmr::set<char> &subOptions, int argNumber, const char **arguments);

    static void version(const std::pmr::set<char> &subOptions, int argNumber, const char **arguments);

    static void set(const std::pmr::set<char> &subOptions, int argNumber, const char **arguments);

    static void random(const std::pmr::set<char> &subOptions, int argNumber, const char **arguments);

    static void list(const std::pmr::set<char> &subOptions, int argNumber, const char **arguments);

public:
    OptionExecutor(const OptionExecutor &) = delete;

    OptionExecutor operator=(const OptionExecutor &) = delete;

    static OptionExecutor &getInstance();

    void execute(int argNumber, const char **arguments);
};
