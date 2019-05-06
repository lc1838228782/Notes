#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define MAXBUF 32

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "usage: %s <IP Adress>\n", argv[0]);
		exit(0);
	}

	struct in_addr inaddr;
	char buf[MAXBUF];

	sscanf(argv[1], "%s", buf);
	uint32_t addr;

	inet_pton(AF_INET, buf, &inaddr);
	addr = ntohl(inaddr.s_addr);

	printf("%x\n", addr);

	return 0;
}
