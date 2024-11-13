#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;   // Mutex for read_count
pthread_mutex_t wrt;     // Mutex for writer process
int read_count = 0;      // Number of readers

// Reader function
void *reader(void *arg) {
    int reader_id = *((int *)arg); // Reader ID
    printf("Reader %d is trying to read\n", reader_id);

    pthread_mutex_lock(&mutex);  // Lock mutex for read_count
    read_count++;
    if (read_count == 1) {
        pthread_mutex_lock(&wrt);  // First reader locks writer mutex
    }
    pthread_mutex_unlock(&mutex);  // Unlock mutex

    // Reading section
    printf("Reader %d is reading\n", reader_id);
    sleep(1);  // Simulate reading
    printf("Reader %d has finished reading\n", reader_id);

    pthread_mutex_lock(&mutex);  // Lock mutex for read_count
    read_count--;
    if (read_count == 0) {
        pthread_mutex_unlock(&wrt);  // Last reader unlocks writer mutex
    }
    pthread_mutex_unlock(&mutex);  // Unlock mutex

    return NULL;
}

// Writer function
void *writer(void *arg) {
    int writer_id = *((int *)arg); // Writer ID
    printf("Writer %d is trying to access the resource\n", writer_id);

    pthread_mutex_lock(&wrt);  // Lock writer mutex
    printf("Writer %d is writing\n", writer_id);
    sleep(1);  // Simulate writing
    printf("Writer %d has finished writing\n", writer_id);

    pthread_mutex_unlock(&wrt);  // Unlock writer mutex

    return NULL;
}

int main() {
    pthread_t read[5], write[3];  // 5 readers and 3 writers
    int reader_ids[5], writer_ids[3];

    // Initialize the mutexes
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&wrt, NULL);

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

    // Destroy the mutexes
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&wrt);

    return 0;
}
