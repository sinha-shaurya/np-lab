#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#define MAXSIZE 90
bool is_palindrome(char *s)
{
	int len = strlen(s);
	for (int i = 0; i < len; i++)
	{
		if (s[i] != s[len - i - 1])
			return false;
	}
	return true;
}
int main()
{
	int sockfd, newsockfd, retval, i;
	socklen_t actuallen;
	int recedbytes, sentbytes;
	struct sockaddr_in serveraddr, clientaddr;
	int a = 0;
	int buff, buff_ans;
	char buffer[MAXSIZE];
	memset(buffer, 0, sizeof(buffer));
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if (sockfd == -1)
	{
		printf("\nSocket creation error");
	}
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = htons(3389);
	clientaddr.sin_addr.s_addr = htons(INADDR_ANY);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(3388);
	serveraddr.sin_addr.s_addr = htons(INADDR_ANY);

	retval = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if (retval == 1)
	{
		printf("Binding error");
		close(sockfd);
	}

	for (i = 0;; i += 1)
	{
		actuallen = sizeof(clientaddr);
		recedbytes = recvfrom(sockfd, buffer, MAXSIZE, 0, (struct sockaddr *)&clientaddr, &actuallen);

		if (recedbytes == -1)
		{
			printf("Receiving error\n");
			close(sockfd);
			exit(EXIT_SUCCESS);
		}
		if (buffer[0] == '-' && buffer[1] == '1')
		{
			exit(EXIT_SUCCESS);
		}
		//getting the input in buff(int)
		buff_ans = is_palindrome(buffer);
		printf("%d", buff_ans);
		sentbytes = sendto(sockfd, &buff_ans, sizeof(int), 0, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
		if (sentbytes == -1)
		{
			printf("sending error");
			close(sockfd);
		}
	}
	close(sockfd);
	close(newsockfd);
}
