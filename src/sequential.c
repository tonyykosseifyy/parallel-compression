#include "headers.h"

// Function to check if a directory exists
int directoryExists(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

// Function to create a directory
void createDirectory(const char *path) {
    mkdir(path, 0755); // Creates a directory with read/write/execute permissions for owner, and read/execute permissions for group and others
}

// Function to compress a file using gzip, outputting to a specified directory
void compressFile(const char *inputFilePath, const char *outputDir) {
    char command[1024];
    snprintf(command, sizeof(command), "gzip -c %s > %s/%s.gz", inputFilePath, outputDir, basename(inputFilePath));
    system(command);
}

// Main program
int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input directory path> <output directory path>\n", argv[0]);
        return 1;
    }

    // Check input directory exists
    if (!directoryExists(argv[1])) {
        fprintf(stderr, "Input directory does not exist.\n");
        return 1;
    }

    // Check output directory exists; if not, create it
    if (!directoryExists(argv[2])) {
        createDirectory(argv[2]);
        printf("Output directory created.\n");
    }

    char **filesList = listFiles(argv[1]);
    if (filesList == NULL) {
        fprintf(stderr, "Failed to list files in directory %s\n", argv[1]);
        return 1;
    }

    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    printf("Compressing files...\n");
    for (char **currentFile = filesList; *currentFile != NULL; currentFile++) {
        // Simple loader effect
        printf("."); fflush(stdout);

        compressFile(*currentFile, argv[2]);
    }
    printf("\nDone.\n");

    gettimeofday(&endTime, NULL);
    double totalTime = (double)(endTime.tv_sec - startTime.tv_sec) + (double)(endTime.tv_usec - startTime.tv_usec) / 1000000.0;
    printf("Total time taken to compress all files: %.2f seconds\n", totalTime);

    // Cleanup
    for (char **file = filesList; *file != NULL; file++) {
        free(*file);
    }
    free(filesList);

    return 0;
}
