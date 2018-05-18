#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>

void fillMatrix(int N, double** A, double* b);
void printMatrix(int N, double** A, double* b);
void backSubstitution(int N, double** A, double* b, double* x, int numThreads);
void printSolutionVector(double* x, int N);
void checkAnswer(double** A, double* x, double* b, int N);


int main(int argc, char** argv){

	int N = atoi(argv[1]);
	int numThreads = atoi(argv[2]);
	struct timeval startTime, endElimination, endSubstitution;
	double eliminationTime, substitutionTime, totalTime;
	pthread_t elimination_threads[numThreads];

	double **A = (double **)calloc(N,sizeof(double*));
	for (int q=0; q < N; q++)
		A[q] = (double*)calloc(N,sizeof(double*));

	double* b = (double*) malloc(sizeof(double)*N);
	double* x = (double*) malloc(sizeof(double)*N);

	fillMatrix(N, A, b); 
	if (N <= 8)
	printMatrix(N, A, b);

	backSubstitution(N, A, b, x, numThreads);
	gettimeofday(&endSubstitution, NULL);
}

void fillMatrix(int N, double** A, double* b){
	int i, j;
	for (i=0; i<N; i++){
		for (j=0; j<N; j++){
			A[i][j] = (drand48()*1000);
		}
		b[i] = (drand48()*1000);
	}
}

void printMatrix(int N, double** A, double* b){
	if (N <= 8){
		for (int x=0; x<N; x++){
			printf("| ");
			for(int y=0; y<N; y++)
				printf("%7.2f ", A[x][y]);
			printf("| | %7.2f |\n", b[x]);
		}
	}
	else{
		printf("\nPerforming gaussian elimination with a matrix and vector too large to print out.\n");
		printf("If you would like to see output, try again with a matrix of length 8 or less.\n");
	}
}

void printSolutionVector(double* x, int N){
	if (N <= 8){
		printf("\nSolution Vector (x):\n\n");
		for (int i=0; i<N; i++){
			printf("|%10.6f|\n", x[i]);
		}
	}
}

void backSubstitution(int N, double** A, double* b, double* x, int numThreads){
	int i,j;
        	for(i=0;i<N;i++) x[i]=b[i];
        	for (i=1; i<N; i++){
        		#pragma omp parallel shared(A,x) private(j){
            		#pragma omp for
            		for (j=i; j<N; j++){
            			x[j] = x[j]- A[j][i-1]*x[i-1];
            		}
        		}
        	}
}

void checkAnswer(double** A, double* x, double* b, int N){

	double* result_vector = (double*) malloc(sizeof(double)*N);
	double row_sum; 

	for (int j=0; j<N; j++){
		row_sum = 0;
		for (int k=0; k<N; k++){
			row_sum += A[j][k]*x[k];
		}
		result_vector[j] = row_sum;
	}

	double sumOfSquares = 0;
	double entryOfResidual;
	for (int i=0; i<N; i++){
		entryOfResidual = result_vector[i] - b[i];
		sumOfSquares += entryOfResidual*entryOfResidual;
	}
	sumOfSquares = sqrt(sumOfSquares);
	printf("\nThe L2-Norm of the result vector from Ax-b is: %.20f\n", sumOfSquares);
}
