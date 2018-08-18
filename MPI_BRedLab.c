#include <stdio.h>
#include "mpi.h"
int main(int argc, char *argv[])
{

 int rank, numOfProcesses, x, result;

 MPI_Init(&argc, &argv);

 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 MPI_Comm_size(MPI_COMM_WORLD, &numOfProcesses);

 if(rank == 0)
 {
 x = 2;
 }

 MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);
 x += rank;
 MPI_Reduce(&x, &result, 1, MPI_INT, MPI_SUM, 0,
MPI_COMM_WORLD);
 if(rank == 0)
 {
 printf("Result = %i\n", result);
 }

 MPI_Finalize();

 return 0;
} 
