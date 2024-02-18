#include "headers.h"

int compressWithNForks(const char *folderPath, const char *outputDir);

void compressFile1(const char *filePath, int index, const char *outputDir) {
    char command[256]; // Increased buffer size for command
    // Construct command with proper formatting for index and filePath
    sprintf(command, "gzip -c \"%s\" > \"%s/meteo%d.gz\"", filePath, outputDir, index);
    system(command);
}


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

    int fileCount = countFiles(argv[1]);
    char **fileList = listFiles(argv[1]);

    // compressSequentially(argv[1], argv[2]); // 1 minutes 45 seconds 130 files
    // compressionNProcesses(argv[1], argv[2]); 33 seconds 130 files 

    
    return 0;
}

int compressWithNForks(const char *folderPath, const char *outputDir) {
    time_t start, end;
    start = time(NULL);
    int filesCount = countFiles(folderPath);
    printf("Starting parallel compression with %d processes\n", filesCount);
    char **filesList = listFiles(folderPath);

    pid_t *pids = malloc(filesCount * sizeof(pid_t)); // Allocate an array to store child PIDs

    printf("Compressing files with %d processes\n", filesCount);

    for (int i = 0; i < filesCount; i++) {
        pids[i] = fork();
        if (pids[i] == 0) { // Child process
            compressFile1(filesList[i], i, outputDir); // Correct call
            exit(0);
        } else if (pids[i] < 0) { // Fork failed
            fprintf(stderr, "Failed to fork\n");
            exit(1);
        }
    }

    // Wait for all child processes to exit
    for (int i = 0; i < filesCount; i++) {
        waitpid(pids[i], NULL, 0); // Wait specifically for the process with PID pids[i]
    }

    free(pids); // Free the allocated PID array

    end = time(NULL);
    printf("compression finished in %ld seconds\n", (end - start));
    cleanup(filesList); // Ensure cleanup function correctly frees fileList and its contents
    return (int)(end - start);
}
