//gcc producerConsumerSemaphoreSemaphore.c -o producerConsumerSemaphoreSemaphore -pthread
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5  // Reduced buffer size for quicker full/empty demonstration

int buffer[BUFFER_SIZE];  // Shared buffer
int count = 0;            // Counter for items in the buffer

sem_t empty;  // Semaphore for empty slots
sem_t full;   // Semaphore for full slots
pthread_mutex_t mutex;  // Mutex for critical section

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

        sem_wait(&empty);  // Wait for an empty slot
        pthread_mutex_lock(&mutex);  // Wait to access the buffer

        buffer[count] = item;  // Produce item into buffer
        count++;
        printf("Producer produced: %d\n", item);
        displayBuffer();  // Display buffer state

        pthread_mutex_unlock(&mutex);  // Release buffer
        sem_post(&full);  // Signal that a full slot is available

        sleep(rand() % 2);  // Simulate production time
    }
}

// Consumer function
void* consumer(void* arg) {
    while (1) {
        sem_wait(&full);  // Wait for a full slot
        pthread_mutex_lock(&mutex);  // Wait to access buffer

        int item = buffer[--count];  // Consume item from buffer
        printf("Consumer consumed: %d\n", item);
        displayBuffer();  // Display buffer state

        pthread_mutex_unlock(&mutex);  // Release buffer
        sem_post(&empty);  // Signal that an empty slot is available

        sleep(rand() % 2);  // Simulate consumption time
    }
}

int main() {
    pthread_t prod_thread, cons_thread;

    // Initialize semaphores and mutex
    sem_init(&empty, 0, BUFFER_SIZE);  // Initially, all slots are empty
    sem_init(&full, 0, 0);  // Initially, no slots are full
    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // Wait for threads to finish (optional, typically runs indefinitely)
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    // Clean up
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
