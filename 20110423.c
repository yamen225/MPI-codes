#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc , char * argv[])
{
    int my_rank;		/* rank of process	*/
    int p;			/* number of process	*/
    int source;		/* rank of sender	*/
    int dest;		/* rank of reciever	*/
    int tag = 0;		/* tag for messages	*/
    char message[100];	/* storage for message	*/
    int min_r = 1, max_r = 12, val = 0, row_size1, col_size1, row_size2, col_size2, portion;
    int* arr1, arr2, myPortion1;
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
        int* myPortion1 = (int*)malloc (portion * sizeof(int));
        int* myPortion2 = (int*)malloc (portion * sizeof(int));
        MPI_Recv(myPortion1, portion, MPI_INT, 0,
                 tag, MPI_COMM_WORLD, &status );
        MPI_Recv(myPortion2, portion, MPI_INT, 0,
                 tag, MPI_COMM_WORLD, &status );

        int i;
        for( i = 0; i< portion ; i++)
        {
            myPortion1[i] -= myPortion2[i];
        }

    }
    else
    {
        printf("Enter the rows of the first matrix: \n");
        scanf("%d", &row_size1);
        printf("Enter the columns of the first matrix: \n");
        scanf("%d", &col_size1);

        printf("Enter the rows of the second matrix: \n");
        scanf("%d", &row_size2);
        printf("Enter the columns of the second matrix: \n");
        scanf("%d", &col_size2);

        int* arr1 = (int *) malloc (row_size1 * col_size1 * sizeof(int));
        int* arr2 = (int *) malloc (row_size2 * col_size2 * sizeof(int));

        int i, j;
        for(i = 0 ; i < row_size1 ; i++){
            for (j = 0; j < col_size1; j++)
            {
                arr1[i * col_size1 + j] = (int)(min_r + (max_r - min_r) * (double)rand()/(double)RAND_MAX + 0.5);
                //*(arr1 + i * col_size1 + j) = (int)(min_r + (max_r - min_r) * (double)rand()/(double)RAND_MAX + 0.5);
            }
        }
        printf("first matrix: \n");

        for (i = 0; i < row_size1; i++) {
            for (j = 0; j < col_size1; j++)
                printf("%d\t", *(arr1 + i * col_size1 + j));
        printf("\n");
        }


        for(i = 0 ; i < row_size2 ; i++){
            for (j = 0; j < col_size2; j++)
            {
                arr2[i*col_size2 + j] = (int)(min_r + (max_r - min_r) * (double)rand()/(double)RAND_MAX + 0.5);

                //*(arr2 + i * col_size2 + j) = (int)(min_r + (max_r - min_r) * (double)rand()/(double)RAND_MAX + 0.5);
            }
        }

        printf("second matrix: \n");

        for (i = 0; i < row_size2; i++) {
            for (j = 0; j < col_size2; j++)
                printf("%d\t", *(arr2 + i * col_size2 + j));
        printf("\n");
        }


        if ((row_size1 != row_size2) || (col_size1 != col_size2))
        {
            MPI_Finalize();
            return 0;
        }

        portion = (row_size1*col_size1)/(p-1);
        for( dest = 0; dest< p ; dest++)
        {

            MPI_Send( &portion, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);

            MPI_Send( arr1 + ((dest-1) * portion), portion, MPI_INT, dest, tag, MPI_COMM_WORLD);

            MPI_Send( arr2 + ((dest-1) * portion), portion, MPI_INT, dest, tag, MPI_COMM_WORLD);

        }
    }

    if (my_rank == 0)
    {
        int portion;
        MPI_Recv(&portion, 1, MPI_INT, 0,
                 tag, MPI_COMM_WORLD, &status );
        int* myPortion1 = (int*)malloc (portion * sizeof(int));
        int* myPortion2 = (int*)malloc (portion * sizeof(int));
        MPI_Recv(myPortion1, portion, MPI_INT, 0,
                 tag, MPI_COMM_WORLD, &status );
        MPI_Recv(myPortion2, portion, MPI_INT, 0,
                 tag, MPI_COMM_WORLD, &status );

        int i;
        for( i = 0; i< portion ; i++)
        {
            myPortion1[i] -= myPortion2[i];
        }

    }

    MPI_Gather(&myPortion1, portion, MPI_INT, &arr1, portion, MPI_INT, 0,
    MPI_COMM_WORLD);

    if (my_rank == 0)
    {
        printf("Final matrix: \n");

        for (int i = 0; i < row_size1; i++) {
            for (int j = 0; j < col_size1; j++)
                printf("%d\t", *(arr1 + i * col_size1 + j));
        printf("\n");
        }

    }

    /* shutdown MPI */
    MPI_Finalize();
    return 0;
}
