#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define PATH_TO_FILE  1
#define PATH_TO_DIRECTORY  2
#define INVALID_PATH  -1

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
int main ()
{
  executeWCComand("file.txt");
  printf("%d path checkIfPathExists \n",checkIfPathExists("file.txt"));
  printf("%d path checkIfPathExists \n",checkIfPathExists("file2.txt"));
  printf("%d path checkIfPathExists \n",checkIfPathExists("pasta"));
  return 0;
} 
  
