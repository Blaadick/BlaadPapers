#pragma once

#include <functional>
#include <list>
#include <map>
#include <set>
#include <string_view>

class OptionExecutor {
public:
    static void execute(int argc, char** argv);

private:
    struct Option {
        std::function<void(const std::set<char>&, const std::vector<char*>&)> func;
        std::set<char> allowableSubOptions;
        std::string_view helpMessage;
    };

    static std::map<char, Option> options;
};
