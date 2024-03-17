#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define MATRIX_SIZE 1000
#define TILE_SIZE 100

void multiplyMatrixVector(int* mat, int* vec, int* res, int size, int rows_per_node) {
    for (int i = 0; i < rows_per_node; i++) {
        res[i] = 0;
        for (int j = 0; j < size; j++) {
            res[i] += mat[i * size + j] * vec[j];
        }
    }
}

int main(int argc, char** argv) {
    int node_rank, num_nodes;
    int matrix[MATRIX_SIZE][MATRIX_SIZE], vector[MATRIX_SIZE], result[MATRIX_SIZE];
    int rows_per_node, remaining_rows;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &node_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_nodes);

    rows_per_node = MATRIX_SIZE / num_nodes;
    remaining_rows = MATRIX_SIZE % num_nodes;

    if (node_rank == 0) {
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                matrix[i][j] = rand() % 10;
            }
            vector[i] = rand() % 10;
        }
    }

    
    MPI_Bcast(vector, MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    
    int* local_matrix = (int*)malloc(sizeof(int) * rows_per_node * MATRIX_SIZE);
    MPI_Scatter(matrix, rows_per_node * MATRIX_SIZE, MPI_INT, local_matrix, rows_per_node * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    
    clock_t start_time = clock();
    multiplyMatrixVector(local_matrix, vector, result, MATRIX_SIZE, rows_per_node);

    
    int* gathered_results = NULL;
    if (node_rank == 0) {
        gathered_results = (int*)malloc(sizeof(int) * MATRIX_SIZE);
    }
    MPI_Gather(result, rows_per_node, MPI_INT, gathered_results, rows_per_node, MPI_INT, 0, MPI_COMM_WORLD);

    free(local_matrix);
    MPI_Finalize();

    // time
    clock_t end_time = clock();
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Time taken by MPI Matrix-Vector Multiplication program: %f seconds\n", time_taken);

    return 0;
}
