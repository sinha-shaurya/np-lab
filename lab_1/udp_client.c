#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdbool.h>
#define PORT 8008
#define MAX_SIZE 1024
int main()
{
    int sockfd;
    struct sockaddr_in servaddr, clientaddr;
    char buffer[MAX_SIZE];
    bool ans;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&clientaddr, 0, sizeof(clientaddr));
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_port = htons(PORT);
    clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(sockfd,(struct sockaddr*)&clientaddr,sizeof(clientaddr));

    memset(buffer,0,sizeof(buffer));
    sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&servaddr,sizeof(servaddr));
    ans=false;
    socklen_t size=sizeof(servaddr);
    recvfrom(sockfd,&ans,sizeof(bool),0,(struct sockaddr*)&servaddr,&size);
    printf("%d\n",ans);
    close(sockfd);
    return 0;
}