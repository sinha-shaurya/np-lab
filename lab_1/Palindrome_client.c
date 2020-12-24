#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#define MAXSIZE 90

int main()
{
	int sockfd, retval, i;
	int recedbytes, sentbytes;
	struct sockaddr_in serveraddr, clientaddr;
	// char buff[MAXSIZE];
	int buff, buff_ans;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1)
	{
		printf("\nSocket Creation Error");
		return 0;
	}
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(3388);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = htons(3389);
	clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	retval = bind(sockfd, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
	if (retval == 1)
	{
		printf("Binding error");
		close(sockfd);
	}

	for (i = 0;; i += 1)
	{
		printf("Enter string ,-1 to exit\n");
		char buffer[MAXSIZE];
		scanf("%s", buffer);

		sentbytes = sendto(sockfd, &buffer, strlen(buffer), 0,
						   (struct sockaddr *)&serveraddr, sizeof(serveraddr));
		if (strncmp(buffer, "-1", 2) == 0)
		{
			break;
		}
		if (sentbytes == -1)
		{
			printf("sending error");
			close(sockfd);
		}

		int size = sizeof(serveraddr);
		recedbytes = recvfrom(sockfd, &buff_ans, sizeof(buff_ans), 0, (struct sockaddr *)&serveraddr, &size);
		if (buff_ans == 1)
		{
			printf("Palindrome\n");
		}
		else
		{
			printf("Not palindrome\n");
		}
	}

	printf("\n");
	close(sockfd);
}
