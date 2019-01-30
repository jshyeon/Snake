// give time term same with client snake moving term
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
#include <time.h>

#define MAXBUF 256

short input_lock[2];
short lock;
char seed;
int X1, X2, Y1, Y2;
int client1_sockfd = -10, 
    client2_sockfd = -10;

struct args {
	int client_sockfd;
	int server_sockfd;
	struct sockaddr *clientaddr;
	int *client_len;
};

void *itoa (int i, char *a) {
	int iter = 0;

	while (iter < 3) {
		a[2-iter] = i % 10 + 48;
		i /= 10;
		iter++;
	}
}

void *p_accept(void *argument) {
	struct args arg = *(struct args *) argument;
	char buf[256], temp;
	char *cx1, *cx2, *cy1, *cy2;
	int n;
	short player_name;

	arg.client_sockfd = accept(arg.server_sockfd, arg.clientaddr, arg.client_len);
	printf("client_sockfd = %d is connected\n", arg.client_sockfd);
	lock = 0;

	if (client1_sockfd == -10) {
		client1_sockfd = arg.client_sockfd;
		player_name = 1;
	} else if (client1_sockfd != -10 && client2_sockfd == -10) {
		client2_sockfd = arg.client_sockfd;
		player_name = 2;
	} else {}

	memset(buf, 0x00, 256);
	strncpy(buf, &seed, sizeof(char));
	write(arg.client_sockfd, buf, MAXBUF);

        if (player_name == 1) {
                temp = '1';
                memset(buf, 0x00, 256);
                strncpy(buf, &temp, sizeof(char));
                write(arg.client_sockfd, buf, MAXBUF);
                while(1) {
                        memset(buf, 0x00, 256);
                        read(arg.client_sockfd, buf, 255);

                        n = strcmp(buf, "/quit");       // this could be the problem;
                        if (!n) {break;}                // this could be the problem;

			switch (buf[0]) {
				case 'p':
					X1 += 20; break;
				case 'm':
					X1 -= 20; break;
				default :
					break;

			}
			switch (buf[1]) {
				case 'p':
					Y1 += 20; break;
				case 'm':
					Y1 -= 20; break;
				default :
					break;

			}

			input_lock[0] = 0;
                        while(input_lock[1] == 1) {}
			input_lock[0] = 1;

			itoa(X1, cx1);
			itoa(X1, cy1);
			itoa(Y2, cx2);
			itoa(Y2, cy2);

			strncat(buf, cx2, 3);
			strncat(buf, cy2, 3);
			strncat(buf, cx1, 3);
			strncat(buf, cy1, 3);

			write(client2_sockfd, buf, MAXBUF);
			// maybe need another lock
                }
        } else {
                temp = '2';
                memset(buf, 0x00, 256);
                strncpy(buf, &temp, sizeof(char));
                write(arg.client_sockfd, buf, MAXBUF);
                while(1) {
                        memset(buf, 0x00, 256);
                        read(arg.client_sockfd, buf, 255);

                        n = strcmp(buf, "/quit");       // this could be the problem;
                        if (!n) {break;}                // this could be the problem;
                     
			switch (buf[0]) {
				case 'p':
					X2 += 20; break;
				case 'm':
					X2 -= 20; break;
				default :
					break;

			}
			switch (buf[1]) {
				case 'p':
					Y2 += 20; break;
				case 'm':
					Y2 -= 20; break;
				default :
					break;
			}

			input_lock[1] = 0;
                        while(input_lock[0] == 1) {}
			input_lock[1] = 1;

			itoa(X1, cx1);
			itoa(Y1, cy1);
			itoa(X2, cx2);
			itoa(Y2, cy2);

			strncat(buf, cx1, 3);
			strncat(buf, cy1, 3);
			strncat(buf, cx2, 3);
			strncat(buf, cy2, 3);

			write(client1_sockfd, buf, MAXBUF);
			// maybe need another lock
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
    pthread_t p1 = -11, p2 = -11;

    X1 = 140; Y1 = 140; X2 = 460; Y2 = 460;
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

    srand(time(NULL));
    seed = rand() & 0xFF;

    while(1) {
	if(p1 == -11) {
		args_p1.client_sockfd = client1_sockfd;
		args_p1.server_sockfd = server_sockfd;
		args_p1.clientaddr = &clientaddr;	// what warning?
		args_p1.client_len = &client_len;

		lock = 1;

		n = pthread_create(&p1, NULL, p_accept, &args_p1);
		
		if(n != 0) {
			perror("pthread_create error\n");
			p1 == -11;
		}

		while(lock == 1) {
			sleep(3);
			printf("player1 no connection... \n");
		}
		printf("player1 connected\n");
		
		continue;
	} 
	else if(p1 != -11 && p2 == -11) {
		args_p2.client_sockfd = client2_sockfd;
		args_p2.server_sockfd = server_sockfd;
		args_p2.clientaddr = &clientaddr;	// what warning?
		args_p2.client_len = &client_len;

		lock = 1;

		n = pthread_create(&p2, NULL, p_accept, &args_p2);

		if(n != 0) {
			perror("pthread_create error\n");
			p2 == -11;
		}

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

	printf("Thread End %d %d\n", status1, status2);
	close(server_sockfd);
	break;
    }
    return 0;
}
