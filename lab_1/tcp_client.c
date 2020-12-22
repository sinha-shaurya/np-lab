#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#define PORT 8000
#define MAX_SIZE 1024
bool compare_to(char *a)
{
    return a[0] == 'e' && a[1] == 'x' && a[2] == 'i' && a[3] == 't';
}
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

    for (;;)
    {
        int a[MAX_SIZE];
        char buffer[MAX_SIZE];
        memset(a, 0, sizeof(a));
        int len;
        printf("Enter array length\n");
        scanf("%d", &len);
        printf("Enter array elements\n");
        for (int i = 0; i < len; i++)
        {
            int x;
            scanf("%d", &x);
            a[i] = x;
        }
        send(sockfd, &len, sizeof(int), 0);
        send(sockfd, &a, MAX_SIZE, 0);
        memset(a, 0, sizeof(a));
        recv(sockfd, &a, MAX_SIZE, 0);
        for (int i = 0; i < len; i++)
        {
            printf("%d ", a[i]);
        }
        printf("continue or exit?\n");
        scanf("%s", buffer);
        send(sockfd, buffer, strlen(buffer), 0);
        if(compare_to(buffer))
            break;
    }
    close(sockfd);
    return 0;
}
