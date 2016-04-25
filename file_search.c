#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
  int rank, size;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    printf("I am (G)root\n");
  } else {
    printf("I am process %d out of %d\n", rank, size);
  }

  MPI_Finalize();
  return 0;
}
