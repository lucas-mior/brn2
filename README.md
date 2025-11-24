# brn2
brn2 is a fork of [brn](https://github.com/nimaipatel/brn), a command line tool
used to easily mass-rename files in your preferred text editor.
 
## Usage
```
$ brn2 --help
usage: brn2 [OPTIONS] -- <file1> <file2> ...
usage: brn2 [OPTIONS] -f <filename>
usage: brn2 [OPTIONS] -d <dir>
Rename filenames based on provided arguments.

Options:
  -F, --fatal     : Exit on first renaming error.
  -a, --autosolve : Auto solve name conflicts for equal files.
  -e, --explict   : Only rename files given in the list (default).
  -h, --help      : Display this help message and exit.
  -i, --implict   : Rename files not given in the list of files to rename.
  -q, --quiet     : Quiet mode; suppress output messages.
  -s, --sort      : Disable sorting of original list.
  -v, --verbose   : Verbose mode (default); output messages.

Arguments:
  No arguments             : Rename files of current working directory.
  1 or more arguments      : Rename filenames passed as arguments.
  -d <dir>, --dir=<dir>    : Rename files in directory.
  -f <file>, --file=<file> : Rename filenames listed in this argument.
```

brn2 will open the filenames in your text editor of choice.
You can then edit the filenames in the buffer and the changes
will take place when you save and exit.
- No arguments: filenames in current working directory
- Argument supplied to `-f`: filenames listed in this argument
  * Use - or /dev/stdin to read standard input
- Argument supplied to `-d`: filenames of this dir
- 2 or more arguments: filenames passed as arguments

### Notes
- By default it uses `$EDITOR` and if that is not set, it defaults to `vim`.
- It will not work for more than 2^32 renames at once.
- It will not work for filenames longer than 4096 bytes.
- Newlines in filenames are not allowed.
- Filenames that contain only dots and slashes are ignored.
- Filenames are normalized:
    * `./` at beginning of filenames are removed.
    * consecutive slashes (`/`) are bundled together.
    * trailing slash is added to denote directories
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
  * New file names that match existing files will be compared agains the
    current name. If they have the same content, it will be reported.
    + If the option `--autosolve` is suplied, then the existing file will be
      removed, making the rename work correctly. Otherwise, the user will be
      prompted to fix the rename list.
- If the exact number of filenames isn't provided or if some filenames are
  repeated, the user will be asked to fix the rename buffer or exit.
- Renaming millions or billions of files can be slow. Disabling sorting
  (`-s` option) and printing (`-q` option) might help a bit, but not much,
  because the bottleneck is the filesystem.
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
- Find and rename files:
```
$ find . | brn2 -f -
```
- Find files, edit the rename buffer manually and then rename jpg files:
```
$ find . -iname "*.jpg" > rename
$ vim rename
$ brn2 --file rename
```
- Find and rename only regular files recursively while sorting them by
  modification date (using
  [`find(1)`](https://man7.org/linux/man-pages/man1/find.1.html),
  [`sort(1)`](https://man7.org/linux/man-pages/man1/sort.1.html), and
  [`cut(1)`](https://man7.org/linux/man-pages/man1/cut.1.html)).
```
$ find . -type f -printf "%T@ %p\n" | sort -n | cut -d ' ' -f 2- | brn2 -s -f -
```
 
## Changes over original brn
- Option to rename files listed in file or passed as arguments.
  * By default, won't swap files which were not specified in the rename list.
- Option to also solve rename conflicts for equal files.
- Print renamed files.
- Faster algorithm to check for duplicated filenames.
- Check if original files exist.
 
## License
brn2 is licensed under the GNU AFFERO GENERAL PUBLIC LICENSE.
