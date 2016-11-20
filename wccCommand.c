#include "main.h"
#include "wccCommand.h"


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
