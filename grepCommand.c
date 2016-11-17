#include "main.h"
#include "grepCommand.h"

void executeGREPCommandForSingleFile(char* filePath, char* stringToMatch) {
  FILE *file = fopen ( filePath, "r" );
   if ( file != NULL )
   {
      char line [ 1024 ]; /* or other suitable maximum line size */
      while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
      {
        if(strstr(line , stringToMatch) !=NULL)
        {
          printf("%s:%s" , filePath, line);
        }
        else
        {
            continue;
        }
      }
      fclose ( file );
   }
   else
   {
      perror ( filePath ); /* why didn't the file open? */
   }
}
void executeGREPCommand(char** arguments, int numberOfPaths){
  if (numberOfPaths>1) {
    char* stringToMatch = malloc(PATH_MAX);
    strcpy(stringToMatch,arguments[0]);
    //TODO: Make changes to accept flags, validate paths and ignore invalid paths
    int i = 1;
    while(i<numberOfPaths){
    //convert to absolute path
      char * path = getFullPath(arguments[i]);
      int pathType = checkIfPathExists(path);
    //validating input
      if (pathType == PATH_TO_FILE) {
        executeGREPCommandForSingleFile(arguments[i], stringToMatch);
      } else {
        if (pathType == PATH_TO_DIRECTORY) {
          printf("Invalid argument for grep command: %s is a directory \n",path);
        }
        else {
          printf("Invalid argument for grep command : path %s does not exist \n",path);
        }
      }
      i++;
    }
  }
  else {
    printf("You must provide at least 2 arguments to the grep command: the string to match and a file \n");
  } 
}
