#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"


int main(int argc, char *argv[])
{
	int rank, source, p, dest, tag=0, recv, i;
	int arr[8] = {1,1,1,1,1,1,1,1};
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if (rank==0)
	{
		printf("%d\n", p);
		for (dest = 1; dest < p; dest++)
		{
			MPI_Send(&arr[dest], 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
		}

	/// wait for the workers to finish
		arr[rank]+= rank;
		for (source = 1; source < p; source++)
		{
			MPI_Recv(&recv, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
			arr[source] = recv;
		}

		for (i = 0; i < 8; i++)
		{
			printf("%d\n", arr[i]);
		}
	}else{
		MPI_Recv(&recv, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		recv += rank;
		printf("from rank %d\n", rank);
		MPI_Send(&recv, 1, MPI_INT, 0, tag, MPI_COMM_WORLD); 
	}

	MPI_Finalize();
	return 0;
}