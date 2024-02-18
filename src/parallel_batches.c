#include "headers.h" 



void compressMultipleFiles(char **filesList, int startIdx, int endIdx, const char *outputDir) {
    for (int i = startIdx; i < endIdx; ++i) {
        compressFile(filesList[i], outputDir, i);
    }
}


void compressInBatches(char **filesList, int totalFiles, const char *outputDir) {
    int NB_CORES = sysconf(_SC_NPROCESSORS_ONLN); // Get the number of CPU cores
    int filesPerCore = totalFiles / NB_CORES;
    int extraFiles = totalFiles % NB_CORES; // Extra files to distribute

    for (int i = 0; i < NB_CORES; ++i) {
        pid_t pid = fork();
        if (pid == 0) { // Child process
            int start = i * filesPerCore + (i < extraFiles ? i : extraFiles);
            int end = start + filesPerCore + (i < extraFiles ? 1 : 0);
            compressMultipleFiles(filesList, start, end, outputDir);
            exit(0); // Exit after processing
        } else if (pid < 0) {
            // Fork failed
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all child processes to complete
    while (wait(NULL) > 0);
}
