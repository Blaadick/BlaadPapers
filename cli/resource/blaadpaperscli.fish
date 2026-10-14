function __blaadpaperscli_current_subcommand
    set -l tokens (commandline -opc)
    if test (count $tokens) -ge 2
        echo $tokens[2]
    end
end

function __blaadpaperscli_needs_subcommand
    set -l tokens (commandline -opc)
    test (count $tokens) -eq 1
end

function __blaadpaperscli_has_subcommand
    not __blaadpaperscli_needs_subcommand
end

function __blaadpaperscli_using_subcommand
    set -l cmd (__blaadpaperscli_current_subcommand)
    test "$cmd" = "$argv[1]"
end

function __blaadpaperscli_is_second_arg
    set -l tokens (commandline -opc)
    test (count $tokens) -eq 2
end

function __blaadpaperscli_description
    blaadpaperscli help --json | jq -r '.description'
end

function __blaadpaperscli_subcommands
    blaadpaperscli help --json | jq -r '.options[] | "\(.name)\t\(.description)"'
end

function __blaadpaperscli_flags
    set -l cmd (__blaadpaperscli_current_subcommand)
    set -l tokens (commandline -opc)

    blaadpaperscli help --json \
        | jq -r --arg cmd "$cmd" '.options[] | select(.name == $cmd) | .flags[] | "--\(.name)\t\(.description)"' \
        | while read -l line
            set -l flag_name (string split -m1 \t -- $line)[1]
            if not contains -- $flag_name $tokens
                echo $line
            end
        end
end

function __blaadpaperscli_wallpaper_ids
    blaadpaperscli list --json | jq -r '.[] | "\(.id)\t\(.name)"'
end

complete -c blaadpaperscli -f -d (__blaadpaperscli_description)
complete -c blaadpaperscli -n __blaadpaperscli_needs_subcommand -a "(__blaadpaperscli_subcommands)"
complete -c blaadpaperscli -n __blaadpaperscli_has_subcommand -a "(__blaadpaperscli_flags)"

for cmd in apply info remove
    complete -c blaadpaperscli -n "__blaadpaperscli_using_subcommand $cmd; and __blaadpaperscli_is_second_arg" -a "(__blaadpaperscli_wallpaper_ids)"
end

complete -c blaadpaperscli -n "__blaadpaperscli_using_subcommand install" -F
