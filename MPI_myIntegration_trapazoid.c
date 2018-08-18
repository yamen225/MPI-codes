#include <stdio.h>
#include <string.h>
#include "mpi.h"

float f (float x){
  float return_value;
  return_value = x*x ;
  return return_value;
}

float Trap(float local_a, float local_b, int local_n, float h) {
  float integral;
  float i;
  float x;
  integral = (f(local_a) + f(local_b))/2.0;
  x = local_a;
  for (i = 1; i <= local_n-1; i++) {
    /* code */
    x = x + h;
    integral = integral + f(x);
  }
  integral = integral * h;
  //printf(" inside trap: %f ,%f ,%d ,%f ,%f\n", local_a, local_b, local_n, h, integral);
  return integral;
}

int main(int argc, char* argv[]) {
  /* code */
  int my_rank;       /* rank of the process */
  int p;             /* number of processes */
  int source;        /* rank of sender */
  int dest;          /* rank of reciever */
  int tag = 0;       /* tag for messages */
  float a, b, local_a, h; /* storing left, right borders and the width opf trapazoids */
  int n, local_n; /*stores the portion for each process */
  float total = 0;
  MPI_Status status;        /* return status for recieve */

  /* start up MPI */
  MPI_Init(&argc, &argv);

  /* find out process rank */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  /* find out the number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  if (my_rank != 0){
    float integral;
    MPI_Recv(&local_n, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&local_a, 1, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&h, 1, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, &status);
    integral = Trap(local_a, (local_a+(h*local_n)), local_n, h);
    MPI_Send(&integral, 1, MPI_FLOAT, 0, 2, MPI_COMM_WORLD);
  }

  if (my_rank == 0) {
    /* code */
    printf("Enter a, b, and n\n");
    scanf("%f %f %d \n", &a, &b, &n ); /* taking data */

    float h = (b-a)/n;
    for (dest = 1; dest <= p-1; dest++){
      local_n = n/(p-1);
      if (dest == p-1) {
        local_n = (n/(p-1))+(n%(p-1));
        local_a = (a+(dest-1)*h*local_n);
        MPI_Send(&local_n, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
        MPI_Send(&local_a, 1, MPI_FLOAT, dest, 1, MPI_COMM_WORLD);
        MPI_Send(&h, 1, MPI_FLOAT, dest, 1, MPI_COMM_WORLD);

      }

      else {
        local_a = (a+(dest-1)*h*local_n);
        MPI_Send(&local_n, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
        MPI_Send(&local_a, 1, MPI_FLOAT, dest, 1, MPI_COMM_WORLD);
        MPI_Send(&h, 1, MPI_FLOAT, dest, 1, MPI_COMM_WORLD);

      }
    }

    float integral;
    for (source = 1; source < p; source++) {
      /* code */
      MPI_Recv(&integral, 1, MPI_FLOAT, source, 2, MPI_COMM_WORLD, &status);
      printf("%d, %f\n",source, integral );
      total = total + integral;
      printf("%f\n", total);

    }
  }
  /*shutdown MPI*/
  MPI_Finalize();
  return 0;
}
