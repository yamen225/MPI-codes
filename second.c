#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define CHUNKSIZE 2

int main(int argc, char *argv[])
{

    int i = 0, j = 0, k = 0, sum = 0, N = 4;
    int** a = (int **) malloc (N * sizeof(int*));
    int** b = (int **) malloc (N * sizeof(int*));
    int** c = (int **) malloc (N * sizeof(int*));



    /* Some initializations */
    for (i=0; i < N; i++){
        a[i] = (int*) malloc(N * sizeof(int));
        c[i] = (int*) malloc(N * sizeof(int));
        b[i] = (int*) malloc(N * sizeof(int));
    }

    for (i=0; i<N; i++){
        for (j=0; j < N; j++)
        {
            a[i][j] = i+1;
            c[i][j] = 0;
        }
    }

    for (i=0; i<N; i++){
        for (j=0; j < N; j++)
        {
            b[i][j] = j+1;
        }
    }
    
    printf("first matrix\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++)
            printf("%d \t", a[i][j]);
        
        printf("\n");
    }
    printf("second matrix\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++)
            printf("%d \t", b[i][j]);
        
        printf("\n");
    }


    printf("finished initializations\n");
    
            
    #pragma omp parallel shared(c) private(i,j,k)
    {

        #pragma omp for schedule(dynamic)
        for (i = 0; i < N; i++)
        {
            for(j = 0; j < N; j++)
            {
                for(k = 0; k < N; k++){
                    sum = sum + a[i][k]*b[k][j];
                }

                c[i][j] = sum;
                sum = 0;
            }
        }
    }   /* end of parallel region */

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++)
            printf("%d \t", c[i][j]);
        
        printf("\n");
    }

    free(a); free(b); free(c);



}
