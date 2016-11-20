#include "main.h"
#include "cdCommand.h"

int validateCDCommand(char* path) {
  if(checkIfPathExists(getFullPath(path)) == PATH_TO_DIRECTORY) {
    return VALID_COMMAND_INPUT;
  }
  return INVALID_COMMAND_INPUT;
}

void executeCDCommand(char** arguments, int numberOfArguments) {
  // TODO: allocate new directory properly
  //TODO : fix error here! trying to print arguments[0] gives a segmentatio fault
  char* newDirectory = malloc(PATH_MAX*sizeof(char));
  if (numberOfArguments == 0) {
    strcpy(newDirectory, homeDirectory);
  } else {
    if (numberOfArguments == 1) {
      strcpy(newDirectory, arguments[0]);
    }
    else {
      printf("Invalid arguments for cd command \n");
    }
  }
  //TODO: actually validate entries, allow flags
 
  char actualpath [PATH_MAX];
  newDirectory = getFullPath(newDirectory);
  realpath(newDirectory, actualpath);
  int pathType = checkIfPathExists(newDirectory);
  //validating input
  if (pathType == PATH_TO_DIRECTORY) {
    //currentDirectory = newDirectory;
    strcpy(currentDirectory, actualpath);
  } else {
    if (pathType == PATH_TO_FILE) {
      printf("Invalid argument for cd command: %s is a file \n",newDirectory);
    }
    else {
      printf("Invalid argument : path %s does not exist \n",newDirectory);
    }
  }
}
