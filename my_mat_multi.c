#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[]) {
  int nRow0, nColumn0, nRow1, nColumn1, iRow = 0, iCol = 0, sum = 0;
  int nRow2, nColumn2;
  int** poBuffer0 = NULL;
  int** poBuffer1 = NULL;
  int** poBuffer2 = NULL;
  time_t t;
  srand((unsigned) time(&t));

  printf("Enter the number of rows of the first matrix: \n" );
  scanf("%d", &nRow0);
  printf("Enter the number of columns of the first matrix: \n" );
  scanf("%d", &nColumn0);
  printf("Enter the number of rows of the second matrix: \n" );
  scanf("%d", &nRow1);
  printf("Enter the number of columns of the second matrix: \n" );
  scanf("%d", &nColumn1);

  if (nRow1 != nColumn0) return 1;
  else{
    nRow2 = nRow0;
    nColumn2 = nColumn1;
  }

  poBuffer0 = (int**) malloc(nRow0 * sizeof(int*));
  poBuffer1 = (int**) malloc(nRow1 * sizeof(int*));
  poBuffer2 = (int**) malloc(nRow2 * sizeof(int*));

  if ((poBuffer0 == NULL) || (poBuffer1 == NULL) || (poBuffer2 == NULL)){
    return 1;
  }

  for (iRow = 0; iRow < nRow0; iRow++) {
    poBuffer0[iRow] = (int*) malloc(nColumn0 * sizeof(int*));
    if (poBuffer0[iRow] == NULL) {
      return 1;
    }
  }

  for (iRow = 0; iRow < nRow1; iRow++) {
    poBuffer1[iRow] = (int*) malloc(nColumn1 * sizeof(int*));
    if (poBuffer1[iRow] == NULL) {
      return 1;
    }
  }

  for (iRow = 0; iRow < nRow2; iRow++) {
    poBuffer2[iRow] = (int*) malloc(nColumn2 * sizeof(int*));
    if (poBuffer1[iRow] == NULL) {
      return 1;
    }
  }

  for (iRow = 0; iRow < nRow0; iRow++) {
    /* code */
    for (iCol = 0; iCol < nColumn0; iCol++){
      poBuffer0[iRow][iCol] = rand() % 10;
    }
  }
  printf("First Matrix: \n");
  for (iRow = 0; iRow < nRow0; iRow++) {
    /* code */
    for (iCol = 0; iCol < nColumn0; iCol++){
      printf("%d\t",poBuffer0[iRow][iCol]);
    }
    printf("\n");
  }

  for (iRow = 0; iRow < nRow1; iRow++) {
    /* code */
    for (iCol = 0; iCol < nColumn1; iCol++){
      poBuffer1[iRow][iCol] = rand() % 10;
    }
  }

  printf("Second Matrix: \n");
  for (iRow = 0; iRow < nRow1; iRow++) {
    /* code */
    for (iCol = 0; iCol < nColumn1; iCol++){
      printf("%d\t", poBuffer1[iRow][iCol]);
    }
    printf("\n");
  }

  for (iRow = 0; iRow < nRow0; iRow++){
    for (iCol = 0; iCol < nColumn1; iCol++ ){
      for (int iRow1 = 0; iRow1 < nRow1; iRow1++){
        sum = sum + poBuffer0[iRow][iRow1] * poBuffer1[iRow1][iCol];
      }
      poBuffer2[iRow][iCol] = sum;
      sum = 0;
    }
  }


  printf("Result Matrix: \n");
  for (iRow = 0; iRow < nRow1; iRow++) {
    /* code */
    for (iCol = 0; iCol < nColumn1; iCol++){
      printf("%d\t", poBuffer1[iRow][iCol]);
    }
    printf("\n");
  }

  return 0;
}
