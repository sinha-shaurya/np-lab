#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#define PORT 8000
#define SIZE 1024
#define MAX_REQUESTS 5
int comparator_increasing(const void *a, const void *b)
{
    return *(char *)a - *(char *)b;
}
int comparator_decreasing(const void *a, const void *b)
{
    return *(char *)b - *(char *)a;
}
int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)))
    {
        perror("Socket binding failed");
        exit(EXIT_FAILURE);
    }
    if (listen(sockfd, MAX_REQUESTS) != 0)
    {
        perror("Cannot listen");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    int csockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &len);
    char buffer[SIZE];

    if ((recv(csockfd, buffer, SIZE, 0)) != -1)
    {
        char numbers[SIZE];
        char alphabets[SIZE];
        int i = 0;
        int j = 0;
        int k = 0;
        while (i < strlen(buffer))
        {
            if (isalpha(buffer[i]))
                alphabets[j++] = buffer[i];
            else if (isdigit(buffer[i]))
                numbers[k++] = buffer[i];
            i = i + 1;
        }
        alphabets[j]=0;
        //sort numbers
        int pid = fork();
        if (pid == 0)
        {
            int id=getpid();
            char s[10];
            qsort(numbers, k, sizeof(char), comparator_increasing);
            sprintf(s," PID=%d",id);
            strcat(numbers,s);
            send(csockfd, numbers, SIZE, 0);
        }
        else
        {
            int id=getpid();
            char s[10];
            sprintf(s," PID=%d",id);
            qsort(alphabets, j, sizeof(char), comparator_decreasing);
            strcat(alphabets,s);
            send(csockfd, alphabets, SIZE, 0);
        }
    }
    close(sockfd);
    return 0;
}