#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXBUF 32

int main(int argc, char **argv)
{
	if (2 != argc) {
		fprintf(stderr, "usage: %s <hex number>\n", argv[0]);
		exit(0);
	}

	struct in_addr inaddr;     //address in network bytes order
	uint32_t addr;            // adress in host byte order
	char buf[MAXBUF];

	sscanf(argv[1], "%x", &addr);
	inaddr.s_addr = htonl(addr);

	if (!inet_ntop(AF_INET, &inaddr, buf, MAXBUF)) {

	}

	printf("%s\n", buf);
	return 0;
}
