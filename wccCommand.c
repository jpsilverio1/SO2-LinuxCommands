#include "main.h"
#include "wccCommand.h"

int parseWCCommand(char** arguments, int size, int* cFlag, int* lFlag, int* wFlag) {
  int internalCFlag = 0;
  int internalLFlag = 0;
  int internalWFlag = 0;
  int index;
  int option;
  optind = 1;
  while ((option = getopt (size, arguments, "cwl")) != -1)
    switch (option)
     {
      case 'c':
        internalCFlag = 1;
        break;
      case 'w':
        internalWFlag = 1;
        break;
      case 'l':
        internalLFlag = 1;
        break;
      default:
        break;
      }
      if (internalLFlag == 1 || internalCFlag == 1 || internalWFlag == 1) {
        *cFlag = internalCFlag;
        *lFlag = internalLFlag;
        *wFlag = internalWFlag;
      }
     
       for (index = optind; index < size; index++)
    printf ("Non-option argument %s\n", arguments[index]);
      return optind;
    for (index = optind; index < size; index++)
    printf ("Non-option argument %s\n", arguments[index]);
  
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
      char line [ 1024 ]; 
      while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
      {
        calculateForLine(line, lineCount, wordCount, charCount);
      }
      fclose ( file );
   }
   else
   {
      perror ( filePath ); 
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

    int cFlag = 1, lFlag = 1, wFlag= 1;
    int initialArgumentIndex = parseWCCommand(filePaths, numberOfPaths, &cFlag, &lFlag, &wFlag);
     printf("flag values c: %d l: %d w: %d \n", cFlag, lFlag, wFlag);
    printf("initialArgumentIndex = %d \n",initialArgumentIndex);
    int i = initialArgumentIndex;
    while(i<numberOfPaths){
    //convert to absolute path
      char * path = getFullPath(filePaths[i]);
      int pathType = checkIfPathExists(path);
    //validating input
      if (pathType == PATH_TO_FILE) {
        executeWCCommandForSingleFile(filePaths[i],&lineCountForCurrentFile, &wordCountForCurrentFile, &charCountForCurrentFile);
        if (lFlag == 1) {
          printf("\t %d",lineCountForCurrentFile);
        }
        if (wFlag == 1) {
          printf("\t %d",wordCountForCurrentFile);
        }
        if (cFlag == 1) {
          printf("\t %d",charCountForCurrentFile);
        }
        printf("\t %s\n",filePaths[i]);
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
      if (lFlag == 1) {
          printf("\t %d",totalLineCount);
        }
        if (wFlag == 1) {
          printf("\t %d",totalWordCount);
        }
        if (cFlag == 1) {
          printf("\t %d",totalCharCount);
        }
        printf("\t total\n");
    }
  }
  else {
    printf("You must provide at least one file to the wc command \n");
  } 
}
