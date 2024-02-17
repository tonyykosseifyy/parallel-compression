#include "headers.h"

void compressSequentially(char *inputDir, char *outputDir) {
    char **filesList = listFiles(inputDir);
    int fileCount = countFiles(inputDir);

    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    printf("Compressing files\n[");
    for (int i = 0; i < fileCount; i++) {
        compressFile(filesList[i], outputDir, i);

        printf("."); fflush(stdout);
    }

    printf("]\nCompression Finished.\n");

    gettimeofday(&endTime, NULL);
    double totalTime = (double)(endTime.tv_sec - startTime.tv_sec) + (double)(endTime.tv_usec - startTime.tv_usec) / 1000000.0;
    printf("Total time taken to compress all files: %.2f seconds\n", totalTime);

    cleanup(filesList);
}
