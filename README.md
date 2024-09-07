# liblmf

LMF is a file format for Interleafed and Multimedia files that can be encrypted. LMF contains headers that can specify which LMF type a file is, and  can execute them accordingly. LMF files can be scripts, interleaf files, binaries, configuration files, argument lists and so on.

## Installation

Clone the repository with `git clone https://github.com/catriverr/liblmf.git`. Then add the `liblmf/bin` folder to your `$PATH` environment variable. You can achieve this with the `setx` command on Windows in an Administrator Command Prompt, and with the `export PATH=~:$PWD/liblmf/bin:\$PATH >> ~/.bashrc` command in UNIX based systems. If you get an error when running the `lmf` command, build the executable first with `make` while in the `liblmf` directory.

## Usage

Running `lmf` on the commandline will print information on how to use the cli.

## Example File

```make
pedantic=false;type=6;moveindex=0
#$! this is a comment, and the above line is a header. Headers are the first line of the LMF file, and should follow the same syntax as the one shown above.

#FILE hello.js
console.log('Hello There! This is an example file stored in this LMF file.');
#END_FILE

#DIRECTORY text_files
#$! currently directories can not have spaces in their names

#FILE hello.txt
Hello! this file is stored in: test.lmf/text_files/hello.txt!
#END_FILE

#FILE passwords.txt
Google Password: completelydefinitelyvalidpassword123
iCloud Password: i_am_bored_as_hell_man
#END_FILE

#END_DIRECTORY
```
`(test.lmf)`

running `lmf test.lmf` will list the directories and files within the file, you may also use `lmf test.lmf [file_inside_lmf]` to get the content of a single file. (For example `lmf test.lmf text_files/passwords.txt`)
