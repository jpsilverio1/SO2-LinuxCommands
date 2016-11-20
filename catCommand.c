#include "main.h"
#include "catCommand.h"
int parseCATCommand(char** arguments, int size, int* flag) {
  int nFlag = 0;
  int index;
  int option;
  optind = 1;
    option = getopt (size, arguments, "n");
    switch (option)
     {
      case 'n':
        nFlag = 1;
		printf("entrei aqui \n");
        break;
      default:
		break;
      }
      printf("flag = %d \n",nFlag);
      *flag = nFlag;
      return optind;
    for (index = optind; index < size; index++)
    printf ("Non-option argument %s\n", arguments[index]);
  
}
void executeCATCommand(char ** filePaths, int numberOfPaths){
  if (numberOfPaths == 0) {
    printf("You must provide at least one file to the cat command \n");
    return;
  }
  int flag;
  int initialIndex = parseCATCommand(filePaths, numberOfPaths, &flag);
  //TODO: Make changes to accept flags, validate paths and ignore invalid paths

  int i = initialIndex;
    while(i<numberOfPaths){
    // convert to absolute path
    char * path = getFullPath(filePaths[i]);
    int pathType = checkIfPathExists(path);
    //validating input
      if (pathType == PATH_TO_FILE) {
        displayFileContent(path,flag);
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
