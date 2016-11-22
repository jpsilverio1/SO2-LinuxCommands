#define _GNU_SOURCE
#include "main.h"
#include "wccCommand.h"
#include "catCommand.h"
#include "cdCommand.h"
#include "echoCommand.h"
#include "manCommand.h"
#include "pwdCommand.h"
#include "grepCommand.h"

int checkIfPathExists(char* path) {
  struct stat data;
  int err = stat(path, &data);
  if(err == -1) {
    if(ENOENT == errno) {
        return INVALID_PATH;
    } else {
        perror("Internal Error!");
        exit(1);
    }
} else {
    if(S_ISDIR(data.st_mode)) {
        return PATH_TO_DIRECTORY;
    } else {
        return PATH_TO_FILE;
    }
}
}


char* getFullPath (char* path) {
  
  char *fullPath = malloc(PATH_MAX);
  //if the path is relative(it does not start with slash) -> fullPath = current + "/" + path
  if(path[0] == '/'){
  strcpy(fullPath,path);
  }
  //if the path is absolute (starts with slash) -> fullPath = path
  else {
    strcpy(fullPath,currentDirectory);
    strcat(fullPath,"/");
    strcat(fullPath, path);
  }

  return fullPath;
  
}

void displayFileContent(char * filePath, int flag) {
  FILE *file = fopen(filePath, "r");

  if (file != NULL)
  {
    char line [ 6000 ];
	int count = 0; 
	while ( fgets ( line, sizeof line, file ) != NULL ) {
		count += 1;
		if (flag == 1) {
			printf("%d\t%s",count,line);		
		} else {
			printf("%s",line);
		}
	}
    fclose(file);
  }
}







char* getUserHomeDirectory(){
  return getpwuid(getuid())->pw_dir;
}

void initializeEnvironment(){
  homeDirectory = getUserHomeDirectory();
  if (getcwd(currentDirectory,PATH_MAX) == NULL) {
    exit(1);
   } 
  strcpy(workingDirectory, currentDirectory);
}
char* getCommand(char* commandLine) {
  return strtok (commandLine," ");
}
char** getArgumentsFromLine(char** res){
	return res+1;
}
char** splitBySpaceIntoArray(char* str, int* arraySize) {
  char ** res  = NULL;
  char *  p    = strtok (str, " ");
  int n_spaces = 0, i;

  while (p) {
    res = realloc (res, sizeof (char*) * ++n_spaces);
    if (res == NULL)
      exit (-1); 

    res[n_spaces-1] = p;

    p = strtok (NULL, " ");
  }

  // here we eliminate '\n' from the last element
  char * last_element = res[n_spaces-1];
  last_element[strlen(last_element) - 1] = 0;
  res[n_spaces-1] = last_element;

  //checking if last argument is actually whitespace
  if(strspn(res[n_spaces-1], " \r\n\t") == strlen(res[n_spaces-1])){
    n_spaces--;
  }
  //setting size pointer
  //this is a way of returning the size of the array
  *arraySize = n_spaces;
  //only returning the non-command elements
  return res;
}

void interpretCommand(char* commandLine) {
  int size;
  //vector of strings containing all the elements after the command
  // after calling this "size" now stores the array size
  char ** elements = splitBySpaceIntoArray(commandLine, &size);
  //single string containing command
  char* command = getCommand(commandLine);
  char** commandLineWithoutCommand = getArgumentsFromLine(elements);
  int sizeWithoutCommand = size -1;
  if (strcmp(command,"pwd\n") == 0 || strcmp(command,"pwd") == 0) {
      executePWDCommand();
  } else {
    if (strcmp(command,"wc\n") == 0 || strcmp(command,"wc") == 0) {
      executeWCCommand(elements, size);
    }
    else {
      if (strcmp(command,"cd\n") == 0 || strcmp(command,"cd") == 0) {
          if (sizeWithoutCommand==0 || sizeWithoutCommand == 1) {
            executeCDCommand(commandLineWithoutCommand,sizeWithoutCommand);
          }
          else {
            printf("Invalid arguments for cd command. The command takes zero or 1 argument \n");
          }
      }
      else {
        if (strcmp(command,"cat\n") == 0 || strcmp(command,"cat") == 0) {
        executeCATCommand(elements, size);
        }
        else {
          if (strcmp(command,"echo\n") == 0 || strcmp(command,"echo") == 0) {
        executeECHOCommand(commandLineWithoutCommand[0]);
          }
          else {
            if (strcmp(command,"grep\n") == 0 || strcmp(command,"grep") == 0) {
              executeGREPCommand(elements, size);
            }
            else {
              if (strcmp(command,"man\n") == 0 || strcmp(command,"man") == 0) {
        executeMANCommand(commandLineWithoutCommand[0]);
              }
              else {
                printf("Invalid command! \n");
              }
            }
          }
        }
      }
    }
  }
  
}
int main (int argc, char *argv[])

{

  int endExecution = 0;
  initializeEnvironment();
  char commandLine [MAX_COMMAND_SIZE];
  while (!endExecution) {
    printf("%s:~$ ",currentDirectory);
    fgets (commandLine, MAX_COMMAND_SIZE, stdin);
    if (strcmp(commandLine,"exit\n") == 0) {
      endExecution = 1;
    }
    else {
      interpretCommand(commandLine);
    }

  }

  return 0;
} 
