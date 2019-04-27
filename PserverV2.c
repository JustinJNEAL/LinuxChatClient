#include <stdio.h> // Input & output
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define CHARNUM 160
#define MAX_CLIENT 5
#define PORT 3333

// Error function that takes message as parameter
char buffer1[CHARNUM];
int sockfd, n, newsockfd, temp, sockhd[MAX_CLIENT], CountOnlineUser;

void error(const char *warning)
{
	perror(warning);
	exit(1);
}

void *Write(int *arg)
{
	int *temp = (int *)arg;
	int newTemp = *temp;

	while (1)
	{

		memset(buffer1, 0, sizeof(buffer1));
		if (recv(sockhd[newTemp - 1], buffer1, sizeof(buffer1), 0) < 0)
		{
			perror("Error reading");
		}
		int j = 0;
		for (j = 0; j < CountOnlineUser; j++)
		{
			//n = send(sockhd[j], buffer1, strlen(buffer1)
			if (send(sockhd[j], buffer1, strlen(buffer1), 0) < 0)
			{
				perror("Error writing");
			}
		}
	}
	pthread_exit(0);
}

int main(int argc, char *argv[])
{
	struct sockaddr_in server_addr, client_addr;
	socklen_t clientLen;

	// Create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		// Print socket connection error
		error("Error opening socket.");
	}

	memset((char *)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	// Set socket option (for reuse of port immediately after disconnection)
	int yes = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		// Print binding error
		error("Binding failed");
	}

	pthread_t tid[MAX_CLIENT];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	// Lets users see that the server is on and listening
	puts("Waiting for incoming connections...");

	for (int i = 0; i < MAX_CLIENT; i++)
	{
		// Listen for connection, allow maximum of 5 clients
		listen(sockfd, MAX_CLIENT);
		clientLen = sizeof(client_addr);
		//long long V=(long long)i;
		newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &clientLen);
		sockhd[i] = newsockfd;

		if (newsockfd < 0)
		{
			// Print accepting error
			error("Error accepting.");
		}

		pthread_create(&tid[i], &attr, Write, &i);
		int *ptr = 100;
		wait(ptr);
		CountOnlineUser++;
	}

	int j = 0;
	for (j = 0; j < MAX_CLIENT; j++)
	{
		pthread_join(tid[j], NULL);
	}

	close(newsockfd);
	close(sockfd);

	return 0;
}
