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
char *replaceWordInText(const char *text, const char *oldWord, const char *newWord)
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
    char *c = replaceWordInText(s, oldword, newword);
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
    servaddr.sin_port = htons(8009);
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("Socket binding failed");
        exit(EXIT_FAILURE);
    }

    //accept connections from client
    struct sockaddr_in clientaddr;
    memset(&clientaddr, 0, sizeof clientaddr);
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_port = htons(PORT);
    clientaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    socklen_t len = sizeof(clientaddr);
    char buffer[SIZE];
    memset(buffer, 0, sizeof(buffer));
    int recv_bytes = recvfrom(sockfd, &buffer, SIZE, 0, (struct sockaddr *)&clientaddr, &len);
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
        sendto(sockfd, &buffer, SIZE, 0, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, "1.Search 2.Replace 3.Reorder 4.Exit");
        sendto(sockfd, &buffer, SIZE, 0, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
        recvfrom(sockfd, &choice, sizeof(int), 0, (struct sockaddr *)&clientaddr, &len);
        printf("Chosen option is %d\n", choice);

        //file operations using C
        switch (choice)
        {
        case 1:
            memset(buffer, 0, sizeof(buffer));
            recvfrom(sockfd, &buffer, SIZE, 0, (struct sockaddr *)&clientaddr, &len);
            int count = wc(filename, buffer);
            printf("%d\n", wc(filename, buffer));
            sendto(sockfd, &count, sizeof(int), 0, (struct sockaddr *)&clientaddr, len);
            break;
        case 2:

            memset(old, 0, sizeof(old));
            memset(new, 0, sizeof(new));
            recvfrom(sockfd, &old, SIZE, 0, (struct sockaddr *)&clientaddr, &len);
            recvfrom(sockfd, &new, SIZE, 0, (struct sockaddr *)&clientaddr, &len);
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
            sendto(sockfd, &buffer, strlen(buffer), 0, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
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
        printf("%s", buffer);
        sendto(sockfd, &buffer, strlen(buffer), 0, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
    }
    close(sockfd);
}
