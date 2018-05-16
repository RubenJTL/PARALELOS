#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int column_orient(int n, int A[n][n], int b[n], int x[n], int thread_count){
  int col;
  int row;
  int d = 0;
  while(d < 10){
    for(row = 0; row < n; row++)
      x[row] = b[row];

    for(col = n-1; col >= 0; col--){
      x[col] /= A[col][col];
#pragma omp parallel for num_threads(thread_count)
      for(row = 0; row < col; row++)
	x[row] -= A[row][col]*x[col];
      
    }
    
    
    int z;
    for(z = 0; z < n; z++)
      printf("%d ", x[z]);
    printf("\n");
    d++;
    
  }
  return 0;
}

int row_orient(int n, int A[n][n], int b[n], int x[n], int thread_count){
  
  int col;
  int row;
  int d = 0;
  while(d < 10){
    //#pragma omp parallel for num_threads(thread_count)
    for(row = n-1; row >= 0; row--){
      x[row] = b[row];
      
      #pragma omp parallel for num_threads(thread_count)
      for(col=row+1; col < n; col++){
	x[row] -= A[row][col]*x[col];

      }
      x[row] /= A[row][row];
    }
    int z;
    for(z = 0; z < n; z++)
      printf("%d ", x[z]);
    printf("\n");
    d++;
  }   
      return 0;
}
int main(int argc, char *argv[]){
  int n = strtol(argv[2], NULL, 10);
  int x[n];
  int b[n];
  int A[n][n];
  int i;
  int t;
  srand(time(NULL));
  int r;
  for(i = 0; i < n; i++){
      r = rand() ; 
      b[i] = r;
  }
  for(i = 0; i < n; i++){
    for(t = 0; t < n ; t++){
      r = rand();
      A[i][t] = r;
    }
  }
  int thread_count = strtol(argv[1], NULL, 10);
  if(row_orient(n, A, b, x, thread_count) == 0)
    printf("ROW ORIENT IS DONE\n");

  if(column_orient(n, A, b, x, thread_count) == 0)
    printf("COLUMN ORIENT IS DONE\n");

  return 0;
}
