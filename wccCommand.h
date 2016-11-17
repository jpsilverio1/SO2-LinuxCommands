#include "main.h"

void executeWCCommandForSingleFile(char* filePath, int* lineCount, int* wordCount, int* charCount);
void executeWCCommand(char** filePaths, int numberOfPaths);
int countWordsInLine(char* line);
void calculateForLine(char* line, int* lineCount, int* wordCount, int* charCount);
