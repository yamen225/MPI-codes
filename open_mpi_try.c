#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4

int main(int argc, char const *argv[]) 
{
	int m, n, p, q, c, d, k, sum = 0;
	int first[N][N], second[N][N], multiply[N][N];
  	time_t t;
  	srand((unsigned) time(&t));

  	for (c = 0; c < N; c++)
    	for (d = 0; d < N; d++)
      		first[c][d] = c+1;


	for (c = 0; c < N; c++)
    	for (d = 0; d < N; d++)
       		second[c][d] = d+1;

    	for (c = 0; c < N; c++) {
  	    	for (d = 0; d < N; d++) {
    	   		for (k = 0; k < N; k++) {
        		  	sum = sum + first[c][k]*second[k][d];
        			}

        		multiply[c][d] = sum;
       			sum = 0;

      		}
   		}

    printf("Product of the matrices:\n");
/*
    for (c = 0; c < N; c++) {
      for (d = 0; d < N; d++)
        printf("%d\t", multiply[c][d]);

      printf("\n");
    }
*/
  
  return 0;
}
