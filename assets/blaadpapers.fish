function __blaadpapers_needs_completion
    set -l tokens (commandline -opc)

    if test (count $tokens) -eq 2; and string match -q -- "*-S*" -- $tokens[2]
        return 0
    end

    return 1
end

complete -c blaadpapers -f
complete -c blaadpapers -n "__blaadpapers_needs_completion" -a "(blaadpapers -l)"
