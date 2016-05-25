#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define MAX_LEN 256

int main(int argc, char **argv) {
    int rank, size;
    FILE *fp;
    char message[MAX_LEN];

    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        strcpy(message, argv[1]);
        MPI_Bcast(message, MAX_LEN, MPI_CHAR, 0, MPI_COMM_WORLD);
    } else {
        MPI_Bcast(message, MAX_LEN, MPI_CHAR, 0, MPI_COMM_WORLD);
        char filename[MAX_LEN];
        sprintf(filename, "%d/%s", rank, message);
        // printf("read %s\n\n", filename);
        fp = fopen(filename, "r");
        if (fp != NULL) {
            char content[MAX_LEN] = "";
            char ch;
            do {
                ch = fgetc(fp);
                printf("%c", ch);
                // sprintf(content, "%s%c", content, ch);
            } while (ch != EOF);

            // fgets(content, MAX_LEN, fp);
            // printf("Message from process %d:\n%s\n#### END PROCESS ####\n\n", rank, content);
            // printf("%s", content);
        }
    }

  MPI_Finalize();

  return 0;
}
