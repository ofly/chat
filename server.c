#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define BUFFLEN 1024

/* main starts */
int main(int argc, char *argv[]) {
    int sockfd, newfd;
	struct sockaddr_in	s_addr, c_addr;
	socklen_t len;
	unsigned int port, listnum;
	char buff[BUFFLEN];

	/* create socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(errno);
	} else {
		printf("socket create success!\n");
	}

	bzero(&s_addr, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	if (argv[1]) {
		s_addr.sin_addr.s_addr = inet_addr(argv[1]);
	} else {
		s_addr.sin_addr.s_addr = INADDR_ANY;
	}

	if (argc > 3) {
		port = atoi(argv[2]);
	} else {
		port = 4567;
	}
	port = 4567;
	s_addr.sin_port = htons(port);

	if (argc > 4) {
		listnum = atoi(argv[3]);
	} else {
		listnum = 3;
	}
	printf("port = %d\n", port);

	/* bind ipaddr and port to socket */
	if ((bind(sockfd, (struct sockaddr*) &s_addr, sizeof(struct sockaddr))) == -1) {
		perror("bind");
		exit(errno);
	} else {
		printf("bind success!\n");
	}

	/* listen local port */
	if (listen(sockfd, listnum) == -1) {
		perror("listen");
		exit(errno);
	} else {
		printf("the server is listening!\n");
	}

	while (1) {
		printf("************** chatting ***************");
		len = sizeof(struct sockaddr);
		if ((newfd = accept(sockfd, (struct sockaddr*) &c_addr, &len)) == -1) {
			perror("accept");
			exit(errno);
		} else {
			printf("client is: %s: %d\n", inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));
		}

		while (1) {
			bzero(buff, BUFFLEN);
			printf("server: ");
			fgets(buff, BUFFLEN, stdin);
			if (!strncasecmp(buff, "quit", 4)) {
				printf("server stopped the chat!\n");
				break;
			}
			if (!strncmp(buff, "\n", 1)) {
				continue;
			}
			if (strchr(buff, '\n')) {
				len = send(newfd, buff, strlen(buff)-1, 0);
			} else {
				len = send(newfd, buff, strlen(buff), 0);
			}
			if (len > 0) {
				printf(" [send succeed!] \n");
			} else {
				printf(" [send failed!] \n");
			}
			/* read massg*/
			bzero(buff, BUFFLEN);
			len = recv(newfd, buff, BUFFLEN, 0);
			if (len > 0) {
				printf("client: %s\n", buff);
			} else {
				if (len < 0) printf(" [receive failed] \n");
				else printf(" [client exit!] ");
				break;
			}
		}
		close(newfd);
		printf("quit?(N|y) ");
		bzero(buff, BUFFLEN);
		fgets(buff, BUFFLEN, stdin);
		if (!strncasecmp(buff, "y", 1)) {
			printf(" [server exit!] ");
			break;
		}
	}

	close(sockfd);

    return 0;
}
