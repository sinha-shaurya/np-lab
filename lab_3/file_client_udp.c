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
    struct sockaddr_in clientaddr;
    socklen_t len;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8009);
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    memset(&clientaddr, 0, sizeof(clientaddr));
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_port = htons(PORT);
    clientaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    bind(sockfd, (struct sockaddr *)&clientaddr, sizeof(clientaddr));

    char s[MAX_SIZE];
    memset(s, 0, sizeof(s));
    printf("Enter filename\n");
    scanf("%s", s);
    //send filename
    int sent_bytes = sendto(sockfd, &s, strlen(s), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("%d", sent_bytes);
    //receive file exists or not response
    memset(s, 0, sizeof(s));
    recv(sockfd, &s, MAX_SIZE, 0);
    puts(s);
    if (strcmp(s, "File does not exist") != 0)
    {
        memset(s, 0, sizeof(s));
        len = sizeof(servaddr);
        recvfrom(sockfd, &s, MAX_SIZE, 0, (struct sockaddr *)&servaddr, &len);
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
            sendto(sockfd, &s, MAX_SIZE, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
            int count;
            recvfrom(sockfd, &count, sizeof(int), 0, (struct sockaddr *)&servaddr, &len);
            printf("Number of occurences are %d\n", count);
            break;
        case 2:
            printf("Enter old word\n");
            memset(s, 0, sizeof(s));
            scanf("%s", s);
            sendto(sockfd, &s, sizeof(s), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
            printf("Enter new word\n");
            memset(s, 0, sizeof(s));
            scanf("%s", s);
            sendto(sockfd, &s, sizeof(s), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
        case 3:
            memset(s, 0, sizeof(s));
            recvfrom(sockfd, &s, MAX_SIZE, 0, (struct sockaddr *)&servaddr, &len);
            puts(s);
            break;
        }
    }
    close(sockfd);
    return 0;
}
