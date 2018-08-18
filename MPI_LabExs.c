#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char const *argv[]) {
  /* code */
  int size, rank;   /*number of process, rank of processes*/
  MPI_Init(&argc, &argv);  /*initialize MPI*/
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);  /*recive the ranks*/
  MPI_Comm_size(MPI_COMM_WORLD, &size);  /*recive the number of processes*/

  
  return 0;
}
