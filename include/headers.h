#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <libgen.h> 


int getNumCPUs();
int countFiles(const char *folderPath);
char ** listFiles(const char *folderPath); 
int directoryExists(const char *path);
void createDirectory(const char *path);
void compressFile(const char *inputFilePath, const char *outputDir);
void cleanup(char **filesList);