#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>


int getNumCPUs();
int countFiles(const char *folderPath);
char ** listFiles(const char *folderPath); 