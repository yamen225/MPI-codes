#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mpi.h"

int main(int argc, char *argv[]) {
  time_t t;
  int nRow, nColumn, portion, local_a[100], swap;
  struct timeval start, stop;
  int* a = NULL;
  int my_rank;       /* rank of the process */
  int p;             /* number of processes */
  int source;        /* rank of sender */
  int dest;          /* rank of reciever */
  int tag = 0;       /* tag for messages */
  int i, j;            /*iterator*/
  MPI_Status status;

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &my_rank );
  MPI_Comm_size( MPI_COMM_WORLD, &p );
  srand((unsigned) time(&t));

  /*Master*/
  if(my_rank == 0){
    gettimeofday(&start, 0);

    printf("Enter the number of rows of the first matrix: \n" );
    scanf("%d", &nRow);
    printf("Enter the number of columns of the first matrix: \n" );
    scanf("%d", &nColumn);
    a = (int*) malloc(nRow* nColumn * sizeof(int));
    for (i=0; i<nRow; i++){
      for (j=0; j<nColumn; j++){
        a[i*nColumn+j]= rand() % 12;
      }
    }

    printf("First Matrix: \n");
    for (int iRow = 0; iRow < nRow; iRow++) {

      for (int iCol = 0; iCol < nColumn; iCol++){
        printf("%d\t",a[iRow*nColumn + iCol]);
      }
      printf("\n");
    }

    portion = (nRow/p)*nColumn;

  }
  MPI_Bcast(&nColumn, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&portion, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(a, portion, MPI_INT, &local_a, portion, MPI_INT, 0,
    MPI_COMM_WORLD);


  for (i=0; i<(portion/nColumn); i++){
    //printf("%d\n", i);
    for (j=i*nColumn; j<(nColumn+i*nColumn)-1; j++){
      //printf("%d\n", j);
      for (int k=i*nColumn; k<(nColumn+i*nColumn)-j-1; k++){
        printf("%d\n", k);
        if (local_a[k] > local_a[k+1]){
          swap = local_a[k];
          local_a[k] = local_a[k+1];
          local_a[k+1] = swap;
        }
      }
    }
    //printf("this is raw n#: %d\t\n", (my_rank+i+1));
    for (int s = i*nColumn; s<(nColumn+i*nColumn); s++){
      //printf("%d\t", local_a[s]);
    }
    //printf("\n");
  }

  MPI_Gather(&local_a, portion, MPI_INT, a, portion, MPI_INT, 0,
  MPI_COMM_WORLD);

  if (my_rank == 0) {

    printf("First Matrix: \n");
    for (int iRow = 0; iRow < nRow; iRow++) {

      for (int iCol = 0; iCol < nColumn; iCol++){
        printf("%d\t",a[iRow*nColumn + iCol]);
      }
      printf("\n");
    }

  }

  MPI_Finalize();
  return 0;
}
