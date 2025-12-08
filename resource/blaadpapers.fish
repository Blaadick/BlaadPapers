function needs_wallpaper
    set -l cmd (commandline -opc)

    if test (count $cmd) -eq 2
        if string match -q -r '^-[ADI]' -- $cmd[2]
            return 0
        end
    end

    return 1
end

function get_wallpapers
    blaadpapers -Lj | jq -r '.[] | "\(.id)\t\(.name)"'
end

complete -c blaadpapers -f
complete -c blaadpapers -n "needs_wallpaper" -a "(get_wallpapers)"
