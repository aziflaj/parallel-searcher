#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAX_LEN 256

int main(int argc, char **argv) {
    int rank, size;

    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        int sockfd, newsockfd, clilen, rc, portno=8888;
        char buffer[MAX_LEN], message[MAX_LEN];
        struct sockaddr_in serv_addr, cli_addr;
        int msglen;

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            printf("\tError opening socket\n");
            exit(1);
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(portno);

        if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
            printf("\tError binding on port %d\n", portno);
            exit(1);
        }

        listen(sockfd, size - 1);

        MPI_Bcast("server ready", strlen("server ready"), MPI_CHAR, 0, MPI_COMM_WORLD);

        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);
        if (newsockfd < 0) {
            printf("\tError on accept\n");
            exit(1);
        }

        msglen = recv(newsockfd, buffer, MAX_LEN - 1, 0);
        if (msglen < 0) {
            printf("ERROR reading from socket");
            exit(1);
        }

        buffer[msglen] = '\0';
        printf("content: %s\n", buffer);
    } else {
        char filename[MAX_LEN], message[MAX_LEN], buffer[MAX_LEN];
        struct sockaddr_in serv_addr;
        struct hostent *server;
        int sockfd, rc, portno = 8888;;

        // wait for the server to be ready
        MPI_Bcast(filename, MAX_LEN, MPI_CHAR, 0, MPI_COMM_WORLD);
        sprintf(filename, "%d/%s", rank, argv[1]);

        FILE *fp = fopen(filename, "r");
        if (fp != NULL) {
            char content[MAX_LEN];
            fgets(content, MAX_LEN, fp);

            // open connection to the server process
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) {
                printf("ERROR opening socket");
                exit(1);
            }

            server = gethostbyname("localhost");
            if (server == NULL) {
                printf("ERROR, no such host\n");
                exit(1);
            }

            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(portno);

            bcopy((char*) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);
            if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
                error("ERROR connecting");
            }

            rc = send(sockfd, content, strlen(content), 0);
            if (rc < 0) {
                error("error writing to socket");
            }
        }
    }

    MPI_Finalize();
    return 0;
}
