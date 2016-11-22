#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pwd.h>
#include <limits.h>

#define PATH_TO_FILE  1
#define PATH_TO_DIRECTORY  2
#define INVALID_PATH  -1
#define VALID_COMMAND_INPUT 1
#define INVALID_COMMAND_INPUT -1
#define MAX_COMMAND_SIZE 6000


char currentDirectory[PATH_MAX];
char* homeDirectory;
char workingDirectory[PATH_MAX];


int main (int argc, char *argv[]);
void interpretCommand(char* commandLine);
char** splitBySpaceIntoArray(char* str, int* arraySize);
char* getCommand(char* commandLine);
char* getUserHomeDirectory();
void initializeEnvironment();
void displayFileContent(char * filePath, int flag);
char* getFullPath (char* path);
int checkIfPathExists(char* path);
