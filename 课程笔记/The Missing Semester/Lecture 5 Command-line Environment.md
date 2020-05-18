## Job Control

### Killing a process

Ctrl-C deliver a `SIGINT` signal to the process.

Ctrl-\ `SIGQUIT`

While `SIGINT` and `SIGQUIT` are both usually associated with terminal related requests, a more generic signal for asking a process to exit gracefully is the `SIGTERM` signal. To send this signal we can use the [`kill`](http://man7.org/linux/man-pages/man1/kill.1.html) command, with the syntax `kill -TERM `.

### Pausing and backgrounding processes

`SIGSTOP` pauses a process. In the terminal, typing `Ctrl-Z` will prompt the shell to send a `SIGTSTP` signal, short for Terminal Stop (i.e. the terminal’s version of `SIGSTOP`).

The [`jobs`](http://man7.org/linux/man-pages/man1/jobs.1p.html) command lists the unfinished jobs associated with the current terminal session.

We can then continue the paused job in the foreground or in the background using [`fg`](http://man7.org/linux/man-pages/man1/fg.1p.html) or [`bg`](http://man7.org/linux/man-pages/man1/bg.1p.html), respectively.

To refer to the last backgrounded job you can use the `$!` special parameter.

`&` suffix in a command will run the command in the background, giving you the prompt back, although it will still use the shell’s STDOUT which can be annoying (use shell redirections in that case).

To background an already running program you can do `Ctrl-Z` followed by `bg`. Note that backgrounded processes are still children processes of your terminal and will die if you close the terminal (this will send yet another signal, `SIGHUP`). To prevent that from happening you can run the program with [`nohup`](http://man7.org/linux/man-pages/man1/nohup.1.html) (a wrapper to ignore `SIGHUP`), or use `disown` if the process has already been started. Alternatively, you can use a terminal multiplexer as we will see in the next section.

Example

```shell
$ sleep 1000
^Z
[1]  + 18653 suspended  sleep 1000

$ nohup sleep 2000 &
[2] 18745
appending output to nohup.out

$ jobs
[1]  + suspended  sleep 1000
[2]  - running    nohup sleep 2000

$ bg %1
[1]  - 18653 continued  sleep 1000

$ jobs
[1]  - running    sleep 1000
[2]  + running    nohup sleep 2000

$ kill -STOP %1
[1]  + 18653 suspended (signal)  sleep 1000

$ jobs
[1]  + suspended (signal)  sleep 1000
[2]  - running    nohup sleep 2000

$ kill -SIGHUP %1
[1]  + 18653 hangup     sleep 1000

$ jobs
[2]  + running    nohup sleep 2000

$ kill -SIGHUP %2

$ jobs
[2]  + running    nohup sleep 2000

$ kill %2
[2]  + 18745 terminated  nohup sleep 2000

$ jobs
```

A special signal is `SIGKILL` since it cannot be captured by the process and it will always terminate it immediately. However, **it can have bad side effects such as leaving orphaned children processes**.

more signals [here](https://en.wikipedia.org/wiki/Signal_(IPC)), man signal.

## Terminal Multiplexers

The most popular terminal multiplexer these days is [`tmux`](http://man7.org/linux/man-pages/man1/tmux.1.html).

`tmux` expects you to know its keybindings, and they all have the form ` x` where that means (1) press `Ctrl+b`, (2) release `Ctrl+b`, and then (3) press `x`. `tmux` has the following hierarchy of objects:

- Sessions \- a session is an independent workspace with one or more windows
  - `tmux` starts a new session.
  - `tmux new -s NAME` starts it with that name.
  - `tmux ls` lists the current sessions
  - Within `tmux` typing ` d` detaches the current session
  - `tmux a` attaches the last session. You can use `-t` flag to specify which
- Windows \- Equivalent to tabs in editors or browsers, they are visually separate parts of the same session
  - ` c` Creates a new window. To close it you can just terminate the shells doing ``
  - ` N` Go to the *N* th window. Note they are numbered
  - ` p` Goes to the previous window
  - ` n` Goes to the next window
  - ` ,` Rename the current window
  - ` w` List current windows
- Panes \- Like vim splits, panes let you have multiple shells in the same visual display.
  - ` "` Split the current pane horizontally
  - ` %` Split the current pane vertically
  - ` ` Move to the pane in the specified *direction*. Direction here means arrow keys.
  - ` z` Toggle zoom for the current pane
  - ` [` Start scrollback. You can then press `` to start a selection and `` to copy that selection.
  - ` ` Cycle through pane arrangements.

[here](https://www.hamvocke.com/blog/a-quick-and-easy-guide-to-tmux/) is a quick tutorial on `tmux` and [this](http://linuxcommand.org/lc3_adv_termmux.php) has a more detailed explanation that covers the original `screen` command.

## Alias

```shell
alias alias_name="command_to_alias arg1 arg2"
```

Example

```
# Make shorthands for common flags
alias ll="ls -lh"

# Save a lot of typing for common commands
alias gs="git status"
alias gc="git commit"
alias v="vim"

# Save you from mistyping
alias sl=ls

# Overwrite existing commands for better defaults
alias mv="mv -i"           # -i prompts before overwrite
alias mkdir="mkdir -p"     # -p make parent dirs as needed
alias df="df -h"           # -h prints human readable format

# Alias can be composed
alias la="ls -A"
alias lla="la -l"

# To ignore an alias run it prepended with \
\ls
# Or disable an alias altogether with unalias
unalias la

# To get an alias definition just call it with alias
alias ll
# Will print ll='ls -lh'
```

Note that aliases do not persist shell sessions by default. To make an alias persistent you need to include it in shell startup files, like `.bashrc` or `.zshrc`.

## Dotfiles

Many programs are configured using plain-text files known as *dotfiles* (because the file names begin with a `.`, e.g. `~/.vimrc`.

blog: [Shell startup scripts](https://blog.flowblok.id.au/2013-02/shell-startup-scripts.html)

Some other examples of tools that can be configured through dotfiles are:

- `bash` - `~/.bashrc`, `~/.bash_profile`
- `git` - `~/.gitconfig`
- `vim` - `~/.vimrc` and the `~/.vim` folder
- `ssh` - `~/.ssh/config`
- `tmux` - `~/.tmux.conf`

How should you organize your dotfiles? They should be in their own folder, under version control, and **symlinked** into place using a script. This has the benefits of:

- **Easy installation**: if you log in to a new machine, applying your customizations will only take a minute.
- **Portability**: your tools will work the same way everywhere.
- **Synchronization**: you can update your dotfiles anywhere and keep them all in sync.
- **Change tracking**: you’re probably going to be maintaining your dotfiles for your entire programming career, and version history is nice to have for long-lived projects.

Ways to do: Learn about tool setting from man page. Read someone's blog. Look through other people's dotfile.

Guide about dotfiles [here](https://dotfiles.github.io/).

All of the class instructors have their dotfiles publicly accessible on GitHub: [Anish](https://github.com/anishathalye/dotfiles), [Jon](https://github.com/jonhoo/configs), [Jose](https://github.com/jjgo/dotfiles).

## Remote Machines

`ssh foo@bar.mit.edu`

### Executing Command

`ssh foobar@server ls | grep PATTERN`

### SSH Keys

`ssh-keygen` 

You should choose a passphrase, to avoid someone who gets hold of your private key to access authorized servers. Use [`ssh-agent`](http://man7.org/linux/man-pages/man1/ssh-agent.1.html) or [`gpg-agent`](https://linux.die.net/man/1/gpg-agent) so you do not have to type your passphrase every time.

`ssh-copy-id -i .ssh/id_ed25519.pub foobar@remote`

### Copying files from remote

`ssh + tee`: `cat localfile | ssh remote_server tee serverfile`

`scp`: `scp path/to/local_file remote_host:path/to/remote_file`

`rsync`: Improved scp.

### Port Forwarding

#### Local Port Forwarding

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/Local Port Forwarding.png)

#### Remote Port Forwarding

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/Remote Port Forwarding.png)

[What's ssh port forwarding and what's the difference between ssh local and remote port forwarding [duplicate]](https://unix.stackexchange.com/questions/115897/whats-ssh-port-forwarding-and-whats-the-difference-between-ssh-local-and-remot)

### SSH Configuration

`~/.ssh/config`

```
Host vm
    User foobar
    HostName 172.16.174.141
    Port 2222
    IdentityFile ~/.ssh/id_ed25519
    LocalForward 9999 localhost:8888

# Configs can also take wildcards
Host *.mit.edu
    User foobaz
```

Server side configuration is usually specified in `/etc/ssh/sshd_config`.

### Miscellaneous

`Mosh` Improve connection issue

[`sshfs`](https://github.com/libfuse/sshfs)mount a remote folder. 

## Shells & Frameworks

**Frameworks** can improve your shell as well. Some popular general frameworks are [prezto](https://github.com/sorin-ionescu/prezto) or [oh-my-zsh](https://github.com/robbyrussll/oh-my-zsh), and smaller ones that focus on specific features such as [zsh-syntax-highlighting](https://github.com/zsh-users/zsh-syntax-highlighting) or [zsh-history-substring-search](https://github.com/zsh-users/zsh-history-substring-search). Shells like [fish](https://fishshell.com/) include many of these user-friendly features by default.

## Terminal Emulators

here is a [comparison](https://anarc.at/blog/2018-04-12-terminal-emulators-1/)

Since you might be spending hundreds to thousands of hours in your terminal it pays off to look into its settings. Some of the aspects that you may want to modify in your terminal include:

- Font choice
- Color Scheme
- Keyboard shortcuts
- Tab/Pane support
- Scrollback configuration
- Performance (some newer terminals like [Alacritty](https://github.com/jwilm/alacritty) or [kitty](https://sw.kovidgoyal.net/kitty/) offer GPU acceleration).

## Some Commands

`pgrep`