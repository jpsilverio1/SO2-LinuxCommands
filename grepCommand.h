#include "main.h"

int parseGREPCommand(char** arguments, int size, int* iFlag);

void executeGREPCommandForSingleFile(char* filePath, char* stringToMatch, int iFlag);

void executeGREPCommand(char** arguments, int numberOfPaths);
