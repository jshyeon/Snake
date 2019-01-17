//Asynchronous Chatting Server
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

/*
Global structure for managing sockfd and its nickname and maybe room number
*/

int main( int argc, char *argv[] ) {
   int sockfd, newsockfd, portno, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int  n, status;
   bool lock = false;
   pid_t pid, child_pid;

   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = 5000;

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);

   clilen = sizeof(cli_addr);
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
   listen(sockfd, 256);

   clilen = sizeof(cli_addr);
   while (1) {
     newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
     pid = fork();
     if (pid == 0) {
	//close(sockfd);		// necessary?
	child_pid = getpid();
	while (1) {
		while (lock == true) {}
		lock = true;
        	bzero(buffer, 256);
		scanf("%s", buffer, 255);	// automatically, NULL is added
		n = strcmp(buffer, "/quit");	// is this right? //
		if (!n) {lock = false; break;}	// is this right? //
        	printf("Write: %s\n", buffer);
        	write(newsockfd, buffer, 256);
		lock = false;
	}
	lock = false;
	printf("Quit\n");
	close(newsockfd);
	exit(0);
     } else if (pid == -1) {
        perror("Fail in fork()");   // basically \n is involved
	exit(0);
     } else {
	while(1) {
		while (lock == true) {}
		lock = true;
		bzero(buffer, 256);
		read(newsockfd, buffer, 255);	// how could i check "read" is failed????? //
////////////////// check "read" is failed or not //////////////////////////////
		n = strcmp(buffer, "/quit");	// is this right? //
		if (!n) {lock = false; break;}	// is this right? //
		printf("Read: $s\n", buffer);
		lock = false;
	}
	lock = false;
	kill(child_pid, SIGKILL);
	close(newsockfd);   // necessary?
     }
   }
   return 0;
}
