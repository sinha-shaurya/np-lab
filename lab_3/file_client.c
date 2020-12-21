#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 8008
#define MAX_SIZE 1024
int main()
{
	int sockfd;
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int n = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	char s[MAX_SIZE];
	memset(s, 0, sizeof(s));
	printf("Enter filename\n");
	scanf("%s", s);
	//send filename
	int sent_bytes = send(sockfd, &s, strlen(s), 0);
	printf("%d", sent_bytes);
	//receive file exists or not response
	memset(s, 0, sizeof(s));
	recv(sockfd, &s, MAX_SIZE, 0);
	puts(s);
	if (strcmp(s, "File does not exist") != 0)
	{
		memset(s, 0, sizeof(s));
		recv(sockfd, &s, MAX_SIZE, 0);
		puts(s);
		int choice;
		printf("Enter choice\n");
		scanf("%d", &choice);
		send(sockfd, &choice, sizeof(int), 0);
		switch (choice)
		{
		case 1:
			memset(s, 0, sizeof(s));
			scanf("%s", s);
			send(sockfd, &s, MAX_SIZE, 0);
		}
		puts(s);
	}
	close(sockfd);
	return 0;
}
