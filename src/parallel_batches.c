#include "headers.h" 

void compressMultipleFiles(char **filesList, int startIdx, int endIdx, const char *outputDir);
char* concat(int NB_CORES) ;

void compressInBatches(char *inputDir, const char *outputDir, int NB_CORES) {
    int totalFiles = countFiles(inputDir);
    char **filesList = listFiles(inputDir);

    bool testing_many_cores = NB_CORES != 0 ? true : false;
    char *performance_path = !testing_many_cores ? "./results/techniques/performance.txt" : "./results/cores/performance.txt"; 

    NB_CORES =  NB_CORES != 0 ? NB_CORES : sysconf(_SC_NPROCESSORS_ONLN);

    int filesPerCore = totalFiles / NB_CORES;
    int extraFiles = totalFiles % NB_CORES; // Extra files to distribute

    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    printf("Compressing with NB CORES = %d in Batches\n", NB_CORES);

    for (int i = 0; i < NB_CORES; ++i) {
        pid_t pid = fork();
        if (pid == 0) { 
            int start = i * filesPerCore + (i < extraFiles ? i : extraFiles);
            int end = start + filesPerCore + (i < extraFiles ? 1 : 0);
            compressMultipleFiles(filesList, start, end, outputDir);
            exit(0); // Exit after processing
        } else if (pid < 0) {
            // Fork failed
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all child processes to complete
    while (wait(NULL) > 0);


    printf("Compression Finished.\n");
    gettimeofday(&endTime, NULL);

    double totalTime = (double)(endTime.tv_sec - startTime.tv_sec);
    printf("Total time taken to compress all files: ");
    formatTime(totalTime);

    char* coresString = concat(NB_CORES);

    if (testing_many_cores == true) {
        writePerformance(coresString, totalTime, performance_path);
        free(coresString);
    } else {
        writePerformance("Batches", totalTime, performance_path);
    }
    cleanup(filesList);
}

void compressMultipleFiles(char **filesList, int startIdx, int endIdx, const char *outputDir) {
    for (int i = startIdx; i < endIdx; ++i) {
        compressFile(filesList[i], outputDir, i);
    }
}


char* concat(int NB_CORES) {
    // Calculate needed size (+1 for null terminator)
    int size = snprintf(NULL, 0, "%d", NB_CORES) + 1;
    char *result = malloc(size); // Allocate memory
    if (result != NULL) { // Check allocation success
        snprintf(result, size, "%d", NB_CORES);
    }
    return result;
}
