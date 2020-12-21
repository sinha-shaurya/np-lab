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
int comparator_increasing(int* a,int* b)
{
	return *(int*)a-*(int*)b;
}
int comparator_decreasing(const void *a,const void* b)
{
	return *(int*)b-*(int*)a;
}
int search(int *a,int size,int key)
{
    for(int i=0;i<size;i++)
    {
        if(a[i]==key) return i;
    }
    return -1;
}
void display(int *a,int n)
{
	for(int i=0;i<n;i++)
	{
		printf("%d ",a[i]);
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
    int size = 0;
    int choice=0;
	int key=0;
    memset(a, 0, MAX_SIZE);
    int i = 0;
    for (;;)
    {
        char c[MAX_SIZE];
        recv(csockfd, &c, MAX_SIZE, 0);
        printf("%s\n",c);
        if(compare_to(c)==true)
        {
          printf("breaking");
          close(sockfd);
	      exit(EXIT_SUCCESS);
          break;
        }   
        int receive_array = recv(csockfd, &a, MAX_SIZE, 0);
        int receive_size = recv(csockfd, &size, sizeof(int), 0);
		int receive_choice=recv(csockfd,&choice,sizeof(int),0);
        switch(choice)
		{
			case 1:
			qsort(a,size,sizeof(int),comparator_inceasing);
			send(csockfd,&a,MAX_SIZE,0);
			case 2:
			qsort(a,size,sizeof(int),comparator_decreasing);
			send(csockfd,&a,MAX_SIZE,0);
			case 3:
			
        i++;
    }

    close(sockfd);
    return 0;
}
