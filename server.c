// sholud put '-pthread' option in compile
// ex) gcc -pthread -o a a.c

#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define MAXBUF 256

short lock;
int client1_sockfd, 
    client2_sockfd;

struct args {
	int client_sockfd;
	int server_sockfd;
	struct sockaddr *clientaddr;
	int *client_len;
};

void *p_accept(void *argument) {
	struct args arg = *(struct args *) argument;
	char buf[256];
	int n;
	short player_name;

	// pthread_detach(pthead_self()); // make this thread independent
	arg.client_sockfd = accept(arg.server_sockfd, arg.clientaddr, arg.client_len);
	printf("client_sockfd = %d is connected\n", arg.client_sockfd);
	lock = 0;

	if (client1_sockfd == -10) {
		client1_sockfd == arg.client_sockfd;
		player_name = 1;
	} else if (client1_sockfd != -10 && client2_sockfd == -10) {
		client2_sockfd == arg.client_sockfd;
		player_name = 2;
	} else {}

	while(1) {
		if (buf[0] != '$') {	/*case1*/
			memset(buf, 0x00, 256);
			read(arg.client_sockfd, buf, 255);	// 255? 256?
			
			n = strcmp(buf, "/quit");	// this is the problem;
			if (!n) {break;}		// this is the problem;
			
			memset(buf, 0x00, MAXBUF);
			strcpy(buf, "Player Connected!\n");
			write(arg.client_sockfd, buf, MAXBUF);
		} else {		/*case2*/
			memset(buf, 0x00, 256);
			read(arg.client_sockfd, buf, 255);	// 255? 256?
			if (player_name == 1) {
				write(client2_sockfd, buf, MAXBUF);
			}
			else {
				write(client1_sockfd, buf, MAXBUF);
			}
			
			n = strcmp(buf, "/quit");	// this is the problem;
			if (!n) {break;}		// this is the problem;
		}
	}
	close(arg.client_sockfd);
	pthread_exit((void *) 0);
	return argument;
}

int main(int argc, char **argv)
{
    int server_sockfd, client1_sockfd, client2_sockfd, client_len;
    int n, status1, status2;
    char buf[MAXBUF];
    struct sockaddr_in clientaddr, serveraddr;
    struct args args_p1, args_p2;
    pthread_t p1 = -10, p2 = -10;
    client1_sockfd = -10;
    client2_sockfd = -10;

    client_len = sizeof(clientaddr);
    if ((server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
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
		args_p1.client_sockfd = client1_sockfd;
		args_p1.server_sockfd = server_sockfd;
		args_p1.clientaddr = &clientaddr;	// what warning?
		args_p1.client_len = &client_len;

		lock = 1;

		n = pthread_create(&p1, NULL, p_accept, &args_p1);
		
		while(lock == 1) {
			sleep(3);
			printf("player1 no connection... \n");
		}
		printf("player1 connected\n");
		
		continue;
	} 
	else if(p1 != -10 && p2 == -10) {
		args_p2.client_sockfd = client2_sockfd;
		args_p2.server_sockfd = server_sockfd;
		args_p2.clientaddr = &clientaddr;	// what warning?
		args_p2.client_len = &client_len;

		lock = 1;

		n = pthread_create(&p2, NULL, p_accept, &args_p2);

		while(lock == 1) {
			sleep(3);
			printf("player2 no connection... \n");
		}
		printf("player2 connected\n");
		
		continue;
	} 
	else {}

	pthread_join(p1, (void **)&status1);
	pthread_join(p2, (void **)&status2);

printf("client1_sockfd = %d / client2_socfd = %d\n", client1_sockfd, client2_sockfd);
	printf("Thread End %d %d\n", status1, status2);
	close(server_sockfd);
	break;
    }
    return 0;
}
