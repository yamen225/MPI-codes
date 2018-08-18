#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define  MASTER		0

int main (int argc, char *argv[])
{
    int  data[16000];
    int ARRAYSIZE ;
    int   numtasks, taskid, rc, dest, offset, i, j, tag1, tag2, tag3, source, r,n, result=1 , myfact=1;
    int max  , max_index ;
    double start ,end ;

    MPI_Status status;

    /***** Initializations *****/

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);

    tag2 = 1;
    tag1 = 2;
    tag3 = 3;

    /***** Master task only ******/
    if (taskid == MASTER)
    {
        /* Initialize the array */

        printf("Enter integer : ") ;
        scanf ("%d" , &n ) ;
        start=MPI_Wtime() ;

        r = n / (numtasks-1) ;

        for (dest=1; dest<numtasks; dest++)
        {
            MPI_Send(&r, 1, MPI_INT, dest, tag1, MPI_COMM_WORLD);
            MPI_Send(&n, 1, MPI_INT, dest, tag1, MPI_COMM_WORLD);
        }


        for (i=1; i<numtasks; i++)
        {

            source = i;
            MPI_Recv(&myfact, 1, MPI_INT, source, tag3, MPI_COMM_WORLD, &status);
            result = result * myfact ;
        }


        /* Master printing the result */

        printf("After reduction, P0 will have factorial(%d)  = %d \n" , n ,result) ;
        end = MPI_Wtime()  ;
        printf("the execution time of the program %lf \n" , (end - start)) ;

    } /* end of master */


    /***** Non-master tasks only *****/

    if (taskid > MASTER)
    {

        /* Receive my portion of array from the master task */

        source = MASTER;
        MPI_Recv(&r, 1, MPI_INT, source, tag1, MPI_COMM_WORLD, &status);
        MPI_Recv(&n, 1, MPI_INT, source, tag1, MPI_COMM_WORLD, &status);

        int a = (r*taskid)-r+1 ;
        printf("a = %d\n", a);
        int b = r*taskid ;
        if (n%(numtasks-1) !=0 && taskid == numtasks-1)
        {
            b +=  n%(numtasks-1) ;
        }
        for ( i = a ; i <= b ; i++ )
        {
            myfact = myfact * i ;
        }

        printf("p%d: calculate partial factorial from %d to %d = %d\n" ,taskid , a , b , myfact) ;


        /* Send my results back to the master task */
        dest = MASTER;
        MPI_Send(&myfact, 1, MPI_INT, dest, tag3, MPI_COMM_WORLD);

    } /* end of non-master */

    MPI_Finalize();

}   /* end of main */
