#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 



typedef float Num;
typedef int cantidad;
// CUDAKernel. Cada Thread tomara un elemento de C

int tamano_bloque, tamano_grid;

__global__ void vecAddKernel(Num *A, Num *B, Num *C, int n)
{
    // Obtener el Id del tread
    int id = blockIdx.x*blockDim.x+threadIdx.x;
    if (id < n) C[id] = A[id] + B[id];
}
 
void vecAdd(Num* A, Num* B,Num* C,cantidad n){
	int size=n*sizeof(Num);
	Num *d_A, *d_B, *d_C;
	cudaMalloc((void**) &d_A, size);
    cudaMemcpy( d_A, A, size, cudaMemcpyHostToDevice);
    cudaMalloc((void**) &d_B, size);
    cudaMemcpy( d_B, B, size, cudaMemcpyHostToDevice);
    
    cudaMalloc((void**) &d_C, size);

    tamano_grid = (cantidad)ceil((float)n/tamano_bloque);
	
	cudaEventRecord(start);
	vecAddKernel<<<tamano_grid, tamano_bloque>>>(d_A, d_B, d_C, n);
	cudaEventRecord(stop);   
    
 	cudaEventSynchronize(stop);
	float milisec = 0;
	cudaEventElapsedTime(&milisec, start, stop);

	printf("%f\n", milisec );
    cudaMemcpy( C, d_C, size, cudaMemcpyDeviceToHost );
    cudaFree(d_A);cudaFree(d_B);cudaFree(d_C);

}

int main( int argc, char** argv )
{
	if(argc != 3){
		printf("Faltan Argumentos <blockSize> <tamVector>\n");
		return 0;
	}    
    int n = atoi(argc[2]);;
 	tamano_bloque=atoi(argc[1]);
    
    Num *A;
    Num *B;
    
    Num *C;
	
	size_t size = n*sizeof(Num);
 
    A = (Num*)malloc(size);
    B = (Num*)malloc(size);
    C = (Num*)malloc(size);
 
    int i;
    
    for( i = 0; i < n; i++ ) {
        A[i] = sin(i)*sin(i);
        B[i] = cos(i)*cos(i);
    }
    vecAdd(A,B,C,n);
 
    Num sum = 0;
    for(i=0; i<n; i++)
        sum += C[i];
    printf("final result: %f\n", sum/n);
 
    free(A);
    free(B);
    free(C);
 
    return 0;
}
