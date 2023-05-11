# bulk rename files
 
brn2 is a fork of brn,
a command line tool similar to [vimv](https://github.com/thameera/vimv/).
It can be used to easily mass-rename files in your preferred text editor (i.e.
vim).
 
## Usage
```
brn2 [files.txt]
```
Without arguments, it opens a buffer in your default text editor with the list
of filenames in the current directory.  If given 1 argument, it is interpreted
as the text filename with a list of filenames to rename.  You can then edit the
filenames in the buffer and the changes will take place when you save and exit.
 
By default it uses `$EDITOR` and if that is not set then `$VISUAL`.

### Notes
- It will not work for filenames longer than PATHMAX characters
- Newlines in filenames are also not allowed.
 
## Install
 
Clone the repo and
```
sudo make install
```
 
## Why use brn2 over something like vimv?
 
* It can handle swapping names. It uses GNU/Linux's `renameat2` system call to
  atomically swap the names of two files which means no temporary files are made
  either (yes, this also means you can't compile it on other Unixes).
 
* It is written in C instead of bash which makes its behavior more robust and
  predictable.
 
* It has error handling, it will safely abort if the exact number of
  filenames isn't provided or if some filenames are repeated.
 
## License
brn2 is licensed under the GNU AFFERO GENERAL PUBLIC LICENSE.
 
## Changes over original brn
- Option to rename files listed in file given as first argument.
- Print renamed files.
- Faster algorithm to check for duplicated filenames.
