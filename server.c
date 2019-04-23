/*
* To run server, first compile the program then run this command in the terminal:
*   ./server [PORT]
*
*/

#include <stdio.h>   // Input & output
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define CHARNUM 159
#define USRCAP 10
#define MAX_CLIENT 5
#define PORT 8080

// Error function that takes message as parameter
void error(const char *warning)
{
    perror(warning);
    exit(1);
}

void *handler(void *);


int main(int argc, char *argv[])
{
    int sockfd, newsockfd, *sock, n, yes = 1;
    char buffer[CHARNUM], username[USRCAP];
    struct sockaddr_in server_addr, client_addr;
    socklen_t clientLen;

    // Create socket
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        // Print socket connection error
        error("Error opening socket.");
    }
    puts("...Socket created");

    memset((char *) &server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        // Print binding error
        error("Binding failed.");
    }
    puts("...Bind successful");
    
    // Listen for connection, allow maximum of 5 clients
    listen(sockfd, MAX_CLIENT);
    clientLen = sizeof(client_addr);
    // Lets users see that the server is on and listening
    puts("Waiting for incoming connections...");
    
    while ((newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, (socklen_t*)&clientLen)) > 0)
    {
        puts("...Connection established");
        
        int fd = *(int*)sockfd;
    int recvSize;
    char buffer[CHARNUM], username[USRCAP], *msg, usrGreeting[31], s_response[1000];
    
    memset(username, '\0', sizeof(username));
    if ((recv(fd, username, sizeof(username), 0)) < 0)
    {
        error("Error reading.");
    }
    
    memset(usrGreeting, '\0', sizeof(usrGreeting));
    
    
  

    msg = "Now go on and chat to your heart's content!\n";
    send(fd, msg, strlen(msg), 0);

    // Clear buffer
    
    // Recieve buffer

    while ((recvSize = recv(fd, buffer, sizeof(buffer), 0)) > 0)
    {
        // Concatenate fbuffer
        memset(s_response, '\0', sizeof(s_response));
        strncpy(s_response, username, strlen(username));
        strncat(s_response, ": ", strlen(": "));
        strncat(s_response, buffer, sizeof(buffer));

        // Send response to client
        send(fd, s_response, strlen(s_response), 0);

        // Clear buffer
        memset(buffer, '\0', sizeof(buffer));
        memset(s_response, '\0', sizeof(s_response));
        
        // Initialize i to compare the first 5 bytes in buffer to ':exit'
        int i = strncmp(":exit", buffer, 5);
        // If i == 0, end connection
        if (recvSize == 0 || i == 0)
        {
            printf("%s has left the chat\n", username);
        }
    
    }
    
    close(fd);
    free(sockfd);
    }
    
    return 0;
}

void * sendGreeting(int fd, char usr)
{
    char usrGreeting[31];

    strncpy(usrGreeting, "Welcome ", 8);
    strncat(usrGreeting, usr, strlen(usr));
    strncat(usrGreeting, " to the chat!", sizeof(" to the chat!"));
    if (send(fd, usrGreeting, sizeof(usrGreeting), 0) < 0)
    {
        error("Error writing");
    }
}

void *handler(void *sockfd)
{
    int fd = *(int*)sockfd;
    int recvSize;
    char buffer[CHARNUM], username[USRCAP], *msg, usrGreeting[31], s_response[1000];
    
    memset(username, '\0', sizeof(username));
    if ((recv(fd, username, sizeof(username), 0)) < 0)
    {
        error("Error reading.");
    }
    
    memset(usrGreeting, '\0', sizeof(usrGreeting));
    
    
  

    msg = "Now go on and chat to your heart's content!\n";
    send(fd, msg, strlen(msg), 0);

    // Clear buffer
    
    // Recieve buffer

    while ((recvSize = recv(fd, buffer, sizeof(buffer), 0)) > 0)
    {
        // Concatenate fbuffer
        memset(s_response, '\0', sizeof(s_response));
        strncpy(s_response, username, strlen(username));
        strncat(s_response, ": ", strlen(": "));
        strncat(s_response, buffer, sizeof(buffer));

        // Send response to client
        send(fd, s_response, strlen(s_response), 0);

        // Clear buffer
        memset(buffer, '\0', sizeof(buffer));
        memset(s_response, '\0', sizeof(s_response));
        
        // Initialize i to compare the first 5 bytes in buffer to ':exit'
        int i = strncmp(":exit", buffer, 5);
        // If i == 0, end connection
        if (recvSize == 0 || i == 0)
        {
            printf("%s has left the chat\n", username);
        }
    
    }
    
    close(fd);
    free(sockfd);

    return 0;
}




