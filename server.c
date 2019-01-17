//Multiuser Chatting Server
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

/*
Global structure for managing sockfd and its nickname and maybe room number
*/

int main( int argc, char *argv[] ) {
   int sockfd, newsockfd, portno, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int  n;
   pid_t pid;

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
   // TODO: Accept actual connection from the client
   clilen = sizeof(cli_addr);
   while (1) {
     newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
     pid = fork();
     if (pid == 0) {
       close(sockfd);
       while (1) {
         bzero(buffer, 256);
         read(newsockfd, buffer, 256);
         printf("Client said: %s\n", buffer);
         write(newsockfd, buffer, 256);

         n = strcmp(buffer, "/quit");
         if (!n) {

       }
       close(newsockfd);
       exit(0);
     } else if (pid == -1) {
        perror("Fail in fork()");   // basically \n is involved
     } else {
        close(newsockfd);   // necessary?
        continue;
     }
   }
   return 0;
}
