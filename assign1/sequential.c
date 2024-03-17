// How to execute
// gcc sequential.c
// ./a.out matrix_size vector_size


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to allocate memory for the required matrix
double **allocate_matrix_memory(int rows, int cols) {
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

// Function to allocate memory for the required vector
double *allocate_vector_memory(int size) {
    double *vector = (double *)malloc(size * sizeof(double));
    if (vector == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return vector;
}

// Function to fill the required matrix and vector with some random values
void fill_random_numbers(double **matrix, double *vector, int matrix_rows, int matrix_cols, int vector_size) {
    for (int i = 0; i < matrix_rows; i++) {
        for (int j = 0; j < matrix_cols; j++) {
            matrix[i][j] = (double)rand() / RAND_MAX; // Random value between 0 and 1
        }
    }
    for (int i = 0; i < vector_size; i++) {
        vector[i] = (double)rand() / RAND_MAX; // Random value between 0 and 1
    }
}

// Function to perform the required matrix-vector multiplication
double *multiply_matrix_vector(double **matrix, double *vector, int matrix_rows, int matrix_cols) {
    double *result = allocate_vector_memory(matrix_rows);
    for (int i = 0; i < matrix_rows; i++) {
        result[i] = 0.0;
        for (int j = 0; j < matrix_cols; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    return result;
}

// Function to free the memory allocated for a matrix
void free_matrix_memory_allocation(double **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Function to free the memory allocated for a vector
void free_vector_memory_allocation(double *vector) {
    free(vector);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s matrix_size vector_size\n", argv[0]);
        return EXIT_FAILURE;
    }

    int matrix_rows = atoi(argv[1]);
    int matrix_cols = atoi(argv[1]);
    int vector_size = atoi(argv[2]);

    // select a Seed for the random number generator
    srand(time(NULL));

    // Allocate the memory required for matrix and vector
    double **matrix = allocate_matrix_memory(matrix_rows, matrix_cols);
    double *vector = allocate_vector_memory(vector_size);

    // Fill the matrix and vector with random values
    fill_random_numbers(matrix, vector, matrix_rows, matrix_cols, vector_size);
    
    clock_t begin_time = clock();
    
    // Perform the required matrix-vector multiplication
    double *result = multiply_matrix_vector(matrix, vector, matrix_rows, matrix_cols);
    
    clock_t end_time = clock();

    double execution_time = ((double)(end_time - begin_time)) / CLOCKS_PER_SEC;
    printf("Time taken by the program: %f seconds\n", execution_time);

    // Free the allocated memory
    free_matrix_memory_allocation(matrix, matrix_rows);
    free_vector_memory_allocation(vector);
    free(result);

    return EXIT_SUCCESS;
}
