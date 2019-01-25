//Multiuser Chatting Client
#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

int main(int argc, char *argv[]) {
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[256];

  if (argc!=4) {
    printf("Usage: %s <chatting server address> <room number> <nickname>\n",argv[0]);
    return -1;
  }
  /*
  argv[1] = server address
  argv[2] = room number
  argv[3] = nickname
  */

// assign server addr and portno
  char* token;
  int rnum = atoi(argv[2]);
  char* nickname;
  nickname = (char*)malloc(sizeof(argv[3]));
  strcpy(nickname, argv[3]);


  token = strtok(argv[1], ":");
  server = gethostbyname(token);

  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  token = strtok(NULL,":");

  if (token == NULL) {
    printf("Error: port number miss\n");
    return -1;
  }
  portno = atoi(token);

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);

  // TODO : create socket and get file descriptor
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  // TODO : connect to server with server address which is set above (serv_addr)
  if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0) {
    printf("Error: connection fail\n");
    return -1;
  }

  // bzero(buffer, 256);
  // // write something to the buffer
  
  // write(sockfd, buffer, 256);
  // // TODO : inside this while loop, implement communicating with read/write or send/recv function
  while (1) {
    printf("You: ");
    bzero(buffer,256);
    scanf("%s", buffer); // this does not allow blank char
    write(sockfd, buffer, 256);
    n = strcmp(buffer, "/quit");
    if (!n)
      break;
    read(sockfd, buffer, 256);
    printf("Opponent: %s\n", buffer);

    // ==> here
    //write(sockfd, buffer, 256);


/* # TODO leave message
     if first string = "Good bye"
     close connection

    n = strcmp(buffer, );

    if (!n)
      break;
*/
  }
  close(sockfd);
  return 0;
}
