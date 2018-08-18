#include <omp.h>
#include <stdio.h>

int main ()
{

    int var1, var2, var3;

    // serial code

    #pragma omp parallel private(var1, var2) shared(var3)
    {
        printf ("hello from thread %d\n" , omp_get_thread_num());
    }


}
