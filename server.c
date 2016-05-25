#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <string.h>

void error(const char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {
  int sockfd, newsockfd, portno, clilen, rc;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  FILE *fp;
  char command[256];
  char message[256] = "";
  char *filename;

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
  newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);
  if (newsockfd < 0) {
    printf("\tError on accept\n");
    exit(4);
  } else {
    printf("CLIENT CONNECTED\n\n");
  }

  n = recv(newsockfd, buffer, 255, 0);
  if (n < 0) {
    error("ERROR reading from socket");
  }

  buffer[n] = '\0';
  strcpy(command, "mpirun -n 5 search ");
  strcat(command, buffer);
  printf("%s\n", command);
  fp = popen(command, "r");

  if (fp != NULL) {
    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        sprintf(message, "%s%c", message, ch);
    }
    // fgets(message, 255, fp);
    if (strlen(message) == 0 ) {
      printf("message length: %d", ((int)strlen(message)));
      strcpy(message, "File doesn't exist");
    }
  } else {
    strcpy(message, "Some error occurred while searching");
  }

  rc = send(newsockfd, message, strlen(message), 0);
  if (rc < 0) {
    error("error writing to socket");
  }

  // close(newsockfd);
  // close(sockfd);
  return 0;
}
