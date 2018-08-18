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
    int min_r = 1, max_r = 12, val = 0, mat_size1, mat_size2;
    MPI_Status status;	/* return status for 	*/
    /* recieve		*/

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
        int max = -100000;
        for( i = 0; i< portion ; i++)
        {
            if (myPortion[i]>max)
            {
              max = myPortion[i];
            }
        }
        MPI_Send( &max, 1,MPI_INT, 0, tag, MPI_COMM_WORLD);

    }
    else
    {
        start = MPI_Wtime() ;
        printf("Enter the size of the first matrix: \n");
        scanf()
        int* arr = (int*) malloc (10 * sizeof(int));
	      time_t t;
	      srand((unsigned) time(&t));
        int i;
        for(i = 0 ; i < 10 ; i++)
        {
            arr[i] = rand() % 1000 ;
            printf("arr[i] = %d\n", arr[i]);
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
        int totalMax = -10000;
        for( dest = 1; dest< p ; dest++)
        {
            int max;
            MPI_Recv(&max, 1, MPI_INT, dest,
                     tag, MPI_COMM_WORLD, &status );
            if (max > totalMax)
            {
              totalMax = max;
            }
        }

        printf("MAX = %d \n",totalMax);
        end = MPI_Wtime();
        printf("the execution time of the program %lf \n" , (end - start)) ;

    }

    /* shutdown MPI */
    MPI_Finalize();
    return 0;
}
