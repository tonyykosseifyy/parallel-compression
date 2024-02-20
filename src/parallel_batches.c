#include "headers.h" 

void compressMultipleFiles(char **filesList, int startIdx, int endIdx, const char *outputDir);

void compressInBatches(char *inputDir, const char *outputDir) {
    int totalFiles = countFiles(inputDir);
    char **filesList = listFiles(inputDir);

    int NB_CORES = sysconf(_SC_NPROCESSORS_ONLN); // Get the number of CPU cores

    int filesPerCore = totalFiles / NB_CORES;
    int extraFiles = totalFiles % NB_CORES; // Extra files to distribute

    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    printf("Compressing with NB CORES = %d in Batches\n", NB_CORES);

    for (int i = 0; i < NB_CORES; ++i) {
        pid_t pid = fork();
        if (pid == 0) { 
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


    printf("Compression Finished.\n");
    gettimeofday(&endTime, NULL);

    double totalTime = (double)(endTime.tv_sec - startTime.tv_sec);
    printf("Total time taken to compress all files: ");
    formatTime(totalTime);

    writePerformance("Batches", totalTime);

    cleanup(filesList);

}

void compressMultipleFiles(char **filesList, int startIdx, int endIdx, const char *outputDir) {
    for (int i = startIdx; i < endIdx; ++i) {
        compressFile(filesList[i], outputDir, i);
    }
}
