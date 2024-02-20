#include "headers.h"

void compressSequentially(char *inputDir, char *outputDir) {
    char **filesList = listFiles(inputDir);
    int fileCount = countFiles(inputDir);
    

    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    printf("Compressing %d files Sequentially\n", fileCount);
    
    for (int i = 0; i < fileCount; i++) {
        compressFile(filesList[i], outputDir, i);
    }

    printf("Compression Finished.\n");

    gettimeofday(&endTime, NULL);
    
    double totalTime = (double)(endTime.tv_sec - startTime.tv_sec);
    
    printf("Total time taken to compress all files: ");
    formatTime(totalTime);


    writePerformance("sequential", totalTime);

    cleanup(filesList);
}
