#include <stdio.h>
#include "mpi.h"
int main( int argc, char **argv )
{
  int isend[6], irecv[2], irecvRoot[100];
  int rank, size, i, portion, pTotal = 0;
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  if(rank == 0){
    for(i=0;i<6;i++) isend[i] = i+1.0;
    portion = 6 / size;
  }
  MPI_Bcast(&portion, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(&isend, portion, MPI_INT, &irecv, portion, MPI_INT, 0,
  MPI_COMM_WORLD);
  for(i = 0; i<portion; i++) {
    pTotal += irecv[i];
  }

  MPI_Gather(&pTotal, 1, MPI_INT, &irecvRoot, 1, MPI_INT, 0,
  MPI_COMM_WORLD);
  if (rank == 0){
    double avg = 0.0;
    for (i = 0; i< size; i++) {
      avg += (double)irecvRoot[i]/(double)size;
      printf("avg : %f and i : %d\n", avg, i);
    }
    printf("The avg is: %f\n", avg);
  }
  MPI_Finalize();
  return 0;
}
