/*
* To run server, first compile the program then run this command in the terminal:
*   ./server
*
*/

#include <stdio.h>   // Input & output
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define CHARNUM 160
#define USRCAP 10
#define MAX_CLIENT 5
#define PORT 3333

// Error function that takes warning message as parameter
void error(const char *warning)
{
    perror(warning);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, yes = 1, recvSize;
    char buffer[CHARNUM], username[USRCAP], fnl[2000], usrGreeting[40];
    struct sockaddr_in server_addr, client_addr;
    socklen_t clientLen;

    // Step 1
    // Create socket
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        // Socket connection error
        error("Error opening socket.");
    }
    else if (sockfd > 0)
    {
        // Socket connection success
        puts("... Socket created");
    }

    // Clear server address
    memset((char *) &server_addr, 0, sizeof(server_addr));
    // Declare server address fields
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Step 2
    // Set socket option (for reuse of port immediately after disconnection)
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        error("Binding failed.");
    }
    else
    {
        puts("... Bind successful");
    }
    
    // Listen for connection, allow maximum of 5 clients
    listen(sockfd, MAX_CLIENT);
    // Lets users see that the server is on and listening
    puts("Waiting for incoming connections...");
    
    clientLen = sizeof(client_addr);
    // Step 3
    // Accept new connections
    newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &clientLen);
    if (newsockfd < 0)
    {
        error("Connection could not be established.");
    }
    else
    {
        puts("...Connection established");
    }

    memset(fnl, 0, sizeof(fnl));
    while((recvSize = recv(newsockfd, fnl, sizeof(fnl), 0)) > 0)
    {
        if (send(newsockfd, fnl, strlen(fnl), 0) < 0)
        {
            perror("Writing failed");
        }
    }
    

   
    close(newsockfd);
    close(sockfd);
    
    return 0;
}

