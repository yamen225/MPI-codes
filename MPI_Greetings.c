#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char* argv[]) {
  /* code */
  int my_rank;       /* rank of the process */
  int p;             /* number of processes */
  int source;        /* rank of sender */
  int dest;          /* rank of reciever */
  int tag = 0;       /* tag for messages */
  char message[100]; /* storage for message */
  MPI_Status status;        /* return status for recieve */

  /* start up MPI */
  MPI_Init(&argc, &argv);

  /* find out process rank */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  /* find out the number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  if (my_rank != 0){
    sprintf(message, "Greetings from process %d!", my_rank);
    dest = 0;
    MPI_Send(message, strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
  }
  else {
    for (source = 1; source < p; source++) {
      /* code */
      MPI_Recv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
      printf("%s\n", message);
    }
  }
  /*shutdown MPI*/
  MPI_Finalize();
  return 0;
}
