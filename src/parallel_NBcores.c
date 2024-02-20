#include "headers.h" 

void compressBatch(char **filesList, int startIdx, int endIdx, const char *outputDir) {
    for (int i = startIdx; i < endIdx; ++i) {
        pid_t pid = fork();
        if (pid == 0) { 
            compressFile(filesList[i], outputDir, i);
            exit(0);
        } else if (pid < 0) {
            // Handle fork failure
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process waits for all children in this batch to complete
    while (wait(NULL) > 0);
}

void compressWithNBCores(const char *inputDir, const char *outputDir) {
    int fileCount = countFiles(inputDir);
    char **filesList = listFiles(inputDir);
    
    int NB_CORES = sysconf(_SC_NPROCESSORS_ONLN); // Number of cores


    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    printf("Compressing with NB_CORES = %d\n", NB_CORES);


    for (int i = 0; i < fileCount; i += NB_CORES) {
        int endIdx = i + NB_CORES < fileCount ? i + NB_CORES : fileCount;
        compressBatch(filesList, i, endIdx, outputDir);
    }

    printf("Compression Finished.\n");
    gettimeofday(&endTime, NULL);

    double totalTime = (double)(endTime.tv_sec - startTime.tv_sec);
    printf("Total time taken to compress all files: ");
    formatTime(totalTime); 

    writePerformance("8_CORES", totalTime);

    // Cleanup
    cleanup(filesList); // Assuming cleanup properly frees the filesList
}
