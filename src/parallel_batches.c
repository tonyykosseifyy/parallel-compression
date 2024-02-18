#include "headers.h" 

void compressFilesInBatches(const char *inputDir, const char *outputDir) {
    int totalFiles = countFiles(inputDir);
    char **filesList = listFiles(inputDir);
    if (!filesList) {
        fprintf(stderr, "Failed to list files\n");
        return;
    }

    int NB_CORES = sysconf(_SC_NPROCESSORS_ONLN); // Get the number of CPU cores.
    int filesPerProcess = totalFiles / NB_CORES;
    int remainingFiles = totalFiles % NB_CORES; // Extra files that need to be distributed.

    pid_t pids[NB_CORES];

    for (int i = 0; i < NB_CORES; ++i) {
        int startIdx = i * filesPerProcess + (i < remainingFiles ? i : remainingFiles);
        int endIdx = startIdx + filesPerProcess + (i < remainingFiles ? 1 : 0);

        pids[i] = fork();
        if (pids[i] == 0) { // Child process
            for (int j = startIdx; j < endIdx; ++j) {
                compressFile(filesList[j], outputDir, j); // Adjust this call as necessary.
            }
            exit(0);
        } else if (pids[i] < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process waits for all child processes to complete.
    for (int i = 0; i < NB_CORES; ++i) {
        waitpid(pids[i], NULL, 0);
    }

    cleanup(filesList); // Assuming cleanup properly frees the fileList
}

