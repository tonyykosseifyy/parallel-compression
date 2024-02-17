#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <libgen.h> 
#include <pthread.h>

// global functions
int getNumCPUs();
int countFiles(const char *folderPath);
char ** listFiles(const char *folderPath); 
int directoryExists(const char *path);
void createDirectory(const char *path);
void compressFile(const char *inputFilePath, const char *outputDir, int index);
void cleanup(char **filesList);
void formatTime(double totalTime);

// compression techniques
void compressSequentially(char *inputDir, char *outputDir);
void parallel_2(char *inputDir, char *outputDir);


#endif // HEADERS_H
