https://stackoverflow.com/questions/30968734/reopen-stdout-and-stderr-after-closing-them

I am running this on an Ubuntu machine, so I am not sure if it'll work for you, but this is what I did:

```sh
$ exec 1>&0
$ exec 2>&0
```

Suddenly, I had STDOUT and STDERR reconnected. Magic!

**Explanation:** Running the following commands, we get the following output:

```sh
$ ls -l /dev/stdout
lrwxrwxrwx 1 root root 15 Jun 11 23:39 /dev/stdout -> /proc/self/fd/1

$ ls -l /proc/self/fd/1
lrwx------ 1 jay jay 64 Jun 22 01:34 /proc/self/fd/1 -> /dev/pts/10

$ ls -l /proc/self/fd/
total 0
lrwx------ 1 jay jay 64 Jun 22 01:35 0 -> /dev/pts/10
lrwx------ 1 jay jay 64 Jun 22 01:35 1 -> /dev/pts/10
lrwx------ 1 jay jay 64 Jun 22 01:35 2 -> /dev/pts/10
lr-x------ 1 jay jay 64 Jun 22 01:35 3 -> /proc/12224/fd
```

Since all three fd's point to the same thing, we can return them back to normal just by pointing to /dev/pts/10 which the `exec 1>&0` and `exec 2>&0` do