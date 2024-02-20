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
#include "compressions.h"


int getNumCPUs();
int countFiles(const char *folderPath);
char ** listFiles(const char *folderPath); 
int directoryExists(const char *path);
void createDirectory(const char *path);
void compressFile(const char *inputFilePath, const char *outputDir, int index);
void cleanup(char **filesList);
void formatTime(double totalTime);
void writePerformance(char *techniqueName, double totalTime);

#endif 
