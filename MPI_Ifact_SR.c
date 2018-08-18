#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc , char * argv[])
{
    double start, end;
    int my_rank;		/* rank of process	*/
    int p;			/* number of process	*/
    int source;		/* rank of sender	*/
    int dest;		/* rank of reciever	*/
    int tag = 0;		/* tag for messages	*/
    char message[100];	/* storage for message	*/
    MPI_Status status;	/* return status for 	*/
    /* recieve		*/
    start = MPI_Wtime() ;
    /* Start up MPI */
    MPI_Init( &argc , &argv );

    /* Find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* Find out number of process */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if( my_rank != 0)
    {
        int portion;
        MPI_Recv(&portion, 1, MPI_INT, 0,
                 tag, MPI_COMM_WORLD, &status );
        int* myPortion = (int*)malloc (portion * sizeof(int));
        MPI_Recv(myPortion, portion, MPI_INT, 0,
                 tag, MPI_COMM_WORLD, &status );
        int i;
        int sum = 0;
        for( i = 0; i< portion ; i++)
        {
            sum+= myPortion[i];
        }
        MPI_Send( &sum, 1,MPI_INT, 0, tag, MPI_COMM_WORLD);

    }
    else
    {
        int* arr = (int*) malloc (10 * sizeof(int));
	      time_t t;
	      srand((unsigned) time(&t));
        int i;
        for(i = 0 ; i < 10 ; i++)
        {
            arr[i] = rand() % 50 ;
        }
        int portion = 10/(p-1);
        for( dest = 1; dest< p ; dest++)
        {
            if (dest == p-1)
            {
              portion = (10/(p-1))+(10%(p-1));
              MPI_Send( &portion, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);

              MPI_Send( arr + ((dest-1) * (portion-(10%(p-1)))), portion, MPI_INT,
               dest, tag, MPI_COMM_WORLD);

            }
            MPI_Send( &portion, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);

            MPI_Send( arr + ((dest-1) * portion), portion, MPI_INT, dest, tag, MPI_COMM_WORLD);

        }
        int totalSum = 0;
        for( dest = 1; dest< p ; dest++)
        {
            int sum;
            MPI_Recv(&sum, 1, MPI_INT, dest,
                     tag, MPI_COMM_WORLD, &status );
            totalSum += sum;
        }

        printf("SUM = %d \n",totalSum);
    }

    end = MPI_Wtime();
    printf("the execution time of the program %lf \n" , (end - start)) ;
    /* shutdown MPI */
    MPI_Finalize();
    return 0;
}
