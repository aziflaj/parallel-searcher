#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <string.h>

void error(const char *msg){
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {
  int sockfd, newsockfd, portno, clilen, rc;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;

  if (argc != 2) {
    printf("\tusage: %s <port>\n", argv[0]);
    exit(1);
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    printf("\tError opening socket\n");
    exit(2);
  }

  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
    printf("\tError on binding\n");
    exit(3);
  }

  listen(sockfd, 5);

  clilen = sizeof(cli_addr);
  printf("clilen: %d\n", clilen);
  newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);
  if (newsockfd < 0) {
    printf("\tError on accept\n");
    exit(4);
  } else {
    printf("CLIENT CONNECTED\n\n");
  }

  while (1) {
    n = recv(newsockfd, buffer, 255, 0);
    if (n < 0) {
      error("ERROR reading from socket");
    }

    // if (!strcmp(buffer,"hello")) {
    //   printf("hello back!\n");
    // }

    printf("Client says: %s (%ld)\n", buffer, strlen(buffer));
  }

  close(newsockfd);
  close(sockfd);
  return 0;
}
