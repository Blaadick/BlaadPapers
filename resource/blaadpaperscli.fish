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

function __blaadpaperscli_subcommands
    blaadpaperscli help --json 2>/dev/null | jq -r '.options[] | "\(.name)\t\(.description)"'
end

function __blaadpaperscli_flags
    set -l cmd (__blaadpaperscli_current_subcommand)
    test -n "$cmd"; or return

    set -l tokens (commandline -opc)

    blaadpaperscli help --json 2>/dev/null \
        | jq -r --arg cmd "$cmd" '.options[] | select(.name == $cmd) | .flags[] | "--\(.name)\t\(.description)"' \
        | while read -l line
            set -l flag_name (string split -m1 \t -- $line)[1]
            if not contains -- $flag_name $tokens
                echo $line
            end
        end
end

function __blaadpaperscli_wallpaper_ids
    blaadpaperscli list --json 2>/dev/null | jq -r '.[] | "\(.id)\t\(.name)"'
end

function __blaadpaperscli_supported_formats
    blaadpaperscli help --json 2>/dev/null | jq -r '.supported_formats[]' | string trim -l -c '.'
end

function __blaadpaperscli_add_paths
    set -l formats (__blaadpaperscli_supported_formats)
    if test (count $formats) -eq 0
        __fish_complete_path
        return
    end

    set -l token (commandline -ct)

    __fish_complete_directories $token

    set -l dir
    if string match -q '*/' -- $token
        set dir (string sub -l (math (string length -- $token) - 1) -- $token)
    else
        set dir (dirname -- $token)
        test "$dir" = "."; and set dir ""
    end
    set -l search_dir $dir
    test -z "$search_dir"; and set search_dir "."

    for entry in $search_dir/*
        test -f $entry; or continue
        set -l ext (string split -r -m1 . -- $entry)[2]
        test -n "$ext"; or continue
        contains -- (string lower -- $ext) $formats; or continue
        if test -z "$dir"
            basename -- $entry
        else
            echo $dir/(basename -- $entry)
        end
    end
end

complete -c blaadpaperscli -f

complete -c blaadpaperscli -n __blaadpaperscli_needs_subcommand -a "(__blaadpaperscli_subcommands)"

complete -c blaadpaperscli -n __blaadpaperscli_has_subcommand -a "(__blaadpaperscli_flags)"

for cmd in apply info remove
    complete -c blaadpaperscli -n "__blaadpaperscli_using_subcommand $cmd; and __blaadpaperscli_is_second_arg" -a "(__blaadpaperscli_wallpaper_ids)"
end

complete -c blaadpaperscli -n "__blaadpaperscli_using_subcommand add" -f -a "(__blaadpaperscli_add_paths)"
