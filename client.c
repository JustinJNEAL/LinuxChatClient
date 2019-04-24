/*
* ./client [host ip] [username]
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
/*
void * createSocket(int fd)
{
    fd = socket(PF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        error("Error opening socket.");
    }
    return 0;
}
*/
void * connectServer(int sockfd, struct sockaddr_in *s_address)
{
    if ((connect(sockfd, (struct sockaddr *) s_address, sizeof(*s_address))) < 0)
    {
        error("Connection failed.");
    }
    else
    {
        puts("Connected.");
    }
}

void * recvMessage(int sockfd, char *response)
{
    int recvSize;
    char buffer[CHARNUM];
    memset(buffer, 0, CHARNUM);

    while(1)
    {
        recvSize = recv(sockfd, buffer, CHARNUM, 0);
        if (recvSize < 0)
        {
            error("Error reading");
        }
        else if (recvSize == 0)
        {
            puts("Client disconnect.");
        }
        else
        {
            printf("\nServer: %s", buffer);
            printf("%s", response);
            fflush(stdout);
        }
    }
    return 0;
}

void * sendMessage(char s_response[1000], int sockfd, struct sockaddr_in *s_address)
{
    printf("%s", s_response);
    char buffer[CHARNUM], fnl[2000];

    while(fgets(buffer, CHARNUM, stdin) != NULL)
    {
        memset(fnl, 0, 2000);
        strcat(fnl, s_response);
        strcat(fnl, buffer);
        printf("\n%s", s_response);

        if (strncmp(buffer, ":exit", 5) == 0) {
            error("Disconnected");
        }

        send(sockfd, fnl, strlen(fnl) + 1, 0);
    }
    return 0;
}


int main(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char buffer[CHARNUM], username[USRCAP], s_response[1000], fnl[2000];

    // Check for correct amount of line arguments
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s [HOST IP] [USERNAME]\n", argv[0]);
        exit(1);
    }

    // Checks if username is too long
    if (strlen(argv[2]) >= sizeof(username))
    {
        error("Username is too long.");
    }
    strcpy(username, argv[2]);

    username[strlen(username) - 1] = 0;
    strncpy(s_response, username, strlen(username));
    strcat(s_response, ": ");

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
    else
    {
        puts("Connected.");
    }

    send(sockfd, username, strlen(username), 0);

    while(1)
    {
        memset(buffer, 0, CHARNUM);
        memset(fnl, 0, strlen(fnl));
        fgets(buffer, CHARNUM, stdin);
        if (strncmp(buffer, ":exit", 5) == 0)
        {
            break;
        }
        strcat(fnl, s_response);
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
