#include "headers.h"


void compressSequentially(char *inputDir, char *outputDir) {
    // Assuming listFiles returns a dynamically allocated list of file names from the input directory
    char **filesList = listFiles(inputDir);
    if (filesList == NULL) {
        fprintf(stderr, "Failed to list files in directory %s\n", inputDir);
        return;
    }

    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    printf("Compressing files...\n");
    for (char **currentFile = filesList; *currentFile != NULL; currentFile++) {
        // Simple loader effect
        printf("."); fflush(stdout);

        // Assuming compressFile is a function that takes the full path to an input file
        // and the directory where the compressed file should be saved
        compressFile(*currentFile, outputDir);
    }
    printf("\nDone.\n");

    gettimeofday(&endTime, NULL);
    double totalTime = (double)(endTime.tv_sec - startTime.tv_sec) + (double)(endTime.tv_usec - startTime.tv_usec) / 1000000.0;
    printf("Total time taken to compress all files: %.2f seconds\n", totalTime);

    // Assuming cleanup is intended to free the memory allocated for the list of file names
    cleanup(filesList); // Updated to use the correct function name
}
