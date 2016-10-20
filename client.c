#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFLEN 1024

int main(int argc, char *argv[]) {
	int sockfd;
	struct sockaddr_in	s_addr;
	socklen_t len;
	unsigned int port;
	char buff[BUFFLEN];

	/* create socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(errno);
	} else {
		printf("socket create success!\n");
	}

	/* set server port */
	if (argv[2]) {
		port = atoi(argv[2]);
	} else {
		port = 4567;
	}

	/* set server ip */
	bzero(&s_addr, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(port);
	if (inet_aton(argv[1], (struct in_addr *) &s_addr.sin_addr.s_addr) == 0) {
		perror(argv[1]);
		exit(errno);
	}

	/* connect to server */
	if (connect(sockfd, (struct sockaddr*) &s_addr, sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(errno);
	} else {
		printf("connect success!\n");
	}

	while (1) {
		bzero(buff, BUFFLEN);
		len = recv(sockfd, buff, BUFFLEN, 0);
		if (len > 0) {
			printf("server: %s\n", buff);
		} else {
			if (len < 0) printf(" [receive failed!] \n");
			else printf(" [server exit!] ");
			break;
		}
		/* send message */
		bzero(buff, BUFFLEN);
		printf("client: ");
		fgets(buff, BUFFLEN, stdin);
		if (!strncasecmp(buff, "quit", 4)) {
			printf(" [client exit!] ");
			break;
		}
		if (!strncmp(buff, "\n", 1)) {
			continue;
		}
		if (strchr(buff, '\n')) {
			len = send(sockfd, buff, strlen(buff)-1, 0);
		} else {
			len = send(sockfd, buff, strlen(buff), 0);
		}

		if (len > 0) {
			printf(" [send success!] ");
		} else {
			printf(" [send failed!] ");
			break;
		}
	}
	close(sockfd);

	return 0;
}
