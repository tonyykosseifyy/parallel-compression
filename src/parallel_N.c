#include "headers.h"
#include <sys/wait.h> 

void compressionNProcesses(char *inputDir, const char *outputDir) {
    int totalFiles = countFiles(inputDir);
    char **filesList = listFiles(inputDir);

    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    printf("Compressing with N Processes files\n");

    pid_t *pids = (pid_t *)malloc(totalFiles * sizeof(pid_t));
    for (int i = 0; i < totalFiles; ++i) {
        pids[i] = fork(); // Create a new process
        if (pids[i] == 0) { // Child process
            // Perform compression in the child process
            compressFile(filesList[i], outputDir, i);
            exit(0); // Child exits after compression
        } else if (pids[i] < 0) {
            // Fork failed
            fprintf(stderr, "Error: could not fork for file %s\n", filesList[i]);
            exit(EXIT_FAILURE);
        }
    }

    // Parent process waits for all children to complete
    for (int i = 0; i < totalFiles; ++i) {
        waitpid(pids[i], NULL, 0);
    }

    printf("\nCompression Finished.\n");
    gettimeofday(&endTime, NULL);

    double totalTime = (double)(endTime.tv_sec - startTime.tv_sec);
    printf("Total time taken to compress all files: ");
    formatTime(totalTime); // Ensure you have a function to format and print the time

    // Cleanup
    free(pids);
    cleanup(filesList); // Ensure this function frees the file list appropriately
}

// Make sure to adjust or implement compressFile and cleanup functions as necessary.
