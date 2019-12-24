#include "csapp.h"


//int open_clientfd(char *hostname, char *port);


int main(int argc, char **argv)
{
	int clientfd;
	char *host, *port, buf[MAXLINE];
	rio_t rio;

	if (argc != 3) {
		fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
		exit(0);
	}
	host = argv[1];
	port = argv[2];

	clientfd = Open_clientfd(host, port);
	Rio_readinitb(&rio, clientfd);

	while (Fgets(buf, MAXLINE, stdin) != NULL) {
		Rio_writen(clientfd, buf, strlen(buf));
		Rio_readlineb(&rio, buf, MAXLINE);
		Fputs(buf, stdout);
	}

	Close(clientfd);

	return 0;
}

/*
int open_clientfd(char *hostname, char *port)
{
	struct addrinfo hints, *listp, *p;
	int clientfd;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_NUMERICSERV;
	hints.ai_flags |= AI_ADDRCONFIG;

	int errcode;
	if ((errcode = getaddrinfo(hostname, port, &hints, &listp)) != 0) {
		fprintf(stderr, "%s\n", gai_strerror(errcode));
	}

	for (p = listp; p != NULL; p = p->ai_next) {
		if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
			continue;
		}

		if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1) {
			break;
		}

		close(clientfd);
	}

	freeaddrinfo(listp);
	if(!p)
	  return -1;
	else
	  return clientfd;
}
*/
