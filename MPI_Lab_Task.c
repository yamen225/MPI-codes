#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc , char * argv[])
{
    int* vector, matrix;
    int vSize, mRows, mColumns; /* vars holds the size of the arrays */
    int max, totalMax = 1;
    int max_iv, max_im;
    int my_rank;		/* rank of process	*/
    int p;			/* number of process	*/
    int source;		/* rank of sender	*/
    int dest;		/* rank of reciever	*/
    int tag = 0;		/* tag for messages	*/
    char message[100];	/* storage for message	*/
    MPI_Status status;	/* return status for 	*/
    /* recieve		*/

    /* Start up MPI */
    MPI_Init( &argc , &argv );

    /* Find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* Find out number of process */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (my_rank == 0)
    {
        printf("my rank %d\n", my_rank);
        printf("Enter vector size : \n") ;
        scanf ("%d \n" , &vSize ) ;
        printf("   \n");
        printf("Enter matrix rows : \n") ;
        scanf ("%d \n" , &mRows ) ;
        printf("Enter matrix columns : \n") ;
        scanf ("%d \n" , &mColumns ) ;
        int* vector = (int*) malloc (vSize * sizeof(int));
        int* matrix = (int*) malloc ((mRows * mColumns)* sizeof(int));


        int i;
        for(i = 0 ; i < vSize ; i++)
        {
            printf("Enter vector's elements : \n") ;
            scanf ("%d\n" , &vector[i] ) ;
        }

        for(i = 0 ; i < (mRows * mColumns) ; i++)
        {
            printf("Enter matrix's elements : \n") ;
            scanf ("%d \n" , &matrix[i] ) ;
        }

        int portion = (mRows * mColumns)/(p-1);
        for( dest = 1; dest< p ; dest++)
        {
            if (dest == p-1)
            {
              portion = ((mRows * mColumns)/(p-1))+((mRows * mColumns)%(p-1));
              MPI_Send( &portion, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);

              MPI_Send( matrix + ((dest-1) * (portion-((mRows * mColumns)%(p-1))))
              , portion, MPI_INT, dest, tag, MPI_COMM_WORLD);

            }
            MPI_Send( &portion, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);

            MPI_Send( matrix + ((dest-1) * portion), portion, MPI_INT, dest, tag, MPI_COMM_WORLD);

        }
        MPI_Bcast(&vSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&vector, vSize, MPI_INT, 0, MPI_COMM_WORLD);

    }
/*
    MPI_Bcast(&vSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&vector, vSize, MPI_INT, 0, MPI_COMM_WORLD);*/

    if (my_rank != 0)
    {
        printf("my rank: %d\n", my_rank);
        int portion;
        MPI_Recv(&portion, 1, MPI_INT, 0,
                 tag, MPI_COMM_WORLD, &status );
        int* myPortion = (int*)malloc (portion * sizeof(int));
        MPI_Recv(myPortion, portion, MPI_INT, 0,
                 tag, MPI_COMM_WORLD, &status );
        int i, j;
        max = -100000;
        max_im = 0;
        max_iv = 0;
        for(j = 0; j< vSize; j++ )
        {
            for (i = 0; i< portion ; i++)
            {
              printf("vector [j] = %d, mat[i] = %d\n", vector[j], myPortion[i]);
              if ((vector[j] + myPortion[i]) > max)
              {
                max = (vector[j] + myPortion[i]);
                max_im = i;
                max_iv = j;
              }
            }
        }
        MPI_Send( &max_im, 1,MPI_INT, 0, tag, MPI_COMM_WORLD);
        MPI_Send( &max_iv, 1,MPI_INT, 0, tag, MPI_COMM_WORLD);

    }

    MPI_Reduce(&max, &totalMax, 1, MPI_INT, MPI_MAX , 0, MPI_COMM_WORLD);

    if (my_rank == 0)
    {
      /*for( dest = 1; dest< p ; dest++)
      {

          MPI_Recv(&max_im, 1, MPI_INT, dest,
                  tag, MPI_COMM_WORLD, &status );
          MPI_Recv(&max_iv, 1, MPI_INT, dest,
                          tag, MPI_COMM_WORLD, &status );
          if (max > totalMax)
          {
            totalMax = max;
          }
    }*/
    printf("After reduction the greater max  = %d \n" , totalMax) ;

  }

    printf("MAX = %d \n",totalMax);


    /* shutdown MPI */
    MPI_Finalize();
    return 0;
}
