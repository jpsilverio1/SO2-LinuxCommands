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
  printf(" word count = %d \n",wordCount);
  return wordCount;
}
void calculateForLine(char* line, int* lineCount, int* wordCount, int* charCount) {
  *lineCount +=1;
  *charCount+=strlen(line);
  *wordCount += countWordsInLine(line);
}
void executeWCComandForSingleFile(char* filePath, int* lineCount, int* wordCount, int* charCount) {
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
         printf("%d | %d | %d \n",*lineCount,*wordCount,*charCount);
      }
      fclose ( file );
   }
   else
   {
      perror ( filePath ); /* why didn't the file open? */
   }
}
void executeWCComand(char* filePath){
  int lineCountForCurrentFile;
  int wordCountForCurrentFile;
  int charCountForCurrentFile;
  int totalLineCount =0;
  int totalWordCount =0;
  int totalCharCount =0;

  //TODO: Receive a list of paths as input and loop through the list and perform validation of the options for this command
  //accumulate total counts and, at the end..if the size of the array of paths is bigger than 1, display the total counts
  executeWCComandForSingleFile(filePath,&lineCountForCurrentFile, &wordCountForCurrentFile, &charCountForCurrentFile);
  printf("partial result = %d | %d | %d \n",lineCountForCurrentFile,wordCountForCurrentFile,charCountForCurrentFile);
}
void executePWDCommand()
{
  printf("%s\n",currentDirectory);
}  
char* getFullPath (char* path) {
  //TODO
  //if the path is relative(it does not start with slash) -> fullPath = current + "/" + path
  //if the path is absolute (starts with slash) -> fullPath = path
  return NULL;
  
}
int validateCDCommand(char* path) {
  if(checkIfPathExists(getFullPath(path)) == PATH_TO_DIRECTORY) {
    return VALID_COMMAND_INPUT;
  }
  return INVALID_COMMAND_INPUT;
}
int validatePWDCommand() {
  //TODO
  //return INVALID_COMMAND_INPUT;
  return VALID_COMMAND_INPUT;
}
void executeCDCommand(char* newDirectory) {
  //error here - fix it!!
  strcpy(currentDirectory, newDirectory);
  //currentDirectory = newDirectory;
  
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
}
char* getCommand(char* commandLine) {
  return strtok (commandLine," ");
}
void interpretCommand(char* commandLine) {
  char* command = getCommand(commandLine);
  printf("co = %s", command);
  if (strcmp(command,"pwd\n") == 0 || strcmp(command,"pwd") == 0) {
      executePWDCommand();
  } else {
    if (strcmp(command,"wc\n") == 0 || strcmp(command,"wc") == 0) {
    
    }
    else {
      if (strcmp(command,"cd\n") == 0 || strcmp(command,"cd") == 0) {
    
      }
      else {
        if (strcmp(command,"cat\n") == 0 || strcmp(command,"cat") == 0) {
    
        }
        else {
          if (strcmp(command,"echo\n") == 0 || strcmp(command,"echo") == 0) {
    
          }
          else {
            if (strcmp(command,"grep\n") == 0 || strcmp(command,"grep") == 0) {
    
            }
            else {
              if (strcmp(command,"man\n") == 0 || strcmp(command,"man") == 0) {
    
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
  /*switch(command) {
    //just a model, cd, for example, has to receive inputs. The controller also needs to receive inputs
    case "pwd":
      if (validatePWDCommand() == VALID_COMMAND_INPUT) {
	executePWDCommand();
      }
      break;
    case "cd" : 
      if (validateCDCommand("TODO") == VALID_COMMAND_INPUT) {
	executeCDCommand("TODO");
      }
      break;
    default:
      printf("Invalid command! \n");
  }*/
  
}
int main ()
{
  int endExecution = 0;
  initializeEnvironment();
  char commandLine [MAX_COMMAND_SIZE];
  while (!endExecution) {
    printf("/%s:~$ ",currentDirectory);
    fgets (commandLine, MAX_COMMAND_SIZE, stdin);
    printf("command = %s ",commandLine);
    if (strcmp(commandLine,"exit\n") == 0) {
      endExecution = 1;
    }
    else {
      interpretCommand(commandLine);
    }

  }
  printf("home folder = %s\n",getenv("HOME"));
  printf("home folder = %s\n",getpwuid(getuid())->pw_dir);
  
  printf("home directory = %s\n", homeDirectory);
  printf("currentDirectory = %s\n",currentDirectory);
  executeWCComand("file.txt");
  printf("%d path checkIfPathExists \n",checkIfPathExists("file.txt"));
  printf("%d path checkIfPathExists \n",checkIfPathExists("file2.txt"));
  printf("%d path checkIfPathExists \n",checkIfPathExists("pasta"));
  printf("%d path checkIfPathExists \n",checkIfPathExists("/etc"));
  printf("%d path checkIfPathExists \n",checkIfPathExists("etc"));
  executePWDCommand();
  return 0;
} 
  
