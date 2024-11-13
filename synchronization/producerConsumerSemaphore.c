#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5  // Size of the buffer

int buffer[BUFFER_SIZE];
int in = 0, out = 0;   // Buffer indices for producer and consumer

// Semaphores
sem_t empty;           // Tracks empty slots in the buffer
sem_t full;            // Tracks filled slots in the buffer
sem_t mutex;           // Binary semaphore for mutual exclusion

// Producer function
void *producer(void *arg) {
    int producerId = *(int *)arg;
    
    for (int i = 0; i < 10; i++) { // Produce 10 items
        int item = rand() % 100;   // Randomly generate an item

        sem_wait(&empty);          // Wait if buffer is full
        sem_wait(&mutex);          // Lock mutex for exclusive access

        // Produce an item (critical section)
        buffer[in] = item;
        printf("Producer %d produced: %d\n", producerId, item);
        in = (in + 1) % BUFFER_SIZE; // Move to the next buffer slot

        sem_post(&mutex);           // Unlock mutex
        sem_post(&full);            // Signal that buffer has one more filled slot

        sleep(1); // Simulate time taken to produce an item
    }

    return NULL;
}

// Consumer function
void *consumer(void *arg) {
    int consumerId = *(int *)arg;

    for (int i = 0; i < 10; i++) { // Consume 10 items
        sem_wait(&full);           // Wait if buffer is empty
        sem_wait(&mutex);          // Lock mutex for exclusive access

        // Consume an item (critical section)
        int item = buffer[out];
        printf("Consumer %d consumed: %d\n", consumerId, item);
        out = (out + 1) % BUFFER_SIZE; // Move to the next buffer slot

        sem_post(&mutex);           // Unlock mutex
        sem_post(&empty);           // Signal that buffer has one more empty slot

        sleep(1); // Simulate time taken to consume an item
    }

    return NULL;
}

int main() {
    pthread_t producers[2], consumers[2];
    int producerIds[2] = {1, 2};
    int consumerIds[2] = {1, 2};

    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE); // Start with all buffer slots empty
    sem_init(&full, 0, 0);            // Start with no filled slots
    sem_init(&mutex, 0, 1);           // Mutex for critical section

    // Create producer and consumer threads
    for (int i = 0; i < 2; i++) {
        pthread_create(&producers[i], NULL, producer, &producerIds[i]);
        pthread_create(&consumers[i], NULL, consumer, &consumerIds[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < 2; i++) {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
