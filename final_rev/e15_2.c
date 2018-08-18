#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

int IsPrime (unsigned int number);

int main(int argc, char *argv[])
{
	int rank, p, source, dest, tag=0, i;
	int a, b, local_a, local_b, local_count=0, total_count=0, ip;
	double start, end;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Barrier(MPI_COMM_WORLD);
	

	if (rank == 0)
	{
		scanf("%d", &a);
		scanf("%d", &b);
		start = MPI_Wtime();
	}
	MPI_Bcast(&a, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&b, 1, MPI_INT, 0, MPI_COMM_WORLD);

	local_a = a + rank*((b-a)/p);

	if (rank == p-1)
	{
		local_b = b;
	}
	else{
		local_b = local_a + ((b-a)/p);
	}

	for (i = local_a; i < local_b; ++i)
	{
		ip = IsPrime(i);
		if(ip) {
			local_count +=1;
			printf("%d\n", i);
			}		
	}

	MPI_Reduce(&local_count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	
	if(rank == 0)
	{
		end = MPI_Wtime();
		printf("total count : %d\n", total_count);
		printf("time elapssed = %f\n", end-start);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();


	return 0;
}

int IsPrime(unsigned int number) {
    if (number <= 1) return 0; // zero and one are not prime
    unsigned int i;
    for (i=2; i*i<=number; i++) {
        if (number % i == 0) return 0;
    }
    return 1;
}