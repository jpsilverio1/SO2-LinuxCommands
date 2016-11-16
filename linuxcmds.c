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
  //printf(" word count = %d \n",wordCount);
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
         //printf("%d | %d | %d | %s\n",*lineCount,*wordCount,*charCount);
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

  	executeWCCommandForSingleFile(filePaths[i],&lineCountForCurrentFile, &wordCountForCurrentFile, &charCountForCurrentFile);
  	printf("%d | %d | %d %s\n",lineCountForCurrentFile,wordCountForCurrentFile,charCountForCurrentFile, filePaths[i]);
	totalLineCount += lineCountForCurrentFile;
	totalWordCount += wordCountForCurrentFile;
	totalCharCount += charCountForCurrentFile;
	i++;
  }

  if(numberOfPaths>1){
	printf("%d | %d | %d total\n" ,totalLineCount ,totalWordCount ,totalCharCount);
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
	//TODO: Make changes to accept flags, validate paths and ignore invalid paths

	int i = 0;
	  while(i<numberOfPaths){
		// convert to absolute path
	  	char * path = getFullPath(filePaths[i]);

		displayFileContent(path);	  	

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
void executeCDCommand(char* newDirectory) {
  //TODO: actually validate entries, allow flags

  char actualpath [PATH_MAX];
  newDirectory = getFullPath(newDirectory);
  realpath(newDirectory, actualpath);

  //currentDirectory = newDirectory;
  strcpy(currentDirectory, actualpath);
  
}

void executeECHOCommand(char * argument){
	//TODO: validate maybe? Add more stuff?
	printf("%s\n" ,argument); 
}

void executeMANCommand(char * commandName){
	if (strcmp(commandName,"pwd") == 0 || strcmp(commandName,"wc") == 0 || strcmp(commandName,"cd") == 0 || strcmp(commandName,"cat") == 0
	 || strcmp(commandName,"echo") == 0 || strcmp(commandName,"grep") == 0 || strcmp(commandName,"man") == 0) {
	    
	}
	else {
		// INVALID ARGUMENT
		return;
	}

	strcat(commandName, ".txt");

	char* filePath = getFullPath(commandName);
	
	printf("path obtido: %s\n" , filePath);

	displayFileContent(filePath);
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

char** splitBySpaceIntoArray(char* str, int* arraySize) {
  //printf("string = %s\n", str);
  //char    str[]= "ls -l";
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
  //printf(" number = %d \n",n_spaces);
  for(i = 0; i<n_spaces; i++)
  {
     //printf("\n Element is %s \n", res[i]);
     //printf("i = %d \n",i);
  }

  // here we eliminate '\n' from the last element
  char * last_element = res[n_spaces-1];
  last_element[strlen(last_element) - 1] = 0;
  res[n_spaces-1] = last_element;

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
  //printf("co = %s", elements[0]);
  if (strcmp(command,"pwd\n") == 0 || strcmp(command,"pwd") == 0) {
      executePWDCommand();
  } else {
    if (strcmp(command,"wc\n") == 0 || strcmp(command,"wc") == 0) {
    	executeWCCommand(elements, size);
	
    }
    else {
      if (strcmp(command,"cd\n") == 0 || strcmp(command,"cd") == 0) {
	executeCDCommand(elements[0]);
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


int main (int argc, char *argv[])
{
  int endExecution = 0;
  initializeEnvironment();
  char commandLine [MAX_COMMAND_SIZE];
  while (!endExecution) {
    printf("%s:~$ ",currentDirectory);
    fgets (commandLine, MAX_COMMAND_SIZE, stdin);
    //printf("command = %s ",commandLine);
    if (strcmp(commandLine,"exit\n") == 0) {
      endExecution = 1;
    }
    else {
      interpretCommand(commandLine);
    }

  }
  /*
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
  executePWDCommand();*/
  //splitBySpaceIntoArray(argv[1]);

  return 0;
} 
  
