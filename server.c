#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define MAXBUF 256

void *p_accept(void *arg) {
// ref "client_sockfd = accept(server_sockfd, (struct sockaddr *) &clientaddr, &client_len);"
	arg.client_sockfd = accept(arg.server_sockfd, arg.clientaddr, arg.client_len);

	return (void *)arg.client_sockfd;	// I will not use this return
}

struct args {
	int client_sockfd;
	int server_sockfd;
	sockaddr *clientaddr;
	int *client_len;
}

int main(int argc, char **argv)
{
    int server_sockfd, client1_sockfd, client2_sockfd;
    int client_len;
    int n;			// return value for several function
    char buf[MAXBUF];
    struct sockaddr_in clientaddr, serveraddr;
    bool lock;
    pthread_t p1 = -10, p2 = -10;

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
	if(p1 == -10) {
		args args_p1;
		args_p1.client_sockfd = client1_sockfd;
		args_p1.server_sockfd = server_sockfd;
		args_p1.clientaddr = &clientaddr;
		args_p1.client_len = &client_len;
		n = pthread_create(&p1, NULL, p_accept, &args_p1);
		// automatically p1 is saved
		
		if (args_p1.client_sockfd == -1) {
			perror("accept error\n");
			continue;
		}
		else {
			memset(buf, 0x00, MAXBUF);
			strcpy(buf, "Connected!\n");
			write(args_p1.client_sockfd, buf, MAXBUF);
		}

		memset(buf, 0x00, MAXBUF);
		strcpy(buf, "wait for player 2");
		write(args_p1.client_sockfd, buf, MAXBUF);

		continue;
	} 
	else if(p1 != -10 && p2 == -10) {
		args args_p2;
		args_p2.client_sockfd = client2_sockfd;
		args_p2.server_sockfd = server_sockfd;
		args_p2.clientaddr = &clientaddr;
		args_p2.client_len = &client_len;
		n = pthread_create(&p2, NULL, p_accept, &args_p2);

		if (args_p1.client_sockfd == -1) {
			perror("accept error\n");
			continue;
		}
		else {
			memset(buf, 0x00, MAXBUF);
			strcpy(buf, "Connected!\n");
			write(args_p1.client_sockfd, buf, MAXBUF);
		}

		continue;
	} 
	else {
		// n = read(client_sockfd, buf, MAXBUF);
		// write(client1_sockfd, buf, MAXBUF);

		/// network communication ///

	}
		close(client1_sockfd);
		close(client2_sockfd);
		close(server_sockfd);
		break;
    }
    return 0;
}
