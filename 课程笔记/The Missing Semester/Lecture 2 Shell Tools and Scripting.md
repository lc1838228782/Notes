## Create Shell Function

`mcd.sh`

```shell
mcd () {
    mkdir -p "$1"
    cd "$1"
}
```

After `source mcd.sh`, we can use mcd func.

`mcd test` == `mkdir -p test && cd test`

## Special Variable

- `$0` - Name of the script
- `$1` to `$9` - Arguments to the script. `$1` is the first argument and so on.
- `$@` - All the arguments
- `$#` - Number of arguments
- `$?` - Return code of the previous command
- `$$` - Process Identification number for the current script
- `!!` - Entire last command, including arguments. A common pattern is to execute a command only for it to fail due to missing permissions, then you can quickly execute it with sudo by doing `sudo !!`
- `$_` - Last argument from the last command. If you are in an interactive shell, you can also quickly get this value by typing `Esc` followed by `.`

## Exit Code and Conditionally Execute

A value of 0 usually means everything went OK, anything different from 0 means an error occurred.

The `true` program will always have a 0 return code and the `false` command will always have a 1 return code.

```shell
false || echo "Oops, fail"
# Oops, fail

true || echo "Will not be printed"
#

true && echo "Things went well"
# Things went well

false && echo "Will not be printed"
#

false ; echo "This will always run"
# This will always run
```

## $()   <()

Whenever you place `$( CMD )` it will execute `CMD`, get the output of the command and substitute it in place.

A lesser known similar feature is *process substitution*, `<( CMD )` will execute `CMD` and place the output in a temporary file and substitute the `<()` with that file’s name. This is useful when commands expect values to be passed by file instead of by STDIN. For example, `diff <(ls foo) <(ls bar)` will show differences between files in dirs `foo` and `bar`.

```shell
#!/bin/bash

echo "Starting program at $(date)" # Date will be substituted

echo "Running program $0 with $# arguments with pid $$"

for file in $@; do
    grep foobar $file > /dev/null 2> /dev/null
    # When pattern is not found, grep has exit status 1
    # We redirect STDOUT and STDERR to a null register since we do not care about them
    if [[ $? -ne 0 ]]; then
        echo "File $file does not have any foobar, adding one"
        echo "# foobar" >> "$file"
    fi
done
```

When performing comparisons in bash try to use double brackets `[[ ]]` in favor of simple brackets `[ ]`. Details [here](http://mywiki.wooledge.org/BashFAQ/031).

## wildcard

```shell
$ convert image.{png,jpg}
# Will expand to
$ convert image.png image.jpg

$ cp /path/to/project/{foo,bar,baz}.sh /newpath
# Will expand to
$ cp /path/to/project/foo.sh /path/to/project/bar.sh /path/to/project/baz.sh /newpath

# Globbing techniques can also be combined
$ mv *{.py,.sh} folder
# Will move all *.py and *.sh files


$ mkdir foo bar
# This creates files foo/a, foo/b, ... foo/h, bar/a, bar/b, ... bar/h
$ touch {foo,bar}/{a..h}
$ touch foo/x bar/y
# Show differences between files in foo and bar
$ diff <(ls foo) <(ls bar)
# Outputs
# < x
# ---
# > y
```

## Shebang

```python
#!/usr/local/bin/python
import sys
for arg in reversed(sys.argv[1:]):
    print(arg)
```

or `#!/usr/bin/env python`

## Finding files

- `find`

  ```shell
  # Find all directories named src
  $ find . -name src -type d
  # Find all python files that have a folder named test in their path
  $ find . -path '**/test/**/*.py' -type f
  # Find all files modified in the last day
  $ find . -mtime -1
  # Find all zip files with size in range 500k to 10M
  $ find . -size +500k -size -10M -name '*.tar.gz'
  
  # Delete all files with .tmp extension
  $ find . -name '*.tmp' -exec rm {} \;
  # Find all PNG files and convert them to JPG
  $ find . -name '*.png' -exec convert {} {.}.jpg \;
  ```

- `fd`

## Finding code

- `grep`

  `-C ` `grep -C 5` will print 5 lines before and after the match. 

  `-v` print all lines that do **not** match the pattern.

  `-R` since it will **R**ecursively go into directories and look for text files for the matching string.

- alternatives `ack, ag, rg`

  `rg` example

  ```shell
  # Find all python files where I used the requests library
  $ rg -t py 'import requests'
  # Find all files (including hidden files) without a shebang line
  $ rg -u --files-without-match "^#!"
  # Find all matches of foo and print the following 5 lines
  $ rg foo -A 5
  # Print statistics of matches (# of matched lines and files )
  $ rg --stats PATTERN
  ```

## Finding commands

`history`

`Ctrl + R` backwards search through history `fzf` enhance preview



**history-based autosuggestions** can be enabled in [zsh](https://github.com/zsh-users/zsh-autosuggestions).



 **if you start a command with a leading space it won’t be added to your shell history.**

## Directory Navigation

`ls -R`

`tree`

[`fasd`](https://github.com/clvv/fasd)

[`broot`](https://github.com/Canop/broot)

[`nnn`](https://github.com/jarun/nnn)

[`ranger`](https://github.com/ranger/ranger)

## Some Commands

`xargs`

Read standard input, then generate command argument(s).

Default delimiter is space. Default command is echo, means `xargs` = `xargs echo`

`-d`: delimiter character

`-p` `--interactive`: Prompt the user about whether to run each command line

`-t` `--verbose`:  Print the command line on the standard error output before executing it.

`-0`: `find Pictures/tecmint/ -name "*.png" -type f -print0 | xargs -0 tar -cvzf images.tar.gz`
`-L`: Use at most max-lines nonblank input lines per command line.

`-n`: Use at most max-args arguments per command line.

`-I`: Replace occurrences of replace-str in the initial-arguments with names read from standard input.

```shell
$ cat foo.txt
one
two
three

$ cat foo.txt | xargs -I file sh -c 'echo file; mkdir file'
one 
two
three

$ ls 
one two three
```

`-P` `--max-procs`: `sudo docker -q | sudo xargs -n 1 -P 0 docker kill`

## Useful Tools

[`shellcheck`](https://www.shellcheck.net/)

[`tldr`](https://tldr.sh/) Finding command usage example

[`fd`](https://github.com/sharkdp/fd) Finding files

[`rg`](https://github.com/BurntSushi/ripgrep) ripgrep

