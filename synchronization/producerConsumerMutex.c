#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5  // Buffer size

int buffer[BUFFER_SIZE];  // Shared buffer
int count = 0;  // Counter to track buffer fill level

pthread_mutex_t mutex;  // Mutex for critical section
pthread_cond_t cond_full;  // Condition variable for buffer full
pthread_cond_t cond_empty;  // Condition variable for buffer empty

// Function to display the buffer's current state
void displayBuffer() {
    printf("Buffer: [ ");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (i < count)
            printf("%d ", buffer[i]);
        else
            printf("- ");  // Empty slot
    }
    printf("] ");
    
    if (count == 0) {
        printf("(Buffer is empty)\n");
    } else if (count == BUFFER_SIZE) {
        printf("(Buffer is full)\n");
    } else {
        printf("\n");
    }
}

// Producer function
void* producer(void* arg) {
    while (1) {
        int item = rand() % 100;  // Produce a random item

        pthread_mutex_lock(&mutex);  // Lock the mutex before accessing the buffer

        // Wait if buffer is full
        while (count == BUFFER_SIZE) {
            printf("Producer waiting, buffer full...\n");
            pthread_cond_wait(&cond_full, &mutex);
        }

        // Produce item into buffer
        buffer[count] = item;
        count++;
        printf("Producer produced: %d\n", item);
        displayBuffer();  // Display buffer state

        // Signal that the buffer is no longer empty
        pthread_cond_signal(&cond_empty);

        pthread_mutex_unlock(&mutex);  // Unlock the mutex

        sleep(rand() % 2);  // Simulate production time
    }
}

// Consumer function
void* consumer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);  // Lock the mutex before accessing the buffer

        // Wait if buffer is empty
        while (count == 0) {
            printf("Consumer waiting, buffer empty...\n");
            pthread_cond_wait(&cond_empty, &mutex);
        }

        // Consume item from buffer
        int item = buffer[--count];
        printf("Consumer consumed: %d\n", item);
        displayBuffer();  // Display buffer state

        // Signal that the buffer is no longer full
        pthread_cond_signal(&cond_full);

        pthread_mutex_unlock(&mutex);  // Unlock the mutex

        sleep(rand() % 2);  // Simulate consumption time
    }
}

int main() {
    pthread_t prod_thread, cons_thread;

    // Initialize mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_full, NULL);
    pthread_cond_init(&cond_empty, NULL);

    // Create producer and consumer threads
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // Wait for threads to finish (optional, typically runs indefinitely)
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    // Clean up
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_full);
    pthread_cond_destroy(&cond_empty);

    return 0;
}
