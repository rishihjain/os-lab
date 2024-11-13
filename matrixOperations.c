#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define SIZE 3
int matrix1[SIZE][SIZE], matrix2[SIZE][SIZE], result[SIZE][SIZE];

void *add_matrices(void *arg);
void *subtract_matrices(void *arg);
void *multiply_matrices(void *arg);
void initialize_matrices();
void display_matrix(int matrix[SIZE][SIZE]);

void initialize_matrices() {
    printf("Matrix Initialization:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrix1[i][j] = rand() % 10;
            matrix2[i][j] = rand() % 10;
        }
    }
    printf("Matrix 1:\n");
    display_matrix(matrix1);
    printf("Matrix 2:\n");
    display_matrix(matrix2);
}

void display_matrix(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void *add_matrices(void *arg) {
    printf("Addition Thread Started.\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
    printf("Addition Result:\n");
    display_matrix(result);
    printf("Addition Thread Finished.\n");
    
    int *exit_status = malloc(sizeof(int));  
    *exit_status = 0;  
    pthread_exit(exit_status);  
}
void *subtract_matrices(void *arg) {
    printf("Subtraction Thread Started.\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            result[i][j] = matrix1[i][j] - matrix2[i][j];
        }
    }
    printf("Subtraction Result:\n");
    display_matrix(result);
    printf("Subtraction Thread Finished.\n");
    
    int *exit_status = malloc(sizeof(int));  
    *exit_status = 1;
    pthread_exit(exit_status);  // Exit the thread with the status
}

void *multiply_matrices(void *arg) {
    printf("Multiplication Thread Started.\n");
    int *k = (int *)malloc(sizeof(int));  // Allocate memory for the pointer
    *k = 1;  // Assign a value to *k

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            result[i][j] = 0;
            for (*k = 0; *k < SIZE; (*k)++) {  // Use *k as the loop variable
                result[i][j] += matrix1[i][*k] * matrix2[*k][j];
            }
        }
    }
    printf("Multiplication Result:\n");
    display_matrix(result);
    printf("Multiplication Thread Finished.\n");
    
    free(k);  // Free the allocated memory
    
    int *exit_status = malloc(sizeof(int));  // Allocate memory for the exit status
    *exit_status = 2;  // Set the exit status
    pthread_exit(exit_status);  // Exit the thread with the status
}

int main() {
    pthread_t tid1, tid2, tid3;
    void *status;

    initialize_matrices();

    if (pthread_create(&tid1, NULL, add_matrices, NULL) != 0) {
        perror("Error creating addition thread");
        return 1;
    }
    if (pthread_create(&tid2, NULL, subtract_matrices, NULL) != 0) {
        perror("Error creating subtraction thread");
        return 1;
    }
    if (pthread_create(&tid3, NULL, multiply_matrices, NULL) != 0) {
        perror("Error creating multiplication thread");
        return 1;
    }

    if (pthread_join(tid1, &status) != 0) {
        perror("Error joining addition thread");
        return 1;
    }
    printf("Addition Thread Exit Status: %d\n", *((int *)status));
    free(status);  // Free the allocated memory for exit status

    if (pthread_join(tid2, &status) != 0) {
        perror("Error joining subtraction thread");
        return 1;
    }
    printf("Subtraction Thread Exit Status: %d\n", *((int *)status));
    free(status);  // Free the allocated memory for exit status

    if (pthread_join(tid3, &status) != 0) {
        perror("Error joining multiplication thread");
        return 1;
    }
    printf("Multiplication Thread Exit Status: %d\n", *((int *)status));
    free(status);  // Free the allocated memory for exit status

    return 0;
}
