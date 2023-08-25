# bulk rename files
 
brn2 is a fork of brn,
a command line tool similar to [vimv](https://github.com/thameera/vimv/).
It can be used to easily mass-rename files in your preferred text editor (i.e.
vim).
 
## Usage
```
brn2                   # rename files in current working directory
brn2 <files.txt>       # rename files listed in <files.txt>
brn2 [file1 file2 ...] # rename files passed as arguments
```
brn2 will open the filenames in your text editor of choice.
You can then edit the filenames in the buffer and the changes
will take place when you save and exit.
- No arguments: filenames in current working directory
- Single argument: filenames listed in this argument
- 2 or more arguments: filenames passed as arguments
 
By default it uses `$EDITOR` and if that is not set, it defaults to `vim`.

Note that the filenames `.` and `..` are ignored,
as well as any filename which starts with `..`, even when listen in
`<files.txt>`

### Notes
- It will not work for filenames longer than PATHMAX characters
- Newlines in filenames are also not allowed.
- If a filename listed in `<files.txt>` or passed as argument does not correspond to an existing file,
  it will be ignored.
 
## Install
 
Clone the repo and
```
sudo make install
```

## Examples
- Find and rename jpg files (recursively, using `find`):
```
$ find . -iname "*.jpg" | brn2 -
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
  either (yes, this also means you can't compile it on other Unixes).
 
* It is written in C instead of bash which makes it faster, more robust and
  predictable.
 
* It has error handling, it will safely abort if the exact number of
  filenames isn't provided or if some filenames are repeated.
 
## License
brn2 is licensed under the GNU AFFERO GENERAL PUBLIC LICENSE.

 
## Changes over original brn
- Option to rename files listed in file given as first argument.
- Print renamed files.
- Faster algorithm to check for duplicated filenames.
