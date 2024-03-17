//  How to execute
// mpicc mpiNaive.c -o mpiNaive
// mpirun -np 4 ./mpiNaive 4 4

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define MAX_VAL 10

void gen_rand_matrix(int *mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat[i * cols + j] = rand() % (MAX_VAL + 1);
        }
    }
}

void gen_rand_vector(int *vec, int size) {
    for (int i = 0; i < size; i++) {
        vec[i] = rand() % (MAX_VAL + 1);
    }
}

void print_mat(int *mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", mat[i * cols + j]);
        }
        printf("\n");
    }
}

void print_vec(int *vec, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", vec[i]);
    }
    printf("\n");
}

void mat_vec_mult(int *mat, int *vec, int *res, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        res[i] = 0;
        for (int j = 0; j < cols; j++) {
            res[i] += mat[i * cols + j] * vec[j];
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, world_size;
    int *mat, *vec, *res;
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

    // Allocating memory for the matrix, vector, and result
    mat = (int *)malloc(rows * cols * sizeof(int));
    vec = (int *)malloc(cols * sizeof(int));
    res = (int *)malloc(rows * sizeof(int));

    if (rank == 0) {
        // Generate the required random matrix and vector
        srand(time(NULL));
        gen_rand_matrix(mat, rows, cols);
        gen_rand_vector(vec, size);
    }

    // Broadcasting the matrix and vector to all processes
    MPI_Bcast(mat, rows * cols, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vec, cols, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculating local portion of result
    int local_rows = rows / world_size;
    int *local_res = (int *)malloc(local_rows * sizeof(int));

    double start_time = MPI_Wtime();

    mat_vec_mult(mat + rank * local_rows * cols, vec, local_res, local_rows, cols);

    double end_time = MPI_Wtime();

    // Gathering the local results from the root process
    MPI_Gather(local_res, local_rows, MPI_INT, res, local_rows, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("\nTime taken by Naive MPI: %.6f seconds\n", end_time - start_time);
    }

    // Freeing the allocated memory
    free(mat);
    free(vec);
    free(res);
    free(local_res);

    MPI_Finalize();
    return 0;
}
