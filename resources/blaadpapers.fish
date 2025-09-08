function __blaadpapers_needs_wallpaper
    set -l cmd (commandline -opc)

    if test (count $cmd) -eq 2; and string match -q -- '-S*' $cmd[2]
        return 0
    end

    return 1
end

complete -c blaadpapers -f
complete -c blaadpapers -n "__blaadpapers_needs_wallpaper" -a "(blaadpapers -L)"
