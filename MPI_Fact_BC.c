#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    int   numtasks, taskid, rc, dest, r , i, r1 = 1 , result  = 1 , chunksize , counter = 0 ,c ,n , myfact = 1;
    double start ,end ;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);

    if (taskid == 0 )
    {
        printf("Enter integer : ") ;
        scanf ("%d" , &n ) ;
        start=MPI_Wtime() ;
        r = n / (numtasks-1) ;
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&r, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if ( taskid != 0 )
    {

        int a = (r*taskid)-r+1 ;
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


    }
    MPI_Reduce(&myfact, &result, 1, MPI_INT, MPI_PROD , 0, MPI_COMM_WORLD);


    if (taskid == 0 )
    {
        end = MPI_Wtime()  ;
        printf("After reduction, P0 will have factorial(%d)  = %d \n" , n ,result) ;
        printf("the execution time of the program %lf \n" , (end - start)) ;
    }


    MPI_Finalize();

}
