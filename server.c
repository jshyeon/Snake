#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define MAXBUF 1024	// same length with client
int main(int argc, char **argv)
{
    int server_sockfd, client1_sockfd, client2_sockfd;
    int client_len, n, status, waiting;
    char buf[MAXBUF];
    char *quit = "quit";
    struct sockaddr_in clientaddr, serveraddr;
    pid_t p1 = 100, p2 = 100;

    client_len = sizeof(clientaddr);

    if ((server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        perror("socket error : ");
        exit(0);
    }
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(atoi(argv[1]));

    bind (server_sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(server_sockfd, 5);

    while(1) {
		if (p1 == 100) {client1_sockfd = accept(server_sockfd, (struct sockaddr *) &clientaddr, &client_len);}
		else if (p1 != 100 && p2 == 100) {client2_sockfd = accept(server_sockfd, (struct sockaddr *) &clientaddr, &client_len);}
		else {
			waitpid(p1, &stautus, 0); 
			waitpid(p2, &stautus, 0);
			close(server_sockfd);
			return 0;
		}

		pid = fork();
		if (pid == 0) {
			if (p1 == 100) {
				p1 = getpid();
				write(client1_sockfd, buf, MAXBUF);	// waiting message
				while (lock != 1) {}			// waiting
			}
			else {
				p2 = getpid();
				lock = 1;
				
			}
		} else if (pid == -1) {
			perror("fork() error : ");
			continue;
		} else {
			// close(server_sockfd);
			continue;
		}
    }
    return 0;
}
/*///////////////////////////////////////
	        memset(buf, 0x00, MAXBUF);
	        printf("New Client Connect: %s\n", inet_ntoa(clientaddr.sin_addr));
	
	        if (n = read(client_sockfd, buf, MAXBUF)) {
printf("READ\n");
		if (n <= 0) {
                	close(client_sockfd);
                	continue;
		}
            	if (strcmp(buf, quit) == 0) {close(client_sockfd);}
            	else {printf("read : %s", buf);}
		
		memset(buf, 0x00, MAXBUF);
        }

        else if (scanf("%1024s", buf)) {
printf("WRITE\n");
		n = write(client_sockfd, buf, MAXBUF);
		if (n <= 0) {
			perror("write error : ");
        		close(client_sockfd);
		}
		if (strcmp(buf, quit) == 0) {close(client_sockfd);}
 		else {printf("write : %s", buf);}
		
		memset(buf, 0x00, MAXBUF);
	}
*////////////////////////////////////////
