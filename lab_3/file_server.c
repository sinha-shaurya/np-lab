#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 8000
#define SIZE 1024
#define MAX_REQUESTS 5
int wc(char *file_path, char *word)
{
	FILE *fp;
	int count = 0;
	int ch, len;

	if (NULL == (fp = fopen(file_path, "r")))
		return -1;
	len = strlen(word);
	for (;;)
	{
		int i;
		if (EOF == (ch = fgetc(fp)))
			break;
		if ((char)ch != *word)
			continue;

		for (i = 1; i < len; ++i)
		{
			if (EOF == (ch = fgetc(fp)))
				goto end;
			if ((char)ch != word[i])
			{
				fseek(fp, 1 - i, SEEK_CUR);
				goto next;
			}
		}
		++count;
	next:;
	}
end:
	fclose(fp);
	return count;
}
int comp(const void *a, const void *b)
{
	return *(char *)a - *(char *)b;
}
char* replaceWordInText(const char *text, const char *oldWord, const char *newWord)
{
	int i = 0, cnt = 0;
	int len1 = strlen(newWord);
	int len2 = strlen(oldWord);
	for (i = 0; text[i] != '\0'; i++)
	{
		if (strstr(&text[i], oldWord) == &text[i])
		{
			cnt++;
			i += len2 - 1;
		}
	}
	char *newString = (char *)malloc(i + cnt * (len1 - len2) + 1);
	i = 0;
	while (*text)
	{
		if (strstr(text, oldWord) == text)
		{
			strcpy(&newString[i], newWord);
			i += len1;
			text += len2;
		}
		else
			newString[i++] = *text++;
	}
	printf("New String: %s\n", newString);
	return newString;
	
}
void replace_all(char *filepath, char *oldword, char *newword)
{
	FILE *f = fopen(filepath, "r");
	char s[SIZE];
	while ((fgets(s, SIZE, f)) != NULL)
		;
	char *c=replaceWordInText(s, oldword, newword);
	fclose(f);
	remove(filepath);
	FILE *new_f = fopen(filepath, "w");
	fputs(c, new_f);
	fclose(new_f);
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

	//accept connections from client
	struct sockaddr_in clientaddr;
	socklen_t len = sizeof(clientaddr);
	int csockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &len);
	char buffer[SIZE];
	memset(buffer, 0, sizeof(buffer));
	int recv_bytes = recv(csockfd, &buffer, SIZE, 0);
	printf("%d", recv_bytes);
	printf("Buffer value\n");
	puts(buffer);

	//check if file exists
	FILE *file = fopen(buffer, "r+");
	char filename[SIZE];
	strcpy(filename, buffer);
	char old[SIZE];
	char new[SIZE];
	if (file != NULL)
	{
		int choice = 0;
		//fclose(file);
		memset(buffer, 0, sizeof(buffer));
		strcpy(buffer, "File exists");
		send(csockfd, &buffer, SIZE, 0);
		memset(buffer, 0, sizeof(buffer));
		strcpy(buffer, "1.Search 2.Replace 3.Reorder 4.Exit");
		send(csockfd, &buffer, SIZE, 0);
		recv(csockfd, &choice, sizeof(int), 0);
		printf("Chosen option is %d\n", choice);

		//file operations using C
		switch (choice)
		{
		case 1:
			memset(buffer, 0, sizeof(buffer));
			recv(csockfd, &buffer, SIZE, 0);
			int count = wc(filename, buffer);
			printf("%d\n", wc(filename, buffer));
			send(csockfd, &count, sizeof(int), 0);
			break;
		case 2:

			memset(old, 0, sizeof(old));
			memset(new, 0, sizeof(new));
			recv(csockfd, &old, SIZE, 0);
			recv(csockfd, &new, SIZE, 0);
			replace_all(filename, old, new);
			break;
		case 3:
			memset(buffer, 0, sizeof(buffer));
			char st[SIZE];
			memset(st, 0, sizeof(st));
			while ((fgets(st, SIZE, file)) != NULL)
				;
			qsort(st, strlen(st), sizeof(char), comp);
			puts(st);
			printf("Number of characters in file are %zu", strlen(st));
			fclose(file);
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, "File is reordered");
			send(csockfd, &buffer, strlen(buffer), 0);
			FILE *f = fopen(filename, "w");
			fputs(st, f);
			fclose(f);
			break;
		case 4:
			close(sockfd);
			exit(EXIT_SUCCESS);
		} //end of switch
	}
	else
	{
		memset(buffer, 0, sizeof(buffer));
		strcpy(buffer, "File does not exist");
		send(csockfd, &buffer, strlen(buffer), 0);
	}
	close(sockfd);
	close(csockfd);
}
