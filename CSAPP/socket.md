## 地址数据结构

```c
#include <arpa/inet.h>
struct in_addr {
    uint32_t s_addr;
}
```
存储网络字节序的地址

## 地址转换方法
1. 转换字节序

`uint32_t htonl(uint32_t hostlong)` 

`uint16_t htons(uint16_t hostshort)`

`uint32_t ntohl(uint32_t hostlong)`

`uint16_t ntohs(uint16_t hostshort)`

host to network，在主机与网络的字节序之间转换


2. IP地址与点分十进制字符串之间的转化
```c
int inet_pton(AF_INET, CONST char *src, void *dst);
// 成功返回1， src非法返回0， 出错-1
```

将点分十进制IP地址字符串表示转换成二进制网络序

示例 ：`inet_pton(AF_INET, buf, &in_addr)` 


```c
const char *inet_ntop(AF_INET, const void *src, void *dst, socketlen_t size);
// 成功返回字符串表示指针，失败NULL
```

将网络序IP地址转换为十进制字符串表示

示例：`inet_ntop(AF_INET, &in_addr, buf, sizeof(buf))`


## 套接字地址结构

```c
/* IP socket address structure */
struct sockaddr_in {
    uint16_t sin_family;     /* protocol family (always AF_INET) */
    uint16_t sin_port;       /* Port number in network byte order */
    struct in_addr sin_addr; /* IP adress in network byte order */
    unsigned char sin_zero[8];/* Pad to sizeof(struct sockaddr) */
}
```

```c
/*Generic socket address structure (for connect, bind, and accept) */
struct sockaddr {
    uint16_t sa_family;     /* Protocol family */
    char sa_date[14];       /* address data */
}
```

为了兼容各种套接字地址结构，定义套接字函数要求指向通用socketaddr结构的指针（当时没有通用的void *指针），然后将应用程序与协议特定的结构的指针强制转化为通用结构。一般定义` typedef struct sockaddr SA ` 来用。

## 套接字方法

```c
#include <sys/types.h>
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
```

创建套接字描述符。

成功返回非负描述符，出错返回 -1。

示例：`clienfd = Socket(AF_INET, SOCK_STREAM, 0) `

AF_INET表示正在使用32位地址，SOCK_STREAM表示这个套接字是连接的一个端点。最好用getaddrinfo函数来自动生成参数，这样就协议无关了。此时描述符仅是部分打开，还不能用于读写。完成打开取决于是客户端还是服务器。



```c
#include <sys/socket.h>
int connect(int clientfd, const struct sockaddr *addr, socklen_t addrlen);
```

客户端通过调用connect函数与服务器连接。

成功返回0，出错返回-1。

connect函数会一直阻塞直到连接成功或者发生错误。成功就准备好读写了。

addrlen是sizeof(sockaddr_in)。



```c
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

服务器绑定端口。

成功返回0，出错返回-1。

bind函数告诉内核将addr中的服务器套接字地址与套接字描述符sockfd联系起来。

addrlen是sizeof(sockaddr_in)。



```c
#include <sys/socket.h>
int listen(int sockfd, int backlog);
```

成功返回0，失败-1

告诉内核描述符是被服务器使用的。将一个主动套接字转换为监听套接字。

backlog参数暗示了内核在开始拒绝连接请求之前，队列中要排队的未完成的连接请求的数量。确切理解需要TCP/IP协议。



```c
#include <sys/socket.h>
int accept(int listenfd, struct sockaddr *addr, int *addr_len);
```

服务器通过调用accept函数来等待来自客户端的连接请求。

成功返回非负连接描述符，出错返回-1。

返回一个已连接描述符，这个描述符可被用来Unix IO函数与客户端通信。



```c
struct addrinfo {
    int ai_flags;     /* Hints argument flags */
    int ai_family;    /* First arg to socket function */
    int ai_socktype;  /* Second arg to scoket function */
    int ai_protocol;   /* Third arg to socket function */
    char *ai_canonname; /* Canonical hostname */
    size_t ai_addrlen;   /* Size of ai_addr struct */
    struct sockaddr *ai_addr; /* ptr to socket address structure */
    struct addinfo *ai_next;  /* ptr to next */
}
```

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int getaddrinfo(const char *host, const char *service,
                const struct addrinfo *hints,
                struct addrinfo **result);

void freeaddrinfo(struct addrinfo *result);

const char *gai_strerror(int errcode);
```

getaddrinfo函数将主机名、主机地址、服务名和端口号的字符串转换为套接字地址结构。

成功返回0，错误为非零错误码，可用gai_strerror将该消息转换为字符串。

客户端调用后会遍历这个列表，依次尝试每个套接字地址，直到socket和connect成功。

服务器会遍历每个套接字地址，直到socket和bind成功。为了避免内存泄漏必须在最后调用freeaddrinfo，释放该链表。

host：可以使域名也可以是数字地址（点分十进制IP地址），可为NULL。

service：可以是服务名也可以是十进制端口号

hints：可选参数，只能设置ai_family，ai_socktype，ai_protocol，和ai_flags。其他必须为0（或者NULL）实际中经常memset清零。

- 默认可返回IPv4地址和IPv6套接字地址。ai_family限制为IPv4或者IPv6地址。
- 对于host关联的每个地址，默认返回最多三个addrinfo结构，每个的ai_socktype不同：一个是连接，一个是数据包，一个是原始套接字。ai_socktype设置为SOCK_STREAM将列表限制为每个地址最多一个addrinfo结构，该结构套接字地址可以作为连接的一个端点。
- ai_flags是一个位掩码字段。以下为几个有用的的值。
  - AI_ADDRCONFIG：他要求当本地主机被配置为IPv4时，getaddrinfo返回IPv4地址。对IPv6也类似。
  - AI_CANONNAME：ai_canonical字段默认为NULL。设置之后就是告诉getaddrinfo将列表第一个addrinfo结构的ai_canonical字段指向host的权威名字。
  - AI_NUMERICSERV：参数service默认可以是端口号或者服务名。这个标志强制使用service使用端口号。
  - AI_PASSIVE：getaddrinfo默认返回套接字地址，客户端可以在调用connect时用作主动套接字。这个标志意味着返回的套接字可能被服务器用作监听套接字。此时host应为NULL。



```c
#include <sys/socket.h>
#include <netdb.h>

int getnameinfo(const struct sockaddr *sa, socklen_t salen,
                char *host, size_t hostlen,
                char *service, size_t servlen, int flags);
```

将套接字地址结构转换成相应的主机和服务名字符串。

成功返回0，错误返回错误码，可用gai_strerror转化为错误消息。

getnameinfo将套接字地址结构sa转换成对应的主机和服务名字，并将他们复制到host和service缓冲区。不想要host可以设置为NULL，0。service一样，但必须设置其一。

参数flags是一个位掩码。两个有用值。

- NI_NUMERICHOST。默认试图返回host中的域名。设置该标志会使该函数返回一个数字地址字符串。
- NI_NUMERICSERV。默认检查/etc/services，尽可能返回服务名。该标志会放弃查找，简单地返回端口号。



















