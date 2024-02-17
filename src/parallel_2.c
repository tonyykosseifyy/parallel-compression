#include "headers.h"

typedef struct {
    char **filesList;
    int startIdx;
    int endIdx;
    const char *outputDir;
} ThreadArgs;

void *compressFiles(void *args) {
    ThreadArgs *threadArgs = (ThreadArgs *)args;
    for (int i = threadArgs->startIdx; i < threadArgs->endIdx; ++i) {
        compressFile(threadArgs->filesList[i], threadArgs->outputDir, i);

        printf("."); fflush(stdout);
    }
    return NULL;
}

void parallel_2(char *inputDir, char *outputDir) {
    int totalFiles = countFiles(inputDir);
    char **filesList = listFiles(inputDir); 

    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    printf("Compressing files - 2 at a time\n[");

    pthread_t threads[2];
    ThreadArgs args[2];

    int halfSize = totalFiles / 2;
    // Setup arguments for the first half
    args[0].filesList = filesList;
    args[0].startIdx = 0;
    args[0].endIdx = halfSize;
    args[0].outputDir = outputDir;

    // Setup arguments for the second half
    args[1].filesList = filesList;
    args[1].startIdx = halfSize;
    args[1].endIdx = totalFiles;
    args[1].outputDir = outputDir;

    // Create and start threads
    for (int i = 0; i < 2; ++i) {
        if (pthread_create(&threads[i], NULL, compressFiles, (void *)&args[i])) {
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for threads to finish
    for (int i = 0; i < 2; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("]\nCompression Finished.\n");

    gettimeofday(&endTime, NULL);
    
    double totalTime = (double)(endTime.tv_sec - startTime.tv_sec);
    
    printf("Total time taken to compress all files: ");
    formatTime(totalTime);

    cleanup(filesList);
}
