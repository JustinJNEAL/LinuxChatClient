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
#include <pthread.h>

#define CHARNUM 159
char buffer1[CHARNUM];
char buffer2[CHARNUM];
char user[10];
char ExitStr[16];
int sockfd, portNum, n;
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
void* Write(void* arg){
	while (1){	
		char temp[CHARNUM+10];
		stpcpy(temp,user);
		memset(buffer1, 0, sizeof(buffer1));
		fgets(buffer1, sizeof(buffer1), stdin);
		if (strncmp(buffer1, ":exit", 5) == 0)
		{
			break;
		}
		strcat(temp,":");
		strcat(temp,buffer1);
		//stpcpy(buffer1,temp);
		n = send(sockfd, temp, strlen(buffer1), 0);
	
		if (n < 0){
			error("Error writing");
		}
    
		 
    }
    pthread_exit(0);
}
void* Read(void* arg){
	while (1)
    {
        memset(buffer2, 0, sizeof(buffer2));
        n = recv(sockfd, buffer2, sizeof(buffer2), 0);

        if (n < 0)
        {
            error("Error reading");
        }

        printf("%s", buffer2);
		
        int i = strncmp(ExitStr, buffer2, strlen(ExitStr));

        if (i == 0){
            break;
        }
    }
    pthread_exit(0);
}


int main(int argc, char *argv[])
{
    struct sockaddr_in server_addr;
    struct hostent *server;

    
    if (argc < 4)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }

    portNum = atoi(argv[2]);
    stpcpy(user,argv[3]);
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

    stpcpy(ExitStr,user);
	strcat(ExitStr,":exit");
	pthread_t tid[2];

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tid[0], &attr,Write,NULL);
    pthread_create(&tid[1], &attr,Read,NULL);
	int j=0;
	for(j=0;j<2;j++){
		pthread_join(tid[j],NULL);
	}

    close(sockfd);
    
    return 0;
}
