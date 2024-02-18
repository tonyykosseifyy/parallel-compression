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
    // compressWithNBCores(argv[1], argv[2]);  19 seconds 130 files

    return 0;
}
