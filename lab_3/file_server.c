#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define PORT 8008
#define SIZE 1024
#define MAX_REQUESTS 5
int comp(const void *a,const void *b)
{
	return *(char*)a-*(char*)b;
}
int main()
{
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(PORT);
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    if(bind(sockfd,(struct sockaddr*) &servaddr,sizeof(servaddr)))
    {
        perror("Socket binding failed");
        exit(EXIT_FAILURE);
    }
    if(listen(sockfd,MAX_REQUESTS)!=0)
    {
        perror("Cannot listen");
        exit(EXIT_FAILURE);
    }
    
    //accept connections from client
    struct sockaddr_in clientaddr;
    socklen_t len=sizeof(clientaddr);
    int csockfd=accept(sockfd,(struct sockaddr*)&clientaddr,&len);
    char buffer[SIZE];
    memset(buffer,0,sizeof(buffer));
    int recv_bytes=recv(csockfd,&buffer,SIZE,0);
    printf("%d",recv_bytes);
    printf("Buffer value\n");
	puts(buffer);
	
	//check if file exists
    FILE *file=fopen(buffer,"r+");
	char filename[SIZE];
	strcpy(filename,buffer);
    if(file!=NULL)
       {
            int choice=0;
            //fclose(file);
            memset(buffer,0,sizeof(buffer));
            strcpy(buffer,"File exists");
            send(csockfd,&buffer,SIZE,0);
			memset(buffer,0,sizeof(buffer));
            strcpy(buffer,"1.Search 2.Replace 3.Reorder 4.Exit");
			send(csockfd,&buffer,SIZE,0);
			recv(csockfd,&choice,sizeof(int),0);
			printf("Chosen option is %d\n",choice);
			
			//file operations using C
			switch(choice)
			{
				case 1:
				memset(buffer,0,sizeof(buffer));
				recv(csockfd,&buffer,SIZE,0);
				int sum=0;
				char str[SIZE];
				memset(str,0,sizeof(str));
				while((fgets(str,SIZE,file))!=NULL)
				{
					if(strstr(str,buffer)!=NULL)
						sum=sum+1;
				}
				printf("%d\n",sum);
				fclose(file);
				break;
				case 3:
					memset(buffer,0,sizeof(buffer));
					char st[SIZE];
					memset(st,0,sizeof(st));
					while((fgets(st,SIZE,file))!=NULL);
					qsort(st,SIZE,sizeof(char),comp);
					puts(st);
					fclose(file);
					FILE* f=fopen(filename,"w");
					fputs(st,f);
					fclose(f);
				break;
				case 4:
					exit(EXIT_SUCCESS);
			}//end of switch
			
		
				
       }
    else
        {
			memset(buffer,0,sizeof(buffer));
			strcpy(buffer,"File does not exist");
			send(csockfd,&buffer,strlen(buffer),0);
		}
    close(sockfd);
}