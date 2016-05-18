#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

void error(const char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {
  struct sockaddr_in serv_addr;
  struct hostent *server;
  int sockfd, portno, rc;
  char buffer[256];
  char filename[40];

  if (argc != 3) {
    fprintf(stderr,"usage: %s <hostname> <port-no>\n", argv[0]);
    exit(0);
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    error("ERROR opening socket");
  }

  server = gethostbyname(argv[1]);
  portno = atoi(argv[2]);
  if (server == NULL) {
    error("ERROR, no such host\n");
    exit(1);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(portno);
  bcopy((char*) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);

  if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
    error("ERROR connecting");
  } else {
    printf("Connected!\n");
  }

  printf("Emri i file-t: ");
  // fgets(buffer, 255, stdin);
  scanf("%s", filename);
  // rc = send(sockfd, buffer, strlen(buffer), 0);
  rc = send(sockfd, filename, strlen(filename), 0);
  if (rc < 0) {
    error("error writing to socket");
  }

  bzero(buffer, 256);
  rc = recv(sockfd, buffer, 255, 0);
  if (rc < 0) {
    error("ERROR reading from socket");
  } else {
    printf("Content of file:\n====\n%s\n====\nEND\n", buffer);
  }

  // close(sockfd);
  return 0;
}
