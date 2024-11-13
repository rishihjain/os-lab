#include <stdio.h>


#define MAX_FRAMES 10


int findLRU(int frames[], int time[], int frameCount) {
    int min = time[0], minIndex = 0;
    for (int i = 1; i < frameCount; i++) {
        if (time[i] < min) {
            min = time[i];
            minIndex = i;
        }
    }
    return minIndex; // Return the index of the LRU page
}


int isPageInFrames(int frames[], int frameCount, int page) {
    for (int i = 0; i < frameCount; i++) {
        if (frames[i] == page) {
            return 1; // Page found in frames
        }
    }
    return 0; // Page not found
}


int main() {
    int frameCount, pageCount;
    int pageFaults = 0;


    // Input: Number of frames and number of pages in the reference string
    printf("Enter the number of frames: ");
    scanf("%d", &frameCount);


    printf("Enter the number of pages in the reference string: ");
    scanf("%d", &pageCount);


    int pages[pageCount];
    printf("Enter the reference string (space-separated): ");
    for (int i = 0; i < pageCount; i++) {
        scanf("%d", &pages[i]);
    }


    int frames[MAX_FRAMES];
    int time[MAX_FRAMES]; // Array to keep track of the last used time of each frame
    for (int i = 0; i < frameCount; i++) {
        frames[i] = -1; // Initialize frames as empty
        time[i] = 0; // Initialize the last used time
    }


    // Processing each page in the reference string
    for (int i = 0; i < pageCount; i++) {
        int currentPage = pages[i];


        // Check if the current page is already in the frames
        if (!isPageInFrames(frames, frameCount, currentPage)) {
            // Page fault occurs as the page is not in frames
            int lruIndex = findLRU(frames, time, frameCount); // Find the index of the LRU page
            frames[lruIndex] = currentPage; // Replace the LRU page with the current page
            pageFaults++; // Increment page faults
        }


        // Update the time of the current page
        for (int j = 0; j < frameCount; j++) {
            if (frames[j] == currentPage) {
                time[j] = i; // Update the last used time for the current page
                break;
            }
        }
    }


    printf("\nTotal Page Faults: %d\n", pageFaults);


    return 0;
}

// 3
// 12
// 7 0 1 2 0 3 0 4 2 3 0 3


corrected :
#include <stdio.h>

int find_LRU(int time[], int n) {
    int min = time[0], pos = 0;
    for (int i = 1; i < n; i++) {
        if (time[i] < min) {
            min = time[i];
            pos = i;
        }
    }
    return pos;
}

int page_faults_LRU(int pages[], int n, int capacity) {
    int frame[capacity]; // Holds pages in memory
    int time[capacity];  // Tracks the time each page was last used
    int page_faults = 0, counter = 0;

    // Initialize frames with -1 to represent empty slots
    for (int i = 0; i < capacity; i++) {
        frame[i] = -1;
    }

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;

        // Check if the page is already in the frame
        for (int j = 0; j < capacity; j++) {
            if (frame[j] == page) {
                found = 1;  // Page is found in frame
                time[j] = counter++; // Update last used time
                break;
            }
        }

        // If page is not found, it is a page fault
        if (!found) {
            int pos = -1;

            // If there is an empty slot, find it
            for (int j = 0; j < capacity; j++) {
                if (frame[j] == -1) {
                    pos = j;
                    break;
                }
            }

            // If no empty slot, replace the LRU page
            if (pos == -1) {
                pos = find_LRU(time, capacity);
            }

            // Replace the page at the LRU position with the new page
            frame[pos] = page;
            time[pos] = counter++;
            page_faults++;
        }

        // Display current frame contents
        printf("Frame: ");
        for (int j = 0; j < capacity; j++) {
            if (frame[j] == -1)
                printf("- ");
            else
                printf("%d ", frame[j]);
        }
        printf("\n");
    }

    return page_faults;
}
