#include "main.h"
#include "manCommand.h"

void executeMANCommand(char * commandName){
  if (strcmp(commandName,"pwd") == 0 || strcmp(commandName,"wc") == 0 || strcmp(commandName,"cd") == 0 || strcmp(commandName,"cat") == 0
   || strcmp(commandName,"echo") == 0 || strcmp(commandName,"grep") == 0 || strcmp(commandName,"man") == 0) {
      
	  strcat(commandName, ".txt");

      char filePath[PATH_MAX];

	  strcpy(filePath,workingDirectory);
	  strcat(filePath,"/");
	  strcat(filePath,commandName);

      displayFileContent(filePath,0);
  }
  else {
    printf("No manual entry for %s \n",commandName);
    return;
  }

  
}
