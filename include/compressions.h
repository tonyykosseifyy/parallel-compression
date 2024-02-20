void compressSequentially(char *inputDir, char *outputDir);
void compressionNProcesses(char *inputDir, const char *outputDir);
void compressWithNBCores(const char *inputDir, const char *outputDir);
void compressInBatches(char *inputDir, const char *outputDir, int NB_CORES);