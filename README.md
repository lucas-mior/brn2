# bulk rename files
 
brn2 is a fork of brn,
a command line tool similar to [vimv](https://github.com/thameera/vimv/).
It can be used to easily mass-rename files in your preferred text editor (i.e.
vim).
 
## Usage
```
brn2 --help
usage: brn2 [-h | --help | <filename> | file1 file2 ... ] 
No arguments        : rename filenames in current working directory. 
Single argument     : rename filenames listed in this argument. 
2 or more arguments : rename filenames passed as arguments. 
-h | --help         : display this help message. 
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
  reading from `<files.txt>`.
- Newlines in filenames are not allowed.
- Filenames listed in `<files.txt>` or given as arguments which
  do not correpond to existing files will generate errors while renaming
- Filenames `.` and `..` are ignored, as well as any filename
  which starts with `..`, even when listen in `<files.txt>`
- Filenames are normalized (`./` are removed, consecutive slashes are
  bundled, and trailing slashes are removed)
- Repeated filenames in <files.txt> are removed.
 
## Install
 
Clone the repo and
```
sudo make install
```

## Examples
- Find and rename jpg files (recursively, using `find`):
```
$ find . -iname "*.jpg" > rename
$ brn2 rename
```
- Rename jpg files in current working directory:
```
$ brn2 *.jpg
```
- Rename all files in current working directory:
```
$ brn2
```
 
## Why use brn2 over something like vimv?
 
* It can handle swapping names. It uses GNU/Linux's `renameat2` system call to
  atomically swap the names of two files which means no temporary files are made
  either (only on GNU/Linux; on other operating systems, swapping files will
  safely fail without renaming).
 
* It is written in C instead of bash which makes it faster, more robust and
  predictable.
 
* It has error handling, it will safely abort if the exact number of
  filenames isn't provided or if some filenames are repeated.

## Changes over original brn
- Option to rename files listed in file given as first argument.
- Option to rename files passed as arguments.
- Print renamed files.
- Faster algorithm to check for duplicated filenames.
 
## License
brn2 is licensed under the GNU AFFERO GENERAL PUBLIC LICENSE.
