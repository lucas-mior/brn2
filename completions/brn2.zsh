#compdef brn2

_arguments \
  '-h[Display this help message and exit.]'\
  '--help[Display this help message and exit.]'\
  '-q[Quiet mode; suppress output messages.]'\
  '--quiet[Quiet mode; suppress output messages.]'\
  '-v[Verbose mode (default); output messages.]'\
  '--verbose[Verbose mode (default); output messages.]'\
  '-c[Check if original file names exist.]'\
  '--check[Check if original file names exist.]'\
  '-s[Disable sorting of original list.]'\
  '--sort[Disable sorting of original list.]'\
  '-F[Exit on first renaming error.]'\
  '--fatal[Exit on first renaming error.]'\
  '-i[Rename files not given in the list of files to rename.]'\
  '--implict[Rename files not given in the list of files to rename.]'\
  '-e[Only rename files given in the list of files to rename (default).]'\
  '--explict[Only rename files given in the list of files to rename (default).]'\
  '-f[Rename files listed in]:filename:_files'\
  '--filename[Rename files listed in]:filename:_files'\
  '*:remaining argument:_files'
