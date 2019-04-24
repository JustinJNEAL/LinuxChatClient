/*
* ./client [server ip] [PORT]
* 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define CHARNUM 159

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portNum, n;
    struct sockaddr_in server_addr;
    struct hostent *server;

    char buffer[CHARNUM];
    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }

    portNum = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Error opening socket.");
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Error. Host does not exist.");
    }

    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy((char *) &server_addr.sin_addr.s_addr, (char *) server->h_addr_list[0], server->h_length);
    server_addr.sin_port = htons(portNum);
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        error("Connection failed");
    }

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);
        n = send(sockfd, buffer, strlen(buffer), 0);

        if (n < 0)
        {
            error("Error writing");
        }
        
        memset(buffer, 0, sizeof(buffer));
        n = recv(sockfd, buffer, sizeof(buffer), 0);

        if (n < 0)
        {
            error("Error reading");
        }

        printf("Server: %s", buffer);

        int i = strncmp(":exit", buffer, 6);

        if (i == 0)
            break;
        
    }

    close(sockfd);
    
    return 0;
}
