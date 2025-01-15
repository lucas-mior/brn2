# brn2
brn2 is a fork of [brn](https://github.com/nimaipatel/brn), a command line tool
used to easily mass-rename files in your preferred text editor.
 
## Usage
```
$ brn2 --help
usage: brn2 [OPTIONS] -- <file1> <file2> ...
usage: brn2 [OPTIONS] -f <filename>
usage: brn2 [OPTIONS] -d <dir>
usage: brn2 [OPTIONS] -r <dir>
Rename filenames based on provided arguments.

Options:
  -c, --check   : Check if original file names exist.
  -e, --explict : Only rename files given in the list of files to rename (default).
  -F, --fatal   : Exit on first renaming error.
  -h, --help    : Display this help message and exit.
  -i, --implict : Rename files not given in the list of files to rename.
  -q, --quiet   : Quiet mode; suppress output messages.
  -s, --sort    : Disable sorting of original list.
  -v, --verbose : Verbose mode (default); output messages.

Arguments:
  No arguments              : Rename filenames in the current working directory.
  1 or more arguments       : Rename filenames passed as arguments.
  -d <dir>, --dir=<dir>     : Rename files in directory.
  -f <file>, --file=<file>  : Rename filenames listed in this argument.
  -r <dir>, --recurse=<dir> : Recursively find files to rename.
```

brn2 will open the filenames in your text editor of choice.
You can then edit the filenames in the buffer and the changes
will take place when you save and exit.
- No arguments: filenames in current working directory
- Argument supplied to `-f`: filenames listed in this argument
- Argument supplied to `-d`: filenames of this dir
- Argument supplied to `-r`: filenames recursively found on this dir
- 2 or more arguments: filenames passed as arguments

### Notes
- By default it uses `$EDITOR` and if that is not set, it defaults to `vim`.
- It will not work for more than 2^32 renames at once.
- It will not work for filenames longer than PATH_MAX (typically 4096) bytes.
- Newlines in filenames are not allowed.
- Filenames listed in `<filename>` or given as arguments which
  do not correpond to existing files will generate errors while renaming.
- Filenames that contain only dots and slashes are ignored.
- Filenames are normalized:
    * `./` at beginning of filenames are removed.
    * consecutive slashes (`/`) are bundled together.
    * if possible, trailing slash is added to denote directories
- Repeated filenames in `<filename>` or repeated arguments are removed.
- It can handle swapping names. It uses GNU/Linux's `renameat2` system call to
  atomically swap the names of two files which means no temporary files are made
  either (only on GNU/Linux; on other operating systems, swapping files will
  safely fail without renaming).
  * By default, files which were not specified in the rename list will be
    unaffected. If you try to rename `a` to `b`, but `b` was not specified in
    the rename list, this rename won't happen. Supply the `--implict` command
    line option to force this swap, then `a` will become `b` and `b` will become
    `a`.
- If the exact number of filenames isn't provided or if some filenames are
  repeated, the user will be asked to fix the rename buffer or exit.
- Renaming millions or billions of files can be slow. Disabling sorting
  (`-s` option) and printing (`-q` option) might help a bit.
- If you want to filter/organize the files to rename, use command line utilities
  like `find` and output it to a file. Edit this file as you like and then
  launch brn2 with the `-f` option. See examples below.

#### Be careful when renaming in depth
If you supply the files:
```
dir/
dir/file
```
and proceed to rename `dir/`, `dir/file` will always fail because it no longer
exists.
There is no good default way to deal with this, because you can either:
- update names recursively while renaming
- skip outdated names

Both alternatives are cumbersome and error prone to implement.  So the best
thing to do is to launch brn2 multiple times and operate in steps. Try renaming
the regular files first, and then directories, starting from the most deep.
 
## Install
 
### AUR
```
$ yay -S brn2-git
```

### Manual
```
$ git clone https://github.com/lucas-mior/brn2
$ cd brn2
$ ./build.sh
$ sudo ./build.sh install
```

## Examples
- Rename all files in current working directory:
```
$ brn2
```
- Rename all files in current working directory without sorting:
```
$ brn2 -s
```
- Rename jpg files in current working directory:
```
$ brn2 *.jpg
```
- Rename files `a`, `b`, and `c`,
  allowing for swapping with other files which already exist.
```
$ brn2 -i a b c
```
- Find and rename files (recursively):
```
$ brn2 -r .
```
- Find and rename jpg files in quiet mode (recursively, using `find`):
```
$ find . -iname "*.jpg" > rename
$ brn2 --quiet --file rename
```
- Find and rename only regular files recursively while sorting them by
  modification date (using `find(1)`, `sort(1)` and `cut(1)`). Also check if
  each line correspond to an existing file (`--check` option).
```
$ find . -type f -printf "%T@ %p\n" | sort -n | cut -d ' ' -f 2- > rename
$ brn2 --check --file rename
```
 
## Changes over original brn
- Option to rename files listed in file or passed as arguments.
  * By default, won't swap files which were not specified in the rename list.
- Print renamed files.
- Faster algorithm to check for duplicated filenames.
- Option to check if original files exist.
 
## License
brn2 is licensed under the GNU AFFERO GENERAL PUBLIC LICENSE.
