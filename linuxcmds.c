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
int countWordsInLine(char* line) {
  char * word;
  int wordCount =0;
  char* stringDelimiters = " \f\t\n\r\t\v";
  word = strtok (line,stringDelimiters);
  while (word != NULL)
  {
    wordCount+=1;
    word = strtok (NULL, stringDelimiters);
  }
  return wordCount;
}
void calculateForLine(char* line, int* lineCount, int* wordCount, int* charCount) {
  *lineCount +=1;
  *charCount+=strlen(line);
  *wordCount += countWordsInLine(line);
}
void executeWCCommandForSingleFile(char* filePath, int* lineCount, int* wordCount, int* charCount) {
  FILE *file = fopen ( filePath, "r" );
  *lineCount =0;
  *wordCount =0;
  *charCount =0;
   if ( file != NULL )
   {
      char line [ 1024 ]; /* or other suitable maximum line size */
      while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
      {
        calculateForLine(line, lineCount, wordCount, charCount);
      }
      fclose ( file );
   }
   else
   {
      perror ( filePath ); /* why didn't the file open? */
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

void executeWCCommand(char** filePaths, int numberOfPaths){
  if (numberOfPaths>0) {
    int lineCountForCurrentFile;
    int wordCountForCurrentFile;
    int charCountForCurrentFile;
    int totalLineCount =0;
    int totalWordCount =0;
    int totalCharCount =0;

    //TODO: Make changes to accept flags, validate paths and ignore invalid paths
    int i = 0;
    while(i<numberOfPaths){
    //convert to absolute path
      char * path = getFullPath(filePaths[i]);
      int pathType = checkIfPathExists(path);
    //validating input
      if (pathType == PATH_TO_FILE) {
        executeWCCommandForSingleFile(filePaths[i],&lineCountForCurrentFile, &wordCountForCurrentFile, &charCountForCurrentFile);
        printf("\t %d \t %d \t %d \t %s\n",lineCountForCurrentFile,wordCountForCurrentFile,charCountForCurrentFile, filePaths[i]);
        totalLineCount += lineCountForCurrentFile;
        totalWordCount += wordCountForCurrentFile;
        totalCharCount += charCountForCurrentFile;
      } else {
        if (pathType == PATH_TO_DIRECTORY) {
          printf("Invalid argument for wc command: %s is a directory \n",path);
        }
        else {
          printf("Invalid argument for wc command : path %s does not exist \n",path);
        }
      }
      i++;
    }
    if(numberOfPaths>1){
      printf("%d | %d | %d total\n" ,totalLineCount ,totalWordCount ,totalCharCount);
    }
  }
  else {
    printf("You must provide at least one file to the wc command \n");
  } 
}

void displayFileContent(char * filePath){
  FILE *file = fopen(filePath, "r");

  if (file != NULL)
  {
    int c;

      while ((c = fgetc(file)) != EOF)
      {
          putchar(c);
      }
    fclose(file);
  }
}

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

void executePWDCommand()
{
  printf("%s\n",currentDirectory);
}  

int validateCDCommand(char* path) {
  if(checkIfPathExists(getFullPath(path)) == PATH_TO_DIRECTORY) {
    return VALID_COMMAND_INPUT;
  }
  return INVALID_COMMAND_INPUT;
}
int parseCDArguments() {

}
int validatePWDCommand() {
  //TODO
  //return INVALID_COMMAND_INPUT;
  return VALID_COMMAND_INPUT;
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
  printf("new directory2 = %s path type = %d \n", newDirectory,pathType);
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

void executeECHOCommand(char * argument){
  //TODO: validate maybe? Add more stuff?
  printf("%s\n" ,argument); 
}

void executeMANCommand(char * commandName){
  if (strcmp(commandName,"pwd") == 0 || strcmp(commandName,"wc") == 0 || strcmp(commandName,"cd") == 0 || strcmp(commandName,"cat") == 0
   || strcmp(commandName,"echo") == 0 || strcmp(commandName,"grep") == 0 || strcmp(commandName,"man") == 0) {
     
   strcat(commandName, ".txt");

   char* filePath = strcat(workingDirectory,commandName);
   printf("%s\n" ,filePath); 
   displayFileContent(filePath);
 
  }
  else {
    printf("No manual entry for %s \n",commandName);
    return;
  }

  
}

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

char* getUserHomeDirectory(){
  //return getenv("HOME");
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

char** splitBySpaceIntoArray(char* str, int* arraySize) {
  char ** res  = NULL;
  char *  p    = strtok (str, " ");
  int n_spaces = 0, i;


  /* split string and append tokens to 'res' */

  while (p) {
    res = realloc (res, sizeof (char*) * ++n_spaces);
    if (res == NULL)
      exit (-1); /* memory allocation failed */

    res[n_spaces-1] = p;

    p = strtok (NULL, " ");
  }

  // here we eliminate '\n' from the last element
  char * last_element = res[n_spaces-1];
  last_element[strlen(last_element) - 1] = 0;
  res[n_spaces-1] = last_element;

  //checking if last argument is actually whitespace
  if(strspn(res[n_spaces-1], " \r\n\t") == strlen(res[n_spaces-1])){
    printf("detectou que eh soh espaco branco\n");
    n_spaces--;
  }
  //setting size pointer
  //this is a way of returning the size of the array
  *arraySize = n_spaces - 1;
  //only returning the non-command elements
  return res + 1;
}

void interpretCommand(char* commandLine) {
  int size;
  //vector of strings containing all the elements after the command
  // after calling this "size" now stores the array size
  char ** elements = splitBySpaceIntoArray(commandLine, &size);
  //single string containing command
  char* command = getCommand(commandLine);
  printf("command = %s size = %d\n",command,size);
  if (strcmp(command,"pwd\n") == 0 || strcmp(command,"pwd") == 0) {
      executePWDCommand();
  } else {
    if (strcmp(command,"wc\n") == 0 || strcmp(command,"wc") == 0) {
      executeWCCommand(elements, size);
    }
    else {
      if (strcmp(command,"cd\n") == 0 || strcmp(command,"cd") == 0) {
          if (size==0 || size == 1) {
            executeCDCommand(elements,size);
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
        executeECHOCommand(elements[0]);
          }
          else {
            if (strcmp(command,"grep\n") == 0 || strcmp(command,"grep") == 0) {
              executeGREPCommand(elements, size);
            }
            else {
              if (strcmp(command,"man\n") == 0 || strcmp(command,"man") == 0) {
        executeMANCommand(elements[0]);
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
  executeGREPCommandForSingleFile("/Users/jpsilverio/Documents/junk.txt", "is");
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
  
