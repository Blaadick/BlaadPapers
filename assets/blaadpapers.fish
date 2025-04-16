function __blaadpapers_needs_monitor
    set -l cmd (commandline -opc)
    if test (count $cmd) -eq 2
        if string match -q -- '-S*' $cmd[2]; or string match -q -- '-R*' $cmd[2]
            return 0
        end
    end
    return 1
end

function __blaadpapers_needs_wallpaper
    set -l cmd (commandline -opc)
    if test (count $cmd) -eq 3; and string match -q -- '-S*' $cmd[2]
        return 0
    end
    return 1
end

complete -c blaadpapers -f
complete -c blaadpapers -n "__blaadpapers_needs_monitor" -a "(xrandr | grep ' connected' | cut -d' ' -f1; echo all)"
complete -c blaadpapers -n "__blaadpapers_needs_wallpaper" -a "(blaadpapers -L)"
