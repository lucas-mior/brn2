# brn2
brn2 is a fork of [brn](https://github.com/nimaipatel/brn), a command line tool
used to easily mass-rename files in your preferred text editor.
 
## Usage
```
$ brn2 --help
usage: brn2 [OPTIONS] -- [<filename> | <file1> <file2> ...]
Rename filenames based on provided arguments.

Options:
  -h, --help    : Display this help message and exit.
  -q, --quiet   : Quiet mode; suppress output messages.
  -v, --verbose : Verbose mode (default); output messages.

Arguments:
  No arguments        : Rename filenames in the current working directory.
  Single argument     : Rename filenames listed in this argument.
  2 or more arguments : Rename filenames passed as arguments.
```

brn2 will open the filenames in your text editor of choice.
You can then edit the filenames in the buffer and the changes
will take place when you save and exit.
- No arguments: filenames in current working directory
- Single argument: filenames listed in this argument
- 2 or more arguments: filenames passed as arguments

### Notes
- By default it uses `$EDITOR` and if that is not set, it defaults to `vim`.
- It will not work for more than 2^32 renames at once
- It will not work for filenames longer than PATH_MAX (typically 4096) characters when
  reading from `<filename>`.
- Newlines in filenames are not allowed.
- Filenames listed in `<filename>` or given as arguments which
  do not correpond to existing files will generate errors while renaming
- Filenames `.` and `..` are ignored, as well as any filename
  which starts with `..`, even when listen in `<filename>`
- Filenames are normalized
    * `./` at beginning of filenames are removed
    * consecutive slashes (`/`) are bundled together
    * trailing slashes (`/`) are removed
- Repeated filenames in `<filename>` or repeated arguments are removed.
- It can handle swapping names. It uses GNU/Linux's `renameat2` system call to
  atomically swap the names of two files which means no temporary files are made
  either (only on GNU/Linux; on other operating systems, swapping files will
  safely fail without renaming).
- If the exact number of filenames isn't provided or if some filenames are
  repeated, the user will be asked to fix the rename buffer or exit.
 
## Install
 
### AUR
```
$ yay -S brn2
```

### Manual
```
$ git clone https://github.com/lucas-mior/brn2
$ cd brn2
$ make
$ sudo make install
```

## Examples
- Find and rename jpg files in quiet mode (recursively, using `find`):
```
$ find . -iname "*.jpg" > rename
$ brn2 --quiet rename
```
- Rename jpg files in current working directory:
```
$ brn2 *.jpg
```
- Rename all files in current working directory:
```
$ brn2
```
 
## Changes over original brn
- Option to rename files listed in file given as first argument.
- Option to rename files passed as arguments.
- Print renamed files.
- Faster algorithm to check for duplicated filenames.
 
## License
brn2 is licensed under the GNU AFFERO GENERAL PUBLIC LICENSE.
