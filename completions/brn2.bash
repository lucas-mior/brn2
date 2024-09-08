_brn2_completion() {
  local cur commands
  COMPREPLY=()
  cur="${COMP_WORDS[COMP_CWORD]}"
  commands=(
    "-p --print"
    "-i --info"
    "-c --copy"
    "-r --remove"
    "-s --save"
    "-d --daemon"
    "-h --help"
  )

  COMPREPLY=($(compgen -W "${commands[*]}" -- "${cur}"))
}

complete -F _brn2_completion brn2
