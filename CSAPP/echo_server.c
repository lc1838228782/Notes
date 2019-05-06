#include "csapp.h"

void echo(int confd);

int main(int argc, char **argv)
{
	int listenfd;
	int confd;
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;
	char client_hostname[MAXLINE], client_port[MAXLINE];

	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}

	listenfd = Open_listenfd(argv[1]);
	while (1) {
		clientlen = sizeof(struct sockaddr_storage);

		confd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

		Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
		printf("Connected to (%s %s)\n", client_hostname, client_port);
		echo(confd);
		Close(confd);
	}

	return 0;
}

void echo(int confd)
{
	size_t n;
	char buf[MAXLINE];
	rio_t rio;

	Rio_readinitb(&rio, confd);
	while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
		printf("server received %d bytes\n", (int)n);
		Rio_writen(confd, buf, n);
		Rio_writen(1, buf, n);
	}
}
