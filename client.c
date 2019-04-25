/*
* ./client [host ip]
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define CHARNUM 160
#define USRCAP 10
#define PORT 3333

void error(const char *warning)
{
    perror(warning);
    exit(0);
}


int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char buffer[CHARNUM], username[USRCAP], s_response[1000], fnl[2000];

    // Check for correct amount of line arguments
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s [HOST IP]\n", argv[0]);
        exit(1);
    }

    printf("What's your name?\n");
    fgets(username, USRCAP, stdin);
    memset(fnl, 0, sizeof(fnl));
    strcpy(fnl, username);
    strcat(fnl, ": ");

    printf("Welcome to the chat, %s\n", username);

    // Step 1
    // Create socket
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Error opening socket.");
    }

    // Setting 2nd line argument to equal host ip
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Error. Host does not exist.");
    }


    // Clear and set server address
    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy((char *) &server_addr.sin_addr.s_addr, (char *) server->h_addr_list[0], server->h_length);
    // Set server port
    server_addr.sin_port = htons(PORT);

    // Step 2
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        error("Connection failed.");
    }

    while(1)
    {
        memset(buffer, 0, CHARNUM);
        fgets(buffer, CHARNUM, stdin);
        if (strncmp(buffer, ":exit", 5) == 0)
        {
            break;
        }
        strcat(fnl, buffer);
        if (send(sockfd, fnl, strlen(fnl), 0) < 0)
        {
            perror("Writing failed");
        }

        if (recv(sockfd, fnl, strlen(fnl), 0) < 0)
        {
            perror("Reading failed");
        }
        puts(fnl);
    }

    close(sockfd);
    
    return 0;
}
