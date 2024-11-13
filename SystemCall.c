#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

// Function prototypes
void process_related_calls();
void file_related_calls();
void communication_related_calls();
void information_related_calls();

int main() {
    int choice;

    while (1) {
        printf("\nMenu Driven System Call Demonstration\n");
        printf("1. Process Related System Calls\n");
        printf("2. File Related System Calls\n");
        printf("3. Communication Related System Calls\n");
        printf("4. Information Related System Calls\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                process_related_calls();
                break;
            case 2:
                file_related_calls();
                break;
            case 3:
                communication_related_calls();
                break;
            case 4:
                information_related_calls();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice, please try again.\n");
        }
    }

    return 0;
}

void process_related_calls() {
    pid_t pid;
    int status;

    printf("\nProcess Related System Calls\n");

    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) { // Child process
        printf("Child Process ID: %d\n", getpid());
        execlp("/bin/ls", "ls", NULL); // exec system call
        exit(0); // exit system call
    } else { // Parent process
        printf("Parent Process ID: %d\n", getpid());
        wait(&status); // wait system call
        printf("Child process completed with status: %d\n", status);
    }
}

void file_related_calls() {
    int fd;
    char buffer[100];
    ssize_t bytes_read;

    printf("\nFile Related System Calls\n");

    fd = open("testfile.txt", O_CREAT | O_RDWR, 0644); // open system call
    if (fd < 0) {
        perror("Open failed");
        return;
    }

    write(fd, "Hello, World!\n", 14); // write system call

    lseek(fd, 0, SEEK_SET); // reposition file offset to the beginning
    bytes_read = read(fd, buffer, sizeof(buffer)); // read system call

    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("Read from file: %s", buffer);
    }

    close(fd); // close system call
    unlink("testfile.txt"); // unlink system call
}

void communication_related_calls() {
    int fd[2];
    pid_t pid;
    char write_msg[] = "Hello from parent!\n";
    char read_msg[100];

    printf("\nCommunication Related System Calls\n");

    if (pipe(fd) == -1) {
        perror("Pipe failed");
        return;
    }

    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) { // Child process
        close(fd[1]); // Close write end of pipe
        read(fd[0], read_msg, sizeof(read_msg)); // Read from pipe
        printf("Child received: %s", read_msg);
        close(fd[0]); // Close read end of pipe
    } else { // Parent process
        close(fd[0]); // Close read end of pipe
        write(fd[1], write_msg, strlen(write_msg)); // Write to pipe
        close(fd[1]); // Close write end of pipe
        wait(NULL);
    }
}

void information_related_calls() {
    printf("\nInformation Related System Calls\n");

    printf("Sleeping for 2 seconds...\n");
    sleep(2); // sleep system call

    printf("Setting an alarm for 3 seconds...\n");
    alarm(3); // alarm system call

    printf("You will see this message before the alarm goes off.\n");

    // Delay to let the alarm go off
    sleep(4);
}
