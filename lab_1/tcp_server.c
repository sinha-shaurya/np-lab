#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#define PORT 8000
#define MAX_SIZE 1024
#define MAX_REQUESTS 5

bool compare_to(char *a)
{
    return a[0] == 'e' && a[1] == 'x' && a[2] == 'i' && a[3] == 't';
}
int comparator_increasing(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}
void display(int *a, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}
int main()
{
    int sockfd, csockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t client_len = 0;

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
    {
        perror("Could not bind socket\n");
        exit(EXIT_FAILURE);
    }
    if (listen(sockfd, MAX_REQUESTS) > 0)
    {
        perror("Could not open for listening\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Server online\n");
    }
    client_len = sizeof(cliaddr);
    csockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &client_len);
    int a[MAX_SIZE];
    char buffer[MAX_SIZE];
    int size;
    for (;;)
    {
        memset(a, 0, sizeof(a));
        memset(buffer, 0, sizeof(buffer));
        recv(csockfd, &size, sizeof(int), 0);
        recv(csockfd, &a, MAX_SIZE, 0);
        display(a,size);
        qsort(a, size, sizeof(int), comparator_increasing);
        send(csockfd, &a, MAX_SIZE, 0);
        recv(csockfd, buffer, MAX_SIZE, 0);
        puts(buffer);
        if (compare_to(buffer))
            break;
    }

    close(sockfd);
    return 0;
}
