#include <dirent.h>
#include "headers.h"

int getNumCPUs()
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

int countFiles(const char *folderPath)
{
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    dir = opendir(folderPath);
    if (dir == NULL)
    {
        return -1;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG)
            count++;
    }
    closedir(dir);
    return count;
}

char **listFiles(const char *folderPath) {
    DIR *dir;
    struct dirent *entry;
    char **filesList;
    int filesCount = countFiles(folderPath);

    if (filesCount == -1)
    {
        filesList = (char **)malloc(sizeof(char *));
        *filesList=NULL;
        printf("Error opening directory <%s>\n",folderPath);
        return filesList;
    }

    dir = opendir(folderPath);

    filesList = (char **)malloc((filesCount + 1) * sizeof(char *));
    int fileIndex = 0;
    char *full_file_name;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            full_file_name = malloc((strlen(folderPath) + strlen(entry->d_name) + 5) * sizeof(char));
            sprintf(full_file_name, "\"%s/%s\"", folderPath, entry->d_name);
            filesList[fileIndex++] = full_file_name;
        }
    }
    filesList[fileIndex] = NULL;
    closedir(dir);
    return filesList;
}

 // Function to check if a directory exists
int directoryExists(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}


void createDirectory(const char *path) {
    mkdir(path, 0755); // Creates a directory with read/write/execute permissions for owner, and read/execute permissions for group and others
}


void compressFile(const char *filePath, const char *outputDir, int index) {
    char command[1024];
    snprintf(command, sizeof(command), "gzip -c \"%s\" > \"%s/meteo%d.gz\"", filePath, outputDir, index);
    system(command);
}


void cleanup(char **filesList) {
    char **temp = filesList;
    while (*temp != NULL) {
        free(*temp); // Free each string
        temp++;
    }
    free(filesList); // Free the array of pointers itself
}

void formatTime(double totalTime) {
    int minutes = (int)totalTime / 60;
    int seconds = (int)totalTime % 60; 
    printf("%d minutes %d seconds\n\n", minutes, seconds);
}
// /results/performance.txt
void writePerformance(char *techniqueName, double totalTime, char *file_path) {
    FILE *file = fopen(file_path, "a");
    fprintf(file, "%s: %f\n", techniqueName, totalTime);
    fclose(file);
}

void clearPerformanceFile(char *file_path) {
    FILE *file = fopen(file_path, "w");
    if (file == NULL) {
        perror("Error clearing performance.txt");
        return;
    }
    // Closing the file immediately clears its contents
    fclose(file);
}


void visualizePerformance() {
    char *gnuplotCommand = "gnuplot -e \""
        "set terminal png size 800,600; "
        "set output './results/techniques/compression_performance.png'; "
        "set title 'Compression Performance'; "
        "set xlabel 'Compression Technique'; "
        "set ylabel 'Time (Seconds)'; "
        "set boxwidth 0.5; " 
        "set style fill solid; "
        "plot './results/techniques/performance.txt' using 2:xtic(1) with boxes notitle;\""; 

    system(gnuplotCommand);
}

// Function to Generate a Line Chart
void visualizePerformanceLineChart() {
    char *gnuplotCommand = "gnuplot -e \""
        "set terminal png size 800,600; "
        "set output './results/techniques/compression_performance_line.png'; " 
        "set title 'Compression Performance'; "
        "set xlabel 'Compression Technique'; "
        "set ylabel 'Time (Seconds)'; "
        "set grid; "
        "set key outside; "
        "plot './results/techniques/performance.txt' using 2:xtic(1) with linespoints title 'Time';\""; 
    system(gnuplotCommand);
}

void visualizeCoresPerformance() {
    char *gnuplotCommand = "gnuplot -e \""
        "set terminal png size 800,600; "
        "set output './results/cores/compression_performance.png'; "
        "set title 'Compression Performance'; "
        "set xlabel 'Number of Cores'; "
        "set ylabel 'Time (Seconds)'; "
        "set boxwidth 0.5; " 
        "set style fill solid; "
        "plot './results/cores/performance.txt' using 2:xtic(1) with linespoints title 'Time';\""; 

    system(gnuplotCommand);
}