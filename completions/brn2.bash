# bash completion for brn2

_brn2()
{
    local cur prev

    COMPREPLY=()
    cur=${COMP_WORDS[COMP_CWORD]}
    prev=${COMP_WORDS[COMP_CWORD-1]}

    _brn2_compgen()
    {
        local candidate

        COMPREPLY=()
        while IFS= read -r candidate; do
            COMPREPLY+=("$candidate")
        done < <(compgen "$@")
    }

    case "$cur" in
    --dir=*)
        local dir_arg=${cur#--dir=}
        local i
        _brn2_compgen -d -- "$dir_arg"
        for i in "${!COMPREPLY[@]}"; do
            COMPREPLY[$i]=--dir=${COMPREPLY[$i]}
        done
        compopt -o filenames 2>/dev/null
        return
        ;;
    --file=*|--file-test=*)
        local opt=${cur%%=*}
        local file_arg=${cur#*=}
        local i
        _brn2_compgen -f -- "$file_arg"
        for i in "${!COMPREPLY[@]}"; do
            COMPREPLY[$i]=$opt=${COMPREPLY[$i]}
        done
        compopt -o filenames 2>/dev/null
        return
        ;;
    esac

    case "$prev" in
    -d|--dir)
        _brn2_compgen -d -- "$cur"
        compopt -o filenames 2>/dev/null
        return
        ;;
    -f|--file|-t|--file-test)
        if [[ "$cur" == -* ]]; then
            _brn2_compgen -W '- /dev/stdin' -- "$cur"
        else
            _brn2_compgen -f -- "$cur"
        fi
        compopt -o filenames 2>/dev/null
        return
        ;;
    esac

    if [[ "$cur" == -* ]]; then
        _brn2_compgen -W '-h --help -v --verbose -q --quiet -i --implicit -e --explicit -F --fatal -a --autosolve -s --sort -V --vim-split -d --dir -f --file -t --file-test --' -- "$cur"
        return
    fi

    _brn2_compgen -f -- "$cur"
    compopt -o filenames 2>/dev/null
}

complete -F _brn2 brn2
