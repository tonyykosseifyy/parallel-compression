#include "headers.h"


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

    clearPerformanceFile("./results/techniques/performance.txt");
    clearPerformanceFile("./results/cores/performance.txt");

    // compressSequentially(argv[1], argv[2]); // 1 minutes 45 seconds 130 files
    // compressionNProcesses(argv[1], argv[2]); // 33 seconds 130 files 
    // compressWithNBCores(argv[1], argv[2]);  // 19 seconds 130 files
    // compressInBatches(argv[1], argv[2], 0); // 19 seconds 130 files

    // visualizePerformance();
    // visualizePerformanceLineChart();


    // compress and visulize performance for 1, 2, 3, 4, 5, 6, 7, 8 cores
    for (int i = 1; i <= 8; i++) {
        compressInBatches(argv[1], argv[2], i);
    }
    visualizeCoresPerformance();


    printf("\n------------------------------------------------------------------\n");
    printf("                   All compressions finished.\n");
    printf("            Results are saved in the results directory.\n");
    printf("--------------------------------------------------------------------\n");

    return 0;
}
