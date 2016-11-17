#include "main.h"
#include "catCommand.h"

void executeCATCommand(char ** filePaths, int numberOfPaths){
  if (numberOfPaths == 0) {
    printf("You must provide at least one file to the cat command \n");
    return;
  }
  //TODO: Make changes to accept flags, validate paths and ignore invalid paths

  int i = 0;
    while(i<numberOfPaths){
    // convert to absolute path
    char * path = getFullPath(filePaths[i]);
    int pathType = checkIfPathExists(path);
    //validating input
      if (pathType == PATH_TO_FILE) {
        displayFileContent(path);
      } else {
        if (pathType == PATH_TO_DIRECTORY) {
          printf("Invalid argument for cat command: %s is a directory \n",path);
        }
        else {
          printf("Invalid argument for cat command : path %s does not exist \n",path);
        }
      }
    i++;
    }
}
