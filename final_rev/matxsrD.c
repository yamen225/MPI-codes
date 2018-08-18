#include <sys/time.h>
#include <stdio.h>
#include "mpi.h"
#define N               4        /* number of rows and columns in matrix */

MPI_Status status;



int **alloc_2d_int(int rows, int cols);

main(int argc, char **argv)
{
  int numtasks,taskid,numworkers,source,dest,rows,offset,i,j,k;

  int **a, **b, **c;

  a = alloc_2d_int(N, N);
  b = alloc_2d_int(N, N);
  c = alloc_2d_int(N, N);

  struct timeval start, stop;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

  numworkers = numtasks-1;

  /*---------------------------- master ----------------------------*/
  if (taskid == 0) {
    for (i=0; i<N; i++) {
      for (j=0; j<N; j++) {
        a[i][j]= 1;
        b[i][j]= 2;
      }
    }



    gettimeofday(&start, 0);

    /* send matrix data to the worker tasks */
    rows = N/numworkers;
    offset = 0;

    for (dest=1; dest<=numworkers; dest++)
    {
      MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&(a[offset][0]), rows*N, MPI_INT,dest,1, MPI_COMM_WORLD);
      MPI_Send(&(b[0][0]), N*N, MPI_INT, dest, 1, MPI_COMM_WORLD);
      offset = offset + rows;
    }

    /* wait for results from all worker tasks */
    for (i=1; i<=numworkers; i++)
    {
      source = i;
      MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&(c[offset][0]), rows*N, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
    }

    gettimeofday(&stop, 0);

    printf("Here is the result matrix:\n");
    for (i=0; i<N; i++) {
      for (j=0; j<N; j++)
        printf("%d   ", c[i][j]);
      printf ("\n");
    }

    fprintf(stdout,"Time = %.6f\n\n",
         (stop.tv_sec+stop.tv_usec*1e-6)-(start.tv_sec+start.tv_usec*1e-6));

  }

  /*---------------------------- worker----------------------------*/
  if (taskid > 0) {
    source = 0;
    MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&(a[0][0]), rows*N, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&(b[0][0]), N*N, MPI_INT, source, 1, MPI_COMM_WORLD, &status);

    /* Matrix multiplication */
    for (k=0; k<N; k++)
      for (i=0; i<rows; i++) {
        c[i][k] = 0.0;
        for (j=0; j<N; j++)
          c[i][k] = c[i][k] + a[i][j] * b[j][k];
      }


    MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&(c), rows*N, MPI_INT, 0, 2, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}

int **alloc_2d_int(int rows, int cols) {
    int *data = (int *)malloc(rows*cols*sizeof(int));
    int **array= (int **)malloc(rows*sizeof(int*));
    for (int i=0; i<rows; i++)
        array[i] = &(data[cols*i]);

    return array;
}