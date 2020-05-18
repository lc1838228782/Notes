## Debugging

### Printf debugging and logging

Programs like `ls` or `grep` are using [ANSI escape codes](https://en.wikipedia.org/wiki/ANSI_escape_code), which are special sequences of characters to indicate your shell to change the color of the output. For example, executing `echo -e "\e[38;2;255;0;0mThis is red\e[0m"` prints the message `This is red` in red on your terminal. The following script shows how to print many RGB colors into your terminal.

```bash
#!/usr/bin/env bash
for R in $(seq 0 20 255); do
    for G in $(seq 0 20 255); do
        for B in $(seq 0 20 255); do
            printf "\e[38;2;${R};${G};${B}m█\e[0m";
        done
    done
done
```

### Third party logs

UNIX systems, `/var/log/`

`logger`

### Debuggers

Python: `pdb`, `ipdb`

Low level lang: `gdb`, `pwndbg`

### Specialized Tools

trace system calls

Linux `strace`  macOS and BSD `dtrace` `dtruss`

`tcpdump` wireshark

### Static Analysis

pyflakes shellcheck

[static-analysis](https://github.com/analysis-tools-dev/static-analysis) [awesome-lint](https://github.com/caramelomartins/awesome-linters)

A complementary tool to stylistic linting are code formatters such as [`black`](https://github.com/psf/black) for Python, `gofmt` for Go, `rustfmt` for Rust or [`prettier`](https://prettier.io/) for JavaScript, HTML and CSS. 

## Profiling

### Timing

Real User Sys time.

### Profilers

**CPU**

Python cProfile [line profiler](https://github.com/rkern/line_profiler)

**Memory**

C/C++ Valgrind

Python memory-profiler

**Event Profiling**

`perf` can easily report poor cache locality, high amounts of page faults or livelocks.

**Visualization**

Flame Graph

pycallgraph

### Resource Monitoring

**General Monitoring** top [htop](https://hisham.hm/htop/index.php) [glances](https://nicolargo.github.io/glances/) [dstat](http://dag.wiee.rs/home-made/dstat/)

**I/O operations** [iotop](http://man7.org/linux/man-pages/man8/iotop.8.html)

**Disk Usage** [df](http://man7.org/linux/man-pages/man1/df.1.html) [du](http://man7.org/linux/man-pages/man1/du.1.html) [ncdu](https://dev.yorhel.nl/ncdu)

**Memory Usage** `free`

**Open files** lsof

**Network Connections and Config** ss ip

**Network Usage** nethogs iftop

`stress`

### Specialized tools

[hyperfine](https://github.com/sharkdp/hyperfine)

