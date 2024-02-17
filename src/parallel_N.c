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
    }
    return NULL;
}

void startNThreadCompression(char **filesList, int totalFiles, const char *outputDir, int N) {
    pthread_t *threads = (pthread_t *)malloc(N * sizeof(pthread_t));
    ThreadArgs *args = (ThreadArgs *)malloc(N * sizeof(ThreadArgs));

    int filesPerThread = totalFiles / N;
    int remainingFiles = totalFiles % N; // Remaining files after equally dividing

    for (int i = 0; i < N; ++i) {
        args[i].filesList = filesList;
        args[i].startIdx = i * filesPerThread + (i < remainingFiles ? i : remainingFiles);
        args[i].endIdx = args[i].startIdx + filesPerThread + (i < remainingFiles ? 1 : 0);
        args[i].outputDir = outputDir;

        if (pthread_create(&threads[i], NULL, compressFiles, (void *)&args[i])) {
            fprintf(stderr, "Error creating thread #%d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < N; ++i) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(args);
}
