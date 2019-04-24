/*
* To run server, first compile the program then run this command in the terminal:
*   ./server [PORT]
*
*/

#include<stdio.h>   // Input & output
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define CHARNUM 159
#define MAX_CLIENT 5

// Error function that takes message as parameter
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    // If port number is not provided, print error and exit program
    if (argc < 2)
    {
        // Print no port number error
        fprintf(stderr, "Port number not provided. Program terminated.\n");
        exit(1);
    }

    int sockfd, newsockfd, portNum, n;
    char buffer[CHARNUM];

    struct sockaddr_in server_addr, client_addr;
    socklen_t clientLen;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        // Print socket connection error
        error("Error opening socket.");
    }

    memset((char *) &server_addr, 0, sizeof(server_addr));
    portNum = atoi(argv[1]);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(portNum);

    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        // Print binding error
        error("Binding failed.");
    }
    
    // Listen for connection, allow maximum of 5 clients
    listen(sockfd, MAX_CLIENT);
    clientLen = sizeof(client_addr);
    // Lets users see that the server is on and listening
    puts("Waiting for incoming connections...");

    newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &clientLen);

    if (newsockfd < 0)
    {
        // Print accepting error
        error("Error accepting.");
    }

    while (1)
    {
        // Clear buffer
        memset(buffer, 0, sizeof(buffer));
        n = recv(newsockfd, buffer, sizeof(buffer), 0);

        if (n < 0)
        {
            // Print reading error
            error("Error reading.");
        }

        printf("Client: %s\n", buffer);
        // Clear buffer
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);

        n = send(newsockfd, buffer,strlen(buffer), 0);

        if (n < 0)
        {
            // Print write error
            error("Error writing.");
        }

        // Initialize i to compare the first 6 bytes in buffer to ':exit'
        int i = strncmp(":exit", buffer, 5);

        // If i == 0, end connection
        if (i == 0)
            break;
        
    }

    close(newsockfd);
    close(sockfd);
    
    return 0;
}
