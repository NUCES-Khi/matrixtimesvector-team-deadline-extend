// How to execute?
// gcc -o ompparallel_tiling openmpTiling.c -fopenmp
// ./ompparallel_tiling 3 3


#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define TILE_SIZE 16 // Dimension of the tiles

// Allocate the required memory for the matrix
double **allocateMatrix(int rows, int cols) {
    double **matrix = (double **)malloc(rows * sizeof(double *));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double *)malloc(cols * sizeof(double));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }
    return matrix;
}

// Allocate the required memory for the vector
double *allocateVector(int size) {
    double *vector = (double *)malloc(size * sizeof(double));
    if (vector == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return vector;
}

// Function to fill the required matrix and vector with random values
void fillRandomValues(double **matrix, double *vector, int matrixRows, int matrixCols, int vectorSize) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < matrixRows; i++) {
        for (int j = 0; j < matrixCols; j++) {
            matrix[i][j] = (double)rand() / RAND_MAX;
        }
    }
    #pragma omp parallel for
    for (int i = 0; i < vectorSize; i++) {
        vector[i] = (double)rand() / RAND_MAX;
    }
}

// Perform the required matrix-vector multiplication with tiling
void matrixVectorMultiply(double **matrix, double *vector, double *result, int matrixRows, int matrixCols) {
    #pragma omp parallel for
    for (int i = 0; i < matrixRows; i += TILE_SIZE) {
        for (int j = 0; j < matrixCols; j++) {
            double sum = 0.0;
            for (int k = i; k < i + TILE_SIZE && k < matrixRows; k++) {
                sum += matrix[k][j] * vector[k];
            }
            #pragma omp atomic
            result[j] += sum;
        }
    }
}

// Freeing the memory allocated for a matrix
void freeMatrix(double **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Freeing the memory allocated for a vector
void freeVector(double *vector) {
    free(vector);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s matrix_size vector_size\n", argv[0]);
        return EXIT_FAILURE;
    }

    int matrixRows = atoi(argv[1]);
    int matrixCols = atoi(argv[1]);
    int vectorSize = atoi(argv[2]);

    // Provide a Seed to the random number generator
    srand(time(NULL));

    // Allocate the required memory for matrix and vector
    double **matrix = allocateMatrix(matrixRows, matrixCols);
    double *vector = allocateVector(vectorSize);
    double *result = allocateVector(matrixRows);

    // Fill the matrix and vector with random values
    fillRandomValues(matrix, vector, matrixRows, matrixCols, vectorSize);

    // Performing matrix-vector multiplication with tiling concept
    clock_t start = clock();
    matrixVectorMultiply(matrix, vector, result, matrixRows, matrixCols);
    clock_t end = clock();
    double timeTaken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by Parallel Tiling program: %f seconds\n", timeTaken);

    // Freeing allocated memory
    freeMatrix(matrix, matrixRows);
    freeVector(vector);
    freeVector(result);

    return EXIT_SUCCESS;
}
