# fish completion for brn2

complete -c brn2 -s h -l help -d 'Display help and exit'
complete -c brn2 -s v -l verbose -d 'Output messages'
complete -c brn2 -s q -l quiet -d 'Suppress output messages'
complete -c brn2 -s i -l implicit -d 'Allow renaming files not given in the file list'
complete -c brn2 -s e -l explicit -d 'Only rename files given in the file list'
complete -c brn2 -s F -l fatal -d 'Exit on first renaming error'
complete -c brn2 -s a -l autosolve -d 'Auto solve name conflicts for equal files'
complete -c brn2 -s s -l sort -d 'Disable sorting of the original list'
complete -c brn2 -s V -l vim-split -d 'Use vim in vertical split mode'
complete -c brn2 -s d -l dir -r -a '(__fish_complete_directories)' -d 'Rename files in directory'
complete -c brn2 -s f -l file -r -F -a '- /dev/stdin' -d 'Read filenames from file'
complete -c brn2 -s t -l file-test -r -F -a '- /dev/stdin' -d 'Read replacement filenames from file for tests'
complete -c brn2 -F
