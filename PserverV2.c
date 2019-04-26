#include <stdio.h>   // Input & output
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define CHARNUM 160
#define MAX_MAX_CLIENT 25
#define PORT 3333

// Error function that takes message as parameter
char buffer1[CHARNUM];
int MAX_CLIENT=5;
int sockfd, n, newsockfd, temp, sockhd[MAX_MAX_CLIENT], CountOnlineUser;

void error(const char *warning){
    perror(warning);
    exit(1);
}

void* Write(int* arg){
    while (1){
	int* temp=(int*) arg;
	int newTemp=*temp;
	if ((temp = recv(sockhd[newTemp], buffer1, sizeof(buffer1), 0)) < 0){
	        perror("Error reading");
	}
	int j = 0;
	for(j = 0; j < CountOnlineUser; j++){
		
		if (send(sockhd[j], buffer1, strlen(buffer1), 0) < 0){
			perror("Error writing");
		}
    	}	   
    }
    pthread_exit(0);
}


int main(int argc, char *argv[]){
	if(argc==2){
		if(argv[1]<=MAX_MAX_CLIENT){
			MAX_CLIENT=argv[1];
		}else if(argv[1]>MAX_MAX_CLIENT){
			
		}
	}
	struct sockaddr_in server_addr, client_addr;
    	socklen_t clientLen;

    	// Create socket
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    	if (sockfd < 0){
        	// Print socket connection error
        	error("Error opening socket.");
    	}

    	memset((char *) &server_addr, 0, sizeof(server_addr));
    	server_addr.sin_family = AF_INET;
    	server_addr.sin_addr.s_addr = INADDR_ANY;
    	server_addr.sin_port = htons(PORT);

    	if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
        	// Print binding error
        	error("Binding failed");
    	}

	pthread_t tid[MAX_CLIENT];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	//listen(sockfd, MAX_CLIENT);
	// Lets users see that the server is on and listening
	//puts("Waiting for incoming connections...");
	//clientLen = sizeof(client_addr);
    	CountOnlineUser=0;
    	for(int i = 0; i < MAX_CLIENT; i++){
		// Listen for connection, allow maximum of 5 clients
		listen(sockfd, MAX_CLIENT);
		clientLen = sizeof(client_addr);

		newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &clientLen);
		sockhd[i]=newsockfd;
		
		if (newsockfd < 0){
			// Print accepting error
			error("Error accepting.");
		}
		
		pthread_create(&tid[i], &attr,Write,&i);
		CountOnlineUser++;	    
	}
	int j=0;
  	for(j=0;j<MAX_CLIENT;j++){
		pthread_join(tid[j],NULL);
    	}
	close(newsockfd);
	close(sockfd);
    
	return 0;
}
