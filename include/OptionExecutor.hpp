#pragma once

#include <functional>
#include <map>
#include <string_view>

#include "Defaults.hpp"

using namespace std;

class OptionExecutor {
    struct option {
        function<void(const char **, const bool &)> func;
        string_view helpMessage;
    };

    map<char, option> options;

    static void help(const char **arguments, const bool &jsonOutput);

    static void version(const char **arguments, const bool &jsonOutput);

    static void set(const char **arguments, const bool &jsonOutput);

    static void random(const char **arguments, const bool &jsonOutput);

    static void list(const char **arguments, const bool &jsonOutput);

public:
    OptionExecutor() {
        options['H'] = {help, helpHelpMessage};
        options['V'] = {version, versionHelpMessage};
        options['S'] = {set, setHelpMessage};
        options['R'] = {random, randomHelpMessage};
        options['L'] = {list, listHelpMessage};
    }

    void executeOption(const char **arguments);
};
