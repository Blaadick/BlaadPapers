#compdef blaadpaperscli

_blaadpaperscli_subcommands() {
    local -a subs
    local line name desc

    while IFS= read -r line; do
        [[ -n $line ]] || continue
        name=${line%%$'\t'*}
        desc=
        [[ $line == *$'\t'* ]] && desc=${line#*$'\t'}
        subs+=("${name//:/\\:}:${desc}")
    done <<< "$(blaadpaperscli help --json 2>/dev/null | jq -r '.options[] | "\(.name)\t\(.description)"' 2>/dev/null)"

    (( $#subs )) && _describe -t commands 'subcommand' subs
}

_blaadpaperscli_flags() {
    local cmd=$words[2] line name desc
    local -a flags used
    used=(${words[1,CURRENT-1]})

    [[ -n $cmd ]] || return 1

    while IFS= read -r line; do
        [[ -n $line ]] || continue
        name=${line%%$'\t'*}
        desc=
        [[ $line == *$'\t'* ]] && desc=${line#*$'\t'}
        (( ${used[(Ie)$name]} )) && continue
        flags+=("${name//:/\\:}:${desc}")
    done <<< "$(blaadpaperscli help --json 2>/dev/null | jq -r --arg cmd "$cmd" '.options[] | select(.name == $cmd) | .flags[] | "--\(.name)\t\(.description)"' 2>/dev/null)"

    (( $#flags )) && _describe -t flags 'flag' flags
}

_blaadpaperscli_wallpaper_ids() {
    local -a ids
    local line name desc

    while IFS= read -r line; do
        [[ -n $line ]] || continue
        name=${line%%$'\t'*}
        desc=
        [[ $line == *$'\t'* ]] && desc=${line#*$'\t'}
        ids+=("${name//:/\\:}:${desc}")
    done <<< "$(blaadpaperscli list --json 2>/dev/null | jq -r '.[] | "\(.id)\t\(.name)"' 2>/dev/null)"

    (( $#ids )) && _describe -t ids 'wallpaper id' ids
}

_blaadpaperscli_install_paths() {
    local -a formats
    formats=(${(f)"$(blaadpaperscli help --json 2>/dev/null | jq -r '.supported_formats[] | sub("^\\.+"; "")' 2>/dev/null)"})

    if (( $#formats == 0 )); then
        _files
    else
        _files -g "*.(#i)(${(j:|:)${(@b)formats}})"
    fi
}

_blaadpaperscli() {
    local ret=1

    if (( CURRENT == 2 )); then
        _blaadpaperscli_subcommands
        return
    fi

    _blaadpaperscli_flags && ret=0

    case $words[2] in
        apply|info|remove) (( CURRENT == 3 )) && _blaadpaperscli_wallpaper_ids && ret=0 ;;
        install) _blaadpaperscli_install_paths && ret=0 ;;
    esac

    return ret
}

_blaadpaperscli "$@"
