// How to execute
// mpicc openmpNaive.c -o openmpnaive -- compile
// mpirun -np 4 ./openmpNaive 100 100 -- sample execute


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define MAX_VAL 10

void generate_rand_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i * cols + j] = rand() % (MAX_VAL + 1);
        }
    }
}

void generate_rand_vector(int *vector, int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % (MAX_VAL + 1);
    }
}

void print_mat(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i * cols + j]);
        }
        printf("\n");
    }
}

void print_vec(int *vector, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n");
}

void mat_vec_multiply(int *matrix, int *vector, int *result, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        result[i] = 0;
        for (int j = 0; j < cols; j++) {
            result[i] += matrix[i * cols + j] * vector[j];
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, world_size;
    int *matrix, *vector, *result;
    int rows, cols, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (argc != 3) {
        if (rank == 0) {
            printf("Usage: %s <rows> <cols>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    rows = atoi(argv[1]);
    cols = atoi(argv[2]);
    size = rows;

    if (cols != size || size % world_size != 0) {
        if (rank == 0) {
            printf("Invalid matrix and vector dimensions.\n");
        }
        MPI_Finalize();
        return 1;
    }

    // Allocate memory for the required matrix, vector, and result
    matrix = (int *)malloc(rows * cols * sizeof(int));
    vector = (int *)malloc(cols * sizeof(int));
    result = (int *)malloc(rows * sizeof(int));

    if (rank == 0) {
        // Generating a random matrix and vector
        srand(time(NULL));
        generate_rand_matrix(matrix, rows, cols);
        generate_rand_vector(vector, size);
    }

    // Broadcasting
    MPI_Bcast(matrix, rows * cols, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vector, cols, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculating the local portion of result
    int local_rows = rows / world_size;
    int *local_result = (int *)malloc(local_rows * sizeof(int));

    double start_time = MPI_Wtime();

    mat_vec_multiply(matrix + rank * local_rows * cols, vector, local_result, local_rows, cols);

    double end_time = MPI_Wtime();

    // Gathering the local results from the root process
    MPI_Gather(local_result, local_rows, MPI_INT, result, local_rows, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {

        printf("\nTime taken by Custom MPI: %.6f seconds\n", end_time - start_time);
    }

    // Free the allocated memory
    free(matrix);
    free(vector);
    free(result);
    free(local_result);

    MPI_Finalize();
    return 0;
}
