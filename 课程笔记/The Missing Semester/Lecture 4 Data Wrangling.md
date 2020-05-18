## Regular Expression

regexone [tutorial](https://regexone.com/)

- `.` means “any single character” except newline
- `*` zero or more of the preceding match
- `+` one or more of the preceding match
- `[abc]` any one character of `a`, `b`, and `c`
- `(RX1|RX2)` either something that matches `RX1` or `RX2`
- `^` the start of the line
- `$` the end of the line

`[^ ]+`; any non-empty sequence of non-space characters

`*` and `+` are, by default, “greedy”. They will match as much text as they can.

In some regular expression implementations, you can just suffix `*` or `+` with a `?` to make them non-greedy, but sadly `sed` doesn’t support that.

## Wrangling binary data

```shell
$ ffmpeg -loglevel panic -i /dev/video0 -frames 1 -f image2 -
 | convert - -colorspace gray -
 | gzip
 | ssh mymachine 'gzip -d | tee copy.jpg | env DISPLAY=:0 feh -'
```

## Analyzing data

`st` 

 [`R`](https://ggplot2.tidyverse.org/) 

`gnuplot` simple plot

```shell
$ echo "2*($(data | paste -sd+))" | bc -l
$ ssh myserver journalctl
 | grep sshd
 | grep "Disconnected from"
 | sed -E 's/.*Disconnected from (invalid |authenticating )?user (.*) [^ ]+ port [0-9]+( \[preauth\])?$/\2/'
 | sort | uniq -c
 | awk '{print $1}' | R --slave -e 'x <- scan(file="stdin", quiet=TRUE); summary(x)'
 
 $ ssh myserver journalctl
 | grep sshd
 | grep "Disconnected from"
 | sed -E 's/.*Disconnected from (invalid |authenticating )?user (.*) [^ ]+ port [0-9]+( \[preauth\])?$/\2/'
 | sort | uniq -c
 | sort -nk1,1 | tail -n10
 | gnuplot -p -e 'set boxwidth 0.5; plot "-" using 1:xtic(2) with boxes'
```

## Some Commands

1. `sed`

`sed`’s regular expressions are somewhat weird, and will require you to put a `\` before most of these to give them their special meaning. Or you can pass `-E`.

`sed` can also do other handy things like print lines following a given match, do multiple substitutions per invocation, search for things, etc.

```shell
$ ssh myserver journalctl
 | grep sshd
 | grep "Disconnected from"
 | sed 's/.*Disconnected from //'
```

`s/REGEX/SUBSTITUTION/`

2. `awk`

Inside the block, `$0` is set to the entire line’s contents, and `$1` through `$n` are set to the `n`th *field* of that line, when separated by the `awk` field separator (whitespace by default, change with `-F`).

`awk` is a programming language.

3. `uniq`
4. `tail`
5. `sort`
6. `paste`
7. [`pup`](https://github.com/EricChiang/pup) Parsing HTML at the command line
8. [`jq`](https://stedolan.github.io/jq/)

## Some Examples

```shell
$ ssh myserver journalctl
 | grep sshd
 | grep "Disconnected from"
 | sed -E 's/.*Disconnected from (invalid |authenticating )?user (.*) [^ ]+ port [0-9]+( \[preauth\])?$/\2/'
 | sort | uniq -c
 | sort -nk1,1 | tail -n10
 
 $ ssh myserver journalctl
 | grep sshd
 | grep "Disconnected from"
 | sed -E 's/.*Disconnected from (invalid |authenticating )?user (.*) [^ ]+ port [0-9]+( \[preauth\])?$/\2/'
 | sort | uniq -c
 | sort -nk1,1 | tail -n10
 | awk '{print $2}' | paste -sd,
 
# | awk '$1 == 1 && $2 ~ /^c[^ ]*e$/ { print $2 }' | wc -l

```

