# Lab 8

`struct proc_struct`

```c
struct proc_struct {
    /* ... */
    // the file related info(pwd, files_count, files_array, fs_semaphore) of process
    struct files_struct *filesp;                
};
```

`struct files_struct`

```c
struct files_struct {
    struct inode *pwd;      // inode of present working directory
    struct file *fd_array;  // opened files array
    int files_count;        // the number of opened files
    semaphore_t files_sem;  // lock protect sem
};
```

`struct file`

```c
struct file {
    enum {
        FD_NONE, FD_INIT, FD_OPENED, FD_CLOSED,
    } status;
    bool readable;
    bool writable;
    int fd;
    off_t pos;
    struct inode *node;
    int open_count;
};
```

`struct inode`

```c
struct inode {
    union {
        struct device __device_info;
        struct sfs_inode __sfs_inode_info;
    } in_info;
    enum {
        inode_type_device_info = 0x1234,
        inode_type_sfs_inode_info,
    } in_type;
    int ref_count;
    int open_count;
    struct fs *in_fs;
    const struct inode_ops *in_ops;
};
```

## fs_init()

`vfs_init()`->`vfs_devlist_init()`->`list_init(&vdev_list)`初始化vdev_list双向链表。

