#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_SIZE 100

// Function to calculate sum of even numbers in an array
int sumEvenNumbers(int arr[], int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] % 2 == 0) {
            sum += arr[i];
        }
    }
    return sum;
}

// Function to calculate sum of odd numbers in an array
int sumOddNumbers(int arr[], int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] % 2 != 0) {
            sum += arr[i];
        }
    }
    return sum;
}

int main() {
    int arr[MAX_SIZE];
    int size;

    // Taking array input from the user
    printf("Enter the number of elements in the array: ");
    scanf("%d", &size);

    printf("Enter the elements of the array:\n");
    for (int i = 0; i < size; i++) {
        scanf("%d", &arr[i]);
    }

    // Creating a child process
    pid_t pid = fork();

    if (pid < 0) {
        // If fork() fails
        perror("Fork failed");
        return -1;
    }

    // Child process will compute the sum of odd numbers
    if (pid == 0) {
        int oddSum = sumOddNumbers(arr, size);
        printf("Child Process (Odd Sum): %d\n", oddSum);
        exit(0); // Exit child process after printing result
    }
    // Parent process will compute the sum of even numbers
    else {
        int evenSum = sumEvenNumbers(arr, size);
        printf("Parent Process (Even Sum): %d\n", evenSum);
        
        // Wait for child process to finish
        wait(NULL);
    }

    return 0;
}
