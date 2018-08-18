#include <omp.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{

    int i, j, itSum, N = 10;

    #pragma omp parallel  private(i , j, itSum)
    {

        #pragma omp for schedule(dynamic)
        //collapse(2)
        for (i=0; i < N; i++)
        {
//            c[i] = a[i] + b[i];
            int thNum = omp_get_thread_num() ;
            if (thNum != 3)
            {
                for (j=0; j < 1000000; j++)
                {
                    itSum += j;
                }
            }else{
                for (j=0; j < 5; j++)
                {
                    itSum += j;
                }
            }
            printf ("IN thread %d SUM = %d\n" , omp_get_thread_num() , itSum );

        }
    }   /* end of parallel region */

}
