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
#include <arpa/inet.h>
#include <netdb.h>

#define CHARNUM 159
#define USRCAP 10
#define PORT 8080

void error(const char *warning)
{
    perror(warning);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portNum, n;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char buffer[CHARNUM], username[USRCAP], s_response[1000];

    if (argc < 2)
    {
        fprintf(stderr, "Usage %s [HOST IP]\n", argv[0]);
        exit(1);
    }

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
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
    server_addr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        error("Connection failed");
    }

    printf("Who are you?\n");
    memset(username, '\0', sizeof(username));
    fgets(username, sizeof(username), stdin);
    if ((send(sockfd, username, strlen(username), 0)) < 0)
    {
        error("Error writing.");
    }  
    
    while (1)
    {
        memset(buffer, '\0', sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);
        memset(s_response, '\0', sizeof(s_response));

        if ((send(sockfd, buffer, strlen(buffer), 0)) < 0)
        {
            error("Error writing.");
        }

        if ((recv(sockfd, s_response, sizeof(s_response), 0)) < 0)
        {
            error("Error reading.");
        }

        printf("%s: %s", username, s_response);

        int i = strncmp(":exit", buffer, 5);
        if (i == 0)
            break;
        
    }

    close(sockfd);
    
    return 0;
}
