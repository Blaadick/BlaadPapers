#compdef blaadpaperscli

_blaadpaperscli_subcommands() {
    local -a cmds
    cmds=("${(@f)$(blaadpaperscli help --json | jq -r '.options[] | "\(.name):\(.description)"')}")
    _describe -t commands "$(blaadpaperscli help --json | jq -r '.description')" cmds
}

_blaadpaperscli_flags() {
    local -a flags available
    local line
    flags=("${(@f)$(blaadpaperscli help --json | jq -r --arg cmd "$words[2]" '.options[] | select(.name == $cmd) | .flags[] | "--\(.name):\(.description)"')}")
    for line in $flags; do
        (( ${words[(Ie)${line%%:*}]} )) || available+=$line
    done
    _describe -t flags 'flag' available
}

_blaadpaperscli_wallpaper_ids() {
    local -a ids
    ids=("${(@f)$(blaadpaperscli list --json | jq -r '.[] | "\(.id):\(.name)"')}")
    _describe -t wallpapers 'wallpaper' ids
}

_blaadpaperscli() {
    if (( CURRENT == 2 )); then
        _blaadpaperscli_subcommands
        return
    fi

    case $words[2] in
        apply|info|remove)
            (( CURRENT == 3 )) && _blaadpaperscli_wallpaper_ids
            ;;
        add)
            _files
            ;;
    esac

    _blaadpaperscli_flags
}

_blaadpaperscli "$@"
