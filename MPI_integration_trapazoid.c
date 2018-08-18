#include <stdio.h>
#include <string.h>
#include "mpi.h"

float Trap (float local_a, float local_b, int local_n, float h);
/* calculate local integral */
float f (float x);  /* calculate f(x) and return the value*/
int main(int argc, char* argv[]) {
  /* code */
  int my_rank;       /* rank of the process */
  int p;             /* number of processes */
  int source;        /* rank of sender */
  int dest =0;       /* rank of reciever */
  int tag = 0;       /* tag for messages */
  float a = 0.0;     /* left endpoint */
  float b = 1.0;     /* right endpoint */
  int n = 4;      /* number of trapazoids */
  float h;           /* trapazoids base length */
  float local_a;     /* right endpoint for process */
  float local_b;     /* left endpoint for process */
  int local_n;       /* number of trapazoids for process */
  float integral;    /* holds the integral for each process */
  float total;       /* holds the total integration at master process */
  MPI_Status status; /* return status for recieve */
  float start, end;

  /* start up MPI */
  MPI_Init(&argc, &argv);

  /* find out process rank */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  /* find out the number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  h = (b-a)/n;
  local_n = n/p;
  printf("%d %d\n", local_n, my_rank);
  local_a = a + local_n * (my_rank ) * h;
  local_b = local_a + local_n * h;
  integral = Trap(local_a, local_b, local_n, h);

  if (my_rank == 0) {
    /* code */
    total = integral;
    for (source = 1; source < p; source++) {
      /* code */
      MPI_Recv(&integral, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
      total += integral;
    }
  }
  else {
    MPI_Send(&integral, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
    printf(" integral is %f of rank: %d\n", integral, my_rank);
  }

  if (my_rank == 0) {
    /* code */
    printf("with n = %d trapazoids, our estimate is %f\n",n,total );

  }
  MPI_Finalize();
  return 0;
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
  printf(" inside trap: %f ,%f ,%d ,%f ,%f\n", local_a, local_b, local_n, h, integral);
  return integral;
}

float f (float x){
  float return_value;
  return_value = x*x ;
  return return_value;
}
