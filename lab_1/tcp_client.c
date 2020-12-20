#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 8000
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

    int a[MAX_SIZE];
    memset(a, 0, sizeof(a));
    int num = 3;
    char s[MAX_SIZE];
    //input a
    printf("Enter array size\n");
    scanf("%d", &num);
    printf("Enter array elements\n");
    for (int i = 0; i < num; i++)
    {
        int x;
        scanf("%d", &x);
        a[i] = x;
    }
    for (;;)
    {
        scanf("%s", s);
        if (strcmp(s, "exit") == 0)
            break;
        int sent_string=send(sockfd,&s,MAX_SIZE,0);
        int sent_bytes = send(sockfd, &a, MAX_SIZE, 0);
        sent_bytes = send(sockfd, &num, sizeof(int), 0);
        printf("%d\n", sent_bytes);
        printf("%d\n", sent_bytes);
    }
    close(sockfd);
    return 0;
}