#include "headers.h" 

typedef struct {
    const char *filePath;
    const char *outputDir;
    int index;
} ThreadArgs;

void *compressFileTask(void *args) {
    ThreadArgs *threadArgs = (ThreadArgs *)args;
    compressFile(threadArgs->filePath, threadArgs->outputDir, threadArgs->index);

    printf("."); fflush(stdout);

    return NULL;
}

void compressionNThreads(char *inputDir, const char *outputDir) {
    int totalFiles = countFiles(inputDir);
    char **filesList = listFiles(inputDir);
    
    pthread_t *threads = (pthread_t *)malloc(totalFiles * sizeof(pthread_t));
    ThreadArgs *args = (ThreadArgs *)malloc(totalFiles * sizeof(ThreadArgs));

    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    printf("Compressing files - 2 at a time\n[");


    for (int i = 0; i < totalFiles; ++i) {
        args[i].filePath = filesList[i];
        args[i].outputDir = outputDir;
        args[i].index = i;

        if (pthread_create(&threads[i], NULL, compressFileTask, (void *)&args[i])) {
            fprintf(stderr, "Error: could not create thread for file %s\n", filesList[i]);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < totalFiles; ++i) {
        pthread_join(threads[i], NULL);
    }
    printf("]\nCompression Finished.\n");

    gettimeofday(&endTime, NULL);
    
    double totalTime = (double)(endTime.tv_sec - startTime.tv_sec);
    
    printf("Total time taken to compress all files: ");
    formatTime(totalTime);

    // Cleanup
    free(threads);
    free(args);
    cleanup(filesList);
}
