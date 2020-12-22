#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdbool.h>
#define PORT 8000
#define MAX_SIZE 1024
bool is_palindrome(char *a)
{
    int len = strlen(a);
    for (int i = 0; i < len; i++)
    {
        if (a[i] != a[len - i - 1])
            return false;
    }
    return true;
}
int main()
{
    int sockfd;
    int client;
    struct sockaddr_in servaddr, cliaddr;
    int recvbytes, sentbytes;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    memset(&cliaddr, 0, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(PORT);
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    char buffer[MAX_SIZE];
    memset(buffer, 0, sizeof(buffer));
    //receive data from client
    socklen_t len = sizeof(cliaddr);
    recvbytes = recvfrom(sockfd, buffer, MAX_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
    bool ans = is_palindrome(buffer);
    sentbytes = sendto(sockfd, &ans, sizeof(bool), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
    close(sockfd);
    return 0;
}