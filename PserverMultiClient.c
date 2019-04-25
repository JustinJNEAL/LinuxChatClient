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
#include <pthread.h>

#define CHARNUM 159
#define MAX_CLIENT 5

// Error function that takes message as parameter
char buffer1[CHARNUM];
char buffer2[CHARNUM];
int sockfd, portNum, n,newsockfd;
void error(const char *msg){
    perror(msg);
    exit(1);
}
void* Write(void* arg){
	while (1){	
		memset(buffer1, 0, sizeof(buffer1));
		fgets(buffer1, sizeof(buffer1), stdin);
		n = send(arg, buffer1, strlen(buffer1), 0);
	
		if (n < 0){
			error("Error writing");
		}
    
		int i = strncmp(":exit", buffer1, 6);

		if (i == 0){
			break;
		}    
    }
    pthread_exit(0);
}
void* Read(void* arg){
	while (1)
    {
        memset(buffer2, 0, sizeof(buffer2));
        n = recv(arg, buffer2, sizeof(buffer2), 0);

        if (n < 0)
        {
            error("Error reading");
        }

        //printf("Server: %s", buffer2);

        int i = strncmp(":exit", buffer2, 6);

        if (i == 0){
            break;
        }
    }
    pthread_exit(0);
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
	pthread_t tid[MAX_CLIENT*2];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int i=0;
    for(i=0;i<MAX_CLIENT*2;i+=2){
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

		pthread_create(&tid[i], &attr,Write,newsockfd);
		pthread_create(&tid[i+1], &attr,Read,newsockfd);
	}
	int j=0;
	for(j=0;j<MAX_CLIENT*2;j++){
		pthread_join(tid[j],NULL);
	}
    close(newsockfd);
    close(sockfd);
    
    return 0;
}