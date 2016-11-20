#include "main.h"
int parseWCCommand(char** arguments, int size, int* cFlag, int* lFlag, int* wFlag);
void executeWCCommandForSingleFile(char* filePath, int* lineCount, int* wordCount, int* charCount);
void executeWCCommand(char** filePaths, int numberOfPaths);
int countWordsInLine(char* line);
void calculateForLine(char* line, int* lineCount, int* wordCount, int* charCount);
