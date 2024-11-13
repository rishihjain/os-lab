#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex;  // Semaphore for protecting read_count
sem_t wrt;    // Semaphore for writer's mutual exclusion
int read_count = 0;  // Number of readers

// Reader function
void *reader(void *arg) {
    int reader_id = *((int *)arg);  // Reader ID
    printf("Reader %d is trying to read\n", reader_id);

    sem_wait(&mutex);  // Protect read_count
    read_count++;
    if (read_count == 1) {
        sem_wait(&wrt);  // If first reader, lock writer semaphore
    }
    sem_post(&mutex);  // Release protection of read_count

    // Reading section
    printf("Reader %d is reading\n", reader_id);
    sleep(1);  // Simulate reading
    printf("Reader %d has finished reading\n", reader_id);

    sem_wait(&mutex);  // Protect read_count
    read_count--;
    if (read_count == 0) {
        sem_post(&wrt);  // If last reader, unlock writer semaphore
    }
    sem_post(&mutex);  // Release protection of read_count

    return NULL;
}

// Writer function
void *writer(void *arg) {
    int writer_id = *((int *)arg);  // Writer ID
    printf("Writer %d is trying to access the resource\n", writer_id);

    sem_wait(&wrt);  // Lock writer semaphore (no readers or other writers)
    printf("Writer %d is writing\n", writer_id);
    sleep(1);  // Simulate writing
    printf("Writer %d has finished writing\n", writer_id);

    sem_post(&wrt);  // Release writer semaphore

    return NULL;
}

int main() {
    pthread_t read[5], write[3];  // 5 readers and 3 writers
    int reader_ids[5], writer_ids[3];

    // Initialize the semaphores
    sem_init(&mutex, 0, 1);  // Binary semaphore for read_count protection
    sem_init(&wrt, 0, 1);    // Binary semaphore for writer process

    // Create reader and writer threads
    for (int i = 0; i < 5; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&read[i], NULL, reader, &reader_ids[i]);
    }

    for (int i = 0; i < 3; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&write[i], NULL, writer, &writer_ids[i]);
    }

    // Join reader and writer threads
    for (int i = 0; i < 5; i++) {
        pthread_join(read[i], NULL);
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(write[i], NULL);
    }

    // Destroy the semaphores
    sem_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}
